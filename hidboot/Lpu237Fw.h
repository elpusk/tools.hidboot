#pragma once

#include "tg_lpu237_fw.h"

class CLpu237Fw
{
private:
	typedef	DWORD (WINAPI *typeLPU237_fw_on)();
	typedef	DWORD (WINAPI *typeLPU237_fw_off)();
	typedef	DWORD (WINAPI *typeLPU237_fw_close)( HANDLE hDev );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_get_id)( HANDLE hDev, BYTE *sId );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_get_name)( HANDLE hDev, BYTE *sName );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_get_version)( HANDLE hDev, BYTE *sVersion );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_get_version_major)( const BYTE *sVersion );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_get_version_minor)( const BYTE *sVersion );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_cancel_update)();

	typedef	DWORD (WINAPI *typeLPU237_fw_get_list)( TCHAR *ssDevPaths );
	typedef	HANDLE (WINAPI *typeLPU237_fw_open)( const TCHAR *sDevPath );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_save_setting)( HANDLE hDev );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_recover_setting)( HANDLE hDev );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_update)( const BYTE *sId, DWORD dwWaitTime, const TCHAR *sRomFileName, DWORD dwIndex );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_update_callback)( const BYTE *sId, type_lpu237_fw_callback cbUpdate, void *pUser, const TCHAR *sRomFileName, DWORD dwIndex );
	typedef	DWORD (WINAPI *typeLPU237_fw_msr_update_wnd)( const BYTE *sId, HWND hWnd, UINT uMsg, const TCHAR *sRomFileName, DWORD dwIndex );
	typedef	DWORD (WINAPI *typeLPU237_fw_rom_load)( const TCHAR *sRomFileName );
	typedef	DWORD (WINAPI *typeLPU237_fw_rom_get_index)( const TCHAR *sRomFileName, const BYTE *sName, const BYTE *sVersion );

public:
	CLpu237Fw(void);
	CLpu237Fw( LPCTSTR sDll );
	virtual ~CLpu237Fw(void);
	//
	bool Load( LPCTSTR sDll );

	DWORD WINAPI LPU237_fw_on()
	{
		if( m_LPU237_fw_on )
			return m_LPU237_fw_on();
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_off()
	{
		if( m_LPU237_fw_off )
			return m_LPU237_fw_off();
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_get_list( TCHAR *ssDevPaths )
	{
		if( m_LPU237_fw_get_list )
			return m_LPU237_fw_get_list( ssDevPaths );
		else
			return LPU237_FW_RESULT_ERROR;
	}
	HANDLE WINAPI LPU237_fw_open( const TCHAR *sDevPath )
	{
		if( m_LPU237_fw_open )
			return m_LPU237_fw_open( sDevPath );
		else
			return INVALID_HANDLE_VALUE;
	}
	DWORD WINAPI LPU237_fw_close( HANDLE hDev )
	{
		if( m_LPU237_fw_close )
			return m_LPU237_fw_close( hDev );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_save_setting( HANDLE hDev )
	{
		if( m_LPU237_fw_msr_save_setting )
			return m_LPU237_fw_msr_save_setting( hDev );
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_msr_recover_setting( HANDLE hDev )
	{
		if( m_LPU237_fw_msr_recover_setting )
			return m_LPU237_fw_msr_recover_setting( hDev );
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_msr_get_id( HANDLE hDev, BYTE *sId )
	{
		if( m_LPU237_fw_msr_get_id )
			return m_LPU237_fw_msr_get_id( hDev, sId );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_get_name( HANDLE hDev, BYTE *sName )
	{
		if( m_LPU237_fw_msr_get_name )
			return m_LPU237_fw_msr_get_name( hDev, sName );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_get_version( HANDLE hDev, BYTE *sVersion )
	{
		if( m_LPU237_fw_msr_get_version )
			return m_LPU237_fw_msr_get_version( hDev, sVersion );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_get_version_major( const BYTE *sVersion )
	{
		if( m_LPU237_fw_msr_get_version_major )
			return m_LPU237_fw_msr_get_version_major( sVersion );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_get_version_minor( const BYTE *sVersion )
	{
		if( m_LPU237_fw_msr_get_version_minor )
			return m_LPU237_fw_msr_get_version_minor( sVersion );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_cancel_update()
	{
		if( m_LPU237_fw_msr_cancel_update )
			return m_LPU237_fw_msr_cancel_update();
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_update( const BYTE *sId, DWORD dwWaitTime, const TCHAR *sRomFileName, DWORD dwIndex )
	{
		if( m_LPU237_fw_msr_update )
			return m_LPU237_fw_msr_update( sId, dwWaitTime, sRomFileName, dwIndex );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_update_callback( const BYTE *sId, type_lpu237_fw_callback cbUpdate, void *pUser, const TCHAR *sRomFileName, DWORD dwIndex )
	{
		if( m_LPU237_fw_msr_update_callback )
			return m_LPU237_fw_msr_update_callback( sId, cbUpdate, pUser, sRomFileName, dwIndex );
		else
			return LPU237_FW_RESULT_ERROR;
	}

	DWORD WINAPI LPU237_fw_msr_update_wnd( const BYTE *sId, HWND hWnd, UINT uMsg, const TCHAR *sRomFileName, DWORD dwIndex )
	{
		if( m_LPU237_fw_msr_update_wnd )
			return m_LPU237_fw_msr_update_wnd( sId, hWnd, uMsg, sRomFileName, dwIndex );
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_rom_load( const TCHAR *sRomFileName )
	{
		if( m_LPU237_fw_rom_load )
			return m_LPU237_fw_rom_load( sRomFileName );
		else
			return LPU237_FW_RESULT_ERROR;
	}
	DWORD WINAPI LPU237_fw_rom_get_index( const TCHAR *sRomFileName, const BYTE *sName, const BYTE *sVersion )
	{
		if( m_LPU237_fw_rom_get_index )
			return m_LPU237_fw_rom_get_index( sRomFileName, sName, sVersion );
		else
			return LPU237_FW_RESULT_ERROR;
	}

private:
	void ini();
private:
	HMODULE m_hMode;

	typeLPU237_fw_on m_LPU237_fw_on;
	typeLPU237_fw_off m_LPU237_fw_off;
	typeLPU237_fw_get_list m_LPU237_fw_get_list;
	typeLPU237_fw_open m_LPU237_fw_open;
	typeLPU237_fw_close m_LPU237_fw_close;
	typeLPU237_fw_msr_save_setting m_LPU237_fw_msr_save_setting;
	typeLPU237_fw_msr_recover_setting m_LPU237_fw_msr_recover_setting;
	typeLPU237_fw_msr_get_id m_LPU237_fw_msr_get_id;
	typeLPU237_fw_msr_get_name m_LPU237_fw_msr_get_name;
	typeLPU237_fw_msr_get_version m_LPU237_fw_msr_get_version;
	typeLPU237_fw_msr_get_version_major m_LPU237_fw_msr_get_version_major;
	typeLPU237_fw_msr_get_version_minor m_LPU237_fw_msr_get_version_minor;
	typeLPU237_fw_msr_cancel_update m_LPU237_fw_msr_cancel_update;
	typeLPU237_fw_msr_update m_LPU237_fw_msr_update;
	typeLPU237_fw_msr_update_callback m_LPU237_fw_msr_update_callback;
	typeLPU237_fw_msr_update_wnd m_LPU237_fw_msr_update_wnd;
	typeLPU237_fw_rom_load m_LPU237_fw_rom_load;
	typeLPU237_fw_rom_get_index m_LPU237_fw_rom_get_index;
};

