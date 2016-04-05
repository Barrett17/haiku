/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MEDIA_PLAYER_HOST
#define MEDIA_PLAYER_HOST

#include <ObjectList.h>
#include <Messenger.h>

#include "MediaPlayerPlugin.h"


class MediaPlayerHostAgent;

class MediaPlayerHost : public BLocker {
public:
	static MediaPlayerHostAgent*		InstantiateHostAgent(
											BMessenger messenger);

private:
	friend class MediaPlayerHostAgent;

	static MediaPlayerHost*				Get();
										MediaPlayerHost();

	BObjectList<MediaPlayerHostAgent>	fAgents;
	BObjectList<MediaPlayerAddOn>		fAddOns;
};


class MediaPlayerHostAgent {
public:
	virtual								~MediaPlayerHostAgent();

	void								Release();

	int32								CountAddOns() const;
	MediaPlayerAddOn*					AddOnAt(int32 i);

	// Create a new instance of the plugin, this class own it
	MediaPlayerPlugin*					InstantiatePlugin(
											media_player_plugin* handle);
	void								ReleasePlugin(
											MediaPlayerPlugin* plugin);

	bool								Lock();
	void								Unlock();
	bool								IsLocked() const;

	// The functions above should be called locked.

	// Plugin instances control
	int32								CountInstances();
	MediaPlayerPlugin*					InstanceAt(int32 i);
	void								AddInstance(MediaPlayerPlugin* plugin,
											int32 index = -1);
	void								RemoveInstance(
											MediaPlayerPlugin* plugin);
	void								MoveInstance(int32 oldIndex,
											int32 newIndex);

	// Plugins instances callbacks
	void								NotifyFileChanged(entry_ref& ref,
											const char* path);

	//void								Filter(short* buffer,
	//										int32 framecount);
	void								Filter(float* buffer,
											int32 framecount);

	status_t							ArchiveConfiguration(BMessage* msg);
	status_t							UnarchiveConfiguration(BMessage* msg);
private:
										MediaPlayerHostAgent(
											MediaPlayerHost* host,
											BMessenger& messenger);

	void								_ReleasePlugin(
											MediaPlayerPlugin* plugin);

	friend class						MediaPlayerHost;

	MediaPlayerRoster*					fRoster;
	MediaPlayerHost*					fHost;
	BObjectList<MediaPlayerPlugin>		fInstances;
	BLocker								fInstancesLocker;
};

#endif

