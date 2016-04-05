/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MediaPlayerPlugin.h"

#include "MediaPlayerAddOn.h"


MediaPlayerPlugin::MediaPlayerPlugin(media_player_plugin* handle)
	:
	fPlugin(handle)
{

}


MediaPlayerPlugin::~MediaPlayerPlugin()
{

}


void
MediaPlayerPlugin::Init()
{
}


void
MediaPlayerPlugin::Destroy()
{
}


MediaPlayerRoster*
MediaPlayerPlugin::Roster() const
{
	return fRoster;
}


MediaPlayerAddOn*
MediaPlayerPlugin::AddOn() const
{
	return fPlugin->add_on;
}


status_t
MediaPlayerPlugin::Show()
{
	return B_ERROR;
}


void
MediaPlayerPlugin::Hide()
{
}


void
MediaPlayerPlugin::ShowAbout()
{
}


void
MediaPlayerPlugin::FileChanged(const entry_ref& ref, const char* path)
{
}


void
MediaPlayerPlugin::Filter(float* buffer, int32 frameCount, int32 channelCount)
{
}


int32
MediaPlayerPlugin::GetFlags() const
{
	return fPlugin->flags;
}


BView*
MediaPlayerPlugin::GetConfigurationView() const
{
	return NULL;
}


status_t
MediaPlayerPlugin::ArchiveConfiguration(BMessage* msg)
{
	return B_ERROR;
}


status_t
MediaPlayerPlugin::UnarchiveConfiguration(BMessage* msg)
{
	return B_ERROR;
}
