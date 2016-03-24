/*
 * Copyright 2016, Dario Casalinuovo
 * Distributed under the terms of the MIT License.
 */


#include "HTTPMediaIO.h"

#include <Handler.h>


class FileHandler : public BHandler {
public:
					FileHandler()
					{
						fBuffer = new BMallocIO();
					}

	virtual			~FileHandler() {};

	virtual void	MessageReceived(BMessage* msg)
	{
		switch(msg->what) {
			case B_URL_PROTOCOL_NOTIFICATION:
			{
				int8 notification;
				msg->FindInt8("be:urlProtocolMessageType", &notification);
				if (notification == B_URL_PROTOCOL_DATA_RECEIVED) {
					const void* data = NULL;
					ssize_t size = 0;
					status_t result = msg->FindData("url:data",
						B_STRING_TYPE, &data, &size);
					if (result != B_OK)
						return;

					fBuffer->Write(data, size);
				}
				break;
			}
		}
	}

	BPositionIO*	Buffer() const
	{
		return fBuffer;
	}

private:
	BMallocIO*		fBuffer;
};


HTTPMediaIO::HTTPMediaIO(BUrl* url)
	:
	fInitErr(B_OK)
{
	fContext = new BUrlContext();
	fContext->AcquireReference();

	fFileHandler = new FileHandler();

	fListener = new BUrlProtocolDispatchingListener(fFileHandler);

	fReq = new BFileRequest(*url, fListener,fContext);
	fReq->Run();

	//if (!fReq->IsRunning())
	//	return;

	//fInitErr = _IntegrityCheck();
}


HTTPMediaIO::~HTTPMediaIO()
{
	fContext->ReleaseReference();
	delete fFileHandler;
	delete fListener;
	delete fContext;
	delete fReq;
}


status_t
HTTPMediaIO::InitCheck() const
{
	return fInitErr;
}


ssize_t
HTTPMediaIO::ReadAt(off_t position, void* buffer, size_t size)
{
	return fFileHandler->Buffer()->ReadAt(position, buffer, size);
}


ssize_t
HTTPMediaIO::WriteAt(off_t position, const void* buffer, size_t size)
{
	return B_NOT_SUPPORTED;
}


off_t
HTTPMediaIO::Seek(off_t position, uint32 seekMode)
{
	return fFileHandler->Buffer()->Seek(position, seekMode);
}

off_t
HTTPMediaIO::Position() const
{
	return fFileHandler->Buffer()->Position();
}


status_t
HTTPMediaIO::SetSize(off_t size)
{
	return B_NOT_SUPPORTED;
}


status_t
HTTPMediaIO::GetSize(off_t* size) const
{
	// TODO: well we should know it
	return B_NOT_SUPPORTED;
}


bool
HTTPMediaIO::IsSeekable() const
{
	return true;
}


bool
HTTPMediaIO::IsEndless() const
{
	return true;
}


status_t
HTTPMediaIO::_IntegrityCheck()
{
	if (fReq->Status() != B_OK)
		return fReq->Status();

	const BUrlResult& r = dynamic_cast<const BUrlResult&>(fReq->Result());

	if (r.Length() == 0)
		return B_ERROR;

	return B_OK;
}
