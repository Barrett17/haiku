#ifndef SOUNDPLAY_PLAYLIST_H
#define SOUNDPLAY_PLAYLIST_H


#include <Entry.h>
#include <List.h>
#include <Message.h>
#include <SupportKit.h>

struct file_info;

enum {
	PLAYLIST_ADD				= 'pl\0\1',
	PLAYLIST_REMOVE				= 'pl\0\2',
	PLAYLIST_EMPTY				= 'pl\0\3',
	PLAYLIST_DESTROYED			= 'pl\0\4',
	PLAYLIST_LOST_CONTROLS		= 'pl\0\5',
	PLAYLIST_GAINED_CONTROLS	= 'pl\0\6',
	PLAYLIST_CURRENT			= 'pl\0\7',
	PLAYLIST_SORTED				= 'pl\1\0',
	PLAYLIST_ITEM_RENAMED		= 'pl\1\1'
};


class PrivatePlaylist {
public:
		status_t	RestoreFromMessage(const BMessage* mes, int32 index = 0);
		void		SaveToMessage(BMessage& mes) const;

		void		MakeEmpty();
		int32		CountItems() const;
		int32		Add(const entry_ref& ref, int32 index = -1);
		int32		Add(const char* path, int32 index = -1);
		status_t	AddDir(const entry_ref& ref, int32 index = -1);
		status_t	AddDir(const char* path, int32 index = -1);
		status_t	InsertRefsFromMessage(BMessage* message, char* name, int32 index);
		status_t	AddRefsFromMessage(BMessage* message, char* name);
		status_t	SetName(int32 id, const char* name);

		int32		IDForItemAt(int32 index) const;
		const char*	PathForItem(int32 id) const;
		const char*	NameForItem(int32 id) const;
		status_t	Remove(int32 id);

		const char*	CurrentPath() const;
		const char*	CurrentName() const;
		
		status_t	Shuffle();
        status_t	Sort();
        status_t	SortByPath();
		
		int32		CurrentIndex() const;
		int32		CurrentID() const;
		status_t	GetInfo(int32 id, file_info* info);
		
		status_t	AddListener(BHandler* handler);
		status_t	RemoveListener(BHandler* handler);
		
		void 		Play();
		void 		Pause();
		float		Pitch();
		void		SetPitch(float pitch);
		double		Position();
		void		SetPosition(double volume);
		float		Volume();
		void		SetVolume(float);
		void		PlayFile(int32 id);
		void		PlayNext();
		void		PlayPrevious(bool reverse = false);
		
		bool		Lock();
		void		Unlock();
		bool		IsLocked();
		
		bool		HasControls();
		status_t	AddControls();
		bool		IsValid();

private:
					PrivatePlaylist();
					~PrivatePlaylist();

		//BList		entrylist;
		//BList		listeners;
		//BMessage	notification;

		int32		_reserved[10];
};


class PlaylistPtr {
public:
								PlaylistPtr(PrivatePlaylist*p = NULL);
								PlaylistPtr(const PlaylistPtr& p);
								PlaylistPtr(int32 ID);
								~PlaylistPtr();

		PrivatePlaylist* operator-> () { return ppl; }
		PrivatePlaylist& operator* ()  { return *ppl; }
		PlaylistPtr& operator= (const PlaylistPtr& p);
        bool        operator==(const PlaylistPtr& p) const;
        bool        operator!=(const PlaylistPtr& p) const;

private:
		PrivatePlaylist* ppl;
};

#endif
