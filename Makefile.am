include $(top_srcdir)/Makefile.tool.am

EXTRA_DIST = docs/xp-manual.xml

#----------------------------------------------------------------------------
# Headers
#----------------------------------------------------------------------------

pkginclude_HEADERS = \
        cputil.h \
        cputildefs.h

noinst_HEADERS =

#----------------------------------------------------------------------------
# cputil-<platform>
#----------------------------------------------------------------------------

noinst_PROGRAMS  = cputil-@VGCONF_ARCH_PRI@-@VGCONF_OS@
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += cputil-@VGCONF_ARCH_SEC@-@VGCONF_OS@
endif

XPROF_SOURCES_COMMON = cu_main.c

cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_SOURCES      = \
	$(XPROF_SOURCES_COMMON)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_PRI_CAPS@)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_PRI_CAPS@)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_PRI@ \
	$(LINK) \
	$(cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS) \
	$(cputil_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS)

if VGCONF_HAVE_PLATFORM_SEC
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_SOURCES      = \
	$(XPROF_SOURCES_COMMON)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_SEC_CAPS@)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_SEC_CAPS@)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_SEC@ \
	$(LINK) \
	$(cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS) \
	$(cputil_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS)
endif

#----------------------------------------------------------------------------
# vgpreload_cputil-<platform>.so
#----------------------------------------------------------------------------

#noinst_PROGRAMS += vgpreload_cputil-@VGCONF_ARCH_PRI@-@VGCONF_OS@.so
#if VGCONF_HAVE_PLATFORM_SEC
#noinst_PROGRAMS += vgpreload_cputil-@VGCONF_ARCH_SEC@-@VGCONF_OS@.so
#endif
#
#if VGCONF_OS_IS_DARWIN
#noinst_DSYMS = $(noinst_PROGRAMS)
#endif


