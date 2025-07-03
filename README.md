[![No Maintenance Intended](http://unmaintained.tech/badge.svg)](http://unmaintained.tech/)

Khiorules is **no longer supported**.

# DEPRECATED Khiorules
 
Khiorules is a tool which produces a set of rules as a preprocessing for a classification model 
Khiorules is available both in user interface mode and in batch mode, such that it can easily be embedded as a software component in a data mining deployment project.

Khiorules belongs to the Khiops family.

## How to build khiorules

You need to install [CMake](https://cmake.org/). Then run the following command:
```bash
cmake -B build -S . -D CMAKE_BUILD_TYPE=Release
cmake --build build --target khiorukes
```

This source code is based on the [Khiops](https://github.com/KhiopsML/khiops) libraries (supported version is 11.0.0).