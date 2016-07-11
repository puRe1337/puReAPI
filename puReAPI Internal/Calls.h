#pragma once
#include "Client.h"
#include "Globals.h"

#define EXPORTDLL extern "C" __declspec( dllexport )

EXPORTDLL int AddChatMessage( const char* szText );
EXPORTDLL int SendChat( const char* szText );
EXPORTDLL int GetPlayerPos( float& fX, float& fY, float& fZ );
EXPORTDLL int ShowGameText( const char* szText, int iTime, int iStyle );