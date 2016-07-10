#include "Client.h"

Client::Client( boost::asio::io_service& io ) :
	m_socket( io ) {}

Client::~Client( ) {
	if ( m_socket.is_open( ) )
		Disconnect( );
}

bool Client::Connect( ) {

	tcp::resolver resolver( m_socket.get_io_service( ) );
	auto strPort = std::to_string( g_uiPort );
	auto endpoint_it = resolver.resolve( { g_strIP, strPort } );

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
