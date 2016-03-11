
#include "RTSPStreamerPlugin.h"


RTSPStreamer::RTSPStreamer()
{
}


RTSPStreamer::~RTSPStreamer()
{
}



status_t
RTSPStreamer::Sniff(BUrl* url, BMediaIO** source)
{
	
	return B_ERROR;
}


Streamer*
RTSPStreamerPlugin::NewStreamer()
{
	return new RTSPStreamer();
}
