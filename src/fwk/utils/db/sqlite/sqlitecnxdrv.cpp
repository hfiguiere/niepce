/*
 * (c) 2007-2013 Hubert Figuiere
 *
 ***********************************************************
 *This file was part of the Nemiver Project.
 *
 *Nemiver is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Nemiver is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Nemiver;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */


#include "config.h"

#include <string.h>
#include <string>
#include <list>
#include <functional>
#include <mutex>

#include <sqlite3.h>
#include "fwk/utils/exception.hpp"
#include "fwk/utils/buffer.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/utils/db/sqlstatement.hpp"
#include "sqlitecnxdrv.hpp"

namespace db { namespace sqlite {
	
	
		struct Sqlite3Unref {
			void operator () (sqlite3 *a_ptr)
				{
					sqlite3_close(a_ptr) ;
				}
		};//end struct Sqlite3Unref
	
		template<class PointerType,
				 class UnreferenceFunctor>
		class SafePtr
		{
		protected:
			mutable PointerType *m_pointer ;
		
		
		public:
			explicit SafePtr (const PointerType *a_pointer) 
				:	m_pointer(const_cast<PointerType*>(a_pointer))
				{
				}
		
			SafePtr() 
				: m_pointer(NULL)
				{
				}

			~SafePtr ()
				{
					unreference();
				}
		
			operator bool () const
				{
					return (m_pointer != NULL);
				}
		
			void
			reset (const PointerType *a_pointer)
				{
					if (a_pointer != m_pointer) {
						unreference() ;
						m_pointer = const_cast<PointerType*>(a_pointer) ;
					}
				}
		
			PointerType*
			get () const
				{
					return m_pointer ;
				}
		
			void
			unreference ()
				{
					if (m_pointer) {
						UnreferenceFunctor do_unref ;
						do_unref (m_pointer) ;
					}
				}

		};//end class SafePtr



		typedef SafePtr<sqlite3, Sqlite3Unref> Sqlite3SafePtr ;


		struct SqliteCnxDrv::Priv {
			//sqlite3 database connection handle.
			//Must be de-allocated by a call to
			//sqlite3_close ();
			Sqlite3SafePtr sqlite ;

			//the current prepared sqlite statement.
			//It must be deallocated after the result set
			//has been read, before we can close the db,
			//or before another statement is prepared.
			sqlite3_stmt *cur_stmt ;

			//the result of the last sqlite3_step() function, or -333
			int last_execution_result ;

			std::list<std::function<void (void)> > userfunctions;

			Priv():
				sqlite(NULL),
				cur_stmt(NULL),
				last_execution_result(-333)
				{
				}

			bool step_cur_statement() ;

			bool check_offset(uint32_t a_offset) ;
		};

		bool
		SqliteCnxDrv::Priv::step_cur_statement ()
		{
			RETURN_VAL_IF_FAIL (cur_stmt, false) ;
			last_execution_result = sqlite3_step (cur_stmt) ;
			bool result (false) ;

		decide:
			switch (last_execution_result) {
			case SQLITE_BUSY:
				//db file is locked. Let's try again a couple of times.
				for (int i=0;i<2;++i) {
					sleep (1) ;
					last_execution_result = sqlite3_step (cur_stmt) ;
					if (last_execution_result != SQLITE_BUSY)
						goto decide ;
				}
				result = false ;
				break ;
			case SQLITE_DONE:
				//the statement was successfuly executed and
				//there is no more data to fecth.
				//go advertise the good news
				result = true ;
				break ;
			case SQLITE_ROW:
				//the statement was successfuly executed and
				//there is some rows waiting to be fetched.
				//go advertise the good news.
				result = true ;
				break;
			case SQLITE_ERROR:
				LOG_ERROR (std::string("sqlite3_step() encountered a runtime error:")
						   + sqlite3_errmsg (sqlite.get ())) ;
				if (cur_stmt) {
					sqlite3_finalize (cur_stmt) ;
					cur_stmt = NULL ;
				}
				result = false ;
				break;
			case SQLITE_MISUSE:
				LOG_ERROR ("seems like sqlite3_step() has been called too much ...") ;
				if (cur_stmt) {
					sqlite3_finalize (cur_stmt) ;
					cur_stmt = NULL ;
				}
				result = false ;
				break;
			default:
				LOG_ERROR ("got an unknown error code from sqlite3_step") ;
				if (cur_stmt) {
					sqlite3_finalize (cur_stmt) ;
					cur_stmt = NULL ;
				}
				result = false ;
				break;
			}
			return result ;
		}

		bool
		SqliteCnxDrv::Priv::check_offset (uint32_t a_offset)
		{
			if (!cur_stmt
				|| (static_cast<long> (a_offset) >= sqlite3_column_count (cur_stmt)))
				return false ;
			return true ;
		}

