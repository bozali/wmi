#include <wmi/common/secure-string-access.h>
#include <wmi/management/management-query-processor.h>
#include <wmi/management/management-resource.h>
#include <wmi/common/secure-string.h>

#include <iostream>
#include <string>
#include <memory>


int main()
{
	auto resource = std::make_unique<wmi::ManagementResource>(TEXT(""));

	auto query_processor = resource->GetQueryProcessor(TEXT("SELECT * FROM Win32_Processor"));

	return 0;
}
