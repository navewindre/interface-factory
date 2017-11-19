#pragma once

#define NAMESPACE_REGION( x ) namespace x {
#define END_REGION }

//WEE WOO WEE WOO ITS THE DWORD POLICE
using ulong_t = unsigned long;
using uword_t = unsigned short;

NAMESPACE_REGION( util )

template < typename fn > __forceinline fn get_vfunc( void* classbase, int index ) {
	if ( !classbase ) return fn{ };
	return ( fn )( ( uintptr_t* )*( uintptr_t** )classbase )[ index ];
}

static __forceinline std::string unicode_to_ascii( const std::wstring& unicode ) {
	std::string ascii_str( unicode.begin( ), unicode.end( ) );
	return ascii_str;
}

static __forceinline std::wstring ascii_to_unicode( const std::string& ascii ) {
	std::wstring unicode_str( ascii.begin( ), ascii.end( ) );
	return unicode_str;
}

END_REGION
