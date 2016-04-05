/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <MediaPlayerPlugin.h>
#include <SupportDefs.h>
#include <View.h>


struct plugin_descriptor;
struct plugin_info;
struct filter_plugin_ops;
struct interface_plugin_ops;


class SPlayPlugin : public MediaPlayerPlugin {
public:
									SPlayPlugin(media_player_plugin* plugin,
										plugin_descriptor* desc);
	virtual							~SPlayPlugin();

	virtual status_t				Show();
	virtual void					Hide();
	virtual void					ShowAbout();
	virtual BView*					GetConfigurationView() const;

	virtual void					FileChanged(const entry_ref& ref,
										const char* path);

	virtual void					Filter(float* buffer, int32 frameCount,
										int32 channelCount);

	virtual status_t				ArchiveConfiguration(BMessage* msg);
	virtual status_t				UnarchiveConfiguration(BMessage* msg);

protected:
	virtual void					Init();
	virtual void					Destroy();

private:
			plugin_descriptor* 		fDesc;
			plugin_info*			fInfo;
			void*					fData;
			//void*					fOpsDesc;

			filter_plugin_ops*		fFilterOps;
			interface_plugin_ops*	fInterfaceOps;
};
