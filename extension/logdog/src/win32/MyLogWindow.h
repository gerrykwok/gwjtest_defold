//////////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include "cmm_dll.h"

// {BB7BC978-CB99-4A48-A933-5D2B0A8FF80B}
static const IID IID_IGwjLogWindow = { 0xbb7bc978, 0xcb99, 0x4a48, { 0xa9, 0x33, 0x5d, 0x2b, 0xa, 0x8f, 0xf8, 0xb } };

interface IGwjLogWindow : public IUnknown
{
	//��������
	virtual void create(HWND hParent, LPCWSTR title) = 0;
	//���ٴ���
	virtual void destroy() = 0;
	//��ȡ���ھ��
	virtual HWND getHWND() = 0;
	//����log
	//color��ʽ: 0xbbggrr
	virtual void addLog(const char *tag, unsigned int color, const char *message) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

GWJ_SYMBOL HRESULT GWJ_API gwjLogWindowCreate(REFIID riid, void** ppvObj, IUnknown *pUnkOuter);

#ifdef __cplusplus
}
#endif
