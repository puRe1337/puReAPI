#include "Client.h"

Client::Client( boost::asio::io_service& io ) :
	m_socket( io )
{
	
}

bool Client::Connect( const std::string& ip, const uint32_t port ) {
	tcp::resolver resolver( m_socket.get_io_service( ) );
	auto strPort = std::to_string( port );
	auto endpoint_it = resolver.resolve( { ip, strPort } );

	boost::system::error_code ec;

	boost::asio::connect( m_socket, endpoint_it, ec );
	if ( ec ) {
		return false;
	}
	else {
		return true;
	}
}
