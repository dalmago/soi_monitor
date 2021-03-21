# Soil Monitor
ESP8266 based soil humidity monitor using resistive sensors.

Using [Ubidots](http://ubidots.com/) as dashboard and MQTT broker.

# TODO:

* Implement MQTT QoS 2
* Implement multitasking
* Enable OTA update
* Optimize event handling

# Circuit Schematic

The circuit schematic can be seen by using [Falstad Circuit](http://www.falstad.com/circuit/circuitjs.html) and importing the following text:
'''
$ 1 0.000005 11.558428452718767 59 5 43 5e-11
t 320 208 352 208 0 1 -2.776379241778025 0.5850499442985184 100 default
r 352 224 352 272 0 1000
r 256 208 208 208 0 10000
r 320 208 320 272 0 100000
g 320 272 320 288 0 0
g 400 320 400 336 0 0
R 352 160 352 128 0 0 40 5 0 0 0.5
R 192 176 192 144 0 0 40 3.3 0 0 0.5
w 352 160 352 192 0
w 192 176 192 208 0
w 208 208 192 208 0
174 352 272 352 320 0 10000 0.1436 Resistance
w 368 304 400 304 0
w 400 320 400 304 0
w 352 272 416 272 0
174 256 160 320 176 0 500000 0.0545 Resistance
w 256 208 256 160 0
w 288 176 288 208 0
w 288 208 320 208 0
x 360 195 402 198 4 12 2N3904
o 1 64 3 4098 1.25 0.00625 0 1
o 14 64 0 x81016 5 0.00009765625 1 1 0.25 0
'''
