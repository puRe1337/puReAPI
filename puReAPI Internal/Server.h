#pragma once
#include "Connection.h"

using boost::asio::ip::tcp;

class Server {
public:
	explicit Server( boost::asio::io_service& io, uint32_t port );
	void Start( void );

private:
	void handle_accept( const boost::system::error_code& ec );

private:
	tcp::socket m_socket;
	tcp::acceptor m_acceptor;
};
