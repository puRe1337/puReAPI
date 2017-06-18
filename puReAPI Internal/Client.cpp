#include "Client.h"

Client::Client( boost::asio::io_service& io ) :
	m_socket( io ) {}

Client::~Client( ) {
	if ( m_socket.is_open( ) )
		Disconnect( );
}

int Client::Init( ) {
	char szDLLPath[MAX_PATH + 1] = { 0 };
	DWORD dwPId = 0;
	GetModuleFileName( g_hModule, szDLLPath, sizeof szDLLPath );

	HWND hWnd = FindWindow( nullptr, "GTA:SA:MP" );
	if ( !hWnd )
		return 0;
	GetWindowThreadProcessId( hWnd, &dwPId );

	if ( dwPId == 0 )
		return 0;

	auto hHandle = OpenProcess( ( STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFF ), FALSE, dwPId );
	if ( hHandle == 0 || hHandle == INVALID_HANDLE_VALUE )
		return 0;

	auto pAddr = VirtualAllocEx( hHandle, NULL, strlen( szDLLPath ) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if ( pAddr == NULL ) {
		CloseHandle( hHandle );
		return 0;
	}

	BOOL bRetn = WriteProcessMemory( hHandle, pAddr, szDLLPath, strlen( szDLLPath ) + 1, NULL );
	if ( bRetn == FALSE ) {
		VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
		CloseHandle( hHandle );
		return 0;
	}

	DWORD dwBase = 0;

	// Load DLL-file
	{
		auto pFunc = (LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandleA( "kernel32.dll" ), "LoadLibraryA" );
		if ( pFunc == NULL ) {
			VirtualFreeEx( hHandle, pAddr, strlen( szDLLPath ) + 1, MEM_RELEASE );
			CloseHandle( hHandle );
			return 0;
		}

		auto hThread = CreateRemoteThread( hHandle, 0, 0, pFunc, pAddr, 0, 0 );
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

bool Client::Connect( ) {
	tcp::resolver resolver( m_socket.get_io_service( ) );
	auto strPort = std::to_string( Config::g_uiPort );
	auto endpoint_it = resolver.resolve( { Config::g_strIP, strPort } );

	boost::system::error_code ec;

	boost::asio::connect( m_socket, endpoint_it, ec );

	if ( ec ) {
		return false;
	}
	return true;
}

void Client::Disconnect( ) {
	if ( m_socket.is_open( ) ) {
		m_socket.shutdown( boost::asio::socket_base::shutdown_both );
		m_socket.close( );
	}
}

void Client::read( ) {
	boost::system::error_code ec;
	boost::asio::streambuf buff_streambuf;
	boost::asio::read_until( m_socket, buff_streambuf, "\\puReAPI", ec );
	if ( !ec ) {
		std::stringstream ss;
		std::ostream readstream( &buff_streambuf );
		ss << readstream.rdbuf( );
		auto strData = ss.str( );

		boost::replace_first( strData, "\\puReAPI", "" );

		m_msgOut = CMessage( strData );
	}
	Disconnect( );

}

CMessage& Client::outMsg( ) {
	return m_msgOut;
}

void Client::write( const CMessage& msg ) {
	std::string strData = msg.data( );

	boost::system::error_code ec;
	boost::asio::write( m_socket, boost::asio::buffer( strData ), boost::asio::transfer_exactly( strData.size( ) ), ec );
	if ( ec ) {
		Disconnect( );
	}
}
