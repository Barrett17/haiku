/*
 * Copyright 2015, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _MEDIA_CLIENT_H
#define _MEDIA_CLIENT_H

#include <ObjectList.h>
#include <Buffer.h>

#include <MediaAddOn.h>
#include <MediaConnection.h>
#include <MediaDefs.h>
#include <MediaNode.h>

#include "MediaClientNode.h"


namespace BPrivate { namespace media {

// The node can receive media data.
#define B_MEDIA_RECORDER		0x000000001
// The node can send media data to another node.
#define B_MEDIA_PLAYER			0x000000002
// The node specify a control GUI which can be used to configure it.
#define B_MEDIA_CONTROLLABLE	0x000000004

// TODO: Add file interface
// TODO: Offline mode is still missing

// BMediaClient is a general purpose class allowing to create any kind
// of media_node. It automatically manage the expected behavior under
// different run modes, and allow to specify the different capabilities
// which you may want. It work with a central loop allowing to handle,
// calling the right callbacks automatically events at a certain
// performance_time.
class BMediaClient {
public:
	enum notification {
		B_WILL_START = 1,			// performance_time
		B_WILL_STOP,				// performance_time immediate
		B_WILL_SEEK,				// performance_time media_time
		B_WILL_TIMEWARP,			// real_time performance_time

		B_FORMAT_SUGGESTION,		// media_type type, int32 quality,
									// media_format* format
	};

	typedef void					(*notify_hook)(void* cookie,
											notification what,
											...);

	// TODO: Should allow BControllable capabilities
									BMediaClient(const char* name,
										media_type type
											= B_MEDIA_UNKNOWN_TYPE,
										uint64
											capabilities = B_MEDIA_PLAYER
												& B_MEDIA_RECORDER);

	virtual							~BMediaClient();

			status_t				InitCheck() const;

	// Return the capabilities of this BMediaClient instance.
			uint64					Capabilities() const;
			media_type				Type() const;

	// To connect pass the BMediaConnection to this class or to another BMediaClient,
	// also in another team the connection object will be valid.
	// When those functions return, the BMediaConnection is added to the
	// list and is visible to other nodes as not connected.

	// This is supplied to support generic connections not related
	// to a certain destination or source node, however for various ambiguity
	// reasons we want the BMediaConnection to be declared as input or output.
	// You can pass the object returned by this function to another
	// BMediaClient::BeginConnection() and then Connect(), so that it
	// will automatically connect to this node.
	virtual BMediaConnection*		BeginConnection(media_connection_kind kind);

	// Those are used if you want to connect to a precise input/output of
	// another node, the connection returned represents a remote input/output.
	virtual BMediaConnection*		BeginConnection(const media_input& input);
	virtual BMediaConnection*		BeginConnection(const media_output& output);

	// This is used when we want to connect us with another BMediaClient, the
	// returned connection is the one you will use to call the Connect method
	// and will represents it by the point of view of this node. Don't try to
	// use directly connections built by another BMediaClient, it will fail,
	// always pass between this function to get a local connection from the point
	// of view of *this* node and then call Connect().
	virtual BMediaConnection*		BeginConnection(BMediaConnection* connection);

	// Bind internally two connections of the same BMediaClient, so that the
	// input will be automatically forwarded to the output just after the
	// ProcessFunc is called. The buffer is automatically recycled too.
	// Beware that the binding operation is valid only for local connections
	// which belong to this node, otherwise return B_ERROR.
	virtual status_t				Bind(BMediaConnection* input,
										BMediaConnection* output);

	virtual status_t				Unbind(BMediaConnection* input,
										BMediaConnection* output);

	// If the user want a particular format for a connection it should
	// use BMediaConnection::SetAcceptedFormat(), if it's not specified
	// BMediaClient::Format() will be used, in case both aren't specified
	// an error is returned. The connection should always belong to this
	// node, in any other case it will return an error.
	virtual status_t				Connect(BMediaConnection* ourConnection,
										BMediaConnection* theirConnection);

	virtual status_t				Connect(BMediaConnection* ourConnection,
										const dormant_node_info& dormantInfo);

	virtual status_t				Connect(BMediaConnection* ourConnection,
										const media_node& node);

	// Disconnect any connection belonging to this object, to disconnect
	// a single connection use BMediaConnection::Disconnect().
	virtual status_t				Disconnect();

			int32					CountConnections() const;
			int32					CountInputs() const;
			int32					CountOutputs() const;
			BMediaConnection*		InputAt(int32 index) const;
			BMediaConnection*		OutputAt(int32 index) const;

			BMediaConnection*		FindConnection(
										const media_destination& dest) const;
			BMediaConnection*		FindConnection(
										const media_source& source) const;

			bool					IsRunning() const;

	virtual status_t				Start(bool force = false);
	virtual status_t				Stop(bool force = false);
			status_t				Seek(bigtime_t mediaTime,
										bigtime_t performanceTime);
			status_t				Roll(bigtime_t start, bigtime_t stop,
										bigtime_t seek);
			status_t				Preroll();
			status_t				SyncTo(bigtime_t performanceTime,
										bigtime_t timeout = -1);

	// It will be B_INCREASE_LATENCY by default
			BMediaNode::run_mode	RunMode() const;
			status_t				SetRunMode(BMediaNode::run_mode mode);
			status_t				SetTimeSource(media_node timesource);

	// Specify a latency range to allow the node behave correctly.
	// Ideally the minimum latency should be the algorithmic latency you expect
	// from the node and will be used as starting point. The max latency is the
	// maximum acceptable by you, over that point the node will adjust it's
	// performance time to recover if a big delay happen.
			void					SetLatencyRange(bigtime_t min,
										bigtime_t max);

			void					GetLatencyRange(bigtime_t* min,
										bigtime_t* max) const;

	// When in B_OFFLINE it returns OfflineTime().
			bigtime_t				OfflineTime() const;
	// Return the current performance time handled by the object
	// when run_mode != B_OFFLINE.
			bigtime_t				PerformanceTime() const;

	// When a connection is not binded with another, it's your job to send
	// the buffer to the connection you want. You might want
	// to ovverride it so that you can track something, in this case
	// be sure to call the base version.
	// Automatically recycle the BBuffer.
	virtual	status_t				SendBuffer(BMediaConnection* connection,
										BBuffer* buffer);

	// This is supplied to support using this class in a BMediaAddOn.
	// Default version just return NULL.
	virtual	BMediaAddOn*			AddOn(int32* id) const;

	void							SetNotificationHook(notify_hook notifyHook = NULL,
										void* cookie = NULL);

protected:
	virtual void					BufferReceived(BMediaConnection* connection,
										BBuffer* buffer);

	// This is used when the user want to override the BeginConnection
	// mechanism, for example to supply your BMediaConnection derived
	// class. Take ownership of the object.
	virtual void					AddConnection(BMediaConnection* connection);

	// Called from BMediaConnection
			status_t				DisconnectConnection(BMediaConnection* conn);
			status_t				ResetConnection(BMediaConnection* conn);
			status_t				ReleaseConnection(BMediaConnection* conn);
private:

			void					_Init();
			void					_Deinit();

			status_t				_TranslateConnection(BMediaConnection* dest,
										BMediaConnection* source);

			status_t				_Connect(BMediaConnection* conn,
										media_node node);
			status_t				_ConnectInput(BMediaConnection* output,
										input);
			status_t				_ConnectOutput(BMediaConnection* input,
										BMediaConnection* output);

			status_t				fInitErr;

			uint64					fCapabilities;

			bool					fRunning;
			BMediaClientNode*		fNode;

			bigtime_t				fPerformanceTime;
			bigtime_t				fOfflineTime;

			bigtime_t				fMinLatency;
			bigtime_t				fMaxLatency;

			notify_hook				fNotifyHook;

			void*					fNotifyCookie;

			BObjectList<BMediaConnection> fInputs;
			BObjectList<BMediaConnection> fOutputs;

	virtual	void					_ReservedMediaClient0();
	virtual	void					_ReservedMediaClient1();
	virtual	void					_ReservedMediaClient2();
	virtual	void					_ReservedMediaClient3();
	virtual	void					_ReservedMediaClient4();
	virtual	void					_ReservedMediaClient5();
	virtual	void					_ReservedMediaClient6();
	virtual	void					_ReservedMediaClient7();
	virtual	void					_ReservedMediaClient8();
	virtual	void					_ReservedMediaClient9();
	virtual	void					_ReservedMediaClient10();
			uint32					fPadding[64];

	friend class BMediaClientNode;
	friend class BMediaConnection;
};


}

}

using namespace BPrivate::media;

#endif
