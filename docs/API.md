# Overview
The Celeste system to query and process readings from Loggers connected to Solar Panels is described in this document. This system is a [RESTful](https://stackoverflow.com/questions/671118/what-exactly-is-restful-programming) API designed to access the information uploaded by the Loggers. In particular, this is a sub-system that is part of the whole energy-monitoring system. Please refer to the Table of Contents to see the different services offered. Also please see the section [Notes](#notes) to check various compatibility and implementation details.

# Table of Contents


1. [Overview](#overview)
2. [Device](#device)
3. [Model](#model)
4. [Device has Model](#device-has-model)
5. [Point](#point)
6. [Reading](#reading)
7. [Logger](#logger)
8. [Device Status](#device-status) 
9. [HTTP Status Codes](#HTTP-status-codes)
10. [Notes](#notes)

# Device
---

```
@ - - - - - - - - @
|  M is a Model   |
@ - - - - - - - - @

   Device
@-----------@
| M . . . M |                @ o o o o o @
| . .     . |                8           o
| .   .   . | -- @ @ @ @ --> 8  Server   o
| .     . . |    Internet    8           o
| M . . . M |                @ o o o o o @
@-----------@
```

A Device is that which contains the sensors (a.k.a Models) like a Voltmeter or a Thermometer. It is the one in charge of assembling the "Device Data Packets", which are the collection of readings from its models. The API offers these operations:

| Operation   | Method  | Url                   | Description                           |
|:-----------:|:-------:|-----------------------|---------------------------------------|
|   `get`     | GET     | `/celeste/device/`   | Gets a device and its information.    |
|   `insert`  | POST    | `/celeste/device/`   | Inserts a new device into the DB.     |
|   `remove`  | DELETE  | `/celeste/device/`   | Deletes a device on the DB along with its associated records. |

## `get`

### Request

We can query the details for a device using the following query parameters:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `DeviceId`  | Required  | Unique identifier for the device. |

E.g. 

```
http://work.tohtics.com:9001/celeste/device?DeviceId=TEST
```

### Response
```http
Status Code:    200
Status Message: OK
```

```
{
	"DeviceId" : string,
	"man" : string,
	"mod" : string,
	"sn"  : string
}
```

## `insert`
### Request
We can insert a new device into the database with the following format:

```
{
    "DeviceId" : string,
    "man" : string,
    "mod" : string,
    "sn"  : string,
    
    "models" : 
    [
       string,
       string,
       ...
    ]
}
```

Where:

| Field       | R/O       | Description                                      |
|:-----------:|:---------:|--------------------------------------------------|
| `DeviceId`  | Required  | Unique identifier for the device.                |
| `man`       | Required  | Manufacturer of the device.                      |
| `mod`       | Required  | Model of the device.                             |
| `sn`        | Required  | Unique serial number of the device.                   |
| `models`    | Optional  | Array of Model identifiers that already exist on the database. This is used when we want to insert a Device with all the following models. E.g. We want to insert a Device with a Thermometer and GPS.


### Response
```http
Status Code:    200
Status Message: OK
```

## `remove`
The `remove` operation will delete the device on the database **along with every and any record** that the device has sent. **This cannot be undone**.

### Request

```
http://[host]:[port]/celeste/device?DeviceId=[IDENTIFIER]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `DeviceId`  | Required  | Unique identifier for the device. |

### Response
```http
Status Code:    200
Status Message: OK
```
If one attempts to `remove` a device which does not exist in the database, then the operation will return the response.




# Model
---
```
    is read by
M @-------------\
M @--------------\
.                 \            @ - - - - - - - - @
.                 [D]          |  M is a Model   |
.                 /            |  D is a Device  |
M @--------------/             @ - - - - - - - - @
M @-------------/

```
A Model can be a sensor or system such as:

- GPS
- Voltmeter
- Thermometer
- Ammeter

Each Model measures a set of Points. For example, a GPS measures latitude, altitude and longitude. On the other hand, an Ammeter measures the current of an electric circuit. A Device may read various Models and is allowed to have more than two similar Models. For instance, it might have two Thermometers. See `add_model` in the [Device](#device) section for more information.


| Operation   | Method  | Url                   | Description                        |
|:-----------:|:-------:|-----------------------|------------------------------------|
|   `get`     | GET     | `/celeste/model/`    | Gets a model and its information. |
|   `insert`  | POST    | `/celeste/model/`    | Inserts a new model into the DB.  |
|   `remove`  | DELETE  | `/celeste/model/`    | Deletes a model on the DB along with its associated records.                                                              |

## `get`

### Request
We can query the details for a device using the following query parameters:

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `ModelId`   | Required  | Unique identifier for the model.  |

E.g. 

```
http://[host]:[port]/celeste/model?ModelId=[IDENTIFIER]
```

### Response

```
{
	"ModelId": string,
	"ns": string,
	"Points" : [string, ...]
}
```

## `insert`
### Request
We can insert a new device into the database with the following format:

```
{
	"ModelId" : string,
	"ns" : string
}
```

Where:

| Field       | R/O       | Description                          |
|:-----------:|:---------:|--------------------------------------|
| `ModelId`   | Required  | Unique identifier for the Model.     |
| `ns`        | Optional  | Namespace or Notes for the Model. One may use this to annotate this particular model. |



### Response

```http
Status Code:    200
Status Message: OK
```

## `remove`

The `remove` operation will delete the model on the database **along with every and any record** that the model has sent. **This cannot be undone**.

### Request

```
http://[host]:[port]/celeste/model?ModelId=[IDENTIFIER]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `ModelId`  | Required  | Unique identifier for the model.   |

### Response
```http
Status Code:    200
Status Message: OK
```
If one attempts to `remove` a device which does not exist in the database, then the operation will return the response.


# Model is assigned to Device
---

This action associates a model to a device so that it is understood as *the device with identifier `device_id` has a model `model_id`*.

| Operation   | Method  | Url                   | Description                           |
|:-----------:|:-------:|-----------------------|---------------------------------------|
|   `get`     | GET     | `/celeste/device_model/`   | Gets the details of the association between a Device and a Model.    |
|   `associate`  | POST    | `/celeste/device_model/`   | Associates a Model to the Device.     |
|   `dissasociate`  | DELETE  | `/celeste/device_model/`   | Dissasociates a Model from a Device. |

## `get`

This method has the following functionality:

- Gets a particular association given by `DeviceId`, `ModelId`, `idx`. That is, gets the details of the association between the aggregated model `ModelId` and the `DeviceId` with index `idx`.
- Gets all the associations of a `DeviceId` and `ModelId`. That is, gets the details of all the associations between the Model `ModelId` and the Device `DeviceId`.
- Gets all the Model associations that the `DeviceId`. Gets all of the associations the Device `DeviceId` has.

### Request

E.g. 

```
http://[host]:[port]/celeste/device?DeviceId=[IDENTIFIER]&ModelId=[IDENTIFIER]&idx=[INDEX]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `DeviceId`  | Required  | Unique identifier for the model. Must already exist in the database.  |
| `ModelId`   | Optional  | Unique identifier for the model. Must already exist in the database. |
| `idx`   | Optional | The aggregated index for Devices that may have multiple identical Models. If the Device only has one of `ModelId` then you should set `idx = 0`. |

### Response
```http
Status Code:    200
Status Message: OK
```

If all fields are set then:

```
{
	"DeviceId" : string,
	"ModelId" : string,
	"idx" : integer,
	"note" : string
}
```

If only `DeviceId` and `ModelId` are set, it returns the association between the Device and Models with the particular identifier:

```
[
	{
		"DeviceId" : string,
		"ModelId" : string,
		"idx" : integer,
		"note" : string
	},
	.
	.
	.
]
```

If only `DeviceId` is set, then same as above but it will return all of the Models associated with the Device.

## `associate`

Associates a Model with the Device. That is it "adds a Model to the Device". We may exemplify this by the sentence "We add a GPS to the Device with identifier 102". 

### Request

```
{
	"DeviceId" : string,
	"ModelId" : string,
	"note" : string,
	"idx" : integer
}
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `DeviceId`  | Required  | Unique identifier for the model. Must already exist in the database.  |
| `ModelId`   | Required  | Unique identifier for the model. Must already exist in the database. |
| `note`   | Optional  | A note about the association. This may be used to differentiate a duplicate model that was already associated to the device. For example, we may have two Thermometers on the Model; one in the garden and one on the roof. We may differentiate both by setting note to `"note" : "Garden"` and the other to `"note" : "Roof"`.|
| `idx` | Required | Aggregated index of the Device. For example, a Device might have two GPS. In which case, you might pass `idx : 2`. If not set, `idx` is defaulted to zero.

### Response
```http
Status Code:    200
Status Message: OK
```

The field `idx` indicates the index of the inserted model. For example, if there is already a model with the same `ModelId` on the Device then the response will look like:

```
{
	"idx" : 2
}
```

#### Example
Let's assume we have a device with identifier `DeviceId = "A001-3312-312B"`. Let's also assume that we have inserted a model named `PowerMeter` which measures, for example, watts. We would like to tell the server that Device `4001` has a model `PowerMeter`. 

```
 ------------------                       ------------
|  Device          | - - - - - - - - - - | PowerMeter |
| Id:              |    reads from        ------------
| 'A001-3312-312B' |
 ------------------
```

Our request would look like:

```
{
	"DeviceId" : "A001-3312-312B"
	"ModelId" : "PowerMeter"
	"note" : "This sensor is located next to the Inverter!"
}
```

## `dissasociate`

Dissasociates a Model from a Device.

### Request

```
http://[host]:[port]/celeste/device?DeviceId=[IDENTIFIER]&ModelId=[IDENTIFIER]&idx=[INDEX]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `DeviceId`  | Required  | Unique identifier for the model. Must already exist in the database.  |
| `ModelId`   | Required  | Unique identifier for the model. Must already exist in the database. |
| `idx` | Required | The aggregated index of the Model. |

### Response

```http
Status Code:    200
Status Message: OK
```

# Point
---

````
   is sampled by
P @-------------\
P @--------------\          @ - - - - - - - - @
.                 \         |  P is a Point   |
.                 [M]       |  M is a Model   |
.                 /         @ - - - - - - - - @ 
P @--------------/
P @-------------/
````

Points are measurements taken by `Models`. These points can be samples
such as:

- Amperage
- Wattage
- Latitude
- Longitude
- Altitude
- Temperature

A Point needs to have a data type assigned to it. Currently the following types are supported:

| Type      | id  |
| :-------: | :-: |
| `integer` | 0   |
| `float`   | 1   |
| `string`  | 2   |

The API offers the following operations:

| Operation   | Method  | Url                   | Description                           |
|:-----------:|:-------:|-----------------------|---------------------------------------|
|   `get`     | GET     | `/celeste/point/`   | Gets a point and its information.    |
|   `insert`  | POST    | `/celeste/point/`   | Inserts a new point into the DB.     |
|   `remove`  | DELETE  | `/celeste/model/`   | Deletes a point on the DB along with its associated records. |


## `get`

### Request

```
http://[host]:[port]/celeste/point?PointId=[IDENTIFIER]&ModelId=[IDENTIFIER]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `PointId`  | Required  | Unique identifier for the Point. Must already exist in the database.  |
| `ModelId`   | Required  | Unique identifier for the Model to which the Point belongs to. Must already exist in the database. |

### Response

```http
Status Code:    200
Status Message: OK
```

```
{
	"PointId" : string,
	"ModelId" : string,
	"type" : integer,
	"u" : string,
	"d" : string
}
```


## `insert`

### Request

```
{
	"PointId" : string,
	"ModelId" : string,
	"type" : integer,
	"u" : string,
	"d" : string
}
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `PointId`  | Required  | Unique identifier for the Point.  |
| `ModelId`  | Required  | Unique identifier for the Model. Must already exist in the database. |
| `type`     | Required  | Data type id of Point. |
| `u`        | Required  | Measurement units of Point. Eg. watts, volts, celsius, meters, etc.
| `d`        | Optional  | Description of Point. |   

### Response

```http
Status Code:    200
Status Message: OK
```


## `remove`

### Request

```
http://[host]:[port]/celeste/model?PointId=[IDENTIFIER]&ModelId=[IDENTIFIER]
```

Where:

| Field       | R/O       | Description                       |
|:-----------:|:---------:|-----------------------------------|
| `PointId`  | Required  | Unique identifier for the Point. Must already exist in the database.  |
| `ModelId`   | Required  | Unique identifier for the Model. Must already exist in the database. |

### Response

```http
Status Code:    200
Status Message: OK
```


# Reading
---
The Reading resource is a very important part of the Celeste system. It allows to query the server for a measurement of, for example, the power consumption. There are various different types of reading such as `last` and `range`. The API user must know the difference between any of these. To this end, we the following table makes it easy to see all the types available.

| Reading     | Description                                      |
|:-----------:|--------------------------------------------------|
|  `last`     | Gets the last reading from a sensor on a device. |
|  `range`    | Gets a range of readings from one starting date to an ending date. |
| `accumulated` | Gets the accumalated (or total) value of a range of readings. |
| `average` | Gets the average of the readings in a specified range. |
| `day` | Gets an array of the average per hour of a given day's readings. |
| `month` | Gets an array of the average per day of a given month's readings. |
| `year` | Gets an array of the average per month of a given year's readings. |
| `yesterday` | Gets an array of the average per hour of today's readings. |
| `today` | Gets an array of the average per hour of today's readings. |

## Requesting a Reading
To request the server for a reading, you must send certain parameters that allow the server to answer back with the data. 

The API offers the following operations:

| Operation   | Method  | Url                   | Description                           |
|:-----------:|:-------:|-----------------------|---------------------------------------|
|   `get`     | POST     | `/celeste/reading/`   | Gets a "reading" of information. This reading might be an actual reading, a statistic, a document, etc.   |

**The method is POST since we eventually might request more complex readings.**

If you're gonna make a lot of requests, we suggest utilizing a `keep-alive` connection. To do this, simply use the header:

```http
Connection: keep-alive
```

Which will keep the connection alive for up to 20 seconds.

## `last`

Retrieves the last reading uploaded to the server. Note that if the Device is not uploading data then this method will always return the same value.

### Request

```
{
    "DeviceId" : string,
    "ModelId" : string,
    "PointId" : string,
    "method" : "last"
}
```

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Unique identifier of a device.                   |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `last` |
|  `start`      | Start time. |
|  `end`      | End time. |

### Response

```http
Status Code:    200
Status Message: OK
```

```
{
	"value" : string, integer, double,
	"sf" : double
	"t" : string
}
```

Where:

| Field     | Description                                      |
|:---------:|--------------------------------------------------|
|  `value`  | Value of the reading. The type of the value may be one of the following: `string`, `integer`, `double` |\
| `sf` | Scale factor to multiply by. That is, you should perform the following operation: `value * 10^sf` . This may be `null`. |
|  `t`      | Timestamp indicating time and date when the measurement took place.|

## `range` 

Retrieves a range of readings in the interval from start to end. That is, an array of readings. The list is in descending order, with the first entry being the closest time to `end`.


| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Unique identifier of a device.                   |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `range` |
|  `start`      | Start time. |
|  `end`      | End time. |


**\*The range is inclusive from both sides.**

### Response
The response an ordered JSON array of values with their timestamps in ascending order, starting with the earliest date.

```http
Status Code:    200
Status Message: OK
```

```
[
	{
		"value" : value1,
		"sf" : sf1,
		"t" : timestamp1
	},
	{
		"value" : value2,
		"sf" : sf2,
		"t" : timestamp2
	},
	.
	.
	.
	{
		"value" : valueN,
		"sf" : sfN,
		"t" : timestampN
	}
]
```


### Example
An example might be taking requesting all the readings from a Thermometer on a Device with id `"A001-3312-312B"` from `21-12-2017 00:00:01` to `22-12-2017 00:00:01`. The request will look like:

```
{
	"DeviceId" : "A001-3312-312B",
	"ModelId" : "Thermometer",
	"PointId" : "Temperature"
	"method" : "range",
	
	"start" : "21-12-2017 00:00:01",
	"end" : "22-12-2017 00:00:01"
}
```
The response might look like:

```
[
	{
		"value" : 20.2,
		"sf" : 0.0,
		"t" : 21-12-2017 00:00:01
	},
	{
		"value" : 27.4,
		"sf" : 0.0,
		"t" : 21-12-2017 08:00:01
	},
	{
		"value" : 35.1,
		"sf" : 0.0,
		"t" : 21-12-2017 16:00:01
	},
	{
		"value" : 21.7,
		"sf" : 0.0,
		"t" : 22-12-2017 00:00:01
	}
]
```

## `accumulated`

Gets an accumulated reading from the server. That is the total sum of the readings in the interval between `start` and `end`.

### Request
```
{
	"DeviceId" : 
	[
		string,
		string,
		...
	],
	"ModelId" : string,
	"PointId" : string,
	"method" : "accumulated",

	"start" : string in utc format,
	"end" : string in utc format
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Array of Device identifiers.                  |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `range` |
|  `start`      | Start time. |
|  `end`      | End time. |


### Response

```
{
    "DeviceId1" : double,
    "DeviceId2" : double,
    ...
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceIdxxx`    | Accumulated total. |

### Example

We might request this service like this:

```
{
	"DeviceId" : ["A001-3312-312B", "A002-3312-312B"],
	"ModelId" : "potenciometro",
	"PointId" : "consumo",

	"method" : "accumulated",
	"start" : "2017-12-19 17:18:20",
	"end" : "2017-12-19 20:18:20"
}
```
And get the following, example, response:

```
{
	"A001-3312-312B": 3016.0,
	"A002-3312-312B": 1661.0
}
```

## `average`

Returns the average of the interval between `start` and `end`.

### Request

```
{
    "DeviceId" : [string, ...],
    "ModelId" : string,
    "PointId" : string,
    "method" : "average",

    "start" : string in utc format,
    "end" : string in utc format
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Array of Device identifiers.                       |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `average` |
|  `start`      | Start time. |
|  `end`      | End time. |


### Response

```
{
    "DeviceId1" : double,
    "DeviceId2" : double,
    ...
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceIdxxx`    | Accumulated total. |


## `day`

Returns up to 24 entries that represent the average reading per hour of the given day. This method will return an array of pairs per each Device passed. Each pair contains the hour along with the average. You may pass the day in either of the formats:

`yyyy-mm-dd hh:mm:ss`

`yyyy-mm-dd`

Either way, only the day will be extracted.

### Request

```
{
    "DeviceId" : [string, ...],
    "ModelId" : string,
    "PointId" : string,
    "method" : "day",
    "day" : string in utc format
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Array of Device identifiers.                   |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `day` |
| `day` | Given day. |


### Response

The first field `integer` is the hour (0 - 23). The second is the average reading in that hour.

```
{
    "DeviceIdentifier": [
        [
            integer,
            double
        ],
        ...
    ]
    ...
}
```


## `month`

Returns up to 31 entries that represent the average reading per day of the given month. This method will return an array of pairs per each Device passed. Each pair contains the day along with the average. You may pass the day in either of the formats:

`yyyy-mm-dd hh:mm:ss`

`yyyy-mm-dd`

Either way, only the day will be extracted.

### Request

```
{
    "DeviceId" : [string, ...],
    "ModelId" : string,
    "PointId" : string,
    "method" : "month",
    "month" : string in utc format
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Array of Device identifiers.                   |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `month` |
| `month` | Given day. |


### Response

The first field `integer` is the day (1 - 31). The second is the average reading in that day.

```
{
    "DeviceIdentifier": [
        [
            integer,
            double
        ],
        ...
    ]
    ...
}
```

## `year`

Returns up to 12 entries that represent the average reading per month of the given year. This method will return an array of pairs per each Device passed. Each pair contains the month along with the average. You may pass the day in either of the formats:

`yyyy-mm-dd hh:mm:ss`

`yyyy-mm-dd`

Either way, only the day will be extracted.

### Request

```
{
    "DeviceId" : [string, ...],
    "ModelId" : string,
    "PointId" : string,
    "method" : "year",
    "year" : string in utc format
}
```

Where:

| Parameter   | Description                                      |
|:-----------:|--------------------------------------------------|
|  `DeviceId` | Array of Device identifiers.                   |
|  `ModelId`  | Unique identifier of a model on the device.      |
|  `PointId`  | Unique identifier of a point which belongs to the model. |
|  `method`   | `year` |
| `year` | Given day. |


### Response

The first field `integer` is the month (1 - 12). The second is the average reading in that month.

```
{
    "DeviceIdentifier": [
        [
            integer,
            double
        ],
        ...
    ]
    ...
}
```

# Logger
---
Like the [Reading](#reading) resource, the Logger resource is one of the most important parts of the whole Celeste system. This is the resource which will listen for incoming records sent by the Devices. This being said, there are 3 specific formats which the logger will understand: XML, JSON and CelesteRN. Currently only XML is supported.

It will be necessary to specify the following headers whenever you use the logger:

```http
Content-Length: length
Content-Type: application/xml
```
| Header            | Description                           |
|:-----------------:|---------------------------------------|
| `Content-Length`  | The number of characters in the body. |
| `Content-Type`    | One of the following: <br/> `application/xml` <br/> `application/json`<br/> `application/CelesteRN`             |


## Uploading to the Logger
To upload to the Logger, one must send their data to the URL:

`http://[host]:[port]/celeste/logger/upload` 

as a POST method. The server will then answer back with a `200` code only if the records were accepted and no errors were found. This is summarized in the following table:

| Operation   | Method  | Url                           |
|:-----------:|:-------:|-------------------------------|
|   `upload`  | POST    | `/celeste/logger/upload/`     |
|   `upload`  | POST    | `/celeste/logger/upload/verbose/`      |

The first path will have no verbose response.

### Request
We have said that the server will only accept a handful of structured formats. The intention of this section is to give the formal formats that are required.

#### XML
The following is the format to send the records in:

```xml
<SunSpecData v="1.0">
    <d id="[0-9]+" lid="[0-9]+" t="YYYY-MM-DD HH:MM:SS">
        <m id="[a-zA-Z]+" x="[0-9]+">
        	<p id="[a-zA-Z]+" sf="[0-9]+" t="YYY-MM-DD HH:MM:SS">
        		VALUE
        	</p>
        	...
        </m> 
    	...
    </d>
    ...
</SunSpecData>
```
The `...` is meant to indicate a list. So clearly  `SunSpecData` contains a list of `Devices`. Each Device `d` contains a list of Models. Each Model `m` contains a list of Points. Each Point `p` only contains a value. This value may be a `string` or a `number`. 

#### Tag Table
Not all of the attributes defined in the grammar are mandatory. It proves useful to build a table to indicate what each of these tags mean and whether they are optional or mandatory.

| Tag     | R/O      | Description                                                  |
|:-------:|:--------:|--------------------------------------------------------------|
| `v`     | Required | Version number of grammar.                                   |
| `d.id`  | Required | Identifier of Device. E.g. 1443                              |
| `d.t`   | Required | Timestamp at which the records were assembled.               |
| `d.lid` | Optional | Identifier of the Logger which the Device sends its data to. |
| `m.id`  | Required | Identifier of the Model. E.g. Thermometer.                   |
| `m.x`   | Required | Aggregated index of the Model on the Device. Used to differentiate between two identical models on the Device. |
| `p.id`  | Required | Identifier of the Point on the Model. E.g. Celsius.
| `p.sf`  | Optional | Scale factor to use. Needed to calculate: `scaled_value = value * 10^(sf)`. If not specified then default value is `0`.
| `p.t`   | Optional | Timestamp at which the measurement was taken.

### Response
#### Success
A succesful response implies that the records we succesfully persisted into the database and no errors were found. As of writing, it is of no interest to answer back with a verbose message to indicate success. Therefore, the HTTP response will look like:

```http
Status Code:    200
Status Message: OK
HTTP Version:   1.1
HTTP Protocol:  HTTP
```

#### Failure
On failure, if `verbose?=1` then it will answer back with a verbose message. If `verbose?=0` we can expect to only receive a response like the one shown before. That will mostly (and only) depend on the status code. Practically, the non-verbose versions will be the same as the verbose ones, only they won't have the body. Thus, if one reads the following verbose responses, then one knows their non-verbose versions. For each response, we give the HTTP status code returned and what the body will look like. One may notice that the HTTP codes and the `status` field in the response body **will not coincide**. This is expected since they have another meaning. As of current writing, it is in development more specific error messages and codes. The HTTP codes will remain the same, but more codes for the sub-protocol will be added. For now, it is sufficient to follow the HTTP error semantics. 

##### Parsing Error

```http
Status Code:    400
Status Message: BAD REQUEST
```

```xml
<SunSpecDataResponse>
	<status> 400 </status>
	<code> FAILURE </code>
	<reason> Parsing Error </reason>
	<message>
		[Message indicating where the error is]
	</message>
</SunSpecDataResponse>
```


##### Database Error

```http
Status Code:    500
Status Message: INTERNAL SERVER ERROR
```

```xml
<SunSpecDataResponse>
	<status> 400 </status>
	<code> FAILURE </code>
	<reason> Parsing Error </reason>
	<message>
		[Message explaining why the insertion to the database failed]
	</message>
</SunSpecDataResponse>
```

## XML Example
To make the usage of the grammar simpler, we will give an example. Suppose we would like to save some records by a Device with only one Thermometer. Firstly we need to pinpoint the Device by its identifier. Say this Device in particular has the identifier `4001`. The scheme will then look like this:

```
@ - - - - - - - - - @
| Date: 2017-11-21  |
| Time: 15:30:01    |
@ - - - - - - - - - @

                      [Thermometer] = 30 C
                             |
                             |
 --------------              |                @ o o o o o o @
|  Device      |- - - - - -  +                o             o
| Id: "A4001"  |- - - - @ @ @ @ @ @ - - - - > o   Celeste   o
 --------------           Internet            o    Server   o
                       [ Data Packet ]        o             o
                                              @ o o o o o o @
```

We would like to know what `[Data Packet]` looks like. As we have said previously, this data will take the form of an HTTP Response with the body containing the information of the actual measurements. We notice that we would like to tell the server we have recorded a reading of `30` degrees celsius and we assembled the packet at the date and time shown. It is of use to give the values which each field takes at this instance.

| Tag     | Value                 |
|:-------:|:---------------------:|
| `v`     | 1.0                   |
| `d.id`  | A4001                 |
| `d.t`   | 2017-11-21 15:30:01   |
| `m.id`  | Thermometer           |
| `m.x`   | 1                     |
| `p.id`  | Temperature           |

Translating over to XML we get:

```xml
<SunSpecData v="1.0">
    <d id="4001" t="2017-11-21 15:30:01">
    	<m id="Thermometer" x="1">
    		<p id="Temperature">
    			30
    		</p>
    	</m> 
    </d>
</SunSpecData>
```

One need not worry about the units of measurement since it is required to specify them when one first inserts a Point into the database.

# Device Status

It will be necessary to be able to query the Device Status from a client that might manage them, or the Device itself. 

The following operations are valid:

| Operation   | Method  | Url                           |
|:-----------:|:-------:|-------------------------------|
|   `get`     | GET     | `/celeste/device/status/`     |
|   `update`  | PUT     | `/celeste/device/status/`     |


There is no specified format for the Device. The developers of the Device are free to choose their own JSON representation and parse it accordingly.


## `get` - Querying the Status

### Request

```
http://[host]:[port]/celeste/device/status/?DeviceId=[IDENTIFIER]
```

### Response

```
document
```

## `update` - Updating the Device Status

The Device is free to post whatever it wants. The document that gets posted, will be the one that is answered with in the `get` request.

### Request

```
{
	"DeviceId" : string,
	"status" : document
}
```


# Notes

- All the timestamps must be in UTC Posix: `YYYY-MM-DD HH:MM:SS`.
- You may send only the date `YYYY-MM-DD`.
