#pragma once

#ifdef DCICOMMONEXT_MODULE
#define DCICOMMONEXT_MODULE_EXPIMP __declspec(dllexport)
#else
#define DCICOMMONEXT_MODULE_EXPIMP __declspec(dllimport)
#endif

class DCICOMMONEXT_MODULE_EXPIMP TestXXX
{
public:
	TestXXX(void);
	~TestXXX(void);

	CString JoinX(const CString &s1, const CString &s2);
};
