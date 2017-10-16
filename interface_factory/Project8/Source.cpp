#include "factory.hpp"

bool __stdcall DllMain( HINSTANCE inst, ulong_t reason, void* reserved ) {
	if ( reason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls( inst );
		g_factory.dump_interface_list( );
	}

	return true;
}
