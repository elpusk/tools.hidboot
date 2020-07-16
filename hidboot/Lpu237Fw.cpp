#include "pch.h"
#include "framework.h"
#include "Lpu237Fw.h"


CLpu237Fw::CLpu237Fw(void)
	: m_hMode(NULL)
{
	ini();
}

CLpu237Fw::CLpu237Fw( LPCTSTR sDll )
	: m_hMode(NULL)
{
	ini();
	Load( sDll );
}

CLpu237Fw::~CLpu237Fw(void)
{
	if( m_LPU237_fw_off )
		m_LPU237_fw_off();
	//
	if( m_hMode )
		FreeLibrary( m_hMode );
}

void CLpu237Fw::ini()
{
	m_LPU237_fw_on = NULL;
	m_LPU237_fw_off = NULL;
	m_LPU237_fw_get_list = NULL;
	m_LPU237_fw_open = NULL;
	m_LPU237_fw_close = NULL;
	m_LPU237_fw_msr_save_setting = NULL;
	m_LPU237_fw_msr_recover_setting = NULL;
	m_LPU237_fw_msr_get_id = NULL;
	m_LPU237_fw_msr_get_name = NULL;
	m_LPU237_fw_msr_get_version = NULL;
	m_LPU237_fw_msr_get_version_major = NULL;
	m_LPU237_fw_msr_get_version_minor = NULL;
	m_LPU237_fw_msr_cancel_update = NULL;
	m_LPU237_fw_msr_update = NULL;
	m_LPU237_fw_msr_update_callback = NULL;
	m_LPU237_fw_msr_update_wnd = NULL;
	m_LPU237_fw_rom_load = NULL;
	m_LPU237_fw_rom_get_index = NULL;
}

bool CLpu237Fw::Load( LPCTSTR sDll )
{
	m_hMode = ::LoadLibrary( sDll );

	if( m_hMode == NULL )
		m_hMode = ::LoadLibrary( _T(".\\tg_lpu237_fw.dll") );
	//
	if( m_hMode ){
		m_LPU237_fw_on = reinterpret_cast<typeLPU237_fw_on>( ::GetProcAddress( m_hMode, "LPU237_fw_on" ) );
		m_LPU237_fw_off = reinterpret_cast<typeLPU237_fw_off>( ::GetProcAddress( m_hMode, "LPU237_fw_off" ) );
	
		m_LPU237_fw_close = reinterpret_cast<typeLPU237_fw_close>( ::GetProcAddress( m_hMode, "LPU237_fw_close" ) );

		m_LPU237_fw_msr_save_setting = reinterpret_cast<typeLPU237_fw_msr_save_setting>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_save_setting" ) );
		m_LPU237_fw_msr_recover_setting = reinterpret_cast<typeLPU237_fw_msr_recover_setting>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_recover_setting" ) );

		m_LPU237_fw_msr_get_id = reinterpret_cast<typeLPU237_fw_msr_get_id>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_get_id" ) );
		m_LPU237_fw_msr_get_name = reinterpret_cast<typeLPU237_fw_msr_get_name>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_get_name" ) );
		m_LPU237_fw_msr_get_version = reinterpret_cast<typeLPU237_fw_msr_get_version>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_get_version" ) );
		m_LPU237_fw_msr_get_version_major = reinterpret_cast<typeLPU237_fw_msr_get_version_major>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_get_version_major" ) );
		m_LPU237_fw_msr_get_version_minor = reinterpret_cast<typeLPU237_fw_msr_get_version_minor>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_get_version_minor" ) );
		m_LPU237_fw_msr_cancel_update = reinterpret_cast<typeLPU237_fw_msr_cancel_update>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_cancel_update" ) );
#ifdef  UNICODE  
		m_LPU237_fw_get_list = reinterpret_cast<typeLPU237_fw_get_list>( ::GetProcAddress( m_hMode, "LPU237_fw_get_list_w" ) );
		m_LPU237_fw_open = reinterpret_cast<typeLPU237_fw_open>( ::GetProcAddress( m_hMode, "LPU237_fw_open_w" ) );
		m_LPU237_fw_msr_update = reinterpret_cast<typeLPU237_fw_msr_update>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_w" ) );
		m_LPU237_fw_msr_update_callback = reinterpret_cast<typeLPU237_fw_msr_update_callback>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_callback_w" ) );
		m_LPU237_fw_msr_update_wnd = reinterpret_cast<typeLPU237_fw_msr_update_wnd>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_wnd_w" ) );
		m_LPU237_fw_rom_load = reinterpret_cast<typeLPU237_fw_rom_load>( ::GetProcAddress( m_hMode, "LPU237_fw_rom_load_w" ) );
		m_LPU237_fw_rom_get_index = reinterpret_cast<typeLPU237_fw_rom_get_index>( ::GetProcAddress( m_hMode, "LPU237_fw_rom_get_index_w" ) );
#else
		m_LPU237_fw_get_list = reinterpret_cast<typeLPU237_fw_get_list>( ::GetProcAddress( m_hMode, "LPU237_fw_get_list_a" ) );
		m_LPU237_fw_open = reinterpret_cast<typeLPU237_fw_open>( ::GetProcAddress( m_hMode, "LPU237_fw_open_a" ) );
		m_LPU237_fw_msr_update = reinterpret_cast<typeLPU237_fw_msr_update>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_a" ) );
		m_LPU237_fw_msr_update_callback = reinterpret_cast<typeLPU237_fw_msr_update_callback>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_callback_a" ) );
		m_LPU237_fw_msr_update_wnd = reinterpret_cast<typeLPU237_fw_msr_update_wnd>( ::GetProcAddress( m_hMode, "LPU237_fw_msr_update_wnd_a" ) );
		m_LPU237_fw_rom_load = reinterpret_cast<typeLPU237_fw_rom_load>( ::GetProcAddress( m_hMode, "LPU237_fw_rom_load_a" ) );
		m_LPU237_fw_rom_get_index = reinterpret_cast<typeLPU237_fw_rom_get_index>( ::GetProcAddress( m_hMode, "LPU237_fw_rom_get_index_a" ) );
#endif	//UNICODE

		if( m_LPU237_fw_on )
			m_LPU237_fw_on();

		return true;
	}
	else
		return false;
}