#include "MessageHandler.h"

DWORD FindPattern( DWORD addr, DWORD len, BYTE* bMask, char* szMask ) {
	auto dataCompare = []( const BYTE* pData, const BYTE* bMask, const char* szMask ) -> bool {
		for ( ; *szMask; ++szMask , ++pData , ++bMask )
			if ( *szMask == 'x' && *pData != *bMask )
				return false;
		return ( *szMask ) == NULL;
	};
	for ( DWORD i = 0; i < len; i++ )
		if ( dataCompare( ( BYTE* )( addr + i ), bMask, szMask ) )
			return ( DWORD )( addr + i );
	return 0;
}

DWORD GetModuleLength( HMODULE hHandle ) {
	MODULEINFO info;
	GetModuleInformation( GetCurrentProcess( ), hHandle, &info, sizeof( info ) );
	return info.SizeOfImage;
}

int AddChatMessage( const std::string& strText ) {
	static DWORD dwSAMP = ( DWORD )GetModuleHandle( "samp.dll" );
	static DWORD dwSize = GetModuleLength( ( HMODULE )dwSAMP );
	static auto dwAddress = FindPattern( dwSAMP, dwSize, ( PBYTE )"\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x57\x33\xC0\xB9\x00\x00\x00\x00\x8D\x7C\x24\x08\xF3\xAB\x8B\x4D\x0C\x8D\x45\x10\x50\x51\x8D\x54\x24\x10\x52\xE8\x00\x00\x00\x00\x8A\x4C\x24\x14\x83\xC4\x0C\x84\xC9\x8D\x44\x24\x08\x74\x1B\xEB\x03\x8D\x49\x00\x8A\x08\x84\xC9\x7E\x08\x80\xF9\x20\x7D\x03\xC6\x00\x20\x8A\x48\x01\x40\x84\xC9\x75\xEA\x8B\x4D\x08\x8B\x81\x00\x00\x00\x00\x6A\x00\x50\x6A\x00\x8D\x54\x24\x14\x52\x6A\x08",
	                                     "xxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx" )/* wir wollen ja direkt die funktion aufrufen - dwSAMP*/;
	static auto dwChatInfo = *( DWORD* )( *( DWORD* )( FindPattern( dwSAMP, dwSize, ( PBYTE )"\xA1\x00\x00\x00\x00\x68\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x83\xC4\x08\xC2\x04\x00", "x????x????xx????xxxxxx" ) + 0x1 )/* - dwSAMP*/ );
	if ( !dwChatInfo || !dwAddress )
		return 0;
	auto text = strText.c_str( );

	__asm mov ecx, [ dwChatInfo ]
	__asm push text
	__asm push ecx
	__asm call dwAddress
	__asm add esp, 8
	return 1;
}

int SendChat( const std::string& strText ) {
	static DWORD dwSAMP = ( DWORD )GetModuleHandle( "samp.dll" );
	static DWORD dwSize = GetModuleLength( ( HMODULE )dwSAMP );
	static auto dwAddressText = FindPattern( dwSAMP, dwSize, ( PBYTE )"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x56\x8B\xB4\x24\x00\x00\x00\x00\x8B\xC6", "xx????xxx????xxxx????xx????xxxxx????xx" );
	static auto dwAddressCmd = FindPattern( dwSAMP, dwSize, ( PBYTE )"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\xA1\x00\x00\x00\x00\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x85\xC0", "xx????xxx????xx????xxx????xx????xx" );
	if ( !dwAddressText || !dwAddressCmd )
		return 0;
	DWORD dwAddress = strText.at( 0 ) == '/' ? dwAddressCmd : dwAddressText;
	auto text = strText.c_str( );

	__asm push text
	__asm call dwAddress
	return 1;
}

int GetPlayerPos( float& fX, float& fY, float& fZ ) {
	uintptr_t cped = *( uintptr_t* )0xB6F5F0;
	if ( !cped )
		return 0;
	uintptr_t mtrx = *( uintptr_t* )( cped + 0x14 );
	if ( !mtrx )
		return 0;
	fX = *( float* )( mtrx + 0x30 );
	fY = *( float* )( mtrx + 0x34 );
	fZ = *( float* )( mtrx + 0x38 );
	return 1;
}
