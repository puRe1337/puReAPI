#include "Calls.h"

boost::asio::io_service g_io;

int AddChatMessage( const char* szText ) {
	auto client = std::make_shared<Client>( g_io );
	if ( client->Connect( ) ) {
		CMessage msg;
		WRITE( msg, eMessage::AddChatMessage );
		WRITE( msg, ( std::string )szText );

		client->write( msg );

		client->read( );
		READ( client->outMsg( ), int, result );
		client->Disconnect( );
		return result;
	}
	g_io.run( );
	return -1;
}

int SendChat( const char* szText ) {
	auto client = std::make_shared<Client>( g_io );
	if ( client->Connect( ) ) {
		CMessage msg;
		WRITE( msg, eMessage::SendChat );
		WRITE( msg, ( std::string )szText );

		client->write( msg );

		client->read( );
		READ( client->outMsg( ), int, result );
		client->Disconnect( );
		return result;
	}
	g_io.run( );
	return -1;
}

int GetPlayerPos( float& fX, float& fY, float& fZ ) {
	auto client = std::make_shared<Client>( g_io );
	if ( client->Connect( ) ) {
		CMessage msg;
		WRITE( msg, eMessage::GetPlayerPos );

		client->write( msg );

		client->read( );
		READ( client->outMsg( ), int, result );
		READ( client->outMsg( ), float, fX_ );
		READ( client->outMsg( ), float, fY_ );
		READ( client->outMsg( ), float, fZ_ );

		fX = fX_;
		fY = fY_;
		fZ = fZ_;
		client->Disconnect( );
		return result;
	}
	g_io.run( );
	return -1;
}

int ShowGameText( const char* szText, int iTime, int iStyle ) {
	auto client = std::make_shared<Client>( g_io );
	if ( client->Connect( ) ) {
		CMessage msg;
		WRITE( msg, eMessage::ShowGameText );
		WRITE( msg, ( std::string )szText );
		WRITE( msg, iTime );
		WRITE( msg, iStyle );

		client->write( msg );

		client->read( );
		READ( client->outMsg( ), int, result );
		client->Disconnect( );
		return result;
	}
	g_io.run( );
	return -1;
}