#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/asio.hpp>

extern HMODULE g_hModule;
extern boost::asio::io_service g_io;
