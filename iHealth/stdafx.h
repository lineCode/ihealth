
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#ifndef WINVER                          // 指定要求的最低平台是 Windows Vista。
#define WINVER 0x0600           // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINDOWS          // 指定要求的最低平台是 Windows 98。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以适用于 Windows Me 或更高版本。
#endif

#ifndef _WIN32_IE                       // 指定要求的最低平台是 Internet Explorer 7.0。
#define _WIN32_IE 0x0700        // 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


//#include <windows.h>
#include <objbase.h>
#include <zmouse.h>
#include <atlstr.h> 

#include "..\DuiLibEx\UIlib.h"
#include "..\CommonExt\DCICommonExtAll.h"

#pragma comment(lib, "..\\Lib\\libmysql.lib")

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#		pragma comment(lib, "..\\Lib\\toolbox_ud.lib")
#		pragma comment(lib,"..\\Lib\\wke.lib")
#		pragma comment(lib,"..\\Lib\\libzplay.lib")
#		pragma comment(lib, "winmm.lib")
#		pragma comment(lib, "Strmiids.lib")
#		pragma comment(lib, "..\\LIB\\NIDAQmx.lib")
#		pragma comment(lib, "..\\LIB\\APS168.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_calib3d300d.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_core300d.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_highgui300d.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_imgcodecs300d.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_imgproc300d.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_videoio300d.lib")	
#		pragma comment(lib, "..\\LIB\\opencv_world300.lib")
#		pragma comment(lib, "..\\LIB\\msc.lib")	
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#		pragma comment(lib, "..\\Lib\\toolbox_d.lib")
#		pragma comment(lib,"..\\Lib\\wke.lib")
#		pragma comment(lib,"..\\Lib\\libzplay.lib")
//#		pragma comment(lib,"..\\Lib\\libvlc.lib")
//#		pragma comment(lib,"..\\Lib\\libvlccore.lib")
#		pragma comment(lib, "winmm.lib")
#		pragma comment(lib, "Strmiids.lib")
#		pragma comment(lib, "..\\LIB\\NIDAQmx.lib")
#		pragma comment(lib, "..\\LIB\\APS168.lib")	
#   endif
#else
#   ifdef _UNICODE
#		pragma comment(lib, "winmm.lib")
#		pragma comment(lib, "Strmiids.lib")
#		pragma comment(lib, "..\\LIB\\NIDAQmx.lib")
#		pragma comment(lib, "..\\LIB\\APS168.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_calib3d300.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_core300.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_highgui300.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_imgcodecs300.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_imgproc300.lib")	
//#		pragma comment(lib, "..\\LIB\\opencv_videoio300.lib")	
#		pragma comment(lib, "..\\LIB\\opencv_world300.lib")
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#		pragma comment(lib, "..\\Lib\\toolbox.lib")
#		pragma comment(lib,"..\\Lib\\wke.lib")
#		pragma comment(lib,"..\\Lib\\libzplay.lib")
//#		pragma comment(lib,"..\\Lib\\libvlc.lib")
//#		pragma comment(lib,"..\\Lib\\libvlccore.lib")

#		pragma comment(lib, "..\\LIB\\msc.lib")	
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#		pragma comment(lib, "..\\Lib\\toolbox.lib")
#		pragma comment(lib,"..\\Lib\\wke.lib")
#		pragma comment(lib,"..\\Lib\\libzplay.lib")
//#		pragma comment(lib,"..\\Lib\\libvlc.lib")
//#		pragma comment(lib,"..\\Lib\\libvlccore.lib")
#		pragma comment(lib, "winmm.lib")
#		pragma comment(lib, "Strmiids.lib")
#		pragma comment(lib, "..\\LIB\\NIDAQmx.lib")
#		pragma comment(lib, "..\\LIB\\APS168.lib")	
#   endif
#endif

using namespace DuiLib;
// TODO: 在此处引用程序需要的其他头文件
#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
