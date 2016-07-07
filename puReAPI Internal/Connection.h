#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>
#include "Message.hpp"
#include <deque>

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this< Connection > {
public:
	explicit Connection( tcp::socket socket );
	void start( void );

	void read( );
	void write( const CMessage& msg );

private:
	void handle_read( const boost::system::error_code& ec, std::size_t length );
	void do_write( );
	void handle_write( const boost::system::error_code& ec, std::size_t length );

private:
	tcp::socket m_socket;
	std::deque< std::string > m_message_queue;
	std::array< char, 4096 > m_buf;
};
