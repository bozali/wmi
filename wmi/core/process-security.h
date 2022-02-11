#pragma once


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

}
