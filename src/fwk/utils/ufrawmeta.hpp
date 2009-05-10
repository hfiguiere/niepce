/*
 * niepce - fwk/utils/ufrawmeta.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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

/** @brief conversion routine for ufraw meta data (id) */

#include <exempi/xmp.h>
#include <libxml/xmlreader.h>

namespace fwk {

	class UfrawMeta
	{
	public:
		UfrawMeta(const std::string & file);

		/** convert the .ufraw file to a XMP meta. Scrap the Exif
		 * @param file the name of the ufraw filename.
		 * @return true if it loaded it.
		 */
		bool ufraw_to_xmp(XmpPtr xmp);

	private:
		static int get_tag_id(const xmlChar *name);
		bool ufraw_process_xml_node(xmlTextReaderPtr reader, XmpPtr xmp);
		
    std::string m_id_file; /**< the ufraw ID file */

		std::string m_input;
		std::string m_output;
		std::string m_outputpath;
		bool        m_hasSettings;
		int         m_startDepth;
	};


}
