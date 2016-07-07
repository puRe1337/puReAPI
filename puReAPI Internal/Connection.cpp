#include "Connection.h"

Connection::Connection( tcp::socket socket ) :
	m_socket( std::move( socket ) ) { }

void Connection::start( ) { }
