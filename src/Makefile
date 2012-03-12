include ../config.mak

LIBNAME = libmediaprofile
STATIC_LIBNAME = $(LIBNAME).a
SHARED_LIBNAME = $(LIBNAME).so
SHARED_LIBNAME_VERSION = $(SHARED_LIBNAME).$(VERSION)
SHARED_LIBNAME_MAJOR = $(SHARED_LIBNAME).$(shell echo $(VERSION) | cut -f1 -d.)
SHARED_LIBNAME_FLAGS = -shared -Wl,-soname,$(SHARED_LIBNAME_MAJOR)

BUILD_STATIC  = yes
BUILD_SHARED  = no
BUILD_MINGW32 = no

ifeq ($(BUILD_STATIC),yes)
  BUILD_RULES += lib_static
endif
ifeq ($(BUILD_SHARED),yes)
  BUILD_RULES += lib_shared
  ifeq ($(BUILD_DYLIB),yes)
    SHARED_LIBNAME         = $(LIBNAME).dylib
    SHARED_LIBNAME_VERSION = $(LIBNAME).$(VERSION).dylib
    SHARED_LIBNAME_MAJOR   = $(LIBNAME).$(shell echo $(VERSION) | cut -f1 -d.).dylib
    SHARED_LIBNAME_FLAGS   = -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-install_name,$(SHARED_LIBNAME_VERSION)
  else
    ifeq ($(BUILD_MINGW32),yes)
      SHARED_LIBNAME         = $(LIBNAME)-$(shell echo $(VERSION) | cut -f1 -d.).dll
      SHARED_LIBNAME_VERSION = $(SHARED_LIBNAME)
      SHARED_LIBNAME_MAJOR   = $(SHARED_LIBNAME)
      SHARED_LIBNAME_FLAGS   = -shared -Wl,--out-implib=$(LIBNAME).dll.a -Wl,--export-all-symbols -Wl,--enable-auto-import
    endif
  endif
endif

SRCS =  \
        mediaprofile.c \
        mediaprofile_internals.c \
	reader.c \
        bmp.c \
        png.c \
        jpg.c \
        tiff.c \
        id3v1.c \
        mp3.c \

OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

all: depend $(BUILD_RULES)

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

lib_static: $(STATIC_LIBNAME)

lib_shared: $(SHARED_LIBNAME)

$(STATIC_LIBNAME): $(OBJS)
	$(AR) r $(STATIC_LIBNAME) $(OBJS)
	$(RANLIB) $(STATIC_LIBNAME)

$(SHARED_LIBNAME): $(OBJS)
	$(CC) $(SHARED_LIBNAME_FLAGS) \
	  $(OBJS) $(LDFLAGS) -o $(SHARED_LIBNAME_VERSION)
        if [ $(BUILD_MINGW32) = no ]; then \
          $(LN) -sf $(SHARED_LIBNAME_VERSION) $(SHARED_LIBNAME_MAJOR); \
          $(LN) -sf $(SHARED_LIBNAME_MAJOR) $(SHARED_LIBNAME); \
        fi

clean:
	rm -f *.o *.a *.so* *.dll
	rm -f .depend

depend:
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $(SRCS) 1>.depend

.PHONY: clean depend

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
