/*
 * niepce - eng/db/libfile.h
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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

#ifndef __NIEPCE_DB_LIBFILE_H__
#define __NIEPCE_DB_LIBFILE_H__

#include <string>
#include <list>
#include <tr1/memory>

#include "fwk/toolkit/mimetype.hpp"
#include "fwk/base/propertybag.hpp"
#include "engine/db/librarytypes.hpp"
#include "engine/db/keyword.hpp"
#include "engine/db/storage.hpp"
#include "engine/db/fsfile.hpp"

namespace eng {


class LibFile
{
public:
    typedef std::tr1::shared_ptr< LibFile > Ptr;
    typedef std::list< Ptr > List;
    typedef std::tr1::shared_ptr< List > ListPtr;

    enum FileType {
        FILE_TYPE_UNKNOWN = 0,
        FILE_TYPE_RAW = 1,
        FILE_TYPE_RAW_JPEG = 2,
        FILE_TYPE_IMAGE = 3,
        FILE_TYPE_VIDEO = 4
    };

    static FileType mimetype_to_filetype(fwk::MimeType mime);

    LibFile(library_id_t id, library_id_t folderId, library_id_t fsfileid, 
            const std::string & p,
            const std::string & name );
    virtual ~LibFile();

    library_id_t id() const
        { return m_id; }
    library_id_t folderId() const
        { return m_folderId; }
    const std::string & name() const
        { return m_name; }
    const std::string & path() const
        { return m_main_file.path(); }

//		Storage::Ptr storage() const;

    void setOrientation(int32_t v);
    int32_t orientation() const
        { return m_orientation; }
    void setRating(int32_t v);
    int32_t rating() const
        { return m_rating; }
    void setLabel(int32_t v);
    int32_t label() const
        { return m_label; }
    void setFlag(int32_t v);
    int32_t flag() const
        { return m_flag; }
    void setProperty(fwk::PropertyIndex idx, int32_t v);
    int32_t property(fwk::PropertyIndex idx) const;

    /** Setter for the filetype.
     * @param v the FILETYPE of the file
     */
    void setFileType(FileType v);
    /** Getter for the filetype enumeration. */
    FileType fileType() const
        { return m_file_type; }
    
    /** return an URI of the real path
     * because the Gtk stuff want that.
     */
    const std::string uri() const
        { return std::string("file://") + m_main_file.path(); }
    /** check is the library file is at uri
     * @return true of the uri match
     * @todo
     */
//		bool isUri(const char * _uri) const
//			{ return uri() == _uri; }
private:
    library_id_t         m_id;           /**< file ID */
    library_id_t         m_folderId;     /**< parent folder */
    std::string m_name;         /**< name */
    FsFile      m_main_file;
//    boost::filesystem::path  m_path;/**< path name relative to the folder */
//		std::string m_type;
    int32_t     m_orientation;  /**< Exif orientatoin */
    int32_t     m_rating;       /**< rating */
    int32_t     m_label;        /**< Label ID */
    int32_t     m_flag;         /**< Flag */
    FileType    m_file_type;    /**< File type */
};

}


#endif


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
