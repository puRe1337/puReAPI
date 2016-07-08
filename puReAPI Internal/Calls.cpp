#include "Calls.h"

int AddChatMessage( const char* szText ) {
	CMessage msg;
	WRITE( msg, eMessage::AddChatMessage );
	WRITE( msg, ( std::string )szText );
	g_pClient->write( msg );
	return 1;
}

int SendChat( const char* szText ) {
	CMessage msg;
	WRITE( msg, eMessage::SendChat );
	WRITE( msg, ( std::string )szText );
	g_pClient->write( msg );
	return 1;
}

//ToDo: adding response for stuff like GetChatLine, IsChatOpen, IsDialogOpen usw