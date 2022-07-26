#pragma once

#define _WIN32_DCOM

#include <wrl/client.h>

#include <Windows.h>
#include <comdef.h>
#include <WbemIdl.h>

#pragma once(lib, "wbemuuid.lib")


template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
