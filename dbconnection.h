//db_connection.h
/*
 * @author Raymond
 *
 * History:
 *    Augustus-2014 Raymond creation
 *
 *    October-2014  Raymond update, add pscp function
 */
#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

//std includes
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
//#include <stdafx.h>
#include <cstdlib> //for std::system

#include <string>

//#include <tchar.h>

//mysql c++
/*
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "mysql_connection.h"
*/

//mysql c
//#include <my_global.h>
#include <mysql.h>

#define BUFFER_SIZE 516

//
#include <direct.h>

//curl

//#include <conio.h>
//
//#include <curl\curl.h>

//#include <windows.h>

//disable the warnings
/*
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
*/

#define ARRAY_SIZE (BUFFER_SIZE / 4)

typedef struct {
	char face[ARRAY_SIZE];  //store the related image file name
	char finger[ARRAY_SIZE];
	char iris[ARRAY_SIZE];
	char eyes[ARRAY_SIZE];
	bool value;                        //flag
} Image;

//char* CatStr(char* buffer, char* str1, char* str2);

int Initialize(Image* image_ptr);

void PrintFiles(const std::string& object, Image* image_ptr, int num);

char* GetImageName(char* str );

/*
   object     - (eyes, face, finger, iris)
   image_ptr  - pointer to struct Image
   num        - number of the files 
*/
void DownloadFiles(const std::string& object, Image * image_ptr, int num);


/* object  - (eyes, face, finger, iris) 
   file    - file name
*/
void DownloadFile( const std::string& object,std::string& source, char* file_name);

void UploadFile(const std::string& subject);

/*
   object  - (eyes, face, finger, iris)
   suspect - the images in suspectsUpload/eyes/*
   subject - the images from the camera
*/
void UpdateData(const std::string&  object, const std::string& suspect, const std::string& subject, const std::string marched_score, const std::string& camera, const std::string& gate);

//int libcurl_progress_callback (void * clientp, double dltotal,
//                               double dlnow, double ultotal,
//                               double ulnow);
//
//size_t libcurl_read_callback(void * pBuffer, size_t size, size_t nmemb,
//                             void * hFile);
//
//void SSHUpload(char * strFileName, char * strFilePath);
#endif