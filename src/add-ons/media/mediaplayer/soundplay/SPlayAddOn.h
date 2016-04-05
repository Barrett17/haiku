/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <ObjectList.h>
#include <SupportDefs.h>

#include "MediaPlayerAddOn.h"
#include "MediaPlayerPlugin.h"


class PluginContainer;

class SPlayAddOn : public MediaPlayerAddOn {
public:
											SPlayAddOn();
	virtual									~SPlayAddOn();

	virtual int32							Version() const;
	virtual const char*						Name() const;

	virtual int32							CountPlugins() const;
	virtual media_player_plugin*			PluginAt(int32 i);
	virtual	MediaPlayerPlugin*				InstantiatePlugin(
												media_player_plugin* plugin);
	virtual void							ReleasePlugin(
												MediaPlayerPlugin* plugin);

	virtual	const char**					GetNotificationPaths();
	virtual	void							RefAdded(const entry_ref& ref);
	virtual	void							RefRemoved(const entry_ref& ref);

private:
			void							_RegisterAddOns();
			status_t						_RegisterAddOn(const entry_ref& ref);
			status_t						_UnregisterAddOn(const entry_ref& ref);

			BObjectList<PluginContainer>	fPlugins;
};
