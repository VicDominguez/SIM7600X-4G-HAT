/**
*  @filename   :   Waveshare_SIM7600.cpp
*  @brief      :   Implements for sim7600 library
*  @author     :   Kaloha from Waveshare
*
*  Copyright (C) Waveshare     April 27 2018
*  http://www.waveshare.com  http://www.waveshare.net
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

#include "Waveshare_SIM7600.h"

Sim7x00::Sim7x00(){
}

Sim7x00::~Sim7x00(){
}


/**************************Power on Sim7x00**************************/
void Sim7x00::PowerOn(int PowerKey = powerkey){
   uint8_t answer = 0;

  Serial.begin(9600);

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    Serial.print("Starting up...\n");
    
    pinMode(PowerKey, OUTPUT);
    // power on pulse
    digitalWrite(PowerKey, HIGH);
    delay(500);
    digitalWrite(PowerKey, LOW);
    
    // waits for an answer from the module
    while (answer == 0) {     // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);
	  delay(1000);
    }

  }

  delay(5000);

  while ((sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0)
    delay(500);
}

/**************************Phone Calls**************************/
void Sim7x00::PhoneCall(const char* PhoneNumber) {
  char aux_str[30];

//  Serial.print("Enter the phone number:");

//  scanf("%s", PhoneNumber);

  sprintf(aux_str, "ATD%s;", PhoneNumber);
  sendATcommand(aux_str, "OK", 10000);

  // press the button for hang the call 
  //while (digitalRead(button) == 1);

  delay(20000);

  Serial.println("AT+CHUP");            // disconnects the existing call
  Serial.print("Call disconnected\n");
}

/**************************SMS sending and receiving message **************************/
//SMS sending short message
bool Sim7x00::SendingShortMessage(const char* PhoneNumber,const char* Message){
  uint8_t answer = 0;
  char aux_string[30];

  Serial.print("Setting SMS mode...\n");
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  Serial.print("Sending Short Message\n");
    
  sprintf(aux_string,"AT+CMGS=\"%s\"", PhoneNumber);

  answer = sendATcommand(aux_string, ">", 3000);    // send the SMS number
  if (answer == 1)
  {
      Serial.println(Message);
      Serial.write(0x1A);
      answer = sendATcommand("", "OK", 20000);
      if (answer == 1)
      {
          Serial.print("Sent successfully \n"); 
          return true;   
      }
      else
      {
          Serial.print("error \n");
          return false;
      }
  }
  else
  {
 //     Serial.print(answer);
      Serial.print(" error.\n");
      return false;
  }
}

//SMS receiving short message
bool Sim7x00::ReceivingShortMessage(){
  uint8_t answer = 0;
  int i = 0;
  char RecMessage[200];

  Serial.print("Setting SMS mode...\n");
    sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000);    // selects the memory

    answer = sendATcommand("AT+CMGR=1", "+CMGR:", 2000);    // reads the first SMS

  if (answer == 1)
    {
        answer = 0;
        while(Serial.available() == 0);
        // this loop reads the data of the SMS
        do{
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            if(Serial.available() > 0){    
                RecMessage[i] = Serial.read();
                i++;
                // check if the desired answer (OK) is in the response of the module
                if (strstr(RecMessage, "OK") != NULL)    
                {
                    answer = 1;
                }
            }
        }while(answer == 0);    // Waits for the asnwer with time out
        
 //       RecMessage[i] = '\0';
        
        Serial.print(RecMessage); 
        Serial.print("\n");      
        
    }
    else
    {
        Serial.print(answer);
        Serial.print(" error.\n");
    return false;
    }

  return true;
}

/**************************FTP download file to Module EFS , uploading EFS file to FTP**************************/
void Sim7x00::ConfigureFTP(const char* FTPServer,const char* FTPUserName,const char* FTPPassWord){
    char aux_str[50];
  
    // sets the paremeters for the FTP server
  sendATcommand("AT+CFTPPORT=21", "OK", 2000);
  sendATcommand("AT+CFTPMODE=1", "OK", 2000);
  sendATcommand("AT+CFTPTYPE=A", "OK", 2000);

//  Serial.print(aux_str,"AT+CFTPSERV=\"%s\"", FTPServer);

    sprintf(aux_str,"AT+CFTPSERV=\"%s\"", FTPServer);
    sendATcommand(aux_str, "OK", 2000);
    
    sprintf(aux_str, "AT+CFTPUN=\"%s\"", FTPUserName);
    sendATcommand(aux_str, "OK", 2000);
    sprintf(aux_str, "AT+CFTPPW=\"%s\"", FTPPassWord);
    sendATcommand(aux_str, "OK", 2000);
}

void Sim7x00::UploadToFTP(const char* FileName){
  char aux_str[50];

  Serial.print("Upload file to FTP...\n");
  sprintf(aux_str, "AT+CFTPPUTFILE=\"%s\",0", FileName);
  sendATcommand(aux_str, "OK", 2000);
}

void Sim7x00::DownloadFromFTP(const char* FileName){
  char aux_str[50];
  
  Serial.print("Download file from FTP...\n");
  sprintf(aux_str, "AT+CFTPGETFILE=\"%s\",0", FileName);
    sendATcommand(aux_str, "OK", 2000);
}

