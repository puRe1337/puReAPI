#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <map>
#include "Message.hpp"

extern std::map< eMessage, std::function< int( CMessage&, CMessage& ) > > g_mMessage;

/* ToDo: Check if Server is available */

DWORD FindPattern( DWORD addr, DWORD len, BYTE* bMask, char* szMask );
DWORD GetModuleLength( HMODULE hHandle );

int AddChatMessage( CMessage& in, CMessage& out );
int SendChat( CMessage& in, CMessage& out );
int GetPlayerPos( CMessage& in, CMessage& out );
