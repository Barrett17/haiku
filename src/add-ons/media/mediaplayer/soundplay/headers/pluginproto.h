#ifndef SOUNDPLAY_PLUGIN_PROTO_H
#define SOUNDPLAY_PLUGIN_PROTO_H

#include <SupportDefs.h>
#include <View.h>

#include "SPlaylist.h"


const uint32 ENABLE_INTERFACE = '!int';

const uint64 PLUGIN_POSITION_IS_RELATIVE = 0x0000000200000000LL;
const uint64 PLUGIN_REQUIRES_CONTIGUOUS_PHYSICAL_MEMORY = 0x0000000100000000LL;
const uint64 PLUGIN_MINIMAL_BUFFERING = 0x0000000400000000LL;
const uint64 PLUGIN_FILELENGTH_UNKNOWN = 0x0000000000000001LL;
const uint64 PLUGIN_NO_ELAPSEDTIME = 0x0000000000000002LL;


enum plugin_magic {
	PLUGIN_DESCRIPTOR_MAGIC = 'desc',
	PLUGIN_DECODER_MAGIC = 'inpt',
	PLUGIN_FILTER_MAGIC = 'filt',
	PLUGIN_VISUAL_MAGIC = 'visu',
	PLUGIN_INTERFACE_MAGIC = 'face',
	PLUGIN_PLAYLIST_MAGIC = 'edit'
};

enum plugin_version {
	PLUGIN_DESCRIPTOR_VERSION = 3,
	PLUGIN_DECODER_VERSION = 3,
	PLUGIN_FILTER_VERSION = 4,
	PLUGIN_VISUAL_VERSION = 3,
	PLUGIN_INTERFACE_VERSION = 4,
	PLUGIN_PLAYLIST_VERSION = 4
};

enum plugin_type {
	PLUGIN_IS_DECODER = 1,
	PLUGIN_IS_FILTER = 2,
	PLUGIN_IS_VISUAL = 4,
	PLUGIN_IS_INTERFACE = 16,
	PLUGIN_IS_SINGLE_CONTEXT = 0x10000000
};

enum {
	FILTER_HOTKEYS = 1,
	FILTER_REFS = 2
};

enum {
	CONTROLLER_ADD = 'ct\0\1',
	CONTROLLER_REMOVE = 'ct\0\2',
	CONTROLLER_PLAYLISTEDITOR = 'ct\0\3'
};


class MediaPlayerRoster;

class SoundPlayController {
public:
	int32				Version();
	const char* 		VersionString();

	void				Quit();

	void				DisableInterface(const char* id);

	void				HideMainInterface();
	void				ShowMainInterface();
	bool				IsMainInterfaceHidden();

	void				Lock();
	void				Unlock();

	uint32				CountPlaylists();
	PlaylistPtr			PlaylistAt(uint32 index);
	PlaylistPtr			AddPlaylist();
	status_t			AddPlaylist(PlaylistPtr playlist);
	status_t			RemovePlaylist(PlaylistPtr playlist);
	void				OpenEditor(PlaylistPtr playlist);

	void				AddWindow(BWindow* window,
							int32 filterFlags = FILTER_HOTKEYS | FILTER_REFS);

	status_t			AddListener(BHandler* handler);
	status_t			RemoveListener(BHandler* handler);

	status_t			StorePreference(const char* name,
							const BMessage* message);
	status_t			RetrievePreference(const char* name,
							BMessage* message);

private:
	friend class SPlayPlugin;

						SoundPlayController(MediaPlayerRoster* roster);

	BMessage*			fPreferences;
	MediaPlayerRoster*	fRoster;
};


typedef struct plugin_info {
	SoundPlayController* controller;
	entry_ref*		ref;
} plugin_info;

typedef void		op_about();
typedef BView*		op_configure(BMessage* config);
typedef void*		op_instantiate(void** data, const char* name,
						const char* header, uint32 size,
						plugin_info* pluginInfo);
typedef void		op_destroy(void* plugin_ops, void* data);

