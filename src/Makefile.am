
AM_CPPFLAGS = -DNIEPCE_LOCALEDIR=\"${NIEPCE_LOCALEDIR}\"

SUBDIRS = . ext fwk \
	engine libraryclient ncr niepce

RUST_SOURCES = \
	@top_srcdir@/build.rs \
	@top_srcdir@/Cargo.toml \
	@top_srcdir@/Cargo.lock \
	@top_srcdir@/crates/npc-engine/Cargo.toml \
	@top_srcdir@/crates/npc-engine/build.rs \
	@top_srcdir@/crates/npc-engine/src/db/filebundle.rs \
	@top_srcdir@/crates/npc-engine/src/db/fsfile.rs \
	@top_srcdir@/crates/npc-engine/src/db/keyword.rs \
	@top_srcdir@/crates/npc-engine/src/db/label.rs \
	@top_srcdir@/crates/npc-engine/src/db/libfile.rs \
	@top_srcdir@/crates/npc-engine/src/db/libfolder.rs \
	@top_srcdir@/crates/npc-engine/src/db/libmetadata.rs \
	@top_srcdir@/crates/npc-engine/src/db/library.rs \
	@top_srcdir@/crates/npc-engine/src/db/mod.rs \
	@top_srcdir@/crates/npc-engine/src/library/commands.rs \
	@top_srcdir@/crates/npc-engine/src/library/mod.rs \
	@top_srcdir@/crates/npc-engine/src/library/notification.rs \
	@top_srcdir@/crates/npc-engine/src/library/queriedcontent.rs \
	@top_srcdir@/crates/npc-engine/src/library/op.rs \
	@top_srcdir@/crates/npc-engine/src/lib.rs \
	@top_srcdir@/crates/npc-fwk/Cargo.toml \
	@top_srcdir@/crates/npc-fwk/build.rs \
	@top_srcdir@/crates/npc-fwk/src/capi.rs \
	@top_srcdir@/crates/npc-fwk/src/base/date.rs \
	@top_srcdir@/crates/npc-fwk/src/base/debug.rs \
	@top_srcdir@/crates/npc-fwk/src/base/fractions.rs \
	@top_srcdir@/crates/npc-fwk/src/base/mod.rs \
	@top_srcdir@/crates/npc-fwk/src/base/propertybag.rs \
	@top_srcdir@/crates/npc-fwk/src/base/propertyvalue.rs \
	@top_srcdir@/crates/npc-fwk/src/base/rgbcolour.rs \
	@top_srcdir@/crates/npc-fwk/src/lib.rs \
	@top_srcdir@/crates/npc-fwk/src/toolkit/mimetype.rs \
	@top_srcdir@/crates/npc-fwk/src/toolkit/mod.rs \
	@top_srcdir@/crates/npc-fwk/src/toolkit/widgets/mod.rs \
	@top_srcdir@/crates/npc-fwk/src/toolkit/widgets/rating_label.rs \
	@top_srcdir@/crates/npc-fwk/src/utils/exempi.rs \
	@top_srcdir@/crates/npc-fwk/src/utils/exiv2.rs \
	@top_srcdir@/crates/npc-fwk/src/utils/files.rs \
	@top_srcdir@/crates/npc-fwk/src/utils/mod.rs \
	@top_srcdir@/examples/widget-test.rs \
	@top_srcdir@/src/lib.rs \
	@top_srcdir@/src/libraryclient/clientimpl.rs \
	@top_srcdir@/src/libraryclient/clientinterface.rs \
	@top_srcdir@/src/libraryclient/mod.rs \
	@top_srcdir@/src/niepce/mod.rs \
	@top_srcdir@/src/niepce/ui/mod.rs \
	@top_srcdir@/src/niepce/ui/imagetoolbar.rs \
	@top_srcdir@/src/niepce/ui/dialogs/mod.rs \
	@top_srcdir@/src/niepce/ui/dialogs/confirm.rs \
	@top_srcdir@/src/niepce/ui/dialogs/requestnewfolder.rs \
	@top_srcdir@/src/niepce/ui/thumb_nav.rs \
	@top_srcdir@/src/niepce/ui/library_cell_renderer.rs \
	$(NULL)

EXTRA_DIST = \
	$(RUST_SOURCES) \
	$(NULL)

CARGO_VERBOSE = $(cargo_verbose_$(V))
cargo_verbose_ = $(cargo_verbose_$(AM_DEFAULT_VERBOSITY))
cargo_verbose_0 =
cargo_verbose_1 = -vv

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

clippy:
	cd $(top_srcdir) && \
	CARGO_TARGET_DIR=$(CARGO_TARGET_DIR) cargo clippy $(CARGO_VERBOSE) $(CARGO_RELEASE_ARGS) -- -A clippy::missing_safety_doc
