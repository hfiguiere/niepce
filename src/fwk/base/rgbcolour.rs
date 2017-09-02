

use std::num::ParseIntError;
use std::str::FromStr;

#[repr(C)]
#[derive(Clone,Default)]
pub struct RgbColour {
    r: u16,
    g: u16,
    b: u16
}

#[derive(Debug)]
pub enum ColourParseError {
    /// No Error.
    None,
    /// Parse Error.
    ParseError,
    /// Error parsing one of the 3 int components.
    ParseIntError,
}

impl From<ParseIntError> for ColourParseError {
    fn from(e: ParseIntError) -> ColourParseError {
        ColourParseError::ParseIntError
    }
}

impl RgbColour {

    pub fn new(r: u16, g: u16, b: u16) -> RgbColour {
        RgbColour{r: r, g: g, b: b}
    }
}

impl FromStr for RgbColour {

    type Err = ColourParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let v: Vec<&str> = s.split(' ').collect();
        if v.len() != 3 {
            return Err(ColourParseError::ParseError);
        }
        let r = u16::from_str_radix(v[0], 10)?;
        let g = u16::from_str_radix(v[1], 10)?;
        let b = u16::from_str_radix(v[2], 10)?;
        Ok(RgbColour::new(r, g, b))
    }
}
