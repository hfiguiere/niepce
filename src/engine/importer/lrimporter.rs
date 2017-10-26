

use lrcat::{Catalog,CatalogVersion};

use super::libraryimporter::LibraryImporter;


struct LrImporter {
}

impl LibraryImport for LrImporter {

    fn new() -> LrImporter {
        LrImporter { catalog: None }
    }

    fn import_library(&mut self, path: &Path) -> bool {
        let catalog = Catalog::new(path);
        if !catalog.open() {
            return false;
        }

        catalog.load_version();
        if catalog.catalog_version != CatalogVersion::Lr4 {
            return false;
        }

        let folders = catalog.load_folders();
        
        false
    }

    /// Detect if this is a Lr catalog
    /// XXX improve it.
    fn can_import_library(path: &Path) -> bool {
        if let Some(ext) = path.extension() {
            if ext == "lrcat" {
                return true;
            }
        }
        false
    }
}
