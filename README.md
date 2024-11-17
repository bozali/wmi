# WMI

This is a advanced C++ library for WMI.


## Guide

### Initializing

Before we can start using WMI we need to ensure that COM is initialized. To initialize we need to
call `ComManager`. At the end of the application we need to cleanup by
calling `ComManager::Uninitialize`.


```c++

int main()
{
	try
	{
      wmi::ComManager::Initialize(security);

      // ...
	}
	catch (const wmi::ComException& ex)
	{
		std::cout << ex.What() << std::endl;
		std::wcout << ex.Detailed() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	wmi::ComManager::Uninitialize();
  // ...
}
```

### Simple query

One of the core features of WMI is to query system information from remote and local machines.
This library makes it easy to define and query objects. It also allows to combine the responses
with C++ standard library algorithms with iterators.

In the following example we will query all the processes that are running in our machine.

```c++
int main()
{
    // ...

    // Ensure that we are connected to a namespace resource
    auto resource = std::make_shared<wmi::ManagementResource>("root\\cimv2");
    resource->Connect();

    auto query_processor = resource->GetQueryProcessor("SELECT * FROM Win32_Process");
    auto stream = query_processor->GetStream();

    for (const auto& obj : stream)
    {
        std::wcout << std::get<wmi::BasicString>(obj["Name"]) << std::endl;
    }

    // ...
}
```

As you can see we are making usage of many std functions like `get<T>` to access property values
of our `ManagementObject` or use the implemented iterators to enumerate through each item.

### Query with mapping

Always accessing the properties through the `ManagementObject` can be complex and feels
not clean. Therefor it is possible to map WMI classes to our own data structures. To achive this
we need to implement `HandleManagementObjectMapped` for our data structure.

```c++
// Our Win32_Process struct
struct Win32_Process
{
    wmi::Int32 process_id;
    wmi::BasicString name;
};

static void HandleManagementObjectMapped(const wmi::ManagementObject& from, Win32_Process& to)
{
    to.process_id = std::get<wmi::Int32>(from["ProcessId"]);
    to.name = std::get<wmi::BasicString>(from["Name"]);
}

int main()
{
    // ...

    // Ensure that we are connected to a namespace resource
    auto resource = std::make_shared<wmi::ManagementResource>("root\\cimv2");
    resource->Connect();

    auto query_processor = resource->GetQueryProcessor("SELECT * FROM Win32_Process");
    auto stream = query_processor->GetStream<Win32_Process>(); // Pass our structure to template

    for (const auto& obj : stream)
    {
        std::wcout << obj.process << " " << obj.name << std::endl;
    }

    // ...
}
```

### Using STL algorithms

```c++

int main()
{
    // ...

    auto found = std::find_if(std::begin(stream), std::end(stream), [](const Win32_Process& process)
                                                    {
                                                        return wcscmp(process.name, TEXT("code.exe"));
                                                    });

    if (found != std::end(stream))
    {
        std::wcout << (*found).name << std::endl;
    }

    // ...
}

```
