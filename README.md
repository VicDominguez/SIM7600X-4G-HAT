<div align="center">
    <h3 align="center">SIM7600X-4G-HAT</h3>
    <p>Expansion library of SIM7600X-4G-HAT module.</p>
</div>

## Table of Contents

- [🤔 What is this project?](#what-is-this-project-)
- [🎯 Key Features](#key-features-)
- [🚀 Getting Started](#getting started-)
- [💙 Contributing](#contributing-)
- [🙏 Credits](#credits-)

## 🤔 What is this project?

This project is an extension library for the SIM7600X-4G-HAT module, providing useful features for developers who wish to use this module in their projects.

Currently source code supports Arduino, Jetson Nano, Raspberry Pi and STM32, but Raspberry Pi is the only one that has been extended until the moment.

*SIM7600X 4G HAT* [official demo]([SIM7600X 4G & LTE Cat-1 HAT - Waveshare Wiki](https://www.waveshare.com/wiki/SIM7600X_4G_%26_LTE_Cat-1_HAT#Program)) from Waveshare has been taked as starting point.

## 🎯 Key Features (only for Raspberry Pi)

* A simple C wrapper to enable devs to use it on C projects, alongside original C++ implementation.
* Init module function without SIM card, used mainly to use GPS standalone.
* Functions to use GPS as a reader, either single or multiple reads. This functions collect GPS raw data and parse it into a string of "<latitude>, <longitude>".
  * Both measures are a float number
  * If latitude is positive, means North, and if negative South.
  * If longitude is positive, means East, and if negative West.
* Support for init and close GPS module.
* Added support for sending AT commands without traces on stdout.



## 🚀 Getting Started

### Arduino

Code is provided as a typical library, so no special setup is needed.

### Jetson Nano

Code is provided as a set of Python scripts, being examples of usage.

### Raspberry Pi

For this version, C, C++ and Python are supported. 

C++ code can be compiled via Makefiles.

C support is achieved via C++ wrapper thar you can link against it in a typical C setup.

Python version provides a bunch of example scripts, like Jetson Nano.

### STM 32

Not documented yet.

## 💙 Contributing

Any contributions you make are **greatly appreciated**, so if you have any idea of how to make this project better, please [create a pull request](https://github.com/VicDominguez/SIM7600X-4G-HAT/pulls). Also if you find any bug, please [create an issue](https://github.com/VicDominguez/SIM7600X-4G-HAT/issues/new)

## 🙏 Credits

This project was originally created by @VicDominguez to use this module in the context of his Final Degree Project, *Smart Driving Assistant*.
