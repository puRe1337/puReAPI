#include "Server.h"
#include "Client.h"


void Init( void ) { }

int WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved ) {
	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH:
			CreateThread( 0, 0, 0, ( LPTHREAD_START_ROUTINE )Init, 0, 0 );
			break;
	}
	return TRUE;
}
