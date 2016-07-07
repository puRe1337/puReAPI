﻿#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>
#include "Message.hpp"

using boost::asio::ip::tcp;

class Client {
public:
	explicit Client( boost::asio::io_service& io );
	bool Connect( const std::string& ip, const uint32_t port );
private:
	tcp::socket m_socket;
};
