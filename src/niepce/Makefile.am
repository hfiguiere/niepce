
SUBDIRS = ui modules

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src/niepce -I$(top_srcdir)/src  \
	@FRAMEWORK_CFLAGS@ \
	-DNIEPCE_LOCALEDIR=\"@NIEPCE_LOCALEDIR@\"

bin_PROGRAMS = niepce

niepce_LDFLAGS =
niepce_LDADD = \
	$(top_builddir)/src/niepce/ui/libniepceui.a \
	$(top_builddir)/src/libraryclient/liblibraryclient.a \
	$(top_builddir)/src/engine/libniepceengine.a \
	$(top_builddir)/src/niepce/modules/darkroom/libmoduledarkroom.a \
	$(top_builddir)/src/niepce/modules/map/libmodulemap.a \
	$(top_builddir)/src/niepce/modules/interfaces/libinterfaces.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	$(top_builddir)/src/ncr/libncr.a \
	$(top_builddir)/src/ext/libview/libview.a \
	$(top_builddir)/target/debug/libniepce_rust.a \
	@FRAMEWORK_LIBS@ \
	@GPHOTO_LIBS@ \
	@BABL_LIBS@ \
	@GEGL_LIBS@ @OPENRAW_LIBS@ \
	-ldl \
	$(NULL)


niepce_SOURCES = \
	notificationcenter.hpp notificationcenter.cpp \
        notifications.hpp \
	main.cpp

RUST_SOURCES = \
	@top_srcdir@/src/lib.rs \
	@top_srcdir@/src/engine/db/keyword.rs \
	@top_srcdir@/src/engine/db/fsfile.rs \
	@top_srcdir@/src/engine/db/libfile.rs \
	@top_srcdir@/src/engine/db/filebundle.rs \
	@top_srcdir@/src/engine/db/mod.rs \
	@top_srcdir@/src/engine/db/libfolder.rs \
	@top_srcdir@/src/engine/db/library.rs \
	@top_srcdir@/src/engine/mod.rs \
	@top_srcdir@/src/fwk/utils/exempi.rs \
	@top_srcdir@/src/fwk/utils/mod.rs \
	@top_srcdir@/src/fwk/base/fractions.rs \
	@top_srcdir@/src/fwk/base/mod.rs \
	@top_srcdir@/src/fwk/mod.rs \
	@top_srcdir@/src/fwk/toolkit/mimetype.rs \
	@top_srcdir@/src/fwk/toolkit/mod.rs \
	@top_srcdir@/build.rs \
	@top_srcdir@/Cargo.toml \
	$(NULL)

EXTRA_DIST = \
	$(RUST_SOURCES) \
	$(NULL)

@top_builddir@/target/@CARGO_TARGET_SUBDIR@/libniepce_rust.a: $(RUST_SOURCES)
	(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo build $(CARGO_RELEASE_ARGS))

if DEBUG
CARGO_RELEASE_ARGS=
else
CARGO_RELEASE_ARGS=--release
endif

clean-local:
	-(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo clean $(CARGO_RELEASE_ARGS))

check-local:
	(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo test $(CARGO_RELEASE_ARGS))
