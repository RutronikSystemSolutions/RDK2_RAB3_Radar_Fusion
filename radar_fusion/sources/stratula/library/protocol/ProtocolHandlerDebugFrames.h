#ifndef PROTOCOL_HANDLER_DEBUG_FRAMES_H_
#define PROTOCOL_HANDLER_DEBUG_FRAMES_H_ 1

#include "ProtocolHandler.h"

#include <BoardDefinition.h>

#include <impl/chrono.h>
#include <universal/data_definitions.h>


#ifdef PROTOCOL_DEBUG_FRAMES
#define ProtocolHandler_sendDebugFrame(message) ProtocolHandler_sendDebugFrameImpl(message, VIRTUAL_CHANNEL_UNDEFINED, chrono_ticks_to_microseconds(chrono_now()))
#else
#define ProtocolHandler_sendDebugFrame(message)
#endif


#endif /* PROTOCOL_HANDLER_DEBUG_FRAMES_H_ */
