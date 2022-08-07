# WMI library for modern C++

A simple WMI library for C++ 17.

## Simple example

This example shows how to get processes on the local system.

```c++
// ...

int main()
{
  wmi::ComManager::Initialize();
  
  {
    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();

    wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");

    auto stream = processor.GetStream();
    auto object = stream.Current();

    std::wcout << object["Name"].bstrVal << "\n";
  }
  
  wmi::ComManager::Uninitialize();
}
```

## Using STL algorithms

The `ManagementQueryStream` contains methods that returns iterators. With these iterators we are able to use STL algorithms.

Example of copying elements into a `std::vector`.

```c++
// ...

int main()
{
  // ...

  auto stream = processor->GetStream();

  // Copying the objects into a vector
  std::vector<wmi::ManagementObject> objects;
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
                            [](const wmi::ManagementObject& object) {
                                auto name = object["Name"].bstrVal;
                                return wcscmp(name, TEXT("ProcessNameToSearchFor"));
                            });

  if (found != std::end(stream)) {
    // Found
  }

  return 0;
}
```

## Mapping

The library allows to map returned objects from `ManagementQueryStream` to data structures. In this example we will create custom structure called `Win32Process`. After we have queried the data from stream we will map the object to the structure `Win32Process`.

The stream class has a template parameter that allows to map the returned object a specific class.

```c++
// ...

struct Win32Process
{
  bstr_t name;
};

void MapManagementObject(const wmi::ManagementObject& from, Win32Process& to)
{
  to.name = from["Name"].bstrVal;
}

int main()
{
  // ...
  wmi::ManagementResource resource("ROOT\\CIMV2");
  resource.Connect();

  wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");

  auto stream = processor.GetStream<Win32Process>();
  auto object = stream.Current();

  std::wcout << object.name << "\n";
}
```


## Calling class methods

The following example shows how to call the method `Create` from `Win32_Process` and process the results.

```c++
// ...

int main()
{
  // ...
	auto resource = std::make_shared<wmi::ManagementResource>();
	resource->Connect("ROOT\\CIMV2");

  // Prepare the parameters
	std::unordered_map<std::string_view, variant_t> parameters;
	parameters["CommandLine"] = bstr_t("notepad.exe").Detach();

	auto result = resource->ExecuteMethod("Win32_Process", "Create", parameters);

  // Using the ResultObject to get the `ReturnValue` and the `ProcessId` of our newly created process.
  if (result["ReturnValue"].lVal == 0) {
    std::cout << "Successfully created process with the process id: " << result.Get<LONG>("ProcessId") << "\n";
  }

  return 0;
}
```

## Current state

The library works only on Windows. It is possible to just clone, compile and test it yourself. You need to link to the libaries `wbemuuid.lib` and `crypt32.lib`
if you are including it in your project.

## License

The WMI library is licensed under the MIT License.
