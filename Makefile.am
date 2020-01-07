ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = COPYING AUTHORS README \
	Cargo.lock \
	$(NULL)

SUBDIRS =  data third_party src po
DIST_SUBDIRS = po art data doc src

clippy:
	make -C src clippy
