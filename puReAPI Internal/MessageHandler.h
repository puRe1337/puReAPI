#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>

DWORD FindPattern( DWORD addr, DWORD len, BYTE *bMask, char * szMask );
DWORD GetModuleLength( HMODULE hHandle );

int AddChatMessage( const std::string& strText );
int SendChat( const std::string& strText );
int GetPlayerPos( float& fX, float& fY, float& fZ );
