
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifndef WINVER                          // ָ��Ҫ������ƽ̨�� Windows Vista��
#define WINVER 0x0600           // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINDOWS          // ָ��Ҫ������ƽ̨�� Windows 98��
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ���������� Windows Me ����߰汾��
#endif

#ifndef _WIN32_IE                       // ָ��Ҫ������ƽ̨�� Internet Explorer 7.0��
#define _WIN32_IE 0x0700        // ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


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
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)