# HIRO_WIFI
This is a Wifi control library for the H.I.R.O. Model TR-01-A

## Table of Contents
- [Overview](#overview)
- [Keyboard Control](#keyboard-control)
- [Wifi Control](#wifi-control)

## Overview
This library provides multiple example programs for controlling your HIRO robot wirelessly using a TCP server.

## Keyboard Control
WIP
## Wifi Control
This is a modified version of the Serial Control example in the base HIRO library that lets you control your HIRO unit over a wifi connection rather than the Serial Monitor. 
### Setup & Configuration
Before flashing the code to your HIRO unit, you must configure your network preferences in the code. The first of which is whether you want your HIRO unit to create its own access point or if you want it to connect to an existing network. 
```cpp
//set to true to have HIRO create its own wifi network, 
//if false HIRO will instead try to connect to the network listed in Home Wifi Settings
const bool ap = false;
```
If you set HIRO to create its own access point, you can then modify the ap_ssid and ap_password variables to determine the networks id and password will be. If you set HIRO to instead join a network, modify the home_ssid and home_password variables to match your home network. 
```cpp
//AP Settings
const char* ap_ssid = "Hiro";
const char* ap_password = "password123";
//Connnect command(using netcat) MUST BE CONNECTED TO WIFI IN ORDER TO WORK
//nc 192.168.4.1 23

//Home Wifi Settings:
const char* home_ssid = "YOUR_HOME_WIFI";
const char* home_password = "WIFI_PASSWORD";
```

### How to Connect
Once your HIRO unit has been powered on, you can send commands to it using netcat. If you set HIRO to create its own access point, make sure to connect to that access point on your device before proceeding. To connect your HIRO using netcat, run the follwoing command in your terminal(IP address defaults to 192.168.4.1):
```Bash
nc <HIRO_IP_ADDRESS> 23
```
Once you are connected, enter a command number from 1-10 followed by Enter to execute an action like you would using the Serial Control Example.