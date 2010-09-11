CC := $(CROSS_COMPILE)gcc

CFLAGS := -O2 -ggdb -Wall -Wextra -Wno-unused-parameter -ansi -std=c99
LDFLAGS := -Wl,--no-undefined

override CFLAGS += -DGST_DISABLE_DEPRECATED

GST_CFLAGS := $(shell pkg-config --cflags gstreamer-0.10 gstreamer-tag-0.10 gstreamer-riff-0.10 gstreamer-base-0.10)
GST_LIBS := $(shell pkg-config --libs gstreamer-0.10 gstreamer-tag-0.10 gstreamer-riff-0.10 gstreamer-base-0.10)

OGG_CFLAGS := $(shell pkg-config --cflags ogg)
OGG_LIBS := $(shell pkg-config --libs ogg)

all:

version := $(shell ./get-version)
prefix := /usr

D = $(DESTDIR)

# plugin

gst_plugin := libgstxiph.so

$(gst_plugin): plugin.o gstoggdemux.o gstoggstream.o vorbis_parse.o dirac_parse.o \
	gstflacparse.o gstbaseparse.o
$(gst_plugin): override CFLAGS += $(GST_CFLAGS) $(OGG_CFLAGS) -fPIC -DHAVE_CONFIG_H
$(gst_plugin): override LIBS += $(GST_LIBS) $(OGG_LIBS)

targets += $(gst_plugin)

all: $(targets)

# pretty print
ifndef V
QUIET_CC    = @echo '   CC         '$@;
QUIET_LINK  = @echo '   LINK       '$@;
QUIET_CLEAN = @echo '   CLEAN      '$@;
endif

install: $(targets)
	install -m 755 -D libgstxiph.so $(D)$(prefix)/lib/gstreamer-0.10/libgstxiph.so

copy:
	cp $(srcdir)/gst-plugins-base/ext/ogg/{gstoggdemux.[ch],dirac_parse.[ch],vorbis_parse.[ch]} .
	cp $(srcdir)/gst-plugins-base/ext/ogg/gstoggstream.[ch] .
	cp $(srcdir)/gst-plugins-bad/gst/audioparsers/{gstflacparse.[ch],gstbaseparse.[ch]} .

%.o:: %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -MMD -o $@ -c $<

%.so::
	$(QUIET_LINK)$(CC) $(LDFLAGS) -shared -o $@ $^ $(LIBS)

clean:
	$(QUIET_CLEAN)$(RM) -v $(targets) *.o *.d

-include *.d
