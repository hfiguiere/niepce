
AM_CPPFLAGS = -DNIEPCE_LOCALEDIR=\"${NIEPCE_LOCALEDIR}\"

SUBDIRS = . ext fwk \
	engine libraryclient ncr niepce

RUST_SOURCES = \
	@top_srcdir@/build.rs \
	@top_srcdir@/Cargo.toml \
	@top_srcdir@/src/capi.rs \
	@top_srcdir@/src/engine/db/filebundle.rs \
	@top_srcdir@/src/engine/db/fsfile.rs \
	@top_srcdir@/src/engine/db/keyword.rs \
	@top_srcdir@/src/engine/db/label.rs \
	@top_srcdir@/src/engine/db/libfile.rs \
	@top_srcdir@/src/engine/db/libfolder.rs \
	@top_srcdir@/src/engine/db/libmetadata.rs \
	@top_srcdir@/src/engine/db/library.rs \
	@top_srcdir@/src/engine/db/mod.rs \
	@top_srcdir@/src/engine/library/commands.rs \
	@top_srcdir@/src/engine/library/mod.rs \
	@top_srcdir@/src/engine/library/notification.rs \
	@top_srcdir@/src/engine/library/op.rs \
	@top_srcdir@/src/engine/mod.rs \
	@top_srcdir@/src/fwk/base/date.rs \
	@top_srcdir@/src/fwk/base/debug.rs \
	@top_srcdir@/src/fwk/base/fractions.rs \
	@top_srcdir@/src/fwk/base/mod.rs \
	@top_srcdir@/src/fwk/base/propertybag.rs \
	@top_srcdir@/src/fwk/base/propertyvalue.rs \
	@top_srcdir@/src/fwk/base/rgbcolour.rs \
	@top_srcdir@/src/fwk/mod.rs \
	@top_srcdir@/src/fwk/toolkit/mimetype.rs \
	@top_srcdir@/src/fwk/toolkit/mod.rs \
	@top_srcdir@/src/fwk/utils/exempi.rs \
	@top_srcdir@/src/fwk/utils/mod.rs \
	@top_srcdir@/src/lib.rs \
	@top_srcdir@/src/libraryclient/clientimpl.rs \
	@top_srcdir@/src/libraryclient/clientinterface.rs \
	@top_srcdir@/src/libraryclient/libraryclient.rs \
	@top_srcdir@/src/libraryclient/mod.rs \
	@top_srcdir@/src/niepce/mod.rs \
	@top_srcdir@/src/niepce/ui/mod.rs \
	@top_srcdir@/src/niepce/ui/dialogs/mod.rs \
	@top_srcdir@/src/niepce/ui/dialogs/confirm.rs \
	@top_srcdir@/src/niepce/ui/dialogs/requestnewfolder.rs \
	$(NULL)

EXTRA_DIST = \
	$(RUST_SOURCES) \
	$(NULL)

CARGO_VERBOSE = $(cargo_verbose_$(V))
cargo_verbose_ = $(cargo_verbose_$(AM_DEFAULT_VERBOSITY))
cargo_verbose_0 =
cargo_verbose_1 = --verbose

if DEBUG
CARGO_RELEASE_ARGS=
else
CARGO_RELEASE_ARGS=--release
endif

CARGO_TARGET_DIR=@abs_top_builddir@/target

all-local: $(CARGO_TARGET_DIR)/@CARGO_TARGET_SUBDIR@/libniepce_rust.a

@abs_top_builddir@/target/bindings.h: $(CARGO_TARGET_DIR)/@CARGO_TARGET_SUBDIR@/libniepce_rust.a

@abs_top_builddir@/target/@CARGO_TARGET_SUBDIR@/libniepce_rust.a: $(RUST_SOURCES)
	cd $(top_srcdir) && \
	CARGO_TARGET_DIR=$(CARGO_TARGET_DIR) cargo build $(CARGO_VERBOSE) $(CARGO_RELEASE_ARGS)

clean-local:
	cd $(top_srcdir) && \
	CARGO_TARGET_DIR=$(CARGO_TARGET_DIR) cargo clean $(CARGO_VERBOSE) $(CARGO_RELEASE_ARGS)

check-local:
	cd $(top_srcdir) && \
	CARGO_TARGET_DIR=$(CARGO_TARGET_DIR) cargo test $(CARGO_VERBOSE) $(CARGO_RELEASE_ARGS)
