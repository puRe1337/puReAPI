#include "Server.h"

Server::Server( boost::asio::io_service& io, uint32_t port ) :
	m_acceptor( io, tcp::endpoint( tcp::v4( ), port ), false ),
	m_socket( io ) { }

void Server::Start( ) {
	m_acceptor.async_accept( m_socket, std::bind( &Server::handle_accept, this, std::placeholders::_1 ) );
}

void Server::handle_accept( const boost::system::error_code& ec ) {
	if ( !ec ) {
		std::make_shared< Connection >( std::move( m_socket ) )->start( );
	}
	Start( );
}
