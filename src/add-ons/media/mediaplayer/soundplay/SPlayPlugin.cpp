/*
 * Copyright 2016 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SPlayPlugin.h"

#include "pluginproto.h"

#define CONTROLLER_CONFIG	"SPlayPlugin:ControllerConf"
#define OPS_CONFIG			"SPlayPlugin:OpsConf"


SPlayPlugin::SPlayPlugin(media_player_plugin* plugin,
	plugin_descriptor* desc)
	:
	MediaPlayerPlugin(plugin),
	fDesc(desc),
	fInfo(NULL),
	fData(NULL),
	fFilterOps(NULL),
	fInterfaceOps(NULL)
{
}


SPlayPlugin::~SPlayPlugin()
{
}


void
SPlayPlugin::Init()
{
	fInfo = new plugin_info;
	fInfo->controller = new SoundPlayController(Roster());

	void* opsDesc = fDesc->Instantiate_Plugin(&fData, fDesc->name,
		NULL, 0, fInfo);
	if (fDesc->flags & PLUGIN_IS_INTERFACE) {
		fInterfaceOps = (interface_plugin_ops*) opsDesc;
	} else if (fDesc->flags & PLUGIN_IS_FILTER
			|| fDesc->flags & PLUGIN_IS_VISUAL) {
		fFilterOps = (filter_plugin_ops*) opsDesc;
	}
}


void
SPlayPlugin::Destroy()
{
	fDesc->Destroy_Plugin(fInfo, fData);
	delete fInfo->controller;
	delete fInfo;
}


status_t
SPlayPlugin::Show()
{
	if (fInterfaceOps != NULL && fInterfaceOps->Show != NULL)
		return fInterfaceOps->Show(fData);
	return B_ERROR;
}


void
SPlayPlugin::Hide()
{
	if (fInterfaceOps != NULL && fInterfaceOps->Hide != NULL)
		fInterfaceOps->Hide(fData);
}


void
SPlayPlugin::ShowAbout()
{
	if (fDesc->About != NULL)
		fDesc->About();
}


void
SPlayPlugin::FileChanged(const entry_ref& ref, const char* path)
{
	if (fFilterOps != NULL && fFilterOps->FileChange != NULL)
		fFilterOps->FileChange(fData, ref.name, path);
}


BView*
SPlayPlugin::GetConfigurationView() const
{
	if (fFilterOps != NULL && fFilterOps->Configure != NULL)
		return fFilterOps->Configure(fData);

	if (fDesc->Configure != NULL) {
		// TODO: which msg it exactly need?
		BMessage msg;
		return fDesc->Configure(&msg);
	}
	return NULL;
}


void
SPlayPlugin::Filter(float* buffer, int32 frameCount, int32 channelCount)
{
	/*if (fFilterOps != NULL && fFilterOps->FilterFloat != NULL) {
		float* buf = new float[frameCount];
		memcpy(buf, *buffer, frameCount);
		fFilterOps->FilterFloat(fData, &buf, buffer, frameCount, fInfo);
		delete buf;
	}*/
}


status_t
SPlayPlugin::ArchiveConfiguration(BMessage* msg)
{
	BMessage conf;
	if (fFilterOps != NULL && fFilterOps->GetConfig != NULL)
		fFilterOps->GetConfig(fData, &conf);
	else if (fInterfaceOps != NULL && fInterfaceOps->GetConfig != NULL)
		fInterfaceOps->GetConfig(fData, &conf);

	status_t status = msg->AddMessage(OPS_CONFIG, &conf);
	if (status != B_OK)
		return status;

	return msg->AddMessage(CONTROLLER_CONFIG, fInfo->controller->fPreferences);
}


status_t
SPlayPlugin::UnarchiveConfiguration(BMessage* msg)
{
	BMessage conf;
	status_t status = msg->FindMessage(OPS_CONFIG, &conf);
	if (status != B_OK)
		return status;

	if (fFilterOps != NULL && fFilterOps->SetConfig != NULL)
		fFilterOps->SetConfig(fData, &conf);
	else if (fInterfaceOps != NULL && fInterfaceOps->SetConfig != NULL)
		fInterfaceOps->SetConfig(fData, &conf);

	return msg->FindMessage(CONTROLLER_CONFIG, fInfo->controller->fPreferences);
}
