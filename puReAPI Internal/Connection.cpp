#include "Connection.h"
#include "Client.h"

Connection::Connection( tcp::socket socket ) :
	m_socket( std::move( socket ) ) { }

void Connection::start( ) {
	read( );
}

void Connection::read( ) {
	m_socket.async_read_some( boost::asio::buffer( m_buf, m_buf.size( ) ), std::bind( &Connection::handle_read, shared_from_this( ), std::placeholders::_1, std::placeholders::_2 ) );
}

void Connection::write( const CMessage& msg ) {
	std::string strData = msg.data( ) + "\\puReAPI";
	bool write_in_progress = !m_message_queue.empty( );
	m_message_queue.push_back( strData );
	if ( !write_in_progress ) {
		do_write( );
	}
}

void Connection::handle_read( const boost::system::error_code& ec, std::size_t length ) {
	if ( !ec ) {
		if ( length > 0 ) {
			std::string strData( m_buf.begin( ), m_buf.end( ) );
			strData.resize( length );

			CMessage inMsg( strData );
			READ( inMsg, eMessage, type );

			try {
				auto it = g_mMessage.find( type );
				if ( it == g_mMessage.end( ) )
					return;
				if ( !g_mMessage[ type ] )
					return;

				CMessage outMsg;
				int result = g_mMessage[ type ]( inMsg, outMsg );
				WRITE( outMsg, result );
				write( outMsg );

			}
			catch ( ... ) { }
			read( );
		}
	}
	else {
		m_socket.close( );
	}
}

void Connection::do_write( ) {
	boost::asio::async_write( m_socket,
	                          boost::asio::buffer( m_message_queue.front( ) ),
	                          boost::asio::transfer_exactly( m_message_queue.front( ).size( ) ),
	                          std::bind( &Connection::handle_write, shared_from_this( ), std::placeholders::_1, std::placeholders::_2 ) );
}

void Connection::handle_write( const boost::system::error_code& ec, std::size_t length ) {
	if ( ec ) {
		if ( m_socket.is_open( ) ) {
			m_socket.close( );
		}
	}
	else {
		m_message_queue.pop_front( );
		if ( !m_message_queue.empty( ) ) {
			do_write( );
		}
	}
}
