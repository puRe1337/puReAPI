#pragma once
#include "Client.h"

#define EXPORTDLL extern "C" __declspec( dllexport )

EXPORTDLL int AddChatMessage( const char* szText );
EXPORTDLL int SendChat( const char* szText );