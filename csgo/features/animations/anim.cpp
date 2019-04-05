#include "anim.h"

c_animations g_anim;

// never finished.

bool c_animations::init( ) {
	g_csgo.m_entity_list->add_listener_entity( this );

	return true;
}

bool c_animations::remove( ) {
	try {
		g_csgo.m_entity_list->remove_listener_entity( this );

		for( int i = 0; i < 64; i++ ) {
			if( m_track.at( i ).m_hooked ) {
				m_track.at( i ).m_vmt->unhook_all( );
			}
		}
	}
	catch( const std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_ERROR, "Animation track exception.\n\n%s", ex.what( ) );
		return false;
	}

	return true;
}

void c_animations::on_entity_created( c_base_entity *ent ) {
	try {
		if( !ent )
			return;

		const int idx = ent->get_index( );
		if( idx < 0 )
			return;

		client_class *cc = ent->get_client_class( );
		if( !cc )
			return;

		switch( cc->m_class_id ) {
			case CCSPlayer: {

				m_track.at( idx ) = container_t( ent );
				m_players.emplace( idx, idx );

				break;
			}
			default:
				break;
		}
	}
	catch( const std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_ERROR, "Animation track exception.\n\n%s", ex.what( ) );
	}
}

void c_animations::on_entity_deleted( c_base_entity *ent ) {
	try {
		if( !ent )
			return;

		int idx = ent->get_index( );
		if( idx < 0 )
			return;

		m_players.erase( idx );

		const auto it = std::find_if( m_track.begin( ), m_track.end( ), [ & ]( const container_t &data ) {
			return data.m_idx == idx;
		} );

		if ( it == m_track.end( ) )
			return;

		if ( m_track.at( it->m_idx ).m_hooked )
			m_track.at( it->m_idx ).m_vmt->unhook_all( );
	}
	catch( const std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_ERROR, "Animation track exception.\n\n%s", ex.what( ) );
	}
}
