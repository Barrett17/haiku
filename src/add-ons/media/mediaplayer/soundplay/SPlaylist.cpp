/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SPlaylist.h"

#include "pluginproto.h"


status_t
PrivatePlaylist::RestoreFromMessage(const BMessage* mes, int32 index)
{
	return B_ERROR;
}


void
PrivatePlaylist::SaveToMessage(BMessage& mes) const
{
}


void
PrivatePlaylist::MakeEmpty()
{
}


int32
PrivatePlaylist::CountItems() const
{
	return 0;
}


int32
PrivatePlaylist::Add(const entry_ref& ref, int32 index)
{
	return -1;
}


int32
PrivatePlaylist::Add(const char* path, int32 index)
{
	return -1;
}


status_t
PrivatePlaylist::AddDir(const entry_ref& ref, int32 index)
{
	return B_ERROR;
}


status_t
PrivatePlaylist::AddDir(const char* path, int32 index)
{
	return B_ERROR;
}


status_t
PrivatePlaylist::InsertRefsFromMessage(BMessage* message, char* refname, int32 index)
{
	return B_ERROR;
}


status_t
PrivatePlaylist::AddRefsFromMessage(BMessage* message, char* refname)
{
	return B_ERROR;
}


status_t
PrivatePlaylist::SetName(int32 ID, const char* name)
{
	return B_ERROR;
}


int32
PrivatePlaylist::IDForItemAt(int32 index) const
{
	return -1;
}


const char* 
PrivatePlaylist::PathForItem(int32 ID) const
{
	return NULL;
}


const char* 
PrivatePlaylist::NameForItem(int32 ID) const
{
	return NULL;
}


status_t
PrivatePlaylist::Remove(int32 ID)
{
	return B_ERROR;
}


const char* 
PrivatePlaylist::CurrentPath() const
{
	return NULL;
}


const char* 
PrivatePlaylist::CurrentName() const
{
	return NULL;
}


status_t
PrivatePlaylist::Shuffle()
{
	return B_ERROR;
}


status_t
PrivatePlaylist::Sort()
{
	return B_ERROR;
}


status_t
PrivatePlaylist::SortByPath()
{
	return B_ERROR;
}


int32
PrivatePlaylist::CurrentIndex() const
{
	return -1;
}


int32
PrivatePlaylist::CurrentID() const
{
	return -1;
}


status_t
PrivatePlaylist::AddListener(BHandler* handler)
{
	return B_ERROR;
}


status_t
PrivatePlaylist::RemoveListener(BHandler* handler)
{
	return B_ERROR;
}


void
PrivatePlaylist::Play(void)
{
}


void
PrivatePlaylist::Pause(void)
{
}


float
PrivatePlaylist::Pitch(void)
{
	return 0.0f;
}


void
PrivatePlaylist::SetPitch(float)
{
}


double
PrivatePlaylist::Position(void)
{
	return -1;
}


void
PrivatePlaylist::SetPosition(double)
{
}


float
PrivatePlaylist::Volume(void)
{
	return -1;
}


void
PrivatePlaylist::SetVolume(float)
{
}


void
PrivatePlaylist::PlayFile(int32 ID)
{
}


void
PrivatePlaylist::PlayNext()
{
}


void
PrivatePlaylist::PlayPrevious(bool reverse)
{
}


bool
PrivatePlaylist::Lock()
{
	return false;
}


void
PrivatePlaylist::Unlock()
{
}


bool
PrivatePlaylist::IsLocked()
{
	return false;
}


bool
PrivatePlaylist::HasControls()
{
	return false;
}


status_t
PrivatePlaylist::AddControls()
{
	return false;
}


bool
PrivatePlaylist::IsValid()
{
	return false;
}


PrivatePlaylist::PrivatePlaylist()
{
}


PrivatePlaylist::~PrivatePlaylist()
{
}


PlaylistPtr::PlaylistPtr(PrivatePlaylist* p)
{
}


PlaylistPtr::PlaylistPtr(const PlaylistPtr& p)
{
}


PlaylistPtr::PlaylistPtr(int32 ID)
{
}


PlaylistPtr::~PlaylistPtr()
{
}


PlaylistPtr&
PlaylistPtr::operator=(const PlaylistPtr& p)
{
	return* new PlaylistPtr();
}


bool
PlaylistPtr::operator==(const PlaylistPtr& p) const
{
	return false;
}


bool
PlaylistPtr::operator!=(const PlaylistPtr& p) const
{
	return false;
}
