#include "Server.h"
#include "Client.h"
#include <chrono>
#include <thread>

int Inject( HMODULE hMod ) {
	char szDLLPath[ MAX_PATH + 1 ] = { 0 };
	DWORD dwPId = 0;
	BOOL bRetn;
	GetModuleFileName( ( HMODULE )hMod, szDLLPath, sizeof( szDLLPath ) );

	HWND hWnd = FindWindow( 0, "GTA:SA:MP" );
	if ( !hWnd )
		return 0;
	GetWindowThreadProcessId( hWnd, &dwPId );

	if ( dwPId == 0 )
		return 0;

	HANDLE hHandle = OpenProcess( ( STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFF ), FALSE, dwPId );
	if ( hHandle == 0 || hHandle == INVALID_HANDLE_VALUE )
		return 0;

	void* pAddr = VirtualAllocEx( hHandle, NULL, strlen( szDLLPath ) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if ( pAddr == NULL ) {
		CloseHandle( hHandle );
		return 0;
	}

	bRetn = WriteProcessMemory( hHandle, pAddr, szDLLPath, strlen( szDLLPath ) + 1, NULL );
	if ( bRetn == FALSE ) {
		VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
		CloseHandle( hHandle );
		return 0;
	}

	DWORD dwBase = 0;

	// Load DLL-file
	{
		LPTHREAD_START_ROUTINE pFunc = ( LPTHREAD_START_ROUTINE )GetProcAddress( GetModuleHandleA( "kernel32.dll" ), "LoadLibraryA" );
		if ( pFunc == NULL ) {
			VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
			CloseHandle( hHandle );
			return 0;
		}

		HANDLE hThread = CreateRemoteThread( hHandle, 0, 0, pFunc, pAddr, 0, 0 );
		if ( hThread == NULL || hThread == INVALID_HANDLE_VALUE ) {
			VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
			CloseHandle( hHandle );
			return 0;
		}

		WaitForSingleObject( hThread, INFINITE );
		GetExitCodeThread( hThread, &dwBase );
		VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
		CloseHandle( hThread );
	}
	return 1;
}

void StartServer( ) {
	/*
	we're inside gta process
	*/
	boost::asio::io_service io;
	Server server( io, 8055 );
	server.Start( );
	io.run( );
}

void Init( HMODULE hMod ) {

	HWND hWnd = FindWindow( 0, "GTA:SA:MP" );
	if ( hWnd ) {
		DWORD dwPid;
		GetWindowThreadProcessId( hWnd, &dwPid );
		if ( dwPid && dwPid == GetCurrentProcessId( ) ) {
			StartServer( );
		}
		else {
			/*
			client aka keybinder
			*/
			boost::asio::io_service io;
			g_pClient = std::make_shared< Client >( io );
			bool connected = g_pClient->Connect( "127.0.0.1", 8055 );
			while ( !connected ) {
				/*Can't connect, load dll and start server*/
				int res = Inject( hMod );
				while ( !res ) {
					res = Inject( hMod );
					std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
				}

				connected = g_pClient->Connect( "127.0.0.1", 8055 );
				std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
			}
			g_pClient->startRead( );
			io.run( );
		}
	}
}

int WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved ) {
	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( ( HMODULE )hInstance );
			CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )Init, hInstance, 0, 0 );
			break;
	}
	return TRUE;
}
