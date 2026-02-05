# File System Indexer
## Last version updated: v0.1.0-beta

---

## What is FSI?
File System Indexer or FSI is a C++ library that it can be used to index files quickly with an extendible and easy to use API

There is also an API for C which is just for *speed-first* use cases such as indexing or other usages

## Requirements
To compile this program, you will need the following dependencies

1. C11+ / C++17+ compiler
2. jq
3. Bash
4. Ninja / Make (Ninja is prefered)
5. CMake

## How to Compile
### Setting up

The developer can compile the code with the following snippet
```bash
./gen.sh # Generate all the required files and docs
./compile.sh settings # Developer will be prompted for config; If doxygen is not installed, it will still work
```

### Developer Mode
With developer mode, you can get more information about unexpected crashes, have ASAN / LSAN support, debug support, ...

```bash
./compile.sh setup
```

### Production Mode
Do not use dev mode here, just compile as prod; No information about crashes, no debug or ASAN / LSAN support

```bash
./compile.sh setup ndev
```

### Compile the sources
```bash
./compile.sh
```

## Basic usage
In this example you can directly add a specific directory without its parents
```cpp
fsi::IndexerInfo info; // Create object
info.path = "/path/to/fileOrDir"; // Set the path
info.id = "optional-id"; // Recommended to set an ID

indexer.addInfo(info); // Add indexer info
```

Recursively index a specific dir
```cpp
fsi::IndexerInfo info;
info.path = "/path/to/directory";
info.id = "optional-id";

indexer.addExtendedInfo(info); // Add all its subdirs starting from `info.path`
```

## Search and manipulate indexed info

This following example will search everything in the registry and output the matching
In this case, everything that includes `txt`
```cpp
std::vector<std::string> results = indexer.searchMatching("txt");
```

Get all the indexed paths directly in a vector
```cpp
std::vector<std::string> allPaths = indexer.getIndexPaths();
```

Get specific indexed info from the vector registry
```cpp
fsi::IndexerInfo info = indexer.findIndex("/path/to/path");
fsi::IndexerInfo info = indexer.findIndex("id-path"); // Or search via ID
```


```cpp
indexer.removeInfo("current-dir");
```

## How to handle errors automatically

Replace `errorVariable` to a variable that has a type of `IndexerError`
```cpp
if (__FSI_INDEXERERR_CHECK(errorVariable))  
{
	// Error handling here
	// ...
	// Print message by accessing:
	// errorVariable.message
}
```

## Example usage

```cpp
#include <iostream>

#include "/path/to/FSI_Indexer.hpp"

int main()
{
	fsi::Indexer indexer("main-indexer");

	{
		fsi::IndexerInfo info;
		info.path = "/path/to/dir";
		info.id = "current-dir";

		indexer.addExtendedInfo(info);
	}

	for (const auto &x : indexer.searchMatching("cpp"))
	{
		std::cout << x << "\n";
	}

	std::cout << "ID `current-dir` has path:" << indexer.findIndex("current-dir").path << "\n";

	return 0;
}
```

