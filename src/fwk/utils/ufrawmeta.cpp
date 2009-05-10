/*
 * niepce - fwk/utils/ufrawmeta.cpp
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

#include <map>
#include <vector>
#include <utility>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

#include <exempi/xmpconsts.h>

#include "fwk/base/debug.hpp"
#include "ufrawmeta.hpp"
#include "exempi.hpp"
#include "pathutils.hpp"


namespace fwk {

	static const struct string_pair_t {
		const char *first;
		const char *second;
	} s_wb_ufraw_to_xmp[] = {
		{ "Camera WB", "As Shot" },
		{ "camera", "As Shot" },
		{ "Auto WB", "Auto" },
		{ "auto", "Auto" },
		{ "Direct sunlight", "Daylight" },
		{ "Cloudy", "Cloudy" },
		{ "Shade", "Shade" },
		{ "Incandescent", "Tungsten" },
		{ "Fluorescent", "Fluorescent" },
		{ "Flash", "Flash" },
		{ "Manual WB", "Custom" },
		{ 0, 0 }
	};


	typedef std::map<std::string, int> tag_map_t;

	enum {
		UFRAW_TAG_UFRaw = 1,
		UFRAW_TAG_InputFilename,
		UFRAW_TAG_OutputFilename,
		UFRAW_TAG_OutputPath,
		UFRAW_TAG_WB,
		UFRAW_TAG_Temperature,
		UFRAW_TAG_Exposure,
		UFRAW_TAG_Crop,
		UFRAW_TAG_Green,
		UFRAW_TAG_ChannelMultipliers
	};

#define TAG(x) { #x, UFRAW_TAG_##x}

	static const struct tag_pair_t {
		const char * name;
		int          id;
	} s_elements[] = {
		TAG(UFRaw),
		TAG(InputFilename),
		TAG(OutputFilename),
		TAG(OutputPath),
		TAG(WB),
		TAG(Temperature),
		TAG(Exposure),
		TAG(Crop),
		TAG(Green),
		TAG(ChannelMultipliers),
		{ 0, 0 }
	};


	UfrawMeta::UfrawMeta(const std::string & file)
		: m_id_file(file),
		  m_hasSettings(false),
		  m_startDepth(-1)
	{
	}


	int UfrawMeta::get_tag_id(const xmlChar *name)
	{
		static tag_map_t s_tag_map;
		// TODO lock for concurrent access
		if(s_tag_map.empty()) {
			const tag_pair_t* ptag = s_elements;
			DBG_ASSERT(ptag != NULL, "p can't be NULL");
			while(ptag->id != 0) {
				s_tag_map.insert(std::make_pair(ptag->name, ptag->id));
				ptag++;
			}
		}

		tag_map_t::const_iterator iter = s_tag_map.find((const char*)name);
		if(iter == s_tag_map.end()) {
			return -1;
		}
		return iter->second;
	}


	static const char * convertWB(const std::string & wb)
	{
		static std::map<std::string, const char *> wb_map;
		if(wb_map.empty()) {
			const string_pair_t *ppair =  s_wb_ufraw_to_xmp;
			while(ppair->first != 0) {
				wb_map.insert(std::make_pair(ppair->first, ppair->second));
				ppair++;				
			}
		}
		std::map<std::string, const char *>::iterator iter = wb_map.find(wb);
		if(iter == wb_map.end()) {
			return NULL;
		}
		return iter->second;
	}



	static std::string getPropContent(xmlTextReaderPtr reader)
	{
		std::string retval;
		if(xmlTextReaderRead(reader) == 1) {
			const xmlChar * content = xmlTextReaderConstValue(reader);
			if(content) {
				retval = (const char*)content;
			}
			else {
				DBG_OUT("content is NULL");
			}
		}
		return retval;
	}

	

	/** @return true if any data has been found. false means an error
	 * (unknown content)
	 */
	bool UfrawMeta::ufraw_process_xml_node(xmlTextReaderPtr reader, XmpPtr xmp)
	{
		bool ret = true;
		int node_type = xmlTextReaderNodeType(reader);
		int depth = xmlTextReaderDepth(reader);
		if(node_type == XML_READER_TYPE_ELEMENT) {
			const xmlChar *name = xmlTextReaderConstName(reader);
			int tag = get_tag_id(name);
			switch(tag) {
			case UFRAW_TAG_UFRaw:
			{
				if(m_startDepth == -1)
				{
					m_startDepth = depth;
				}
				xmlChar * attrib = xmlTextReaderGetAttribute(reader, 
															 (const xmlChar*)"Version");
				if(attrib != NULL) {
					// other version are 3 or 5. deal with them
					if(strcmp("7", (const char*)attrib) == 0) {
						
					}
					else {
						DBG_OUT("unknown version %s", attrib);
						ret = false;
					}
					xmlFree(attrib);						
				}
				break;
			}
			case UFRAW_TAG_InputFilename:
			case UFRAW_TAG_OutputFilename:
			case UFRAW_TAG_OutputPath:
				if(depth == m_startDepth + 1) {
					switch(tag) {
					case UFRAW_TAG_InputFilename:
						m_input = getPropContent(reader);
						break;
					case UFRAW_TAG_OutputFilename:
						m_output = getPropContent(reader);
						break;
					case UFRAW_TAG_OutputPath:
						m_outputpath = getPropContent(reader);
						break;
					}
				}
				break;
			case UFRAW_TAG_WB:
			{
				if(depth == m_startDepth + 1) {
					std::string wb = getPropContent(reader);
					const char *xmp_wb = convertWB(wb);
					if(xmp_wb != NULL) {
						xmp_set_property(xmp, NS_CAMERA_RAW_SETTINGS,
										 "WhiteBalance", xmp_wb, 0);
						m_hasSettings = true;
					}
					else {
						DBG_OUT("unable to convert WB %s", wb.c_str());
					}
				}
				break;
			}
			case UFRAW_TAG_Temperature:
			{
				if(depth == m_startDepth + 1) {
					std::string temp = getPropContent(reader);
					try {
						int32_t t = boost::lexical_cast<int32_t>(temp);
						xmp_set_property_int32(xmp, NS_CAMERA_RAW_SETTINGS,
											   "Temperature", t, 0);
						m_hasSettings = true;
					} 
					catch(const boost::bad_lexical_cast &)
					{
						ERR_OUT("exception");
					}
				}
				break;
			}
			case UFRAW_TAG_Exposure:
			{
				if(depth == m_startDepth + 1) {
					std::string exp = getPropContent(reader);
					try {
						double exposure = boost::lexical_cast<double>(exp);
						xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
											   "Exposure", exposure, 0);
						m_hasSettings = true;
					} 
					catch(const boost::bad_lexical_cast &)
					{
						ERR_OUT("exception");					
					}
				}
				break;
			}
			case UFRAW_TAG_Crop:
			{ 
				if(depth == m_startDepth + 1) {
					std::string crop = getPropContent(reader);
				// order is left top right bottom
					
					std::vector< std::string > v;
					v.reserve(4);
					boost::split(v, crop, boost::is_any_of(" "));
					if(v.size() == 4) {
						int left, top, bottom, right;
						try {
							std::vector< std::string >::iterator iter = v.begin();
							left = boost::lexical_cast<int>(*iter);
							iter++;
							top = boost::lexical_cast<int>(*iter);
							iter++;
							right = boost::lexical_cast<int>(*iter);
							iter++;
							bottom = boost::lexical_cast<int>(*iter);
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropLeft", left, 0);
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropTop", top, 0);
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropBottom", bottom, 0);
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropRight", right, 0);
							xmp_set_property_bool(xmp, NS_CAMERA_RAW_SETTINGS,
												  "HasCrop", true, 0);
							// units are pixels
							xmp_set_property_int32(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropUnits", 0, 0);
							int width = right - left;
							int height = bottom - top;
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropWidth", width, 0);
							xmp_set_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
												   "CropHeight", height, 0);
						}
						catch(const boost::bad_lexical_cast & e)
						{
							DBG_OUT("exception");
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}
		return ret;
	}


	bool UfrawMeta::ufraw_to_xmp(XmpPtr xmp)
	{
		bool has_data = false;
		if(fwk::path_exists(m_id_file)) {
			xmlTextReaderPtr reader;
			reader = xmlNewTextReaderFilename(m_id_file.c_str());
			if(reader != NULL) {
				int ret = xmlTextReaderRead(reader);
				while(ret == 1) {
					has_data = ufraw_process_xml_node(reader, xmp);
					if(!has_data) {
						break;
					}
					ret = xmlTextReaderRead(reader);
				}
				xmlFreeTextReader(reader);
			}
			else {
				DBG_OUT("xml reader is NULL");
			}
		}
		else {
			DBG_OUT("file not found %s", m_id_file.c_str());
		}
		if(has_data) {
			xmp_set_property_bool(xmp, xmp::UFRAW_INTEROP_NAMESPACE, 
								  "ImportedFromUFraw", true, 0);
		}
		return has_data;
	}

}
