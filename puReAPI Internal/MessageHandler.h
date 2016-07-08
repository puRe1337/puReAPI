#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>

DWORD FindPattern( DWORD addr, DWORD len, BYTE *bMask, char * szMask );
DWORD GetModuleLength( HMODULE hHandle );

void AddChatMessage( const std::string& strText );
void SendChat( const std::string& strText );
