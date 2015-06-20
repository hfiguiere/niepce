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


#include "fwk/base/debug.hpp"
#include "libfile.hpp"
#include "properties.hpp"


namespace eng {
	
LibFile::LibFile(library_id_t _id, library_id_t _folderId, library_id_t _fsfileid, const std::string & p,
                 const std::string & _name )
	: m_id(_id), m_folderId(_folderId),
	  m_name(_name), 
    m_main_file(_fsfileid, p),
	  m_orientation(0), m_rating(0), m_label(0),
    m_flag(0),
    m_file_type(FileType::UNKNOWN)
{
    
}

LibFile::~LibFile()
{
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

void LibFile::setFlag(int32_t v)
{
    m_flag = v;
}

void LibFile::setFileType(FileType v)
{
    m_file_type = v;
}

void LibFile::setProperty(fwk::PropertyIndex idx, int32_t v)
{
    switch(idx) {
    case NpTiffOrientationProp:
        setOrientation(v);
        break;
    case NpXmpRatingProp:
        setRating(v);
        break;
    case NpXmpLabelProp:
        setLabel(v);
        break;
    case NpNiepceFlagProp:
        setFlag(v);
        break;
    default:
        ERR_OUT("set property %u not handled", idx);
        break;
    }
}

int32_t LibFile::property(fwk::PropertyIndex idx) const
{
    switch(idx) {
    case NpTiffOrientationProp:
        return orientation();
    case NpXmpRatingProp:
        return rating();
    case NpXmpLabelProp:
        return label();
    case NpNiepceFlagProp:
        return flag();
    default:
        ERR_OUT("get property %u not handled", idx);
        break;
    }
    return -1;
}

/**
 * Converts a mimetype, which is expensive to calculate, into a FileType.
 * @param mime The mimetype we want to know as a filetype
 * @return the filetype
 * @todo: add the JPEG+RAW file types.
 */
LibFile::FileType LibFile::mimetype_to_filetype(fwk::MimeType mime)
{
    if(mime.isDigicamRaw())
    {
        return FileType::RAW;
    }
    else if(mime.isImage())
    {
        return FileType::IMAGE;
    }
    else if(mime.isMovie())
    {
        return FileType::VIDEO;
    }
    else
    {
        return FileType::UNKNOWN;
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
