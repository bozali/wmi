#pragma once

#include <wmi/common/exports.h>

#include <optional>


namespace wmi {

enum class ImpersonationLevel
{
	kDefault,
	kAnonymouse,
	kIdentify,
	kImpersonate,
	kDelegate
};


enum class AuthenticationLevel
{
	kDefault,
	kNone,
	kConnect,
	kCall,
	kPacket,
	kPacketIntegrity,
	kPacketPrivacy
};


struct ProcessSecurity
{
	ImpersonationLevel impersonation = ImpersonationLevel::kImpersonate;
	AuthenticationLevel authentication = AuthenticationLevel::kDefault;
};


class WMI_DLL ComManager
{
public:
  static void Initialize(std::optional<ProcessSecurity> security = std::nullopt);
	static void Uninitialize();
};

}