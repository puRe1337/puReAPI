#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>
#include "Message.hpp"

class Server {
public:
	void Start( uint32_t port );
};
