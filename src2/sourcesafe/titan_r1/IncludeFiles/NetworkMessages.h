/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Network
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __NETWORK_MESSAGES
#define __NETWORK_MESSAGES

#define EM_NETWORK_MESSAGE_BASE_OFFSET 0x30000000

const uint32 INCOMING_MESSAGE = EM_NETWORK_MESSAGE_BASE_OFFSET + 1;
const uint32 INCOMING_CHAT_LIST = EM_NETWORK_MESSAGE_BASE_OFFSET + 2;
const uint32 INPUT_TO_CHAT = EM_NETWORK_MESSAGE_BASE_OFFSET + 3;
const uint32 INPUT_CONNECT = EM_NETWORK_MESSAGE_BASE_OFFSET + 4;

#endif