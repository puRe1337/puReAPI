#include "Calls.h"

int AddChatMessage( const char* szText ) {
	boost::asio::io_service io;
	auto client = std::make_shared<Client>( io );
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
	return -1;
}

int SendChat( const char* szText ) {
	boost::asio::io_service io;
	auto client = std::make_shared<Client>( io );
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
	return -1;
}

int GetPlayerPos( float& fX, float& fY, float& fZ ) {
	boost::asio::io_service io;
	auto client = std::make_shared<Client>( io );
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
	return -1;
}