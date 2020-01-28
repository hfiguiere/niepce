extern crate cbindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    if env::var("SKIP_CBINDINGS").is_err() {
        // Use cbindgen to generate C bindings.
        let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
        let target_dir = env::var("CARGO_TARGET_DIR").unwrap_or(String::from("./target"));
        let mut target_file = PathBuf::from(target_dir);
        target_file.push("bindings.h");
        cbindgen::Builder::new()
            .with_include_guard("niepce_rust_bindings_h")
            .with_namespace("ffi")
            .with_language(cbindgen::Language::Cxx)
            .with_parse_deps(true)
            .with_parse_exclude(&["exempi", "chrono", "multimap", "npc_engine"])
            .include_item("ColIndex")
            .exclude_item("GdkPixbuf")
            .exclude_item("GtkWindow")
            .exclude_item("GtkToolbar")
            .exclude_item("GtkTreeIter")
            .exclude_item("GtkIconView")
            .exclude_item("GtkListStore")
            .exclude_item("GtkTreePath")
            .exclude_item("GtkCellRenderer")
            .exclude_item("GtkWidget")
            .exclude_item("GFileInfo")
            .exclude_item("RgbColour")
            .with_crate(&crate_dir)
            .generate()
            .expect("Couldn't generate bindings")
            .write_to_file(&target_file);
    }
}
