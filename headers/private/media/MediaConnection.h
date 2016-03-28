/*
 * Copyright 2015, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _MEDIA_CONNECTION_H
#define _MEDIA_CONNECTION_H

#include <BufferGroup.h>
#include <MediaDefs.h>

#include "MediaClientNode.h"


namespace BPrivate { namespace media {


enum media_connection_kind {
	B_MEDIA_INPUT = 0,
	B_MEDIA_OUTPUT = 1
};


class BMediaClient;

// The BMediaConnection class is the swiss knife of BMediaClient.
// It represents a connection between two nodes and allow to create complex
// nodes without dealing with the unneeded complexity. Two local connections,
// can be binded, this means that when you will receive a buffer A as input,
// the Process function will be called so that you can
// process the BBuffer, and once the function returns the output
// will be automatically forwarded to the connection B.
// If you don't bind a connection, you have to call yourself the
// BMediaClient::SendBuffer() method or call BMediaClient::Recycle to
// recycle the buffer when you don't want to do anything further.
class BMediaConnection {
public:
	enum notification {
		B_CONNECTED = 1,
		B_DISCONNECTED,

		B_PREPARE_TO_CONNECT,	// media_format* format, media_source* source,
								// char* name
		B_CONNECT,
		B_DISCONNECT,

		B_FORMAT_PROPOSAL,		// media_format* format

		B_ASK_FORMAT_CHANGE,
		B_FORMAT_CHANGED,
		B_ASK_TIMER
	};

	// This function is called when it has come the moment to handle a buffer.
	typedef void					(*process_hook)(BMediaConnection* connection,
										BBuffer* buffer);

	// Used to notify or inquire the client about what to do when certain
	// events happen.
	typedef status_t				(*notify_hook)(notification what,
										BMediaConnection* connection,
										...);

	virtual							~BMediaConnection();

	// TODO: while most of the objects for both kinds are common
	// it would be worthwile to have a private implementation
	// so that we can better model the differences and avoid
	// problems.
	bool 							IsOutput() const;
	bool							IsInput() const;

	const media_destination& 		Destination() const;
	const media_source&				Source() const;

	bool							HasBinding() const;
	BMediaConnection*				Binding() const;

	// This allow to specify a format that will be used while
	// connecting to another node. See BMediaClient::SetFormat.
	void							SetAcceptedFormat(
										const media_format& format);
	const media_format&				AcceptedFormat() const;

	// Represents the buffer size used by the media_node
	void							SetBufferSize(size_t size);
	size_t							BufferSize() const;

	// Represents the duration of one buffer
	void							SetBufferDuration(bigtime_t duration);
	bigtime_t						BufferDuration() const;

	bool							IsConnected() const;

	void							SetOutputEnabled(bool enabled);
	bool							IsOutputEnabled() const;

	void							SetCookie(void* cookie);
	void*							Cookie() const;

	// Disconnect this connection.
	status_t						Disconnect();

	// TODO: We really need a Reset()?
	// When you reset a connection it can be reused as it was brand new.
	status_t						Reset();

	// Once you are done with this connection you release it, it automatically
	// remove the object from the BMediaClient and free all used resources.
	// This will make the connection to disappear completely, so if you
	// want to preserve it for future connections just Disconnect() and
	// Reset() it.
	status_t						Release();

	// Use this to set your callbacks.
	void							SetHooks(process_hook processHook = NULL,
										notify_hook notifyHook = NULL,
										void* cookie = NULL);

protected:
									BMediaConnection(BMediaClient* owner,
										media_connection_kind kind);
									BMediaConnection(BMediaClient* owner,
										const media_output& output);
									BMediaConnection(BMediaClient* owner,
										const media_input& input);

	// TODO: All notifications should be done into private callbacks like this.
	void							ConnectedCallback(const media_source& source,
											const media_format& format);
	void							DisconnectedCallback(const media_source& source);

	void							ConnectCallback(const media_destination& source);
	void							DisconnectCallback(const media_destination& source);

private:
	void							BuildMediaOutput(media_output* output) const;
	void							BuildMediaInput(media_input* output) const;

	void							_Init();

	bool							fConnected;
	bool							fOutputEnabled;

	media_connection_kind			fKind;
	BMediaClient*					fOwner;
	media_node						fOwnerNode;
	team_id							fOwnerTeam;

	// A connection might be binded so that it will automatically
	// forward or receive the data from/to a local BMediaConnection,
	// see BMediaClient::Bind.
	BMediaConnection*				fBind;

	process_hook					fProcessHook;
	notify_hook						fNotifyHook;
	void*							fBufferCookie;

	media_source					fSource;
	media_destination				fDestination;

	// This represents the node at other end of connection.
	media_node						fRemoteNode;

	media_format					fFormat;
	size_t							fBufferSize;
	bigtime_t						fBufferDuration;

	BBufferGroup*					fBufferGroup;

	virtual	void					_ReservedMediaConnection0();
	virtual	void					_ReservedMediaConnection1();
	virtual	void					_ReservedMediaConnection2();
	virtual	void					_ReservedMediaConnection3();
	virtual	void					_ReservedMediaConnection4();
	virtual	void					_ReservedMediaConnection5();
	virtual	void					_ReservedMediaConnection6();
	virtual	void					_ReservedMediaConnection7();
	virtual	void					_ReservedMediaConnection8();
	virtual	void					_ReservedMediaConnection9();
	virtual	void					_ReservedMediaConnection10();
	uint32							fPadding[64];

	friend class BMediaClient;
	friend class BMediaClientNode;
};


}

}

using namespace BPrivate::media;

#endif
