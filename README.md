# Celeste Server 

![Build Status](https://travis-ci.org/TOHTics/celeste_server.svg?branch=master)
[![GitHub license](https://img.shields.io/badge/license-AGPLv3-blue.svg)](https://raw.githubusercontent.com/carlosb/celeste/master/LICENSE)
[![API doc](https://img.shields.io/badge/api%20doc-master-ff69b4.svg)](https://github.com/TOHTics/celeste_server/blob/master/docs/API.md)
---

The server receives sensor data sent by CelesteAlpha. It saves this data in the cloud where it can later be accessed. It also provides a stable way of  passing actions/messages to CelesteAlpha.

![diagram](SolarPanels.png)


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

# Usage

```
bash > ./celeste_server -h

  -h [ --help ]                    Help screen
  -r [ --api-root ] arg (=celeste) Root where to run the service; where the API
                                   may be accessed.
                                   E.g. http://[host]:[port]/[api-root]/
  -p [ --port ] arg (=9001)        Port where CelesteServer will run.
  -j [ --worker-limit ] arg (=4)   Limit of workers that the server has 
                                   available for use.
  --db arg (=celestesensordata)    The name of the schema.
  --user arg (=root)               Name of user for access to the DB.
  --password arg                   Password to authenticate the user on the DB.
  --db-port arg (=3306)            Port where the DB listens for incoming 
                                   connections.

```

# About
This software is licensed under the AGPL.