/**************************GPS positoning**************************/
bool Sim7x00::GPSPositioning(){

    uint8_t answer = 0;
    bool RecNull = true;
    int i = 0;
    char RecMessage[200];
    char LatDD[3],LatMM[10],LogDD[4],LogMM[10],DdMmYy[7] ,UTCTime[7];
    int DayMonthYear;
    float Lat = 0;
    float Log = 0;
    
    memset(RecMessage, '\0', 200);    // Initialize the string
    memset(LatDD, '\0', 3);    // Initialize the string
    memset(LatMM, '\0', 10);    // Initialize the string
    memset(LogDD, '\0', 4);    // Initialize the string
    memset(LogMM, '\0', 10);    // Initialize the string
    memset(DdMmYy, '\0', 7);    // Initialize the string
    memset(UTCTime, '\0', 7);    // Initialize the string

    Serial.print("Start GPS session...\n");
    sendATcommand("AT+CGPS=1,1", "OK", 1000);    // start GPS session, standalone mode

    delay(2000);

    while(RecNull)
    {
        answer = sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000);    // start GPS session, standalone mode

        if (answer == 1)
        {
            answer = 0;
            while(Serial.available() == 0);
            // this loop reads the data of the GPS
            do{
                // if there are data in the UART input buffer, reads it and checks for the asnwer
                if(Serial.available() > 0){    
                    RecMessage[i] = Serial.read();
                    i++;
                    // check if the desired answer (OK) is in the response of the module
                    if (strstr(RecMessage, "OK") != NULL)    
                    {
                        answer = 1;
                    }
                }
            }while(answer == 0);    // Waits for the asnwer with time out
            
            RecMessage[i] = '\0';
            Serial.print(RecMessage);
            Serial.print("\n");

            if (strstr(RecMessage, ",,,,,,,,") != NULL) 
            {
                memset(RecMessage, '\0', 200);    // Initialize the string
                i = 0;
                answer = 0;
                delay(1000);
            }
            else
            {
                RecNull = false;
                sendATcommand("AT+CGPS=0", "OK:", 1000);
            } 
        }
        else
        {
            Serial.print("error \n");
            return false;
        }
        delay(2000);

    }

    strncpy(LatDD,RecMessage,2);
    LatDD[2] = '\0';
//    Serial.print("LatDD:");
//    Serial.print(LatDD);
    
    strncpy(LatMM,RecMessage+2,9);
    LatMM[9] = '\0';
//    Serial.print(" LatMM:");
//    Serial.print(LatMM);
    
    Lat = atoi(LatDD) + (atof(LatMM)/60);
    if(RecMessage[12] == 'N')
    {
        Serial.print("Latitude is ");    
        Serial.print(Lat);  
        Serial.print(" N\n");   
      }
    else if(RecMessage[12] == 'S')
    {
        Serial.print("Latitude is ");    
        Serial.print(Lat);
        Serial.print(" S\n");    
      }
    else
        return false;

    strncpy(LogDD,RecMessage+14,3);
    LogDD[3] = '\0';
//    Serial.print("LogDD:");
//    Serial.print(LogDD);
    
    strncpy(LogMM,RecMessage+17,9);
    LogMM[9] = '\0';
//    Serial.print("LogMM:");
//    Serial.print(LogMM);
    
    Log = atoi(LogDD) + (atof(LogMM)/60);
    if(RecMessage[27] == 'E')
    {
        Serial.print("Longitude is "); 
        Serial.print(Log);
        Serial.print(" E\n");
      }
    else if(RecMessage[27] == 'W')
    {
        Serial.print("Latitude is ");
        Serial.print(Lat);
        Serial.print(" W\n");      
      }
    else
        return false;

    strncpy(DdMmYy,RecMessage+29,6);
    DdMmYy[6] = '\0';
    Serial.print("Day Month Year is ");
    Serial.print(DdMmYy);
    Serial.print("\n");
    
    strncpy(UTCTime,RecMessage+36,6);
    UTCTime[6] = '\0';
    Serial.print("UTC time is ");
    Serial.print(UTCTime);
    Serial.print("\n");
    
    return true;
}

/**************************Other functions**************************/
uint8_t Sim7x00::sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout) {

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string
    
    delay(100);
    
    while( Serial.available() > 0) Serial.read();    // Clean the input buffer
    
    Serial.println(ATcommand);    // Send the AT command 


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial.available() != 0){    
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial.read();      
//            Serial.print(response[x]);
            x++;
            // check if the desired answer  is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }
         // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));
    
//    Serial.print("\n");   

    return answer;
}

char Sim7x00::sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout){
  uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 

    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial.available() != 0){    
            response[x] = Serial.read();
            Serial.print(response[x]);
            x++;
            // check if the desired answer 1  is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                Serial.print("\n");
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            else if (strstr(response, expected_answer2) != NULL)    
            {
                Serial.print("\n");
                answer = 2;
            }
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;

}

Sim7x00 sim7600 = Sim7x00();



