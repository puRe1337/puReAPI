#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>
#include "Message.hpp"

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this< Connection > {
public:
	explicit Connection( tcp::socket socket );
	void start( void );

private:
	tcp::socket m_socket;
};
