/* ----------------------------------------
 * Author: Victor Manuel Dominguez Rivas
 * Date: 20/06/2021
 * Description: This header contains C SIM 7600 wrapper 
   (using C++ without objects), using this tutorial:
   https://nachtimwald.com/2017/08/18/wrapping-c-objects-in-c/
------------------------------------------------------------------------------*/

#ifndef sim7x00_wrapper
#define sim7x00_wrapper

#ifdef __cplusplus
extern "C" {
#endif

struct sim7x00_s;
typedef struct sim7x00_s sim7x00_t;

sim7x00_t *		sim7x00_create						();
void 			sim7x00_destroy						(sim7x00_t *s);

void 			sim7x00_PowerOn						(sim7x00_t *s,int PowerKey);
void 			sim7x00_PowerOnWithoutSIM			(sim7x00_t *s,int PowerKey);

void 			sim7x00_PhoneCall					(sim7x00_t *s, const char* PhoneNumber);

bool 			sim7x00_SendingShortMessage			(sim7x00_t *s, const char* PhoneNumber,const char* Message);
bool 			sim7x00_ReceivingShortMessage		(sim7x00_t *s);

void 			sim7x00_ConfigureFTP					(sim7x00_t *s, const char* FTPServer,const char* FTPUserName,const char* FTPPassWord);
void 			sim7x00_UploadToFTP					(sim7x00_t *s, const char* FileName);
void 			sim7x00_DownloadFromFTP				(sim7x00_t *s, const char* FileName);


bool 			sim7x00_GPSPositioning				(sim7x00_t *s);
bool 			sim7x00_GPSInitSession				(sim7x00_t *s);
const char * 	sim7x00_GPSReadWithOpenedSession	(sim7x00_t *s);
const char * 	sim7x00_GPSReadWithoutSession		(sim7x00_t *s);
bool 			sim7x00_GPSCloseSession				(sim7x00_t *s);


char 			sim7x00_sendATcommand				(sim7x00_t *s, const char* ATcommand, unsigned int timeout);
char 			sim7x00_sendATcommandAnswer			(sim7x00_t *s, const char* ATcommand, const char* expected_answer, unsigned int timeout);
char 			sim7x00_sendATcommandSilent			(sim7x00_t *s, const char* ATcommand, unsigned int timeout);
char 			sim7x00_sendATcommandSilentAnswer	(sim7x00_t *s, const char* ATcommand, const char* expected_answer, unsigned int timeout);
char 			sim7x00_sendATcommand2				(sim7x00_t *s, const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout);


#ifdef __cplusplus
}
#endif

#endif
