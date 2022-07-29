#pragma once


#if defined(WMI_EXPORTS)
# define WMI_DLL __declspec(dllexport)
#else
# define WMI_DLL __declspec(dllimport)
#endif // WMI_EXPORTS


#define WMI_EXTERN_C extern "C"

