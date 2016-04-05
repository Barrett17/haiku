/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MEDIA_PLAYER_PLUGIN
#define MEDIA_PLAYER_PLUGIN

#include <Message.h>
#include <SupportDefs.h>
#include <View.h>

#include "MediaPlayerAddOn.h"
#include "MediaPlayerRoster.h"


enum mediaplayer_plugin_flags {
	MEDIA_PLAYER_CONFIG,
	MEDIA_PLAYER_ABOUT,
	MEDIA_PLAYER_GUI_AUDIO_CONSUMER,
	MEDIA_PLAYER_GUI_CONFIG,
	MEDIA_PLAYER_GUI_CONTROLS,
	MEDIA_PLAYER_AUDIO_FILTER,
	MEDIA_PLAYER_VIDEO_FILTER,
	MEDIA_PLAYER_FILE_STATUS
};

typedef struct media_player_plugin {
	const char*			name;
	int32				flags;
	MediaPlayerAddOn*	add_on;
} media_player_plugin;


class MediaPlayerPlugin {
public:
									MediaPlayerPlugin(media_player_plugin* plugin);
	virtual							~MediaPlayerPlugin();

			int32					ID() const;
			const char*				Name() const;
			int32					GetFlags() const;
			MediaPlayerAddOn*		AddOn() const;
			MediaPlayerRoster*		Roster() const;

	virtual status_t				Show();
	virtual void					Hide();
	virtual void					ShowAbout();
	virtual BView*					GetConfigurationView() const;

	virtual void					FileChanged(const entry_ref& ref,
										const char* path);

	// NOTE: In a future version of this API we could have the plugin
	// handle any format by passing a media_format.
	virtual void					Filter(float* buffer, int32 frameCount,
										int32 channelCount);

	//virtual void					Filter(BBitmap* buffer, int32 frameCount);

	virtual status_t				ArchiveConfiguration(BMessage* msg);
	virtual status_t				UnarchiveConfiguration(BMessage* msg);

protected:
	virtual void					Init();
	virtual void					Destroy();

private:
	friend class MediaPlayerHostAgent;

			MediaPlayerRoster*		fRoster;
			media_player_plugin*	fPlugin;
			int32					fID;
};

#endif
