#include "MessageHandler.h"

std::map< eMessage, std::function< int( CMessage&, CMessage& ) > > g_mMessage;

uintptr_t FindSignature( const char* szModule, const char* szSignature ) {
	//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation( GetCurrentProcess( ), GetModuleHandleA( szModule ), &modInfo, sizeof( MODULEINFO) );
	DWORD startAddress = ( DWORD )modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for ( DWORD pCur = startAddress; pCur < endAddress; pCur++ ) {
		if ( !*pat ) return firstMatch;
		if ( *( PBYTE )pat == '\?' || *( BYTE* )pCur == getByte( pat ) ) {
			if ( !firstMatch ) firstMatch = pCur;
			if ( !pat[ 2 ] ) return firstMatch;
			if ( *( PWORD )pat == '\?\?' || *( PBYTE )pat != '\?' ) pat += 3;
			else pat += 2; //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}


int AddChatMessage( CMessage& in, CMessage& out ) {
	static auto dwAddress = FindSignature( "samp.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 57 33 C0 B9 ? ? ? ? 8D 7C 24 08 F3 AB 8B 4D 0C 8D 45 10 50 51 8D 54 24 10 52 E8 ? ? ? ? 8A 4C 24 14 83 C4 0C 84 C9 8D 44 24 08 74 1B EB 03 8D 49 00 8A 08 84 C9 7E 08 80 F9 20 7D 03 C6 00 20 8A 48 01 40 84 C9 75 EA 8B 4D 08 8B 81 ? ? ? ? 6A 00 50 6A 00 8D 54 24 14 52 6A 08" )/* wir wollen ja direkt die funktion aufrufen - dwSAMP*/;
	static auto dwChatInfo = *(uintptr_t*)( *(uintptr_t*)( FindSignature( "samp.dll", "A1 ? ? ? ? 68 ? ? ? ? 50 E8 ? ? ? ? 83 C4 08 C2 04 00" ) + 0x1 )/* - dwSAMP*/ );
	if ( !dwChatInfo || !dwAddress )
		return 0;

	READ( in, std::string, strText );
	auto text = strText.c_str( );

	__asm mov ecx, [ dwChatInfo ]
	__asm push text
	__asm push ecx
	__asm call dwAddress
	__asm add esp, 8

	return 1;
}

int SendChat( CMessage& in, CMessage& out ) {
	static auto dwAddressText = FindSignature( "samp.dll", "64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 81 EC ? ? ? ? 53 56 8B B4 24 ? ? ? ? 8B C6" );
	static auto dwAddressCmd = FindSignature( "samp.dll", "64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 A1 ? ? ? ? 64 89 25 ? ? ? ? 81 EC ? ? ? ? 85 C0" );
	if ( !dwAddressText || !dwAddressCmd )
		return 0;

	READ( in, std::string, strText );

	DWORD dwAddress = strText.at( 0 ) == '/' ? dwAddressCmd : dwAddressText;
	auto text = strText.c_str( );

	__asm push text
	__asm call dwAddress
	return 1;
}

int GetPlayerPos( CMessage& in, CMessage& out ) {
	uintptr_t cped = *(uintptr_t*)0xB6F5F0;
	if ( !cped )
		return 0;
	uintptr_t mtrx = *(uintptr_t*)( cped + 0x14 );
	if ( !mtrx )
		return 0;

	float fX, fY, fZ;

	fX = *(float*)( mtrx + 0x30 );
	fY = *(float*)( mtrx + 0x34 );
	fZ = *(float*)( mtrx + 0x38 );

	WRITE( out, fX );
	WRITE( out, fY );
	WRITE( out, fZ );

	return 1;
}

int ShowGameText( CMessage& in, CMessage& out ) {
	static auto dwAddress = FindSignature( "samp.dll", "55 8B EC 81 7D" ); /*"55 8B EC 81 7D ? ? ? ? ? 7F 36 68 ? ? ? ? E8"*/
	if ( !dwAddress )
		return 0;

	READ( in, std::string, strText );
	READ( in, int, iTime );
	READ( in, int, iStyle );

	auto text = strText.c_str( );

	__asm push iStyle
	__asm push iTime
	__asm push text
	__asm call dwAddress

	return 1;
}
