use lazy_static::lazy_static;
use maplit::hashmap;
use npc_fwk::utils::exempi::NS_AUX as NS_EXIF_AUX;
use npc_fwk::utils::exempi::{NS_DC, NS_EXIF, NS_PHOTOSHOP, NS_TIFF, NS_XAP};
mod xmp {
    pub use npc_fwk::utils::exempi::NIEPCE_XMP_NAMESPACE;
}

pub type NiepceProperties = u32;

#[allow(non_upper_case_globals)]
#[allow(unused_parens)]
pub mod np {
    pub const NpFileNameProp: u32 = (0x0f00 << 16 | 0);
    pub const NpFileTypeProp: u32 = (0x0f00 << 16 | 1);
    pub const NpFileSizeProp: u32 = (0x0f00 << 16 | 2);
    pub const NpFolderProp: u32 = (0x0f00 << 16 | 3);
    pub const NpSidecarsProp: u32 = (0x0f00 << 16 | 4);
    pub const NpXmpRatingProp: u32 = (0 << 16 | 1);
    pub const NpXmpLabelProp: u32 = (0 << 16 | 2);
    pub const NpTiffOrientationProp: u32 = (1 << 16 | 0);
    pub const NpTiffMakeProp: u32 = (1 << 16 | 1);
    pub const NpTiffModelProp: u32 = (1 << 16 | 2);
    pub const NpExifAuxLensProp: u32 = (2 << 16 | 0);
    pub const NpExifExposureProgramProp: u32 = (2 << 16 | 1);
    pub const NpExifExposureTimeProp: u32 = (2 << 16 | 2);
    pub const NpExifFNumberPropProp: u32 = (2 << 16 | 3);
    pub const NpExifIsoSpeedRatingsProp: u32 = (2 << 16 | 4);
    pub const NpExifExposureBiasProp: u32 = (2 << 16 | 5);
    pub const NpExifFlashFiredProp: u32 = (2 << 16 | 6);
    pub const NpExifAuxFlashCompensationProp: u32 = (2 << 16 | 7);
    pub const NpExifWbProp: u32 = (2 << 16 | 8);
    pub const NpExifDateTimeOriginalProp: u32 = (2 << 16 | 9);
    pub const NpExifFocalLengthProp: u32 = (2 << 16 | 10);
    pub const NpExifGpsLongProp: u32 = (2 << 16 | 11);
    pub const NpExifGpsLatProp: u32 = (2 << 16 | 12);
    pub const NpIptcHeadlineProp: u32 = (3 << 16 | 0);
    pub const NpIptcDescriptionProp: u32 = (3 << 16 | 1);
    pub const NpIptcKeywordsProp: u32 = (3 << 16 | 2);
    pub const NpNiepceFlagProp: u32 = (4 << 16 | 0);
}
lazy_static! {
    pub static ref PROP_TO_XMP_MAP: std::collections::HashMap<NiepceProperties, (&'static str, &'static str)> = hashmap! {
    np::NpXmpRatingProp => (NS_XAP, "Rating"),
    np::NpXmpLabelProp => (NS_XAP, "Label"),
    np::NpTiffOrientationProp => (NS_TIFF, "Orientation"),
    np::NpTiffMakeProp => (NS_TIFF, "Make"),
    np::NpTiffModelProp => (NS_TIFF, "Model"),
    np::NpExifAuxLensProp => (NS_EXIF_AUX, "Lens"),
    np::NpExifExposureProgramProp => (NS_EXIF, "ExposureProgram"),
    np::NpExifExposureTimeProp => (NS_EXIF, "ExposureTime"),
    np::NpExifFNumberPropProp => (NS_EXIF, "FNumber"),
    np::NpExifIsoSpeedRatingsProp => (NS_EXIF, "ISOSpeedRatings"),
    np::NpExifExposureBiasProp => (NS_EXIF, "ExposureBiasValue"),
    np::NpExifFlashFiredProp => (NS_EXIF, "Flash/exif:Fired"),
    np::NpExifAuxFlashCompensationProp => (NS_EXIF_AUX, "FlashCompensation"),
    np::NpExifWbProp => (NS_EXIF, "WhiteBalance"),
    np::NpExifDateTimeOriginalProp => (NS_EXIF, "DateTimeOriginal"),
    np::NpExifFocalLengthProp => (NS_EXIF, "FocalLength"),
    np::NpExifGpsLongProp => (NS_EXIF, "GPSLongitude"),
    np::NpExifGpsLatProp => (NS_EXIF, "GPSLatitude"),
    np::NpIptcHeadlineProp => (NS_PHOTOSHOP, "Headline"),
    np::NpIptcDescriptionProp => (NS_DC, "description"),
    np::NpIptcKeywordsProp => (NS_DC, "subject"),
    np::NpNiepceFlagProp => (xmp::NIEPCE_XMP_NAMESPACE, "Flag"),
    };
}
