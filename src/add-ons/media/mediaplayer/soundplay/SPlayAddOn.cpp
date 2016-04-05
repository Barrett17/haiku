/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SPlayAddOn.h"

#include <AutoDeleter.h>
#include <Architecture.h>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Path.h>

#include <ImageLoader.h>

#include "MediaPlayerPlugin.h"
#include "pluginproto.h"
#include "SPlayPlugin.h"


class PluginContainer {
public:
	PluginContainer(plugin_descriptor* desc, media_player_plugin* pluginInfo,
		entry_ref entryRef)
		:
		descriptor(desc),
		plugin(pluginInfo),
		ref(entryRef)
	{
	}

	~PluginContainer()
	{
		delete descriptor;
		delete plugin;
	}

	plugin_descriptor*		descriptor;
	media_player_plugin*	plugin;
	entry_ref				ref;
};


MediaPlayerAddOn* make_media_player_addon()
{
	return new SPlayAddOn();
}


SPlayAddOn::SPlayAddOn()
	:
	MediaPlayerAddOn()
{
	_RegisterAddOns();
}


SPlayAddOn::~SPlayAddOn()
{
	for (int32 i = 0; i < fPlugins.CountItems(); i++) {
		PluginContainer* cont = fPlugins.ItemAt(i);
		delete cont;
	}
}


int32
SPlayAddOn::Version() const
{
	return 1;
}


void
SPlayAddOn::_RegisterAddOns()
{
	char** directories = NULL;
	size_t count = 0;

	if (find_paths_etc(get_architecture(), B_FIND_PATH_ADD_ONS_DIRECTORY,
			"media/plugins/mediaplayer/soundplay", B_FIND_PATH_EXISTING_ONLY,
			&directories, &count) != B_OK) {
		return;
	}

	MemoryDeleter directoriesDeleter(directories);

	BPath path;
	for (uint i = 0; i < count; i++) {
		BDirectory directory;
		if (directory.SetTo(directories[i]) == B_OK) {
			entry_ref ref;
			while(directory.GetNextRef(&ref) == B_OK)
				_RegisterAddOn(ref);
		}
	}
}


status_t
SPlayAddOn::_RegisterAddOn(const entry_ref& ref)
{
	BPath path(&ref);

	ImageLoader loader(path);
	status_t status = loader.InitCheck();
	if (status != B_OK)
		return status;

	plugin_descriptor** (*get_plugin_list_func)();

	if (get_image_symbol(loader.Image(), "get_plugin_list",
			B_SYMBOL_TYPE_TEXT, (void**)&get_plugin_list_func) < B_OK) {
		return B_BAD_TYPE;
	}

	plugin_descriptor** plugins = (*get_plugin_list_func)();
	if (plugins == NULL)
		return B_ERROR;

	for (int32 i = 0; plugins[i] != NULL; i++) {
		// We don't support decoders
		if (plugins[i]->flags & PLUGIN_IS_DECODER)
			continue;

		plugin_descriptor* desc = new plugin_descriptor;
		*desc = *plugins[i];

		media_player_plugin* plugin = new media_player_plugin;
		plugin->name = desc->name;
		plugin->add_on = this;

		if (desc->flags & PLUGIN_IS_FILTER
				|| desc->flags & PLUGIN_IS_VISUAL) {
			plugin->flags &= MEDIA_PLAYER_AUDIO_FILTER;
			//plugin->flags &= MEDIA_PLAYER_AUDIO_SHORT_FILTER;
			plugin->flags &= MEDIA_PLAYER_GUI_CONFIG;
		} else if (desc->flags & PLUGIN_IS_INTERFACE) {
			plugin->flags &= MEDIA_PLAYER_GUI_CONTROLS;
			plugin->flags &= MEDIA_PLAYER_GUI_CONFIG;
		} if (desc->flags & PLUGIN_IS_SINGLE_CONTEXT) {
			//TODO
		}
		fPlugins.AddItem(new PluginContainer(desc, plugin, ref));
	}

	delete[] plugins;

	return B_OK;
}


status_t
SPlayAddOn::_UnregisterAddOn(const entry_ref& ref)
{
	//TODO: implement it
	return B_OK;
}


const char*
SPlayAddOn::Name() const
{
	return "SoundPlay Host";
}


int32
SPlayAddOn::CountPlugins() const
{
	return fPlugins.CountItems();
}


media_player_plugin*
SPlayAddOn::PluginAt(int32 i)
{
	if (i < 0 || i >= fPlugins.CountItems())
		return NULL;

	return fPlugins.ItemAt(i)->plugin;
}


MediaPlayerPlugin*
SPlayAddOn::InstantiatePlugin(media_player_plugin* plugin)
{
	if (plugin->add_on != this)
		return NULL;

	for (int32 i = 0; i < fPlugins.CountItems(); i++) {
		if (fPlugins.ItemAt(i)->plugin->name == plugin->name) {
			return new SPlayPlugin(plugin,
				fPlugins.ItemAt(i)->descriptor);
		}
	}
	return NULL;
}


void
SPlayAddOn::ReleasePlugin(MediaPlayerPlugin* plugin)
{
	//TODO: implement it
}


const char**
SPlayAddOn::GetNotificationPaths()
{
	return NULL;
}


void
SPlayAddOn::RefAdded(const entry_ref& ref)
{
	_RegisterAddOn(ref);
}


void
SPlayAddOn::RefRemoved(const entry_ref& ref)
{
	_UnregisterAddOn(ref);
}
