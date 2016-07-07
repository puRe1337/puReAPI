#include "Client.h"

Client::Client( boost::asio::io_service& io ) :
	m_socket( io ) { }

bool Client::Connect( const std::string& ip, const uint32_t port ) {

	tcp::resolver resolver( m_socket.get_io_service( ) );
	auto strPort = std::to_string( port );
	auto endpoint_it = resolver.resolve( { ip, strPort } );

	boost::system::error_code ec;

	logFn( "Trying to connect" );
	boost::asio::connect( m_socket, endpoint_it, ec );

	if ( ec ) {
		logFn( "Connecting failed" );
		return false;
	}
	logFn( "Connected" );
	return true;
}

void Client::startRead( ) {
	read( );
}

void Client::read( ) {
	//m_socket.async_read_some( boost::asio::buffer( m_buf, m_buf.size( ) ), std::bind( &Client::handle_read, shared_from_this( ), std::placeholders::_1, std::placeholders::_2 ) );
	boost::asio::async_read_until( m_socket, m_buff_streambuf, "\\puReAPI", std::bind( &Client::handle_read, shared_from_this( ), std::placeholders::_1, std::placeholders::_2 ) );
}

void Client::write( const CMessage& msg ) {
	std::string strData = msg.data( );
	bool write_in_progress = !m_message_queue.empty( );
	m_message_queue.push_back( strData );
	if ( !write_in_progress ) {
		do_write( );
	}
}

void Client::handle_read( const boost::system::error_code& ec, std::size_t length ) {
	if ( !ec ) {
		if ( length > 0 ) {
			std::stringstream ss;
			std::ostream readstream( &m_buff_streambuf );
			ss << readstream.rdbuf( );
			auto strData = ss.str( );

			boost::replace_first( strData, "\\puReAPI", "" );


			CMessage msg( strData );
			READ( msg, eMessage, type );

			if ( type == eMessage::AddChatMessage ) {
				READ( msg, std::string, texty );
				std::stringstream ss_;
				ss_ << "Client " << texty;
				logFn( ss_.str( ) );
			}

			read( );
		}
	}
}

void Client::do_write( ) {
	boost::asio::async_write( m_socket,
	                          boost::asio::buffer( m_message_queue.front( ) ),
	                          boost::asio::transfer_exactly( m_message_queue.front( ).size( ) ),
	                          std::bind( &Client::handle_write, shared_from_this( ), std::placeholders::_1, std::placeholders::_2 ) );
}

void Client::handle_write( const boost::system::error_code& ec, std::size_t length ) {
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
