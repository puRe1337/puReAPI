#include "Server.h"
#include "Client.h"
#include "Globals.h"
#include <chrono>

void StartServer( ) {
	/*
	we're inside gta process
	*/
	Server server( g_io );
	server.Start( );
	g_io.run( );
}

void Init( ) {
	HWND hWnd = FindWindow( 0, "GTA:SA:MP" );
	if ( hWnd ) {
		DWORD dwPid;
		GetWindowThreadProcessId( hWnd, &dwPid );
		if ( dwPid && dwPid == GetCurrentProcessId( ) ) {
			StartServer( );
		}
	}
}

int WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved ) {
	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH:
			g_hModule = (HMODULE)hInstance;
			DisableThreadLibraryCalls( (HMODULE)hInstance );
			CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0 );
			break;
	}
	return TRUE;
}
