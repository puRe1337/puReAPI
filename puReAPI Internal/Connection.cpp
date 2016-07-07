#include "Connection.h"

Connection::Connection( tcp::socket socket ) :
	m_socket( std::move( socket ) ) { }

void Connection::start( ) {
	read( );
	CMessage msg;
	WRITE( msg, eMessage::AddChatMessage );
	std::string texty = "hey";
	WRITE( msg, texty);
	write( msg );
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

			CMessage msg( strData );
			READ( msg, eMessage, type );

			if(type == eMessage::AddChatMessage ) {
				std::stringstream ss;
				READ( msg, std::string, texty );
			}
			read( );
		}
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
