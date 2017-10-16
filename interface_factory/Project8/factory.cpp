#include <algorithm>

#include "factory.hpp"

factory::interfaces::c_interface_manager g_factory;

NAMESPACE_REGION( factory )
NAMESPACE_REGION( interfaces )

c_interface_manager::c_interface_manager( ) {
	auto teb = reinterpret_cast< PTEB >( __readfsdword( uintptr_t( &static_cast< NT_TIB* >( nullptr )->Self ) ) );
	auto peb = teb->ProcessEnvironmentBlock;

	auto root = &peb->Ldr->InMemoryOrderModuleList;
	//iterate module list
	for ( auto entry = root->Flink->Flink->Flink->Flink; entry != root; entry = entry->Flink ) {
		PLDR_DATA_TABLE_ENTRY	data_table;
		HMODULE					module_base;
		uintptr_t				create_interface_export;
		uintptr_t				create_interface_;
		uintptr_t*				list_iterator_ptr;
		interface_iterator_t*	list_iterator;

		data_table = reinterpret_cast< PLDR_DATA_TABLE_ENTRY >( entry );
		module_base = reinterpret_cast< HMODULE >( data_table->Reserved2[ 0 ] );
		create_interface_export = uintptr_t( GetProcAddress( module_base, "CreateInterface" ) );

		if ( !create_interface_export || !is_createinterface_export( create_interface_export ) ) {
			continue;
		}

		//find the createinterface function
		create_interface_ = follow_createinterface_export( create_interface_export );
		if ( !is_createinterface_fn( create_interface_ ) ) {
			continue;
		}

		//find the list iterator
		list_iterator_ptr = find_list_ptr( create_interface_ );

		//iterate the interface list
		for ( list_iterator = reinterpret_cast< interface_iterator_t* >(
			list_iterator_ptr );
			!!list_iterator;
			list_iterator = list_iterator->m_next
			) {
			std::string name( list_iterator->m_name );
			std::string module_name( util::unicode_to_ascii(
				std::wstring( data_table->FullDllName.Buffer, data_table->FullDllName.Length ) ) );

			uintptr_t ptr = static_cast< uintptr_t( *)( ) >( list_iterator->m_create_fn )( );

			size_t version = [ & ]( ) {
				std::string ret( name );
				ret.erase( std::remove_if( ret.begin( ), ret.end( ),
					[ & ]( int i ) { return !::isdigit( i ); }
				), ret.end( ) );
				return atoi( ret.c_str( ) );
			}( );

			m_interfaces.push_back( interface_data_t{ name, module_name, version, ptr } );
		}
	}
}

END_REGION
END_REGION