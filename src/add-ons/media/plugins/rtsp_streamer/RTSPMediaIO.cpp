
#include "RTSPMediaIO.h"

#include <stdlib.h>

#include <DataIO.h>


// UDP or TCP
#if 1
	const char* transport = "RTP/AVP;unicast;client_port=1234-1235";
#else
	const char* transport = "RTP/AVP/TCP;unicast;client_port=1234-1235";
#endif


static thread_id gPlayThread = -1; 

size_t
RTSPMediaIO::WriteMemoryCallback(void *contents, size_t size,
	size_t nmemb, void* userp)
{
	RTSPMediaIO* io = static_cast<RTSPMediaIO*>(userp);
	//printf("%p\n", userp);
	if (io != NULL && io->fBuffer != NULL) {
		io->fLock.WriteLock();
		off_t pos = io->fBuffer->Position();
		off_t seek = 0;
		size_t realsize = size * nmemb;
		io->fBuffer->GetSize(&seek);
		io->fBuffer->WriteAt(seek, contents, realsize);
		// Restore readers position
		io->fBuffer->Seek(pos, SEEK_SET);
		io->fLock.WriteUnlock();
		return realsize;
	}
	return 0;
}


static size_t
FallbackHeaderFunc(void *contents, size_t size, size_t nmemb, void* userp)
{
	return fwrite(contents, size, nmemb, (FILE*)userp);
}


status_t
RTSPMediaIO::PlayThread(void* data)
{
	//RTSPMediaIO* fOwner = static_cast<RTSPMediaIO*>(data);
	//fOwner->fLock.WriteLock();
	
	//fOwner->fBuffer->Seek(0, SEEK_SET);
	return B_OK;
}


RTSPMediaIO::RTSPMediaIO(BUrl* ourUrl)
	:
	fUrl(ourUrl),
	fInitErr(B_OK)
{
	fBuffer = new BMallocIO();
	const char* url = fUrl->UrlString().String();
	fUri =(char*)malloc(strlen(url) + 32);
	fControl = (char*)malloc(strlen(url) + 32);
	CURLcode res;

	res = curl_global_init(CURL_GLOBAL_ALL);
	if(res == CURLE_OK) {
	  curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
	  fprintf(stderr, "    cURL V%s loaded\n", data->version);

	  fCurl = curl_easy_init();
	  if(fCurl != NULL) {
		my_curl_easy_setopt(fCurl, CURLOPT_VERBOSE, 1L);
		my_curl_easy_setopt(fCurl, CURLOPT_NOPROGRESS, 1L);
		my_curl_easy_setopt(fCurl, CURLOPT_HEADERFUNCTION, FallbackHeaderFunc);
		my_curl_easy_setopt(fCurl, CURLOPT_HEADERDATA, stdout);
		my_curl_easy_setopt(fCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		my_curl_easy_setopt(fCurl, CURLOPT_WRITEDATA, (void*)this);
		my_curl_easy_setopt(fCurl, CURLOPT_URL, url);
		my_curl_easy_setopt(fCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		snprintf(fUri, strlen(url) + 32, "%s", url);
		rtsp_options(fCurl, fUri);

		rtsp_describe(fCurl, fUri);

		get_media_control_attribute(fBuffer->Buffer(), fControl);

		snprintf(fUri, strlen(url) + 32, "%s/%s", url, fControl);
		rtsp_setup(fCurl, fUri, transport);

		delete fBuffer;
		fBuffer = new BMallocIO();

		snprintf(fUri, strlen(url) + 32, "%s/", url);

		my_curl_easy_setopt(fCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		my_curl_easy_setopt(fCurl, CURLOPT_WRITEDATA, (void*)fBuffer);

		if (gPlayThread == -1) {
			gPlayThread = spawn_thread(PlayThread, "play_thread",
				B_NORMAL_PRIORITY, this);
		}

		if (gPlayThread <= 0 || resume_thread(gPlayThread) != B_OK)
			fInitErr = B_ERROR;

		rtsp_play(fCurl, fUri, "0.000-");
		fBuffer->Seek(0, SEEK_SET);

	  } else {
		fprintf(stderr, "curl_easy_init() failed\n");
		fInitErr = B_ERROR;
		return;
	  }
	} else {
		fprintf(stderr, "curl_global_init(%s) failed: %d\n",
			  "CURL_GLOBAL_ALL", res);
		fInitErr = B_ERROR;
		return;
	}
}


RTSPMediaIO::~RTSPMediaIO()
{
	rtsp_teardown(fCurl, fUri);
	curl_easy_cleanup(fCurl);
	fCurl = NULL;
	curl_global_cleanup();
	free(fControl);
	free(fUri);
}


status_t
RTSPMediaIO::InitCheck() const
{
	return fInitErr;
}


ssize_t
RTSPMediaIO::ReadAt(off_t position, void* buffer, size_t size)
{
	if (!fLock.ReadLock())
		return B_ERROR;

	ssize_t ret = fBuffer->ReadAt(position, buffer, size);
	fLock.ReadUnlock();
	return ret;
}


ssize_t
RTSPMediaIO::WriteAt(off_t position, const void* buffer, size_t size)
{
	return B_NOT_SUPPORTED;
}


off_t
RTSPMediaIO::Seek(off_t position, uint32 seekMode)
{
	if (!fLock.WriteLock())
		return B_ERROR;

	off_t ret = fBuffer->Seek(position, seekMode);
	fLock.WriteUnlock();
	return ret;
}

off_t
RTSPMediaIO::Position() const
{
	return fBuffer->Position();
}


status_t
RTSPMediaIO::SetSize(off_t size)
{
	return B_NOT_SUPPORTED;
}


status_t
RTSPMediaIO::GetSize(off_t* size) const
{
	return fBuffer->GetSize(size);
}


bool
RTSPMediaIO::IsSeekable() const
{
	return false;
}


bool
RTSPMediaIO::IsEndless() const
{
	return true;
}
