#pragma once

/*!
*		2017.06.29	- release version 1.0 : starting.......
*
*/
#include <Windows.h>

/*!
*	constant definition.
*/
#define	LPU237_FW_SIZE_ID					16			//device ID is 16 bytes.
#define	LPU237_FW_SIZE_NAME			16			//device Name is 16 bytes.
#define	LPU237_FW_SIZE_VERSION			4			//firmware version is 4 bytes.

/*!
*	return value definition.
*/
#define	LPU237_FW_RESULT_SUCCESS			0		//! processing success.
#define	LPU237_FW_RESULT_ERROR			0xFFFFFFFF	//! processing error.( maybe system or communication error ); (-1)
#define	LPU237_FW_RESULT_CANCEL			0xFFFFFFFE	//! processing is canceled by another reqest.(-2)
#define	LPU237_FW_RESULT_TIMEOUT			0xFFFFFFFC	//! processing is timeout.(-4)
#define	LPU237_FW_RESULT_NO_MSR			0xFFFFFFFb	//! processing not found MSR .(-5)

/*!
*	windows' message wparam value.
*/
#define	LPU237_FW_WPARAM_COMPLETE		0	//firmware update complete.
#define	LPU237_FW_WPARAM_SUCCESS			0	//firmware update complete.
#define	LPU237_FW_WPARAM_FOUND_BL		1	//found bootloader.
#define	LPU237_FW_WPARAM_SECTOR_ERASE	2
#define	LPU237_FW_WPARAM_SECTOR_WRITE	3

#define	LPU237_FW_WPARAM_ERROR			0xFFFF

/*!
*	the callback function type.
*	this type will be used in LPU237_fw_msr_update()
*	
*	parameters
*		1'st - user defined data.
*		2'nd - current processing result : LPU237_FW_RESULT_x
*		3'th - LPU237_FW_WPARAM_x.
*/
typedef	DWORD (WINAPI *type_lpu237_fw_callback)(void*,DWORD,DWORD);


/*!
* function
*	initial lpu237 internal data.
*
* parameters
*
* return
*  	LPU237_FW_RESULT_SUCCESS : only.
*
*/
DWORD WINAPI LPU237_fw_on();

/*!
* function
*	Deinitial lpu237 internal data.
*
* parameters
*
* return
*  	LPU237_FW_RESULT_SUCCESS : only.
*
*/
DWORD WINAPI LPU237_fw_off();

/*!
* function
*	get connected device list.( unicode version )
*
* parameters
*	ssDevPaths : [in/out] Multi string of devices paths. 
*					this value can be NULL(0).
*
*	return
*		if ssDevPaths = NULL, the number of character.(including NULL). one character size = 2 bytes
*		else the number of connected lpu237 device.
*/
DWORD WINAPI LPU237_fw_get_list_w( WCHAR *ssDevPaths );

/*!
* function
*	get connected device list.( Multi Byte Code Set version )
*
* parameters
*	ssDevPaths : [in/out] Multi string of devices paths.
*					this value can be NULL(0).
*
*	return
*		if ssDevPaths = NULL, the number of character.(including NULL). one character size = 1 bytes
*		else the number of connected lpu237 device.
*/
DWORD WINAPI LPU237_fw_get_list_a( CHAR *ssDevPaths	);


/*!
* function 
*	open device.( unicode version )
*
* parameters
*	sDevPath : [in] device path - unicode type zero-string
*
* return 
*	if success, return device handle.
*	else return INVALID_HANDLE_VALUE
*/
HANDLE WINAPI LPU237_fw_open_w( CONST WCHAR *sDevPath );

/*!
* function 
*	open device.( Multi Byte Code Set version )
*
* parameters
*	sDevPath : [in] device path - multi byte code set type zero-string.
*
* return 
*	if success, return device handle.
*	else return INVALID_HANDLE_VALUE
*/
HANDLE WINAPI LPU237_fw_open_a( CONST CHAR *sDevPath );

/*!
* function
*	close lpu237 device.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*
* return
*	if success, return LPU237_FW_RESULT_SUCCESS
*	else return LPU237_FW_RESULT_ERROR
*/
DWORD WINAPI LPU237_fw_close( HANDLE hDev );

/*!
* function
*	save the current lpu237 device setting.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*
* return
*	if success, return LPU237_FW_RESULT_SUCCESS
*	else return LPU237_FW_RESULT_ERROR
*/
DWORD WINAPI LPU237_fw_msr_save_setting( HANDLE hDev );

/*!
* function
*	resetting  lpu237 device with saved setting.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*
* return
*	if success, return LPU237_FW_RESULT_SUCCESS
*	else return LPU237_FW_RESULT_ERROR
*/
DWORD WINAPI LPU237_fw_msr_recover_setting( HANDLE hDev );

/*!
* function
*	get device unique ID.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*	sId : [in/out] A pointer to the buffer that save the device ID.( ID is 16 bytes )
*			this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else the size of ID.[unit byte]
*/
DWORD WINAPI LPU237_fw_msr_get_id( HANDLE hDev, BYTE *sId );

/*!
* function
*	get device internal name.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*	sName : [in/out] A pointer to the buffer that save the device name.
*			this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else the size of internal name.[unit byte]
*/
DWORD WINAPI LPU237_fw_msr_get_name( HANDLE hDev, BYTE *sName );

