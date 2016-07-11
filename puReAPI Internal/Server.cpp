#include "Server.h"

Server::Server( boost::asio::io_service& io ) :
	m_acceptor( io, tcp::endpoint( tcp::v4( ), g_uiPort ), false ),
	m_socket( io ) { }

void Server::Start( ) {
	if ( g_mMessage.empty( ) ) {
		BIND_MSG( AddChatMessage );
		BIND_MSG( SendChat );
		BIND_MSG( GetPlayerPos );
		BIND_MSG( ShowGameText );
	}
	m_acceptor.async_accept( m_socket, std::bind( &Server::handle_accept, this, std::placeholders::_1 ) );
}

void Server::handle_accept( const boost::system::error_code& ec ) {
	if ( !ec ) {
		std::make_shared< Connection >( std::move( m_socket ) )->start( );
	}
	Start( );
}
