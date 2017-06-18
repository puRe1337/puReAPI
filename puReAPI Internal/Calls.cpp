#include "Calls.h"

int AddChatMessage( const char* szText ) {
	auto client = std::make_shared< Client >( g_io );
	CHECK_SERVER(client, 0);
	CMessage msg;
	WRITE( msg, eMessage::AddChatMessage );
	WRITE( msg, ( std::string )szText );
	client->write( msg );

	client->read( );
	READ( client->outMsg( ), int, result );
	g_io.run( ); // maybe redundant
	return result;
}

int SendChat( const char* szText ) {
	auto client = std::make_shared< Client >( g_io );
	CHECK_SERVER(client, 0);
	CMessage msg;
	WRITE( msg, eMessage::SendChat );
	WRITE( msg, ( std::string )szText );

	client->write( msg );

	client->read( );
	READ( client->outMsg( ), int, result );
	g_io.run( );
	return result;
}

int GetPlayerPos( float& fX, float& fY, float& fZ ) {
	auto client = std::make_shared< Client >( g_io );
	CHECK_SERVER(client, 0);
	CMessage msg;
	WRITE( msg, eMessage::GetPlayerPos );

	client->write( msg );

	client->read( );
	READ( client->outMsg( ), float, fX_ );
	READ( client->outMsg( ), float, fY_ );
	READ( client->outMsg( ), float, fZ_ );
	READ(client->outMsg(), int, result);

	fX = fX_;
	fY = fY_;
	fZ = fZ_;
	g_io.run( );
	return result;
}

int ShowGameText( const char* szText, int iTime, int iStyle ) {
	auto client = std::make_shared< Client >( g_io );
	CHECK_SERVER(client,0);
	CMessage msg;
	WRITE( msg, eMessage::ShowGameText );
	WRITE( msg, ( std::string )szText );
	WRITE( msg, iTime );
	WRITE( msg, iStyle );

	client->write( msg );

	client->read( );
	READ( client->outMsg( ), int, result );
	g_io.run( );
	return result;
}
