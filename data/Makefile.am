SUBDIRS = icons themes

desktop_in_files = niepce.desktop.in
desktop_files    = $(desktop_in_files:.desktop.in=.desktop)

%.desktop: %.desktop.in
	$(AM_V_GEN)$(MSGFMT) --desktop --template $< -d $(top_srcdir)/po -o $@

desktopdir   = $(datadir)/applications
desktop_DATA = $(desktop_files)

appdatadir = $(datarootdir)/appdata
appdata_in_files = niepce.appdata.xml.in
appdata_files = $(appdata_in_files:.xml.in=.xml)
appdata_DATA  = $(appdata_files)

%.appdata.xml: %.appdata.xml.in
	$(AM_V_GEN)$(MSGFMT) --xml --template $< -d $(top_srcdir)/po -o $@

EXTRA_DIST = \
        $(desktop_in_files) \
	$(appdata_in_files) \
	$(null)


CLEANFILES = \
	$(desktop_files)                        \
	$(appdata_DATA)	                        \
	$(null)

DISTCLEANFILES =                                \
	$(null)

check:
	desktop-file-validate $(desktop_files)
