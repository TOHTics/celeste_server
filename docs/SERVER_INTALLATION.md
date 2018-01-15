# Overview

This document mainly concerns itself with the installation of the Celeste server on a Ubuntu machine. The installation consists mainly of the following steps:

- Downloading the code
- Compiling
- Installing
- Running the server

It is expected that these steps become easier as the project advances. It is also expected, however, that we have installed some requirements:

- `git` - To download the code
- `>= clang-3.5` - To build the server
- `>= gcc-4.9` - To build the dependencies. You can use clang too.
- `>= cmake-3.0` - To configure the Makefile
- `>= boost1.55` - Dependency

If you don't have any one these dependencies, you can check the section [Dependency Installation](#dependency-installation) on how ot install these.

# Table of Contents


1. [Overview](#overview)
2. [Downloading the Code](#downloading-the-code)
3. [Compiling](#compiling)
4. [Installing](#installing)
5. [Running](#running)
6. [Dependency Installation](#dependency-installation)


# Downloading the Code

The code is hosted over at [Github](https://github.com/TOHTics/celeste_server). To download the code along with the dependencies, please execute the following commands:

```
git clone https://github.com/TOHTics/celeste_server
cd celeste_server
git submodule update --init --recursive
```

This will create a directory `celeste_server` and `cd` into it. It will then initialize and download any submodules it requires.

# Compiling

Compiling the server requires `cmake >= 3.0`, `>=clang-3.5`, and `gcc5` if you are missing some basic dependencies. If you don't care about which dependencies you already have installed, or which are needed please execute the following commands:

```
sudo apt-get update -qq
sudo apt-get install -y -qq git
sudo apt-get install -y -qq  build-essential clang-3.5 cmake libboost1.55-all-dev libssl-dev libc++-dev libc++-helpers libc++1 
```

To compile the code, simply go into the download directory and use `cmake` to configure the build. Then `make` the project, replacing `[NUMBER OF CORES]` with the number of cores your system has:

```
cd celeste_server
mkdir build
cd build
cmake ..
make -j[NUMBER OF CORES]
```

For example, if your system has 4 cores:

```
make -j4
```

or if it has one:

```
make
```

If everything built succesfully, you should see these last lines:

```
[100%] Linking CXX executable celeste_server
[100%] Built target celeste_server
```

## Choosing the Compiler
If you are getting an error during build, you might consider explictly setting  the compiler just before the build:

```
export CC=clang-3.5
export CXX=clang++-3.5

cmake ..
make -j4
```

## What if the build fails?

First, check the solution above. Secondly, check that **all of the dependencies are installed**. If you are still getting an error, go to [Issues](https://github.com/TOHTics/celeste_server/issues) and look for any tickets that describe your problem. If you couldn't find an issue you should report a ticket describing your error. Your ticket should follow the title format:

```
Build fails on [OPERATING SYSTEM NAME AND VERSION] using [COMPILER AND VERSION] 
```

For example:

```
Build fails on macOS Sierra using clang-3.5
```

The body of the ticket **should contain the error log and a description of the error**.

# Installing



# Dependency Installation

