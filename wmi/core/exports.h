#pragma once


#if defined(WMI_EXPORTS)
#	define WMI_DLL __declspec(dllexport)
#else
#	define WMI_DLL __declspec(dllimport)
#endif // WMI_EXPORTS


#if defined(__cplusplus)
#	define WMI_EXTERN_C extern "C"
#else
#	define WMI_EXTERN_C
#endif // __cplusplus
