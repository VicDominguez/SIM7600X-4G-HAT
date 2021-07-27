# Custom SIM7600X-4G-HAT

Realised by Victor Manuel Dominguez Rivas for his Final Degree Project called Smart Driving Assistant.

This repository is based on SIM7600X-4G-HAT Demo of Waveshare page and adds new features of Raspberry version:

* C wrapper, in order to use this functions from C.
* Init module without SIM (GPS usage)
* Send AT commands with less prints
* Functions to init and close GPS session (in order to read multiple times without open and close GPS)
* Obtain GPS coordenates instead printing all datagram. This function process the datagram and returns a string with just the coordenates.
