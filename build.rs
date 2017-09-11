extern crate bindgen;
extern crate pkg_config;

use std::env;
use std::path::PathBuf;

fn main() {
    let exempi = pkg_config::Config::new()
        .print_system_libs(false)
        .probe("exempi-2.0")
        .unwrap();
    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let mut builder = bindgen::Builder::default()
        .enable_cxx_namespaces()
        .generate_comments(false)
        .generate_inline_functions(true)
        // The input header we would like to generate
        // bindings for.
        .whitelisted_type("eng::NiepceProperties")
        .whitelisted_type("eng::LibFileType")
        .whitelisted_type("eng::LibNotificationType")
        .whitelisted_type("eng::LnFileMove")
        .whitelisted_type("eng::LnFolderCount")
        .whitelisted_type("eng::QueriedContent")
        .opaque_type("eng::QueriedContent")
        .whitelisted_type("eng::metadata_desc_t")
        .whitelisted_type("eng::LibraryManaged")
        .whitelisted_type("fwk::FileList")
        // PropertyValue is complicated
        .whitelisted_type("fwk::PropertyValue")
        .opaque_type("fwk::PropertyValue")
        .whitelisted_type("fwk::StringArray")
        .opaque_type("fwk::StringArray")
        .whitelisted_function("fwk::is_empty")
        .whitelisted_function("fwk::is_integer")
        .whitelisted_function("fwk::is_string")
        .whitelisted_function("fwk::is_string_array")
        .whitelisted_function("fwk::is_date")
        .whitelisted_function("fwk::get_integer")
        .whitelisted_function("fwk::get_date")
        .whitelisted_function("fwk::get_string_cstr")
        .whitelisted_function("fwk::get_string_array")
        .whitelisted_function("fwk::string_array_len")
        .whitelisted_function("fwk::string_array_at_cstr")
        .whitelisted_type("eng::IndexToXmp")
        .whitelisted_function("eng::property_index_to_xmp")
        .header("src/engine/db/bindings.hpp")
        .clang_arg("--std=c++11")
        .clang_arg("-DRUST_BINDGEN=1")
        .clang_arg("-I./src");

    for include in exempi.include_paths.into_iter().map(|path| {
        format!("-I{}", path.to_str().unwrap_or("."))
    }) {
        builder = builder.clang_arg(include);
    }

    // Finish the builder and generate the bindings.
    let bindings = builder.generate()
    // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
