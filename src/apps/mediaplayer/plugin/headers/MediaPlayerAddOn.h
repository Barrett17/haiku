/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MEDIA_PLAYER_ADDON
#define MEDIA_PLAYER_ADDON

#include <Entry.h>
#include <SupportDefs.h>


class MediaPlayerPlugin;
struct media_player_plugin;

class MediaPlayerAddOn {
public:
									MediaPlayerAddOn();
	virtual							~MediaPlayerAddOn();

	virtual int32					Version() const = 0;
	virtual const char*				Name() const = 0;

	virtual int32					CountPlugins() const = 0;
	virtual media_player_plugin*	PluginAt(int32 i) = 0;
	virtual	MediaPlayerPlugin*		InstantiatePlugin(media_player_plugin* handle) = 0;
	virtual void					ReleasePlugin(MediaPlayerPlugin* plugin) = 0;

	virtual	const char**			GetNotificationPaths() = 0;
	virtual	void					RefAdded(const entry_ref& ref) = 0;
	virtual	void					RefRemoved(const entry_ref& ref) = 0;
};

extern "C" _EXPORT MediaPlayerAddOn* make_media_player_addon();

#endif
