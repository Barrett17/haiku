#ifndef _MEDIA_IMAGE_LOADER_H
#define _MEDIA_IMAGE_LOADER_H

#include <image.h>
#include <Path.h>

//	#pragma mark - ImageLoader

/*!	The ImageLoader class is a convenience class to temporarily load
	an image file, and unload it on deconstruction automatically.
*/
class ImageLoader {
public:
	ImageLoader(BPath& path)
	{
		fImage = load_add_on(path.Path());
	}

	~ImageLoader()
	{
		if (fImage >= B_OK)
			unload_add_on(fImage);
	}

	status_t InitCheck() const
	{
		return fImage >= 0 ? B_OK : fImage;
	}

	image_id Image() const
	{
		return fImage;
	}

private:
	image_id	fImage;
};

#endif
