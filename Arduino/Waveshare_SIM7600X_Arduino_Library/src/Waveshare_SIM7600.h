/**
*  @filename   :   Waveshare_SIM7600.h
*  @brief      :   Implements for sim7600 library
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

#ifndef Waveshare_sim7x00_h
#define Waveshare_sim7x00_h

#include <stdlib.h>
#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <wiring.h>
#endif

//#define unsigned int uint8_t

/* Sim7x00 Class */
class Sim7x00 {

public:
  // Pin definition
  const static int powerkey = 2;
  int userkey;

  Sim7x00();
  ~Sim7x00();

  // SIM query
  void PowerOn(int PowerKey = powerkey);

  // Phone calls
  void PhoneCall(const char* PhoneNumber);

  // SMS sending and receiving message 
  bool SendingShortMessage(const char* PhoneNumber,const char* Message);
  bool ReceivingShortMessage();

  // FTP download file to Module EFS or uploading EFS file to FTP
  void ConfigureFTP(const char* FTPServer,const char* FTPUserName,const char* FTPPassWord);
  void UploadToFTP(const char* FileName);
  void DownloadFromFTP(const char* FileName);

  // GPS positoning
  bool GPSPositioning();

  //TCP and UDP communication
//  bool PDPSetting(const char* APN);
//  bool TCPClientCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode
//  bool UDPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //UDP Client Command Mode
//  bool TCPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode

  // Other functions.
  uint8_t sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout);
  char sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout);
};

extern Sim7x00 sim7600;

#endif

