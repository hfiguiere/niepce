#ifndef __UTILS_EXEMPI_H__
#define __UTILS_EXEMPI_H__

/*
 * niepce - utils/exempi.h
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <vector>
#include <string>

#include <boost/noncopyable.hpp>

#include <exempi/xmp.h>


namespace xmp {

inline
void release(XmpIteratorPtr ptr)
{
    xmp_iterator_free(ptr);
}

inline
void release(XmpStringPtr ptr)
{
    xmp_string_free(ptr);
}

inline
void release(XmpFilePtr ptr)
{
    xmp_files_free(ptr);
}

inline 
void release(XmpPtr ptr)
{
    xmp_free(ptr);
}

/**
 * @brief a scoped pointer for Xmp opaque types
 * @todo move to Exempi.
 */
template <class T>
class ScopedPtr 
    : boost::noncopyable
{
public:
    ScopedPtr(T p)
        : _p(p)
        {}
    ~ScopedPtr()
        { if (_p) release(_p); }
    operator T() const
        { return _p; }
private:
    T _p;
};


enum MetaDataType {
    META_DT_NONE = 0,
    META_DT_STRING,
    META_DT_STRING_ARRAY,
    META_DT_DATE,
    META_DT_FRAC, 
    META_DT_STAR_RATING
};
	
	
struct MetaDataFormat {
    const char * label;
    const char * ns;
    const char * property;
    MetaDataType     type;
    bool         readonly;
};
	
struct MetaDataSectionFormat {
    const char * section;
    const MetaDataFormat * formats;
};

extern const char * UFRAW_INTEROP_NAMESPACE;	
extern const char * UFRAW_INTEROP_NS_PREFIX;	

}


namespace fwk {

class ExempiManager
    : public boost::noncopyable
{
public:
    struct ns_defs_t {
        const char *ns;
        const char *prefix;
    };
    /** construct with namespaces to initialize */
    ExempiManager(const ns_defs_t * namespaces = 0);
    ~ExempiManager();
};

/** a high-level wrapper for xmp */
class XmpMeta
{
public:
    XmpMeta();
    XmpMeta(const std::string & for_file, bool sidecar_only);
    virtual ~XmpMeta();

    bool isOk() const 
        { return m_xmp != NULL; }
    XmpPtr xmp() const
        { return m_xmp; }
    /** serialize the XMP inline */
    std::string serialize_inline() const;
    /** serialize the XMP (for the sidecar) */
    std::string serialize() const;
    /** load the XMP from the unserialized buffer 
     * (NUL terminated) 
     */
    void unserialize(const char *);

    int32_t orientation() const;
    std::string label() const;
    /** return the rating, -1 is not found (not set) */
    int32_t rating() const;
    time_t  creation_date() const;
    std::string creation_date_str() const;
    const std::vector< std::string > & keywords() const;
private:
    XmpMeta( const XmpMeta & ); // copy constructor

    XmpMeta & operator=(const XmpMeta &); // assignment

    XmpPtr m_xmp;
    // caches
    mutable bool m_keyword_fetched;
    mutable std::vector< std::string > m_keywords;
};
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/

#endif
