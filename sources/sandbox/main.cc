#include <wmi/common/secure-string-access.h>
#include <wmi/common/secure-string.h>

#include <iostream>
#include <string>
#include <memory>


int main()
{
	wmi::SecureString secure(TEXT("Some secure data"));

	if (const auto access = secure.Access())
	{
		const auto x = access->GetData();

		std::wcout << x << std::endl;
	}

	return 0;
}
