/**
*  @filename   :   main.cpp
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

#include "arduPi.h"
#include "sim7x00.h"

// Pin definition
int POWERKEY = 6;

int8_t answer;

/***********************Phone calls**********************/
//char phone_number[15];                          //Store the phone number you want to call
char text_message[]="www.waveshare.com"; 
char phone_number[] = "15099940168";			//15889529268

/***********************FTP upload and download***************************/
char ftp_user_name[] = "**********";
char ftp_user_password[] = "**********";
char ftp_server[] = "www.waveshare.net";
char download_file_name[] = "index3.htm";
char upload_file_name[] = "index2.htm";

/*********************TCP and UDP**********************/
char APN[] = "CMNET";
char aux_string[60];
char ServerIP[] = "113.81.235.90";
char Port[] = "1822";
char RecMessage[20];
char EndSending[10];
int i = 0,j; 


void setup() {
	sim7600.PowerOn(POWERKEY);
//	sim7600.PhoneCall(phone_number);
//	sim7600.SendingShortMessage(phone_number,text_message);
//	sim7600.ReceivingShortMessage();
//	sim7600.ConfigureFTP(ftp_server,ftp_user_name,ftp_user_password);
//	sim7600.UploadToFTP(upload_file_name);
//	sim7600.DownloadFromFTP(download_file_name);
//  sim7600.GPSPositioning();

	/*************Network environment checking*************/
	Serial.println("AT+CSQ");
	sim7600.sendATcommand("AT+CREG?", "+CREG: 0,1", 500);
	Serial.println("AT+CPSI?");
	sim7600.sendATcommand("AT+CGREG?", "+CGREG: 0,1", 500);

	/*************PDP Context Enable/Disable*************/
	/****************************************************
	PDP context identifier setting

	AT+CGDCONT = <cid>,<PDP_type>,<APN>...
	<cid>:minimum value =1
	<PDP_type>: IP、PPP、IPV6、IPV4V6
	<APN>：(Access Point Name)
	****************************************************/
	sprintf(aux_string,"AT+CGSOCKCONT=1,\"IP\",\"%s\"", APN);						 
	sim7600.sendATcommand(aux_string, "OK", 1000);  					//APN setting
	sim7600.sendATcommand("AT+CSOCKSETPN=1", "OK", 1000);  				//PDP profile number setting value:1~16

	/*********************Enable PDP context******************/
	sim7600.sendATcommand("AT+CIPMODE=0", "OK", 1000);					//command mode,default:0
	sim7600.sendATcommand("AT+NETOPEN", "+NETOPEN: 0", 1000);					//Open network
	sim7600.sendATcommand("AT+IPADDR", "+IPADDR:", 1000);				//Return IP address
//	sim7600.sendATcommand("AT+NETCLOSE", "OK", 1000);					//Close network

	memset(aux_string,'\0',30);
	/*********************TCP client in command mode******************/
	snprintf(aux_string, sizeof(aux_string), "AT+CIPOPEN=0,\"%s\",\"%s\",%s", "TCP",ServerIP, Port);
	sim7600.sendATcommand(aux_string, "+CIPOPEN: 0,0", 1000);			//Setting tcp mode、server ip and port
//	Serial.println("AT+CIPSEND=0,5");									//Sending "Hello" to server.
	sim7600.sendATcommand("AT+CIPSEND=0,", ">", 1000);					//If not sure the message number,write the command like this: AT+CIPSEND=0, (end with 1A(hex))
	Serial.println("HELLO");
	sim7600.sendATcommand(",", "OK", 1000);								//End of sending with 26(HEX:1A)										
	sim7600.sendATcommand("AT+CIPCLOSE=0", "+CIPCLOSE: 0,0", 15000);	//close by local
	sim7600.sendATcommand("AT+NETCLOSE", "+NETCLOSE: 0", 1000);			//Close network
}


void loop() {

}

int main() {
	setup();
	while (1) {
		loop();
	}
	return (0);
}