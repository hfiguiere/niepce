/*
 * niepce - eng/db/metadata.hpp
 *
 * Copyright (C) 2008,2011 Hubert Figuiere
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



#ifndef _DB_METADATA_H_
#define _DB_METADATA_H_

namespace eng {

enum {
    META_NS_XMPCORE = 0,
    META_NS_TIFF,
    META_NS_EXIF,
    META_NS_IPTC,
    META_NS_NIEPCE
};

/** Metadata for xmpcore. Combine with %META_NS_XMPCORE */
enum {
    _META_XMPCORE_NONE_ = 0,
    META_XMPCORE_RATING = 1,  // NS_XAP, "Rating"
    META_XMPCORE_LABEL,       // NS_XAP, "Label"

    _META_XMPCORE_LAST_
};

/** Metadata for tiff. Combine with %META_NS_TIFF */
enum {
    META_TIFF_ORIENTATION = 0,
    META_TIFF_MAKE,                // NS_TIFF, "Make"
    META_TIFF_MODEL,               // NS_TIFF, "Model"

    _META_TIFF_LAST_
};

enum {
    META_EXIF_AUX_LENS = 0,            // NS_EXIF_AUX, "Lens"
    META_EXIF_EXPOSUREPROGRAM,         // NS_EXIF, "ExposureProgram"
    META_EXIF_EXPOSURETIME,            // NS_EXIF, "ExposureTime"
    META_EXIF_FNUMBER,                 // NS_EXIF, "FNumber"
    META_EXIF_ISOSPEEDRATINGS,         // NS_EXIF, "ISOSpeedRatings"
    META_EXIF_EXPOSUREBIAS,            // NS_EXIF, "ExposureBiasValue"
    META_EXIF_FLASHFIRED,              // NS_EXIF, "Flash/exif:Fired"
    META_EXIF_AUX_FLASHCOMPENSATION,   // NS_EXIF_AUX, "FlashCompensation"
    META_EXIF_WB,                      // NS_EXIF, "WhiteBalance"
    META_EXIF_DATETIMEORIGINAL,        // NS_EXIF, "DateTimeOriginal"
    META_EXIF_FOCALLENGTH,             // NS_EXIF, "FocalLength"
    META_EXIF_GPSLONGITUDE,            // NS_EXIF, "GPSLongitude"
    META_EXIF_GPSLATITUDE,             // NS_EXIF, "GPSLatitude"
    
    _META_EXIF_LAST_
};

enum {
    META_IPTC_HEADLINE = 0,    // NS_PHOTOSHOP, "Headline"
    META_IPTC_DESCRIPTION = 1, // NS_DC, "description"
    META_IPTC_KEYWORDS = 2,    // NS_DC, "subject"
    
    _META_IPTC_LAST_
};

/** Metadata for Niepce. Combine with %META_NS_NIEPCE */
enum {
    META_NIEPCE_FLAG = 0
};

/** make a metadata index with a namespace and a value */
#define MAKE_METADATA_IDX(x,y) (x<<16|y)
/** get the NS form the metadata index */
#define METADATA_NS(x) (x >>16)
/** get the data from the metadata index */
#define METADATA_DATA(x) (x & 0xffff)

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
