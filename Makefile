include config.mak

MEDIA_PROFILER      = media-profiler
MEDIA_PROFILER_SRCS = profiler.c
MEDIA_PROFILER_OBJS = $(MEDIA_PROFILER_SRCS:.c=.o)

APPS_CPPFLAGS = -Isrc $(CPPFLAGS)
APPS_LDFLAGS = -Lsrc -lmediaprofile $(LDFLAGS)

.SUFFIXES: .c .o

all: lib apps

.c.o:
	$(CC) -c $(CFLAGS) $(APPS_CPPFLAGS) -o $@ $<

lib:
	$(MAKE) -C src

$(MEDIA_PROFILER): $(MEDIA_PROFILER_OBJS)
	$(CC) $(MEDIA_PROFILER_OBJS) $(APPS_LDFLAGS) -o $@

apps-dep:
	$(CC) -MM $(CFLAGS) $(APPS_CPPFLAGS) $(MEDIA_PROFILER_SRCS) 1>.depend

apps: apps-dep lib
	$(MAKE) $(MEDIA_PROFILER)

clean:
	$(MAKE) -C src clean
	rm -f *.o
	rm -f $(MEDIA_PROFILER)
	rm -f .depend

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
