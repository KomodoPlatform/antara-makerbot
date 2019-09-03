# antara-makerbot

An automated LP bot for the Komodo AtomicDEX API.

## Prerequisites

Below is the list of prerequisites to compile `mmbot` on your machine:

-   [CMake](https://cmake.org/download/) 3.14 minimum
-   libcurl
-   gcc-8 (Linux) or clang-8 (Linux / OSX) or msvc15-2017 (Windows) minimum

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Build

To build the project please follow the instructions below:

```bash
mkdir build ## bash or powershell
cd build ## bash or powershell
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --target mmbot --config Release
cmake --build . --target mmbot-test --config Release
```

### Running the tests

Once you have build the project follow the instructions below:

```bash
## Linux / Osx
cd bin
./mmbot-test

## Windows
cd bin/your_build_type # Debug or Release
./mmbot-test.exe
```

### Installing

:construction:

### Deployment

:construction:

## Built With

-   [doctest](https://github.com/onqtam/doctest) - The fastest feature-rich C++11/14/17/20 single-header testing framework for unit tests and TDD http://bit.ly/doctest-docs
-   [restinio](https://github.com/Stiffstream/restinio) - A header-only C++14 library that gives you an embedded HTTP/Websocket server
-   [abseil](https://github.com/abseil/abseil-cpp) - Abseil Common Libraries (C++) https://abseil.io
-   [cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow) - Modern C++ Parallel Task Programming Library https://cpp-taskflow.github.io
-   [doom-st](https://github.com/doom/strong_type) - C++ implementation of strong types
-   [loguru](https://github.com/emilk/loguru) - A lightweight C++ logging library
-   [nlohmann-json](https://github.com/nlohmann/json) - JSON for Modern C++ https://nlohmann.github.io/json/
-   [restclient](https://github.com/mrtazz/restclient-cpp) - C++ client for making HTTP/REST requests http://code.mrtazz.com/restclient-cpp/
-   [reproc](https://github.com/DaanDeMeyer/reproc) - Cross-platform (C99/C++11) process library


## Code of Conduct

Before any contribution please read our [CODE OF CONDUCT](./CODE-OF-CONDUCT.md).

## Contributing

Please read [CONTRIBUTING.md](./CONTRIBUTING.md), contain the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/KomodoPlatform/antara-makerbot/tags).

## Authors

-   **Roman Sztergbaum** - _Co-Creator_ - [Milerius](https://github.com/Milerius)

See also the list of [contributors](./CONTRIBUTORS.md) who participated in this project.

## License

All information about the legal part of the project is available in the `LEGAL` folder at the root of the repository.

- COPYING: GPL V2.0: see the [COPYING](LEGAL/COPYING) file for details about copying the software.
- DEVELOPER-AGREEMENT: see the [DEVELOPER-AGREEMENT](LEGAL/DEVELOPER-AGREEMENT) file for details about authors.
- LICENSE: GPL V2.0: see the [LICENSE](LEGAL/LICENSE) file for details about the license.

## Acknowledgments

-  Viktor Kirilov [onqtam](https://github.com/onqtam) for the awesome `doctest` framework.
-  StiffStream company for the awesome `restinio` framework and the help of the team members during the development of the project.

* * *

| Badges                     |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| -------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Travis (Linux/Osx)         | [![Build Status](https://travis-ci.com/KomodoPlatform/antara-makerbot.svg?branch=master)](https://travis-ci.com/KomodoPlatform/antara-makerbot)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| AppVeyor                   | [![Build status](https://ci.appveyor.com/api/projects/status/a7hya5r8s9b3kt7l/branch/master?svg=true)](https://ci.appveyor.com/project/Milerius/antara-makerbot/branch/master)
| LGTM Quality                      | [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/KomodoPlatform/antara-makerbot.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/KomodoPlatform/antara-makerbot/context:cpp) |
| LGTM Alerts                      | [![Total alerts](https://img.shields.io/lgtm/alerts/g/KomodoPlatform/antara-makerbot.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/KomodoPlatform/antara-makerbot/alerts/) |
| Conventional Commit        | [![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://conventionalcommits.org) |
