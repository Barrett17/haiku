
#include "RTSPMediaIO.h"

#include <DataIO.h>


RTSPMediaIO::RTSPMediaIO(BUrl* url)
	:
	fUrl(url)
{
}


RTSPMediaIO::~RTSPMediaIO()
{
}


ssize_t
RTSPMediaIO::ReadAt(off_t position, void* buffer, size_t size)
{
	return B_NOT_SUPPORTED;
}


ssize_t
RTSPMediaIO::WriteAt(off_t position, const void* buffer, size_t size)
{
	return B_NOT_SUPPORTED;
}


off_t
RTSPMediaIO::Seek(off_t position, uint32 seekMode)
{
	return B_NOT_SUPPORTED;
}

off_t
RTSPMediaIO::Position() const
{
	return B_NOT_SUPPORTED;
}


status_t
RTSPMediaIO::SetSize(off_t size)
{
	return B_NOT_SUPPORTED;
}


status_t
RTSPMediaIO::GetSize(off_t* size) const
{
	return B_NOT_SUPPORTED;
}


bool
RTSPMediaIO::IsSeekable() const
{
	return false;
}

bool
RTSPMediaIO::IsCached() const
{
	return false;
}

bool
RTSPMediaIO::IsEndless() const
{
	return true;
}
