# Celeste ![Build Status](https://travis-ci.org/carlosb/celeste.svg?branch=master) [![GitHub license](https://img.shields.io/badge/license-AGPLv3-blue.svg)](https://raw.githubusercontent.com/carlosb/celeste/master/LICENSE)

---

The system consists in obtaining data from "Solar Plants" which produce electricity
and send relevant data, like power consumption, to the cloud. From there, we can
choose to cut the power or detect faulty devices.

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
