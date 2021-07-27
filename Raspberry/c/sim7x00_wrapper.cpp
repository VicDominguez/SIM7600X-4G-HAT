/* ----------------------------------------
 * Author: Victor Manuel Dominguez Rivas
 * Date: 20/06/2021
 * Description: This file contains C SIM 7600 wrapper 
   (using C++ without objects), using this tutorial:
   https://nachtimwald.com/2017/08/18/wrapping-c-objects-in-c/
------------------------------------------------------------------------------*/

#include <string>
#include <cstring>

#include "sim7x00.h"
#include "sim7x00_wrapper.h"

struct sim7x00_s 
{
	void *obj;
};


sim7x00_t *sim7x00_create()
{
  sim7x00_t *s;
  Sim7x00 *obj;

  s      = (typeof(s))malloc(sizeof(*s));
  obj    = new Sim7x00();
  s->obj = obj;

  return s;
}

void sim7x00_destroy(sim7x00_t *s)
{
  if (s == NULL)
    return;
  //sim7x00 has not close function
  delete static_cast<Sim7x00 *>(s->obj);
  free(s);
}

void sim7x00_PowerOn(sim7x00_t *s, int PowerKey)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->PowerOn(PowerKey);
}

void sim7x00_PowerOnWithoutSIM(sim7x00_t *s, int PowerKey)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->PowerOnWithoutSIM(PowerKey);
}

// Phone calls
void sim7x00_PhoneCall(sim7x00_t *s, const char* PhoneNumber)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->PhoneCall(PhoneNumber);
}

// SMS sending and receiving message 
bool sim7x00_SendingShortMessage(sim7x00_t *s, const char* PhoneNumber,const char* Message)
{
	Sim7x00 *obj;

	if (s == NULL)
		return false;

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->SendingShortMessage(PhoneNumber, Message);
}
bool sim7x00_ReceivingShortMessage(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return false;

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->ReceivingShortMessage();
}

// FTP download file to Module EFS or uploading EFS file to FTP
void sim7x00_ConfigureFTP(sim7x00_t *s, const char* FTPServer,const char* FTPUserName,const char* FTPPassWord)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->ConfigureFTP(FTPServer, FTPUserName, FTPPassWord);
}
void sim7x00_UploadToFTP(sim7x00_t *s, const char* FileName)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->UploadToFTP(FileName);
}
void sim7x00_DownloadFromFTP(sim7x00_t *s, const char* FileName)
{
	Sim7x00 *obj;

	if (s == NULL)
		return;

	obj = static_cast<Sim7x00 *>(s->obj);
	obj->DownloadFromFTP(FileName);
}

// GPS positoning
bool sim7x00_GPSPositioning(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return false;

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->GPSPositioning();
}
bool sim7x00_GPSInitSession(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return false;

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->GPSInitSession();
}
const char * sim7x00_GPSReadWithOpenedSession(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return NULL;

	obj = static_cast<Sim7x00 *>(s->obj);
	//https://stackoverflow.com/questions/22330250/how-to-return-a-stdstring-c-str
	return strdup(obj->GPSReadWithOpenedSession().c_str());
}
const char * sim7x00_GPSReadWithoutSession(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return NULL;

	obj = static_cast<Sim7x00 *>(s->obj);
	//https://stackoverflow.com/questions/22330250/how-to-return-a-stdstring-c-str
	return strdup(obj->GPSReadWithoutSession().c_str());
}
bool sim7x00_GPSCloseSession(sim7x00_t *s)
{
	Sim7x00 *obj;

	if (s == NULL)
		return false;

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->GPSCloseSession();
}


// Other functions.
char sim7x00_sendATcommand(sim7x00_t *s, const char* ATcommand, unsigned int timeout)
{
	Sim7x00 *obj;

	if (s == NULL)
		return '\0';

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->sendATcommand(ATcommand, timeout);
}
char sim7x00_sendATcommandAnswer(sim7x00_t *s, const char* ATcommand, const char* expected_answer, unsigned int timeout)
{
	Sim7x00 *obj;

	if (s == NULL)
		return '\0';

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->sendATcommand(ATcommand, expected_answer, timeout);
}
char sim7x00_sendATcommandSilent(sim7x00_t *s, const char* ATcommand, unsigned int timeout)
{
	Sim7x00 *obj;

	if (s == NULL)
		return '\0';

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->sendATcommandSilent(ATcommand, timeout);
}
char sim7x00_sendATcommandSilentAnswer(sim7x00_t *s, const char* ATcommand, const char* expected_answer, unsigned int timeout)
{
	Sim7x00 *obj;

	if (s == NULL)
		return '\0';

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->sendATcommandSilent(ATcommand, expected_answer, timeout);
}
char sim7x00_sendATcommand2(sim7x00_t *s, const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout)
{
	Sim7x00 *obj;

	if (s == NULL)
		return '\0';

	obj = static_cast<Sim7x00 *>(s->obj);
	return obj->sendATcommand2(ATcommand, expected_answer1, expected_answer2, timeout);
}
