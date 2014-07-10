/*
 * niepce - eng/db/proeprties-def.hpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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

#ifndef DEFINE_PROPERTY
#error DEFINE_PROPERTY must be defined
#endif

// format is 
// - const
// - the value of the id
// - XMP NS - NULL if none
// - XMP property - NULL if none
// - type

DEFINE_PROPERTY(NpXmpRatingProp, MAKE_METADATA_IDX(META_NS_XMPCORE, META_XMPCORE_RATING), NS_XAP, "Rating", int32_t )
DEFINE_PROPERTY(NpXmpLabelProp,  MAKE_METADATA_IDX(META_NS_XMPCORE, META_XMPCORE_LABEL), NS_XAP, "Label", int32_t )

DEFINE_PROPERTY(NpTiffOrientationProp, MAKE_METADATA_IDX(META_NS_TIFF, META_TIFF_ORIENTATION), NS_TIFF, "Orientation", int32_t )
DEFINE_PROPERTY(NpTiffMakeProp, MAKE_METADATA_IDX(META_NS_TIFF, META_TIFF_MAKE), NS_TIFF, "Make", std::string )
DEFINE_PROPERTY(NpTiffModelProp, MAKE_METADATA_IDX(META_NS_TIFF, META_TIFF_MODEL), NS_TIFF, "Model", std::string )

DEFINE_PROPERTY(NpExifAuxLensProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_AUX_LENS), NS_EXIF_AUX, "Lens", std::string )
DEFINE_PROPERTY(NpExifExposureProgramProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_EXPOSUREPROGRAM), NS_EXIF, "ExposureProgram", int32_t)
DEFINE_PROPERTY(NpExifExposureTimeProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_EXPOSURETIME), NS_EXIF, "ExposureTime", int32_t)
DEFINE_PROPERTY(NpExifFNumberPropProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_FNUMBER), NS_EXIF, "FNumber", int32_t)
DEFINE_PROPERTY(NpExifIsoSpeedRatingsProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_ISOSPEEDRATINGS), NS_EXIF, "ISOSpeedRatings", int32_t)
DEFINE_PROPERTY(NpExifExposureBiasProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_EXPOSUREBIAS), NS_EXIF, "ExposureBiasValue", int32_t)
DEFINE_PROPERTY(NpExifFlashFiredProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_FLASHFIRED), NS_EXIF, "Flash/exif:Fired", int32_t)
DEFINE_PROPERTY(NpExifAuxFlashCompensationProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_AUX_FLASHCOMPENSATION), NS_EXIF_AUX, "FlashCompensation", int32_t)
DEFINE_PROPERTY(NpExifWbProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_WB), NS_EXIF, "WhiteBalance", int32_t)
DEFINE_PROPERTY(NpExifDateTimeOriginalProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_DATETIMEORIGINAL), NS_EXIF, "DateTimeOriginal", fwk::Date)
DEFINE_PROPERTY(NpExifFocalLengthProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_FOCALLENGTH), NS_EXIF, "FocalLength", int32_t)
DEFINE_PROPERTY(NpExifGpsLongProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_GPSLONGITUDE), NS_EXIF, "GPSLongitude", std::string)
DEFINE_PROPERTY(NpExifGpsLatProp, MAKE_METADATA_IDX(META_NS_EXIF, META_EXIF_GPSLATITUDE), NS_EXIF, "GPSLatitude", std::string)

DEFINE_PROPERTY(NpIptcHeadlineProp, MAKE_METADATA_IDX(META_NS_IPTC, META_IPTC_HEADLINE), NS_PHOTOSHOP, "Headline", std::string)
DEFINE_PROPERTY(NpIptcDescriptionProp, MAKE_METADATA_IDX(META_NS_IPTC, META_IPTC_DESCRIPTION), NS_DC, "description", std::string)
DEFINE_PROPERTY(NpIptcKeywordsProp, MAKE_METADATA_IDX(META_NS_IPTC, META_IPTC_KEYWORDS), NS_DC, "subject", fwk::StringArray)

DEFINE_PROPERTY(NpNiepceFlagProp, MAKE_METADATA_IDX(META_NS_NIEPCE, META_NIEPCE_FLAG), xmp::NIEPCE_XMP_NAMESPACE, "Flag", int32_t)
