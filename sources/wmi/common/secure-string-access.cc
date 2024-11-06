#include <wmi/common/secure-string-access.h>

using namespace wmi;


SecureStringAccess::SecureStringAccess(const SecureString& secure_str)
	: secure_(secure_str)
{
	secure_.UnprotectMemory();
}


SecureStringAccess::~SecureStringAccess()
{
	secure_.ProtectMemory(secure_.GetData());
}
