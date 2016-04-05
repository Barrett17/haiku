/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MediaPlayerHost.h"

#include <Autolock.h>
#include <SupportDefs.h>

#include "MediaPlayerAddOn.h"
#include "MediaPlayerRoster.h"


static MediaPlayerHost* sHost = NULL;


MediaPlayerHost*
MediaPlayerHost::Get()
{
	if (sHost == NULL)
		sHost = new MediaPlayerHost();

	return sHost;
}


MediaPlayerHost::MediaPlayerHost()
	:
	fAddOns(true)
{
}


MediaPlayerHostAgent*
MediaPlayerHost::InstantiateHostAgent(BMessenger messenger)
{
	MediaPlayerHost* host = MediaPlayerHost::Get();
	MediaPlayerHostAgent* agent = new MediaPlayerHostAgent(host, messenger);
	host->fAgents.AddItem(agent);
	return agent;
}


MediaPlayerHostAgent::MediaPlayerHostAgent(
	MediaPlayerHost* host, BMessenger& messenger)
	:
	fHost(host)
{
	fRoster = new MediaPlayerRoster(messenger);
}


MediaPlayerHostAgent::~MediaPlayerHostAgent()
{
	delete fRoster;
}


void
MediaPlayerHostAgent::Release()
{
	BAutolock _(fHost);
	if (Lock()) {
		int32 count = CountInstances();
		for (int32 i = 0; i < count; i++) {
			RemoveInstance(fInstances.ItemAt(i));
			_ReleasePlugin(fInstances.ItemAt(i));
		}
		Unlock();
	}
	fHost->fAgents.RemoveItem(this);
	delete this;
}


MediaPlayerPlugin*
MediaPlayerHostAgent::InstantiatePlugin(media_player_plugin* plugin)
{
	BAutolock _(fHost);
	MediaPlayerPlugin* newPlugin
		= plugin->add_on->InstantiatePlugin(plugin);
	newPlugin->fRoster = fRoster;
	newPlugin->Init();
	return newPlugin;
}


void
MediaPlayerHostAgent::ReleasePlugin(MediaPlayerPlugin* plugin)
{
	BAutolock _(fHost);
	_ReleasePlugin(plugin);
}


int32
MediaPlayerHostAgent::CountAddOns() const
{
	BAutolock _(fHost);
	return fHost->fAddOns.CountItems();
}


MediaPlayerAddOn*
MediaPlayerHostAgent::AddOnAt(int32 i)
{
	BAutolock _(fHost);
	return fHost->fAddOns.ItemAt(i);
}


bool
MediaPlayerHostAgent::Lock()
{
	return fInstancesLocker.Lock();
}


void
MediaPlayerHostAgent::Unlock()
{
	fInstancesLocker.Unlock();
}


bool
MediaPlayerHostAgent::IsLocked() const
{
	return fInstancesLocker.IsLocked();
}


int32
MediaPlayerHostAgent::CountInstances()
{
	return fInstances.CountItems();
}


MediaPlayerPlugin*
MediaPlayerHostAgent::InstanceAt(int32 i)
{
	return fInstances.ItemAt(i);
}


void
MediaPlayerHostAgent::AddInstance(MediaPlayerPlugin* plugin, int32 index)
{
	if (index == -1)
		index = CountInstances();

	fInstances.AddItem(plugin, index);
}


void
MediaPlayerHostAgent::RemoveInstance(MediaPlayerPlugin* plugin)
{
	fInstances.RemoveItem(plugin);
}


void
MediaPlayerHostAgent::MoveInstance(int32 oldIndex, int32 newIndex)
{
	// TODO
}


void
MediaPlayerHostAgent::NotifyFileChanged(entry_ref& ref, const char* path)
{
}


void
MediaPlayerHostAgent::Filter(float* buffer, int32 framecount)
{
}


status_t
MediaPlayerHostAgent::ArchiveConfiguration(BMessage* msg)
{
	return B_ERROR;
}


status_t
MediaPlayerHostAgent::UnarchiveConfiguration(BMessage* msg)
{
	return B_ERROR;
}


void
MediaPlayerHostAgent::_ReleasePlugin(MediaPlayerPlugin* plugin)
{
	plugin->Destroy();
	plugin->AddOn()->ReleasePlugin(plugin);
	delete plugin;
}