/*!
* function
*	get device firmware version.
*
* parameters
*	hDev : [in] device handle( return value of LPU237_fw_open() )
*	sName : [in/out] A pointer to the buffer that save the device firmware version.( version 4 bytes )
*			this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else the size of version.[unit byte]
*/
DWORD WINAPI LPU237_fw_msr_get_version( HANDLE hDev, BYTE *sVersion );

/*!
* function
*	get major number from firmware version.
*
* parameters
*	sVersion : [in] device firmware version( return value of LPU237_fw_msr_get_version() ).
*			this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else major version number.
*/
DWORD WINAPI LPU237_fw_msr_get_version_major( const BYTE *sVersion );

/*!
* function
*	get minor number from firmware version.
*
* parameters
*	sVersion : [in] device firmware version( return value of LPU237_fw_msr_get_version() ).
*			this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else minor version number.
*/
DWORD WINAPI LPU237_fw_msr_get_version_minor( const BYTE *sVersion );

/*!
* function
*	stop operation of LPU237_fw_msr_update_x.
*
* parameters
*
* return
*	if success, return LPU237_FW_RESULT_SUCCESS
*	else return LPU237_FW_RESULT_ERROR
*
*/
DWORD WINAPI LPU237_fw_msr_cancel_update();

/*!
* function
*	start firmware update.( unicode version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	dwWaitTime : [in] waits time.( unit : mm sec )
*	sRomFileName : [in] rom file name. unicode, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR or LPU237_FW_RESULT_TIMEOUT.
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_w( const BYTE *sId, DWORD dwWaitTime, const WCHAR *sRomFileName, DWORD dwIndex );

/*!
* function
*	start firmware update..( Multi Byte Code Set version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	dwWaitTime : [in] waits time.( unit : mm sec )
*	sRomFileName : [in] rom file name. multi-byte-code-set, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_DLL_RESULT_ERROR or LPU237_FW_RESULT_TIMEOUT
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_a( const BYTE *sId, DWORD dwWaitTime, const CHAR *sRomFileName, DWORD dwIndex );


/*!
* function
*	start firmware update.( unicode version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	cbUpdate : [in] callback function for serving updating information.
*	pUser : [in] user data pointer for calling cbUpdate(). 
*	sRomFileName : [in] rom file name. unicode, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR.
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_callback_w( const BYTE *sId, type_lpu237_fw_callback cbUpdate, void *pUser, const WCHAR *sRomFileName, DWORD dwIndex );

/*!
* function
*	start firmware update..( Multi Byte Code Set version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	cbUpdate : [in] callback function for serving updating information.
*	pUser : [in] user data pointer for calling cbUpdate(). 
*	sRomFileName : [in] rom file name. multi-byte-code-set, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_DLL_RESULT_ERROR
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_callback_a( const BYTE *sId, type_lpu237_fw_callback cbUpdate, void *pUser, const CHAR *sRomFileName, DWORD dwIndex );

/*!
* function
*	start firmware update.( unicode version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	hWnd : [in] window handle for target window.
*	uMsg : [in] window message for target window.
*	sRomFileName : [in] rom file name. unicode, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_wnd_w( const BYTE *sId, HWND hWnd, UINT uMsg, const WCHAR *sRomFileName, DWORD dwIndex );

/*!
* function
*	start firmware update.( Multi Byte Code Set version )
*
* parameters
*	sId : [in] the device ID.( ID is 16 bytes )
*	hWnd : [in] window handle for target window.
*	uMsg : [in] window message for target window.
*	sRomFileName : [in] rom file name. multi-byte-code-set, zero-string type. this value can be NULL(0).
*	dwIndex : [in] the index of firmware in rom file. greater then equal -1
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_msr_update_wnd_a( const BYTE *sId, HWND hWnd, UINT uMsg, const CHAR *sRomFileName, DWORD dwIndex );

/*!
* function
*	load rom file.( unicode version )
*
* parameters
*	sRomFileName : [in] rom file name. unicode, zero-string type. this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_rom_load_w( const WCHAR *sRomFileName );

/*!
* function
*	load rom file.( Multi Byte Code Set version )
*
* parameters
*	sRomFileName : [in] rom file name. multi-byte-code-set, zero-string type. this value can be NULL(0).
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else LPU237_FW_RESULT_SUCCESS
*/
DWORD WINAPI LPU237_fw_rom_load_a( const CHAR *sRomFileName );

/*!
* function
*	get firmware index value( greater then equal 0 ) in rom file.( unicode version ).
*
* parameters
*	sRomFileName : [in] rom file name. unicode, zero-string type. this value can be NULL(0).
*	sName : [in] device internal name. LPU237_fw_msr_get_name' return value.
*	sVersion : [in] device firmware version. LPU237_fw_msr_get_version' return value.
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else firmware index value( greater then equal 0 )
*/
DWORD WINAPI LPU237_fw_rom_get_index_w( const WCHAR *sRomFileName, const BYTE *sName, const BYTE *sVersion );

/*!
* function
*	get firmware index value( greater then equal 0 ) in rom file.( multi-byte-code-set version ).
*
* parameters
*	sRomFileName : [in] rom file name. multi-byte-code-set, zero-string type. this value can be NULL(0).
*	sName : [in] device internal name. LPU237_fw_msr_get_name' return value.
*	sVersion : [in] device firmware version. LPU237_fw_msr_get_version' return value.
*
* return 
* 	if error, return LPU237_FW_RESULT_ERROR
*	else firmware index value( greater then equal 0 )
*/
DWORD WINAPI LPU237_fw_rom_get_index_a( const CHAR *sRomFileName, const BYTE *sName, const BYTE *sVersion );
