#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>

#include "gstoggdemux.h"
#include "gstflacparse.h"

static gboolean
plugin_init(GstPlugin *plugin)
{
	gst_element_register(plugin, "flacparse", GST_RANK_PRIMARY + 1, GST_TYPE_FLAC_PARSE);
	gst_ogg_demux_plugin_init(plugin);

	return TRUE;
}

GstPluginDesc gst_plugin_desc = {
	.major_version = 0,
	.minor_version = 10,
	.name = "xiph",
	.description = (gchar *) "ogg/flac demuxing/parsing",
	.plugin_init = plugin_init,
	.version = VERSION,
	.license = "LGPL",
	.source = "none",
	.package = "none",
	.origin = "none",
};
