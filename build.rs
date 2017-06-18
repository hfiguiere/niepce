extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let builder = bindgen::Builder::default()
        // Do not generate unstable Rust code that
        // requires a nightly rustc and enabling
        // unstable features.
        .no_unstable_rust()
        .enable_cxx_namespaces()
        .generate_comments(false)
        // The input header we would like to generate
        // bindings for.
        .whitelisted_type("eng::NiepceProperties")
        .header("src/engine/db/bindings.hpp")
        .clang_arg("--std=c++11")
        .clang_arg("-DRUST_BINDGEN=1")
        .clang_arg("-I./src");


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
