#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <map>
#include "Message.hpp"

extern std::map< eMessage, std::function< int( CMessage&, CMessage& ) > > g_mMessage;

/* ToDo: Check if Server is available */

uintptr_t FindSignature( const char* szModule, const char* szSignature );

int AddChatMessage( CMessage& in, CMessage& out );
int SendChat( CMessage& in, CMessage& out );
int GetPlayerPos( CMessage& in, CMessage& out );
int ShowGameText( CMessage& in, CMessage& out );
