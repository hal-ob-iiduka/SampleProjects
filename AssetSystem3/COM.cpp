#include "COM.h"
#include <objbase.h>

COM::COM()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
}

COM::COM(COMID comID)
{
	HRESULT hr = CoInitializeEx(NULL, static_cast<DWORD>(comID));
}

COM::~COM()
{
	CoUninitialize();
}