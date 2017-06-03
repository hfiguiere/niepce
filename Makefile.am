ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = COPYING AUTHORS README \
	Cargo.toml \
	Cargo.lock \
	$(NULL)

if CW_BUILD
CW_DIR = camerawire
endif

SUBDIRS =  data src $(CW_DIR) po
DIST_SUBDIRS = po art data doc src camerawire

all-local:
	(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo build --release)

clean-local:
	-(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo clean)

check:
	(cd $(top_srcdir) ; CARGO_TARGET_DIR=@abs_top_builddir@/target cargo test --release)
