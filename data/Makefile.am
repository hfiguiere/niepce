SUBDIRS = icons themes

desktop_in_files = niepce.desktop.in
desktop_files    = $(desktop_in_files:.desktop.in=.desktop)

desktopdir   = $(datadir)/applications
desktop_DATA = $(desktop_files)

appdatadir = $(datarootdir)/appdata
appdata_in_files = niepce.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

EXTRA_DIST = \
        $(desktop_in_files) \
	$(appdata_in_files) \
	$(null)


CLEANFILES = \
	$(appdata_DATA)
	$(null)

DISTCLEANFILES =                                \
        $(desktop_files)                        \
	$(null)
