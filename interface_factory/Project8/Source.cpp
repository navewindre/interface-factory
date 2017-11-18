#include "factory.hpp"

bool __stdcall DllMain( HINSTANCE instance, ulong_t reason, void* reserved ) {
    switch ( reason ) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls( instance );
        g_factory.dump_interface_list( );

        return true;
        break;

    default:
        return true;
        break;
    }
}
