/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MEDIA_PLAYER_ROSTER
#define MEDIA_PLAYER_ROSTER

#include <Locker.h>
#include <Looper.h>
#include <Message.h>
#include <Messenger.h>
#include <SupportDefs.h>


enum {
	B_MEDIA_PLAYER_FILTER_HOTKEYS,
	B_MEDIA_PLAYER_FILTER_REFS	
};


class MediaPlayerRoster {
public:
	//static MediaPlayerRoster* Roster(team_id team = -1,
	//								const char* window = NULL);

	//const char*				ID() const;

	bool						IsLocked() const;
    bool						Lock();
    void						Unlock();

    status_t					Hide() const;
    status_t					Show() const;
    bool						IsHidden() const;
	status_t					Quit();

	status_t					SetVolume(float volume);
	status_t					GetVolume(float* volume);

    status_t					RegisterLooper(BLooper* looper,
									int32 filterFlags
										= B_MEDIA_PLAYER_FILTER_HOTKEYS
											| B_MEDIA_PLAYER_FILTER_REFS);
	status_t					UnregisterLooper(BLooper* looper);

	status_t					RegisterHandler(BHandler* handler);
	status_t					UnregisterHandler(BHandler* handler);
private:
	friend class MediaPlayerHostAgent;

								MediaPlayerRoster(BMessenger& messenger);

	BMessenger					fDestWindowMessenger;
	BLocker						fLock;
};

#endif
