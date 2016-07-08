#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "Message.hpp"
#include <sstream>
#include <fstream>
#include <deque>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this< Client > {
public:
	explicit Client( boost::asio::io_service& io );
	bool Connect( const std::string& ip, const uint32_t port );

	void startRead( );

	void read( );
	void write( const CMessage& msg );
private:
	void handle_read( const boost::system::error_code& ec, std::size_t length );
	void do_write( );
	void handle_write( const boost::system::error_code& ec, std::size_t length );

private:
	tcp::socket m_socket;
	std::deque< std::string > m_message_queue;
	boost::asio::streambuf m_buff_streambuf;
};
extern std::shared_ptr<Client> g_pClient;