typedef void		op_filter_filechange(void* plugin, const char* name,
						const char* path);
typedef status_t	op_filter_filter(void* plugin, short* buffer,
						int32 framecount, void* info);
typedef status_t	op_filter_filter_float(void* plugin, float** input,
						float** output, int32 framecount, void* info);
typedef BView*		op_filter_configure(void* plugin);
typedef void		op_filter_setconfig(void* plugin, BMessage* config);
typedef void		op_filter_getconfig(void* plugin, BMessage* config);

typedef status_t	op_ui_show(void* plugin);
typedef void		op_ui_hide(void* plugin);
typedef void		op_ui_setconfig(void* plugin, BMessage* config);
typedef void		op_ui_getconfig(void* plugin, BMessage* config);


typedef struct supported_type {
	char* 			mimetype;
	char* 			longdesc;
	char* 			shortdesc;
	char* 			extension1;
	char* 			extension2;
} supported_type;

typedef struct plugin_descriptor {
	uint32			desc_magic;
	uint32			desc_version;
	const char*		id;
	uint32			version;
	uint32			flags;
	const char*		name;
	const char*		aboutstring;

					op_about(*About);
					op_configure(*Configure);
					op_instantiate(*Instantiate_Plugin);
					op_destroy(*Destroy_Plugin);
} plugin_descriptor;

typedef struct filter_plugin_ops {
		uint32		ops_magic;
		uint32		ops_version;
					op_filter_filechange(*FileChange);
					op_filter_filter(*Filter);
					op_filter_configure(*Configure);
					op_filter_setconfig(*SetConfig);
					op_filter_getconfig(*GetConfig);
					op_filter_filter_float(*FilterFloat);
} filter_plugin_ops;

typedef struct interface_plugin_ops {
		uint32		ops_magic;
		uint32		ops_version;
					op_ui_show(*Show);
					op_ui_hide(*Hide);
					op_ui_setconfig(*SetConfig);
					op_ui_getconfig(*GetConfig);
} interface_plugin_ops;


extern "C" plugin_descriptor _EXPORT** get_plugin_list();
extern "C" supported_type _EXPORT* get_supported_types();

#define PLUGIN_STRING_LENGTH 256

typedef struct file_info {
	char			name[PLUGIN_STRING_LENGTH];
	char			typedesc[PLUGIN_STRING_LENGTH];
	char			mimetype[PLUGIN_STRING_LENGTH];
	float			samplerate;
	float			bitrate;
	uint32			numchannels;
	uint32			granularity;
	uint32			framecount;
	uint32			samplesize;
	int32			byteorder;
	int32			sampleformat;
	uint64			flags;
} file_info;

// ---- UNIMPLEMENTED

typedef status_t op_decoder_open(void* plugin, const char* name,
					const char* header, uint32 size, BMessage* message);
typedef void op_decoder_close(void* plugin);
typedef status_t op_decoder_info(void* plugin, file_info*);
typedef int32 op_decoder_read(void* plugin, char* buffer, ulong count);
typedef status_t op_decoder_play(void* plugin);
typedef status_t op_decoder_stop(void* plugin);
typedef status_t op_decoder_setvolume(void* plugin, float);
typedef status_t op_decoder_setspeed(void* plugin, float speed);
typedef status_t op_decoder_seek(void* plugin, uint32 position);
typedef uint32 op_decoder_position(void* plugin);
typedef float op_decoder_bufferamount(void* plugin);

typedef struct decoder_plugin_ops {
	uint32 ops_magic;
	uint32 ops_version;
	op_decoder_open(*Open);
	op_decoder_close(*Close);
	op_decoder_info(*Info);
	op_decoder_read(*Read);
	op_decoder_play(*Play);
	op_decoder_stop(*Stop);
	op_decoder_setvolume(*SetVolume);
	op_decoder_setspeed(*SetSpeed);
	op_decoder_seek(*Seek);
	op_decoder_position(*Position);
	op_decoder_bufferamount(*BufferAmount);
} decoder_plugin_ops;

#endif
