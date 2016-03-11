#ifndef _RTSP_STREAMER_PLUGIN_H
#define _RTSP_STREAMER_PLUGIN_H

#include "StreamerPlugin.h"

class RTSPStreamer : public Streamer {
public:
								RTSPStreamer();
	virtual						~RTSPStreamer();

			status_t			Sniff(BUrl* url, BMediaIO** source);
};


class RTSPStreamerPlugin : public StreamerPlugin {
public:
	virtual	Streamer*			NewStreamer();
};

#endif // _RTSP_STREAMER_PLUGIN_H
