#include "../../inc.hpp"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"
#include "../../features/anti-aim/antiaim.h"

HRESULT __stdcall hook::Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion ) {
	static bool once{ false };
	if( !once ) {
		g_renderer.init( device );
		g_menu.init( );
		g_input.init( "Valve001", g_renderer.get_instance( ) );
		once = true;
	}

	g_renderer.start_drawing( device );

	g_visuals.run( );

	g_nadepred.draw( );

	g_notify.draw( );

	if( g_vars.visuals.extra.misc_flags ) {
		int flag_count = 0;

		if( g_cl.m_under_tickrate ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_ENHANCE_9PX ), OSHColor::FromARGB( 230, 230, 43, 10 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( g_renderer.get_renderer().GetDisplaySize().Height / 2 ) - flag_count++ * 21, DROPSHADOW, "FPS" );
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_7PX ), OSHColor::FromARGB( 230, 230, 43, 10 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( g_renderer.get_renderer().GetDisplaySize().Height / 2 ) - 1 * 11, DROPSHADOW, "%i", g_cl.m_client_framerate );
		}
	}

	g_visuals.watermark( );

	g_renderer.end_drawing( device );

	return g_hooks.m_directx.get_old_method< fn::Present_t >( hook::idx::PRESENT )( device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}
