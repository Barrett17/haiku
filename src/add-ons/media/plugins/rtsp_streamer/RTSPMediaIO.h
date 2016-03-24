#ifndef _RTSP_MEDIA_IO_H
#define _RTSP_MEDIA_IO_H

#include <DataIO.h>
#include <Url.h>
#include <RWLocker.h>

#include "rtsp.h"


class RTSPMediaIO : public BMediaIO {
public:
							RTSPMediaIO(BUrl* ourUrl);
	virtual					~RTSPMediaIO();

			status_t		InitCheck() const;

			static status_t	PlayThread(void* data);
			static size_t	WriteMemoryCallback(void *contents, size_t size,
								size_t nmemb, void* userp);

	virtual	ssize_t			ReadAt(off_t position, void* buffer,
								size_t size);
	virtual	ssize_t			WriteAt(off_t position, const void* buffer,
								size_t size);
	virtual	off_t			Seek(off_t position, uint32 seekMode);
	virtual off_t			Position() const;

	virtual	status_t		SetSize(off_t size);
	virtual	status_t		GetSize(off_t* size) const;

	virtual bool			IsSeekable() const;
	//virtual bool			IsCached() const;
	virtual	bool			IsEndless() const;

private:
	BUrl*					fUrl;
	BMallocIO*				fBuffer;
	RWLocker				fLock;

	CURL*					fCurl;
	char*					fUri;
	char*					fControl;
	status_t				fInitErr;
};

#endif
