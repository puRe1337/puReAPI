#pragma once
#include "Connection.h"
#include "Config.h"
#include "MessageHandler.h"

#define BIND_MSG(x) g_mMessage[eMessage::x] = std::bind(x, std::placeholders::_1, std::placeholders::_2);

using boost::asio::ip::tcp;

class Server {
public:
	explicit Server( boost::asio::io_service& io );
	void Start( void );

private:
	void handle_accept( const boost::system::error_code& ec );

private:
	tcp::socket m_socket;
	tcp::acceptor m_acceptor;
};