		SqliteCnxDrv::SqliteCnxDrv (sqlite3 *a_sqlite_handle)
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (a_sqlite_handle) ;
			m_priv.reset (new Priv) ;
			m_priv->sqlite.reset (a_sqlite_handle) ;
		}

		SqliteCnxDrv::~SqliteCnxDrv ()
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			close () ;
			m_priv.reset(NULL);
		}

	    sqlite3 * SqliteCnxDrv::sqlite_handle() const
		{
			if (m_priv && m_priv->sqlite) {
				return m_priv->sqlite.get ();
			}
			return NULL;
		}

		const char*
		SqliteCnxDrv::get_last_error () const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			if (m_priv && m_priv->sqlite) {
				return sqlite3_errmsg (m_priv->sqlite.get ());
			}
			return NULL ;
		}

		bool
		SqliteCnxDrv::start_transaction ()
		{
			THROW_IF_FAIL (m_priv && m_priv->sqlite) ;
			return execute_statement (SQLStatement ("begin transaction")) ;
		}

		bool
		SqliteCnxDrv::commit_transaction ()
		{
			THROW_IF_FAIL (m_priv && m_priv->sqlite) ;
			return execute_statement (SQLStatement ("commit")) ;
		}

		bool
		SqliteCnxDrv::rollback_transaction ()
		{
			THROW_IF_FAIL (m_priv && m_priv->sqlite) ;
			return execute_statement (SQLStatement ("rollback")) ;
		}

		bool
		SqliteCnxDrv::execute_statement (const SQLStatement &a_statement)
		{
		  std::lock_guard<std::recursive_mutex> lock(m_mutex);

		  THROW_IF_FAIL (m_priv && m_priv->sqlite) ;
		  DBG_OUT("sql string: %s", a_statement.to_string().c_str()) ;
		  
		  //if the previous statement
		  //(also contains the resulting context of a query
		  //execution) hasn't been deleted, delete it before
		  //we go forward.
		  if (m_priv->cur_stmt) {
			sqlite3_finalize (m_priv->cur_stmt) ;
			m_priv->cur_stmt = NULL ;
			m_priv->last_execution_result = SQLITE_OK ;
		  }
			
		  if (a_statement.to_string().empty())
			return false ;
			
		  int status = sqlite3_prepare (m_priv->sqlite.get (),
										a_statement.to_string ().c_str (),
										a_statement.to_string ().size(),
										&m_priv->cur_stmt,
										NULL) ;

		  if (status != SQLITE_OK) {
			ERR_OUT("sqlite3_prepare() failed, returning: %d: %s: sql was: '%s'",
					status, get_last_error(), a_statement.to_string().c_str()) ;
			return false ;
		  }

		  const auto & bindings = a_statement.bindings();
		  for(auto iter = bindings.cbegin();
			  iter != bindings.cend(); iter++) 
		  {
			ColumnType ctype = iter->get<0>();
			int idx = iter->get<1>();
			switch(ctype) {
			case ColumnType::STRING:
			{
			  try {
				const std::string & text(boost::any_cast<std::string>(iter->get<2>()));
				sqlite3_bind_text(m_priv->cur_stmt, idx, text.c_str(),
								  text.size(), SQLITE_TRANSIENT);
			  }
			  catch(...)
			  {
				DBG_OUT("e");
			  }
			  break;
			}
			case ColumnType::BLOB:
			{
			  try {
				const fwk::Buffer* blob(boost::any_cast<const fwk::Buffer*>(iter->get<2>()));
				sqlite3_bind_blob(m_priv->cur_stmt, idx, blob->get_data(), 
								  blob->get_len(), SQLITE_TRANSIENT);
			  }
			  catch(...)
			  {
				DBG_OUT("e");
			  }
			  break;
			}
			default:
			  break;
			}
		  }
		  
		  THROW_IF_FAIL (m_priv->cur_stmt) ;
		  if (!should_have_data ()) {
			return m_priv->step_cur_statement () ;
		  }

		  return true ;
		}

        namespace {
            
        void wrapper(sqlite3_context* ctx, int, sqlite3_value**)
        {
            std::function<void (void)> *f;
            f = (std::function<void (void)>*)sqlite3_user_data(ctx);
            (*f)();
        }

        }

        bool 
        SqliteCnxDrv::create_function0(const std::string & name, 
                                       const db::IConnectionDriver::f0_t & f)
        {
            m_priv->userfunctions.push_back(f);
            sqlite3_create_function(m_priv->sqlite.get(), name.c_str(), 
                                    0, SQLITE_ANY, 
                                    (void*)&(m_priv->userfunctions.back()), 
                                    &wrapper, NULL, NULL);

            return true;
        }


		bool
		SqliteCnxDrv::should_have_data () const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			if (get_number_of_columns () > 0)
				return true ;
			return false ;
		}

		bool
		SqliteCnxDrv::read_next_row ()
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;
			if (m_priv->cur_stmt) {
				if (m_priv->last_execution_result == SQLITE_DONE) {
					return false ;
				} else {
					bool res = m_priv->step_cur_statement () ;
					if (res == true) {
						if (m_priv->last_execution_result == SQLITE_DONE) {
							//there is no more data to fetch.
							return false ;
						} else {
							return true ;
						}
					}
				}
			}
			return false ;
		}

		unsigned int
		SqliteCnxDrv::get_number_of_columns () const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;
			if (!m_priv->cur_stmt)
				return 0 ;
			return sqlite3_column_count (m_priv->cur_stmt) ;
		}

		bool
		SqliteCnxDrv::get_column_content (uint32_t a_offset,
										  fwk::Buffer &a_column_content) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			a_column_content.set
				(static_cast<const char*>(sqlite3_column_blob (m_priv->cur_stmt, a_offset)),
				 sqlite3_column_bytes (m_priv->cur_stmt, a_offset)) ;
			return true ;
		}

		bool
		SqliteCnxDrv::get_column_content (uint32_t a_offset,
										  int32_t &a_column_content) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			int type = sqlite3_column_type (m_priv->cur_stmt, a_offset) ;
			if ((type != SQLITE_INTEGER) && (type != SQLITE_NULL)) {
				ERR_OUT("column number %d is not of integer type", 
						static_cast<int>(a_column_content));
				return false ;
			}
			a_column_content = sqlite3_column_int (m_priv->cur_stmt, a_offset) ;
			return true ;
		}

		bool
		SqliteCnxDrv::get_column_content (uint32_t a_offset,
										  int64_t &a_column_content) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			int type = sqlite3_column_type (m_priv->cur_stmt, a_offset) ;
			if ((type != SQLITE_INTEGER) && (type != SQLITE_NULL)) {
				ERR_OUT("column number %d is not of integer type", 
						static_cast<int>(a_column_content));
				return false ;
			}
			a_column_content = sqlite3_column_int64 (m_priv->cur_stmt, a_offset) ;
			return true ;
		}


		bool
		SqliteCnxDrv::get_column_content (uint32_t a_offset,
										  double& a_column_content) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;
			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			int type = sqlite3_column_type (m_priv->cur_stmt, a_offset) ;
			if ((type != SQLITE_FLOAT) && (type != SQLITE_NULL)) {
				ERR_OUT("column number %d is not of type float", (int) a_offset);
				return false ;
			}
			a_column_content = sqlite3_column_double (m_priv->cur_stmt, a_offset) ;
			return true ;
		}

		bool
		SqliteCnxDrv::get_column_content (uint32_t a_offset,
										  std::string& a_column_content) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			int type = sqlite3_column_type (m_priv->cur_stmt, a_offset) ;
			if (type == SQLITE_BLOB) {
				ERR_OUT("column number %d is of type blob", (int) a_offset) ;
				return false ;
			}
			const char * text = reinterpret_cast<const char*>(sqlite3_column_text(m_priv->cur_stmt,a_offset));
			DBG_ASSERT(text, "column content can't be NULL");
			if(text) {
				a_column_content = text;
			}
			else {
				ERR_OUT("text column content is NULL");
			}
			return true ;
		}

		bool
		SqliteCnxDrv::get_column_type (uint32_t a_offset,
									  ColumnType &a_type) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;
			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			int type = sqlite3_column_type (m_priv->cur_stmt, a_offset) ;

			switch (type) {
			case SQLITE_INTEGER:
                                a_type = ColumnType::INT ;
				break;
			case SQLITE_FLOAT:
                                a_type = ColumnType::DOUBLE ;
				break;
			case SQLITE_TEXT:
				a_type = ColumnType::STRING ;
				break ;
			case SQLITE_BLOB:
				a_type = ColumnType::BLOB ;
				break;

			case SQLITE_NULL:
				a_type = ColumnType::BLOB ;
				break ;
			default:
				a_type = ColumnType::UNKNOWN ;
				break;
			}
			return true ;
		}

		bool
		SqliteCnxDrv::get_column_name (uint32_t a_offset, fwk::Buffer &a_name) const
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;
			RETURN_VAL_IF_FAIL (m_priv->check_offset (a_offset), false) ;
			const char* name = sqlite3_column_name (m_priv->cur_stmt, a_offset) ;
			if (!name)
				return false ;
			a_name.set (name, strlen (name)) ;
			return true ;
		}

		void
		SqliteCnxDrv::close ()
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv) ;

			if (m_priv->sqlite) {
				if (m_priv->cur_stmt) {
					sqlite3_finalize (m_priv->cur_stmt) ;
					m_priv->cur_stmt = NULL;
				}
			}
		}

		int64_t
		SqliteCnxDrv::last_row_id()
		{
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
			THROW_IF_FAIL (m_priv);
			if (m_priv->sqlite) {
				return sqlite3_last_insert_rowid(m_priv->sqlite.get());
			}
			return -1;
		}
	}//end namespace sqlite
}//end namespace db


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
