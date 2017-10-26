

use std::path::Path;


/// Interface trait for a library importer.
trait LibraryImporter {

    /// Return a new library importer
    fn new() -> Self;

    /// import the library at path.
    /// if can_import_library returne false this should return false
    /// XXX return an actual Result<>
    fn import_library(&mut self, path: &Path) -> bool;

    /// Return true if or a given path the importer recognize the library
    fn can_import_library(path: &Path) -> bool;

}

