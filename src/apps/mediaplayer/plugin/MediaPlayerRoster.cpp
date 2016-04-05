/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MediaPlayerRoster.h"


MediaPlayerRoster::MediaPlayerRoster(BMessenger& messenger)
{
	fDestWindowMessenger = messenger;
}


status_t
MediaPlayerRoster::Hide() const
{
	BMessage msg(4);
	msg.AddBool("data", true);
	return fDestWindowMessenger.SendMessage(&msg);
}


status_t
MediaPlayerRoster::Show() const
{
	BMessage msg(4);
	msg.AddBool("data", false);
	return fDestWindowMessenger.SendMessage(&msg);
}


bool
MediaPlayerRoster::IsHidden() const
{
	BMessage msg(B_GET_PROPERTY);
	BMessage reply;
	bool result;
	if (fDestWindowMessenger.SendMessage(&msg, &reply) == B_OK
			&& msg.FindBool("data", &result) == B_OK) {
		return result;
	}
	return true;
}


status_t
MediaPlayerRoster::Quit()
{
	//fDestWindowMessenger.SendMessage(M_ROSTER_QUIT);
	return B_ERROR;
}


status_t
MediaPlayerRoster::RegisterLooper(BLooper* looper, int32 filterFlags)
{
	return B_ERROR;
}


status_t
MediaPlayerRoster::UnregisterLooper(BLooper* looper)
{
	return B_ERROR;
}


status_t
MediaPlayerRoster::RegisterHandler(BHandler* handler)
{
	return B_ERROR;
}


status_t
MediaPlayerRoster::UnregisterHandler(BHandler* handler)
{
	return B_ERROR;
}


bool
MediaPlayerRoster::Lock()
{
	return fLock.Lock();
}


void
MediaPlayerRoster::Unlock()
{
	fLock.Unlock();
}


bool
MediaPlayerRoster::IsLocked() const
{
	return fLock.IsLocked();
}
