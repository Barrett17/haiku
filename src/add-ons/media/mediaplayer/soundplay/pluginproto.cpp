/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MediaPlayerRoster.h"

#include "pluginproto.h"


SoundPlayController::SoundPlayController(MediaPlayerRoster* roster)
	:
	fRoster(roster)
{

}


int32
SoundPlayController::Version()
{
	return 1;
}


const char *
SoundPlayController::VersionString()
{
	return "1.0";
}


void
SoundPlayController::Quit()
{
	fRoster->Quit();
}


void
SoundPlayController::DisableInterface(const char* id)
{
	// Unimplemented
}


void
SoundPlayController::HideMainInterface()
{
	fRoster->Hide();
}


void
SoundPlayController::ShowMainInterface()
{
	fRoster->Show();
}


bool
SoundPlayController::IsMainInterfaceHidden()
{
	return fRoster->IsHidden();
}


void
SoundPlayController::Lock()
{
	fRoster->Lock();
}


void
SoundPlayController::Unlock()
{
	fRoster->Unlock();
}


uint32
SoundPlayController::CountPlaylists()
{
	return 0;
}


PlaylistPtr
SoundPlayController::PlaylistAt(uint32 index)
{
	return PlaylistPtr();
}


PlaylistPtr
SoundPlayController::AddPlaylist()
{
	return PlaylistPtr();
}


status_t
SoundPlayController::AddPlaylist(PlaylistPtr playlist)
{
	return B_ERROR;
}


status_t
SoundPlayController::RemovePlaylist(PlaylistPtr playlist)
{
	return B_ERROR;
}


void
SoundPlayController::OpenEditor(PlaylistPtr playlist)
{
}


status_t
SoundPlayController::AddListener(BHandler *handler)
{
	return fRoster->RegisterHandler(handler);
}


status_t
SoundPlayController::RemoveListener(BHandler *handler)
{
	return fRoster->UnregisterHandler(handler);
}


void
SoundPlayController::AddWindow(BWindow* window, int32 filterFlags)
{
	int32 flags = 0;
	if (filterFlags & FILTER_HOTKEYS)
		flags &= B_MEDIA_PLAYER_FILTER_HOTKEYS;

	if (filterFlags & FILTER_REFS)
		flags &= B_MEDIA_PLAYER_FILTER_REFS;

	fRoster->RegisterLooper((BLooper*)window, flags);
}


status_t
SoundPlayController::StorePreference(const char* name, const BMessage *message)
{
	return fPreferences->AddMessage(name, message);
}


status_t
SoundPlayController::RetrievePreference(const char* name, BMessage *message)
{
	return fPreferences->AddMessage(name, message);
}
