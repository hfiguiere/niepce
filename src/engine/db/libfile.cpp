/*
 * niepce - db/libfile.cpp
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


#include "fwk/utils/debug.h"
#include "libfile.h"
#include "metadata.h"

namespace bfs = boost::filesystem;

namespace db {
	
LibFile::LibFile(int _id, int _folderId, int _fsfileid, const bfs::path & p,
                 const std::string & _name )
	: m_id(_id), m_folderId(_folderId),
	  m_name(_name), 
      m_main_file(_fsfileid, p),
	  m_orientation(0), m_rating(0), m_label(0),
      m_file_type(FILE_TYPE_UNKNOWN)
{
    
}

LibFile::~LibFile()
{
}


const Keyword::IdList & LibFile::keywords() const
{
    if(!m_hasKeywordList) {
//			storage()->fetchKeywordsForFile(m_id, m_keywordList);
    }
    return m_keywordList;
}

void LibFile::setOrientation(int32_t v)
{
    m_orientation = v;
}


void LibFile::setRating(int32_t v)
{
    m_rating = v;
}


void LibFile::setLabel(int32_t v)
{
    m_label = v;
}

void LibFile::setFileType(FileType v)
{
    m_file_type = v;
}

void LibFile::setMetaData(int meta, int32_t v)
{
    switch(meta) {
    case MAKE_METADATA_IDX(db::META_NS_XMPCORE, db::META_XMPCORE_RATING):
        setRating(v);
        break;
    case MAKE_METADATA_IDX(db::META_NS_TIFF, db::META_TIFF_ORIENTATION):
        setOrientation(v);
        break;
    default:
        // TODO deal with label as well
        ERR_OUT("unknown meta %d", meta);
        break;
    }
}

/**
 * Converts a mimetype, which is expensive to calculate, into a FileType.
 * @param mime The mimetype we want to know as a filetype
 * @return the filetype
 * @todo: add the Video, JPEG+RAW file types.
 */
LibFile::FileType LibFile::mimetype_to_filetype(framework::MimeType mime)
{
    if(mime.isDigicamRaw())
    {
        return FILE_TYPE_RAW;
    }
    else if(mime.isImage())
    {
        return FILE_TYPE_IMAGE;
    }
    else
    {
        return FILE_TYPE_UNKNOWN;
    }
}


}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
