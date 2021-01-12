AM_CPPFLAGS =								\
	-I$(top_builddir)						\
	-I$(top_srcdir)

sbin_PROGRAMS =								\
	expidus1-kiosk-query

expidus1_kiosk_query_SOURCES =						\
	main.c

expidus1_kiosk_query_CFLAGS =						\
	$(GLIB_CFLAGS)

expidus1_kiosk_query_DEPENDENCIES =					\
	$(top_builddir)/libexpidus1util/libexpidus1util.la

expidus1_kiosk_query_LDFLAGS =						\
	$(top_builddir)/libexpidus1util/libexpidus1util.la
