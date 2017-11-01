# tempmonitor

A simple web interface which subscribes to an MQTT topic branch to
display the information from a temperature sensor.

The sensor is an Arduino which publishes a `1` when it comes online to the topic:

```
'dev/node1/led1'
```

and which otherwise publishes an integer to the

```
'dev/node1/temperature'
```

topic.

The brunt of the work (design, layout, etc.) was based from [mqtt-panel](https://github.com/fabaff/mqtt-panel), written by Fabian Affolter.


The screenshot shows an example:

![screenshot](https://raw.githubusercontent.com/edgarreis/senseiot/master/sense2.0/Dockers/Apache/Monitor/Monitor.png)
