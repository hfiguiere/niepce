ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = COPYING AUTHORS README

INTLTOOL_FILES = intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache

if CW_BUILD
CW_DIR = camerawire
endif
if MG_BUILD
MG_DIR = magellan
endif

SUBDIRS = po data src $(CW_DIR) $(MG_DIR)
DIST_SUBDIRS = po art data doc src camerawire magellan
