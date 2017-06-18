ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = COPYING AUTHORS README \
	Cargo.lock \
	$(NULL)

if CW_BUILD
CW_DIR = camerawire
endif

SUBDIRS =  data src $(CW_DIR) po
DIST_SUBDIRS = po art data doc src camerawire

