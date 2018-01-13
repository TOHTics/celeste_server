# Celeste Server 

![Build Status](https://travis-ci.org/TOHTics/celeste_server.svg?branch=master)
[![GitHub license](https://img.shields.io/badge/license-AGPLv3-blue.svg)](https://raw.githubusercontent.com/carlosb/celeste/master/LICENSE)
[![API doc](https://img.shields.io/badge/api%20doc-master-ff69b4.svg)](https://github.com/TOHTics/celeste_server/blob/master/docs/API.md)
---

The server receives sensor data sent by CelesteAlpha. It saves this data in the cloud where it can later be accessed. It also provides a stable way of  passing actions/messages to CelesteAlpha.

![diagram](SolarPanels.png)

# TODO
- Add error codes
- Finish basic services
- Add better parsing for the point values
- Add JSON support for receiving data from a logger
- Port over to Boost.Beast

# Building
One can build the celeste server by creating another directory and just using cmake:
````
git clone https://github.com/carlosb/celeste.git
cd celeste
git submodule update --init --recursive

mkdir build
cd build
cmake ..
make -j4
````

# About
This software is licensed under the AGPL.
