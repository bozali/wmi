# WMI

A simple WMI library for C++ 17.

## Simple example

This examples shows how to get processes on the local system.

```c++
#include <wmi/core/com-manager.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-processor.h>

#include <iostream>

int main()
{
    wmi::ComManager::Initialize();

    wmi::ManagementContext context;
    auto resource = std::make_shared<wmi::ManagementResource>(context);
    resource->Connect("ROOT\\CIMV2");

    auto processor = std::make_shared<wmi::QueryProcessor>(resource, "SELECT * FROM Win32_Processs");

    auto stream = processor->GetStream();
    
    for (const auto& object : stream) {
        std::wcout << object.Get<BSTR>("Name") << "\n";
    }
    
    wmi::ComManager::Uninitialize();

    return 0;
}
```

Lets break this code snippet down. First we need to initialize with the `ComManager` the COM library. It is possible to use the `ComManager` class or use the function [`CoInitialize`](https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize) from the Win32 API.
After we have initialize the COM library we need to create context and a resource where we want to connect to.

```c++
auto resource = std::make_shared<wmi::ManagementResource>(context);
resource->Connect("ROOT\\CIMV2");
```

After the connection we are able to query data from the library with the `QueryProcessor`. The `QueryProcessor` returns a stream that contains an input iterator that allows us to iterate through the elements or use STL algorithms.

## Using STL algorithms

The `QueryStream` contains methods that returns iterators. With these iterators we are able to use STL algorithms.

Example of copying elements into a `std::vector`.

```c++
// ...

#include <vector>

int main()
{
    // ...

    auto stream = processor->GetStream();

    // Copying the objects into a vector
    std::vector<wmi::ResultObject> objects;
    std::copy(std::begin(objects), std::end(objects), std::back_inserter(objects));

    // Do something with `objects`

    return 0;
}
```

Example of finding a process with a specific name.

```c++
// ...

int main()
{
    // ...

    auto stream = processor->GetStream();

    auto found = std::find_if(std::begin(stream),
                              std::end(stream),
                              [](const wmi::ResultObject& object) {
                                  auto name = object.Get<BSTR>("Name");
                                  return wcscmp(name, TEXT("ProcessNameToSearchFor"));
                              });

    if (found != std::end(stream)) {
        // Found
    }

    return 0;
}
```

## Current state

**The current state is not ready for any production use there will be many changes.**

It is possible to just clone and compile it and test it yourself.


## TODOs

- Refactoring the code
- Better memory management
- Functionality to use Methods and Put on WMI objects
- Documentation
- Maybe more...


## License
This WMI library is licensed under the MIT license.
