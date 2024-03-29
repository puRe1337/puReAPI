﻿#pragma once
#define WIN32_LEAN_AND_MEAN

#include <sstream>
#include <fstream>
#include <deque>
#include <thread>
#include <chrono>

#include <Windows.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "Message.hpp"
#include "Config.h"
#include "Globals.h"


using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this< Client >
{
public:
	explicit Client( boost::asio::io_service& io );
	~Client( );
	static int Init( );
	bool Connect( );
	void Disconnect( );

	void read( );
	CMessage& outMsg( );
	void write( const CMessage& msg );
private:
	void handle_read( const boost::system::error_code& ec, std::size_t length );
	void handle_write( const boost::system::error_code& ec, std::size_t length );

private:
	CMessage m_msgOut;
	tcp::socket m_socket;
};
