#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

enum class eMessage : int {
	SendChat = 1,
	AddChatMessage,
	GetPlayerPos,
	ShowGameText
};

#define READ(MSG, TYPE, VARNAME) TYPE VARNAME; MSG >> VARNAME;
#define WRITE(MSG, VAR) MSG << VAR;

class CMessage {
	typedef boost::archive::text_iarchive iarchive;
	typedef boost::archive::text_oarchive oarchive;
public:
	CMessage( void ) {
		m_oarchive = std::make_shared< oarchive >( m_ss );
	}

	CMessage( std::string& data ) : m_ss( data ) {
		m_iarchive = std::make_shared< iarchive >( m_ss );
	}

	CMessage& operator =( const CMessage& msg ) {
		m_ss = std::stringstream( msg.data( ) );
		if ( !m_iarchive )
			m_iarchive = std::make_shared< iarchive >( m_ss );
		return *this;
	}

	~CMessage( ) {
		m_oarchive.reset( );
		m_iarchive.reset( );
	}

	template <class T>
	CMessage& operator<<( const T& t ) {
		( *m_oarchive ) & t;
		return *this;
	}

	template <class T>
	CMessage& operator >>( T& t ) {
		( *m_iarchive ) & t;
		return *this;
	}

	std::string data( ) const {
		return m_ss.str( );
	}

private:
	std::stringstream m_ss;
	std::shared_ptr< iarchive > m_iarchive;
	std::shared_ptr< oarchive > m_oarchive;
};
