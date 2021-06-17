/**
*  @filename   :   AT.cpp
*  @brief      :   Implements for sim7600c 4g hat raspberry pi demo
*  @author     :   Kaloha from Waveshare
*
*  Copyright (C) Waveshare     April 27 2018
*  http://www.waveshare.com / http://www.waveshare.net
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "../arduPi.h"
#include "../sim7x00.h"

// Pin definition
int POWERKEY = 6;

void setup() {
	sim7600.PowerOn(POWERKEY);
}


void loop() {
	char at_command[100];
	memset(at_command, '\0', 100);    // Initialize the string
	delay(100);
	while (Serial.available() > 0) Serial.read();    // Clean the input buffer

	printf("Please input the AT command\n:");
	scanf("%s", at_command);

	Serial.println(at_command);
	sim7600.sendATcommand(at_command, 2000);
}

int main() {
	setup();
	while (1) {
		loop();
	}
	return (0);
}