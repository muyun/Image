//#include "dbconnection.h"
#include "stdafx.h"
#include "dbconnection.h"

//using namespace std;

/*
 * @author Raymond
 *
 * History:
 *    Augustus-2014 Raymond creation
 *
 *    October-2014  Raymond update, add pscp function
 */

char* CatStr(char* buffer, char* str1, char* str2){
	//char* separator = "; ";

	#pragma warning(disable: 4996)
	//char *strncat(char *dest, const char *src, int n);
	strncat(buffer, str1, strlen(str1) + 1); //"FirstName: "
	strncat(buffer, str2, strlen(str2) + 1); // "wen"
	//strncat(buffer, separator, strlen(separator) + 1);
	#pragma warning(default: 4996)

	// printf("String = %s", buffer);
	return buffer;
}
  
int GetChar(char s[], int lim)  
{
    int c, i;

    for(i = 0; i<lim-1 && (c = getchar()) != EOF && c != '\n'; ++i)
      s[i] = c;

    if( c == '\n')
    {
      s[i] = c;
      ++i;
    }

    //'0' represents the null character, whose value is zero;
    // which marks the end of the string
    s[i] = '\0'; 
    
    return i;
}

/* Copy: copy 'from' to 'to'; */
void Copy(char to[], char from[])
{
    int i;
    
    i = 0;
    while((to[i] = from[i]) != '\0')
      ++i;
    
}

int Initialize(Image* image_ptr){
	//

	MYSQL *conn;

	// the structure is as a connection handler
	conn = mysql_init(NULL);

	if(conn == NULL){
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
	}

	if(mysql_real_connect(conn, 
		                  "localhost", 
						  "test", 
						  "test", 
						  "test", 
						  3306, 
						  NULL, 
						  0) == NULL){
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *fields;
	int num_fields;
	int i;

	int index = 0;  // num of file

	std::string sql_isDownload = "select idbiometData, time, facePath, fingerPath, irisPath, eyesPath from biometData where time <= NOW()";
	//std::cout<<"sql_matched_remark:"<<sql_matched_remark <<std::endl;

	//convert string to char*
	char* cstr_sql_isDownload = new char[sql_isDownload.length() + 1];
	strcpy(cstr_sql_isDownload, sql_isDownload.c_str());
	
	//printf("[DEBUG]cstr_sql_matched_remark: %s\n", cstr_sql_isDownload);
	if(mysql_query(conn, cstr_sql_isDownload)){
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));

		delete[] cstr_sql_isDownload;
        exit(1);
	} else {
		result = mysql_store_result(conn);

		//retrieve the number of rows and fields
		num_fields = mysql_num_fields(result);
		fields = mysql_fetch_field(result);

		//string str;
		//std::string separator = "/";
		
		while((row = mysql_fetch_row(result))){
		//each row
			//init the buffer, memset copies the '\0' to the first sizeof * buffer of buffer
			//memset(buffer, '\0', sizeof * buffer);
			char* str;
			for(i = 0; i < num_fields; i++){

				//printf("[DEBUG]Fields: %s\n",fields[i].name);
                //firstName
				str = "facePath";
				if ( strcmp(str, fields[i].name) == 0 ){
					  
                     // std::string file =  row[0] ;
					  //init face_image[]
					  memset(image_ptr->face, '\0', sizeof(char) * ARRAY_SIZE);
					  //copy row[0] to the face_image[]
					  strcpy(image_ptr->face, row[2] );
					  image_ptr->value = 0;

					 // printf("[DEBUG] row: %s\n",row[0]);
					 // printf("[DEBUG] image: %s\n", image_ptr);
				}

				str = "fingerPath";
				if ( strcmp(str, fields[i].name) == 0 ){
					  
                      //std::string file =  row[1] ;
					  //init face_image[]
					  memset(image_ptr->finger, '\0', sizeof(char) * ARRAY_SIZE);
					  //copy row[0] to the face_image[]
					  strcpy(image_ptr->finger, row[3] );
					  image_ptr->value = 0;

				}

				str = "irisPath";
				if ( strcmp(str, fields[i].name) == 0 ){
					  
                      //std::string file =  row[2] ;
					  //init face_image[]
					  memset(image_ptr->iris, '\0', sizeof(char) * ARRAY_SIZE);
					  //copy row[0] to the face_image[]
					  strcpy(image_ptr->iris, row[4] );
					  image_ptr->value = 0;

				}

				str = "eyesPath";
				if ( strcmp(str, fields[i].name) == 0 ){
					  
                      //std::string file =  row[2] ;
					  //init face_image[]
					  memset(image_ptr->eyes, '\0', sizeof(char) * ARRAY_SIZE);
					  //copy row[0] to the face_image[]
					  strcpy(image_ptr->eyes, row[5] );
					  image_ptr->value = 0;

				}

				
				
			   }//for
			  
			   image_ptr++;
			   index++;

			}//while 

		delete[] cstr_sql_isDownload;
		//free(buffer);
	}

	mysql_free_result(result);
	mysql_close(conn);

	//PrintFiles(image_ptr, "face", num_fields);
	//std::cout<<"index:"<<index<<std::endl;
	if (index < BUFFER_SIZE / 2 )
	    return index;
	else
		return  BUFFER_SIZE / 2 ;
}

/*  print the struct Image info*/
void PrintFiles(const std::string& object, Image* image_ptr, int num){
	int i;
    
	//std::string arr_name = object + "_image";  //like face_image
	for(i = 0; i < num; i++ ){
		printf("[DEBUG]Image:%s\t\t%d\n", image_ptr->face, image_ptr->value);

		image_ptr++;
	}
}

char* GetImageName(char* str ){
	char* pch;
	//array of pointers to char
	//ptr is an array of 3 char pointers
	char* ptr[3];  //suspectsUpload/face/Huan-Jin_744-Huan.jpg

	pch = strtok(str, "/");

	int i = 0;
	while(pch != NULL){
		//printf("string:%s\n", pch);
		ptr[i] = pch;
		//printf("string:%s\n", ptr[i]);
		pch = strtok(NULL, "/");

		i++;
	}
    
	/*
	for(int i = 0; i < 3; i++){
	    printf("Image name %s\n", ptr[i]);
	}
	*/

	return ptr[2];
}

/*
   object - (eyes, face, finger, iris)
   image_ptr  - pointer to struct Image
   num - number of the files 
*/
void DownloadFiles(const std::string& object, Image * image_ptr, int num){
    //mkdir the object dir in local

	//current dir
	std::string curr_dir =  _getcwd( NULL, 0 );
	std::string biomet = curr_dir + "\\biomet\\";
	mkdir(biomet.c_str());
	std::string source = biomet +  object;
	mkdir(source.c_str());

	//std::string source = curr_dir + "\\biomet\\" + object;
	std::cout<<"source dir :"<<source<<std::endl;

	//add matched dir
	std::string matched_dir = biomet + "\\matched";
	mkdir(matched_dir.c_str());

	// for each object -- eyes, face, finger, iris
	// check whether the file has been downloaded

	// download the file
	int i;
	for(i = 0; i < num; i++){
		printf("[DEBUG]Image:%s\t\t%d\n", image_ptr->face, image_ptr->value);

        if (image_ptr->value == 0) {
			//update the flag
			image_ptr->value = 1; 

			char* path = image_ptr->face;
			//get the file name
			//char* token = strtok(image_ptr->face, "/");
			//printf("image name: %s\t\t%s\n", token[1], token[2]);
			char* file_name= GetImageName(path);
			//printf("Image name:%s\n", file_name);

			//download the image
			printf("Downloading file_name\n");
			DownloadFile(object, source, file_name);

       printf("[DEBUG]The Image and value:%s\t\t%d\n", image_ptr->face, image_ptr->value);
	    }

		image_ptr++;
	}
}

void DownloadFile( const std::string& object,std::string& source, char* file_name){
	//std::string command = "php -f download.php " + object;
	//the RSA file
	std::string id_rsa_pub = "c:\\putty\\id_rsa.ppk";

	////mkdir the object dir in local

	////current dir
	//std::string curr_dir =  _getcwd( NULL, 0 );
	//std::string biomet = curr_dir + "\\biomet\\";
	//std::string source = biomet +  object;
	//mkdir(source.c_str());

	////std::string source = curr_dir + "\\biomet\\" + object;
	//std::cout<<"source dir :"<<source<<std::endl;

	////add matched dir
	//std::string matched_dir = biomet + "\\matched";
	//mkdir(matched_dir.c_str());

	//
	//printf("path:%s, name:%s\n", path, file_name);
	//std::string dir = path ;
	std::string file = file_name ;
	//std::cout <<"file:"<<file<<std::endl;
	//To avoid the MySQL server overload, remove the flag function now ...
	//server
	std::string target = "biomet@158.132.8.16:/home/biomet/public_html/project/suspectsUpload/" + object + "/" + file ;
	  //
	std::string command = "pscp -i " + id_rsa_pub + " -r " + target  + " " + source + "\\" + file ;

	//std::cout<<"download command:"<<command<<std::endl;
	char* php_command = new char[command.length() + 1];
	strcpy(php_command, command.c_str());
	printf("[DEBUG]download command: %s", php_command);

	WinExec(php_command, 0);
	//about php in c++
    //std::cout<<std::system(php_command);

	delete[] php_command;
}

void UploadFile(const std::string& subject){
	//std::string command = "php -f upload.php " + subject;
	//std::cout<<"php command:" <<command<< std::endl;
	std::string id_rsa_pub = "c:\\putty\\id_rsa.ppk";
	//current dir
	std::string curr_dir =  _getcwd( NULL, 0 );
	std::string source = curr_dir + "\\biomet\\matched\\" + subject ;
	
	std::string target = "biomet@158.132.8.16:/home/biomet/public_html/project/suspectsUpload/matched";
	std::string command = "pscp -i " + id_rsa_pub  + " " + source + " " + target ;
	
	//std::cout<<"upload command:"<<command<<std::endl;
	char* php_command = new char[command.length() + 1];
	strcpy(php_command, command.c_str());
	printf("[DEBUG]upload command: %s", php_command);

	WinExec(php_command, 0);
	//std::cout<<std::system(php_command);

	delete[] php_command;
}


/*
   object  - (eyes, face, finger, iris)
   suspect - the images in suspectsUpload/eyes/*
   subject - the images from the camera
*/
void UpdateData(const std::string& object,const std::string& suspect,const std::string& subject, const std::string marched_score, const std::string& camera,const std::string& gate){
	 std::cout<<"[DEBUG]object:"<<object <<std::endl;
	 std::cout<<"[DEBUG]suspect:"<<suspect<<std::endl;
	 std::cout<<"[DEBUG]subject:"<<subject<<std::endl;
	//printf("object: %s, suspect:%s, subject:%s:\n", object, suspect, subject);

	//about MYSQL
	//declare a MYSQL structure
	MYSQL *conn;

	// the structure is as a connection handler
	conn = mysql_init(NULL);

	if(conn == NULL){
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
	}

	if(mysql_real_connect(conn, 
		                  "mysql.comp.polyu.edu.hk", 
						  "biomet", 
						  "qwkdjmxn", 
						  "biomet", 
						  3306, 
						  NULL, 
						  0) == NULL){
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *fields;
	int num_fields;
	int i;

	//char* buffer;
    //calloc allocates the requested memory and returns a pointer to it
	//buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
	std::string dir = object + "Path" ; //eyesPath
	std::string suspect_image = "'suspectsUpload/" + object + "/" + suspect + "';";  //'suspectsUpload/eyes/138216560533-88011_L_2_Eyelid.bmp';"
	//"select firstName, lastName, gender, Age from biometData where eyesPath = 'suspectsUpload/eyes/138216560533-88011_L_2_Eyelid.bmp';"
	std::string sql_matched_remark = "select firstName, lastName, gender, Age from biometData where " + dir + " = "  + suspect_image;
	//std::cout<<"sql_matched_remark:"<<sql_matched_remark <<std::endl;

	//convert string to char*
	char* cstr_sql_matched_remark = new char[sql_matched_remark.length() + 1];
	strcpy(cstr_sql_matched_remark, sql_matched_remark.c_str());
	
	printf("[DEBUG]cstr_sql_matched_remark: %s\n", cstr_sql_matched_remark);
	if(mysql_query(conn, cstr_sql_matched_remark)){
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));

		delete[] cstr_sql_matched_remark;
        exit(1);
	} else {
		result = mysql_store_result(conn);

		//retrieve the number of rows and fields
		num_fields = mysql_num_fields(result);
		fields = mysql_fetch_field(result);

		//string str;
		std::string separator = "; ";
		std::string matched_remark;
		while((row = mysql_fetch_row(result))){
		//each row
			//init the buffer, memset copies the '\0' to the first sizeof * buffer of buffer
			//memset(buffer, '\0', sizeof * buffer);
			char* str;
			for(i = 0; i < num_fields; i++){
			   /*
				while(fields = mysql_fetch_field(result)){
				       printf("%s ", fields[i].name);

				}
                printf("\n");
				*/

				printf("[DEBUG]Fields: %s",fields[i].name);
                //firstName
				str = "firstName";
				if ( strcmp(str, fields[i].name) == 0 ){
					  std::string firstname = "FirstName:";
					  
                      matched_remark = firstname + row[0] + separator;
					  //strncat(buffer, separator, strlen(separator) + 1);
					  /*
					   #pragma warning(disable: 4996)
					   //char *strncat(char *dest, const char *src, int n);
					   strncat(buffer, firstname, strlen(firstname) + 1); //"FirstName: "
					   strncat(buffer, row[0], strlen(row[0]) + 1); // "wen"
					   strncat(buffer, separator, strlen(separator) + 1);
					   #pragma warning(default: 4996)
					  */
					 // printf("String = %s", buffer);
				}

				//lastname
				str= "lastName";
				if (strcmp(str, fields[i].name) == 0){
					  // printf("lastname: %s\n", fields[i].name);
                       std::string lastname = "LastName:";

					   matched_remark = matched_remark + lastname + row[1] + separator;
					   //std::cout<<"matched_remark:"<<matched_remark<<std::endl;

				}

				//gender
				str = "gender";
				if (strcmp(fields[i].name, str) == 0){
					  // printf("gender: %s\n", fields[i].name);
                       std::string gender = "gender:";
					   matched_remark = matched_remark + gender + row[2] + separator;
					   //std::cout<<"matched_remark:"<<matched_remark<<std::endl;

				}

				//Age
				str = "Age";
				if (strcmp (fields[i].name, str) == 0){
					  // printf("Age: %s\n", fields[i].name);
                       std::string age = "Age:";
					   matched_remark = matched_remark + age + row[3] + separator;
					   //std::cout<<"matched_remark:"<<matched_remark<<std::endl;

				}

			   }//for

			 //std::string camera_serial = "06";
			 matched_remark = matched_remark + "Camera:" + camera + separator;


			 //std::string gate_num = "02";
			 matched_remark = matched_remark + "GateNum:" + gate + separator;

			}//while 

        std::cout<<"matched_remark:"<<matched_remark<<std::endl;

		//matched images
		std::string basename_image = subject;
		//string basename_image = "138156374231-113AD-080GE__1_00-0C-DF-04-A2-2D2222_F4_L4.jpg";
		std::string matched_dir = "suspectsUpload/matched/";
		std::string matched_image = matched_dir + basename_image;
		//printf("matched_image: %s", matched_image);
        std::cout<<"[DEBUG]matched_image:"<<matched_image<<std::endl;
		
		//marched_score
		//std::string marched_score = "0.6";

		//update the db
		std::string sql_matched;

		sql_matched = "update " + object + "MatchResulted e, biometData b set e.matchedScored = " + marched_score + ", e.matchedPath ='" + matched_image + "', e.matchedRemark = '" + matched_remark + "' where e.idbiometData = b.idbiometData and b." + dir + "= " + suspect_image;
		//update eyesMatchResulted e, biometData b set e.matchedScored = 0.6, e.matchedPath ='$matched_image', e.matchedRemark = '$matched_remark' where e.idbiometData = b.idbiometData and b.eyesPath = 'suspectsUpload/eyes/$db_image';
		
		//std::cout<<"[DEBUG]sql_matched:"<<sql_matched<<std::endl;

		//convert string to char*
		char* cstr_sql_matched = new char[sql_matched.length() + 1];
		strcpy(cstr_sql_matched, sql_matched.c_str());
		printf("[DEBUG]sql_matched: %s", cstr_sql_matched);
		//update the data
		mysql_query(conn,cstr_sql_matched);

		delete[] cstr_sql_matched;

		delete[] cstr_sql_matched_remark;
		//free(buffer);
	}

	UploadFile(subject);

	mysql_free_result(result);
	mysql_close(conn);
	
	//exit(0);

}

//void SSHUpload(char * strFileName, char * strFilePath)
//{
//    char strBuffer[1024];
//    CURL * hCurl;
//    CURLcode ccCurlResult = CURL_LAST;
//    curl_off_t cotFileSize;
//    HANDLE hFile;
//    LARGE_INTEGER liFileSize;
//
//    // check parameters
//    if((strFileName == NULL || strlen(strFileName) == 0) ||
//       (strFilePath == NULL || strlen(strFilePath) == 0))
//           return;
//
//    // parse file path
//    if(strFilePath[strlen(strFilePath) - 1] == '\\')
//        sprintf_s(strBuffer, 1024, "%s%s", strFilePath, strFileName);
//    else
//        sprintf_s(strBuffer, 1024, "%s\\%s", strFilePath, strFileName);
//
//	// create a handle to the file
//    hFile = CreateFileA(strBuffer,             // file to open
//                        GENERIC_READ,          // open for reading
//                        FILE_SHARE_READ,       // share for reading
//                        NULL,                  // default security
//                        OPEN_EXISTING,         // existing file only
//                        FILE_ATTRIBUTE_NORMAL, // normal file
//                        NULL);
//
//    if(hFile != INVALID_HANDLE_VALUE)
//    {
//       // global libcurl initialisation
//       ccCurlResult = curl_global_init(CURL_GLOBAL_ALL);
//       if(ccCurlResult == 0)
//       {
//           // start libcurl easy session
//           hCurl = curl_easy_init();
//           if(hCurl)
//           {
//              // enable verbose operation
//              curl_easy_setopt(hCurl, CURLOPT_VERBOSE, TRUE);
//
//			  // enable uploading
//              curl_easy_setopt(hCurl, CURLOPT_UPLOAD, TRUE);
//
//			  // inform libcurl of the file's size
//              GetFileSizeEx(hFile, &liFileSize);
//              cotFileSize = liFileSize.QuadPart;
//              curl_easy_setopt(hCurl,
//                               CURLOPT_INFILESIZE_LARGE,
//                               cotFileSize);
//              // enable progress report function
//              curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, FALSE);
//              curl_easy_setopt(hCurl,
//                               CURLOPT_PROGRESSFUNCTION,
//                               libcurl_progress_callback);
//
//			  // use custom read function
//              curl_easy_setopt(hCurl,
//                               CURLOPT_READFUNCTION,
//                               libcurl_read_callback);
//
//              // specify which file to upload
//              curl_easy_setopt(hCurl, CURLOPT_READDATA, hFile);
//
//              // specify full path of uploaded file (i.e. server
//              // address plus remote path)
//              sprintf_s(strBuffer,
//                        1024,
//                        "sftp://158.132.8.7/home/biomet/public_html/project/suspectsUpload/matched/",
//                        strFileName);
//              curl_easy_setopt(hCurl, CURLOPT_URL, strBuffer);
//
//              // set SSH server port
//              curl_easy_setopt(hCurl, CURLOPT_PORT, 22);
//
//              // set SSH user name and password in libcurl in this
//              // format "user:password"
//              curl_easy_setopt(hCurl,
//                               CURLOPT_USERPWD,
//                               "biomet:saospiet");
//
//              // set SSH authentication to user name and password
//              curl_easy_setopt(hCurl,
//                               CURLOPT_SSH_AUTH_TYPES,
//                               CURLSSH_AUTH_PASSWORD);
//
//              // execute command
//              ccCurlResult = curl_easy_perform(hCurl);
//
//              // end libcurl easy session
//              curl_easy_cleanup(hCurl);
//         }
//   }
//
//       // release file handle
//       CloseHandle(hFile);
//
//       // global libcurl cleanup
//       curl_global_cleanup();
//
//       if (ccCurlResult == CURLE_OK)
//          printf("File uploaded successfully.\n");
//       else
//          printf("File upload failed. Curl error: %d\n",
//                 ccCurlResult);
//
//    } else
//
//       printf("File upload failed! Could not open local file");
//}
//
//
//size_t libcurl_read_callback(void * pBuffer, size_t size,
//                             size_t nmemb, void * hFile)
//{
//       DWORD dwNumberOfBytesRead = 0;
//       BOOL bResult = ReadFile((HANDLE) hFile, pBuffer, size * nmemb,
//                               &dwNumberOfBytesRead, NULL);
//
//       return dwNumberOfBytesRead;
//}
//
//int libcurl_progress_callback (void * clientp, double dltotal, double dlnow,
//                               double ultotal, double ulnow)
//{
//       printf("Uploaded: %d / %d\n", (int) ulnow, (int) ultotal);
//
//       return 0;
//}


int main(void)
{
	//to test
	std::string object = "face";
	std::string suspect = "138216560533-88011_L_2_Eyelid.bmp";
	std::string subject = "head_2014-8-2-13-5-16_NIR.jpg";
	std::string camera = "06";
	std::string gate = "02";
	//std::string dir = "d:\\wenlong\\workspace\\dbconnection\\dbconnection";
	Image image[BUFFER_SIZE / 2];

	int num_file = Initialize(image);
	//std::cout<<"num:"<<num_file<<std::endl;
	//PrintFiles(image, object, num_file);

	DownloadFiles(object,image, num_file);
	//UpdateData(object, suspect, subject, camera, gate);
    //UploadFile(subject);

	
    //#pragma comment(lib, "libcurl.lib") 
	//SSHUpload(&subject[0], &dir[0]);
	//printf("Press any key to continue...");


 //CURL *curl;
 // CURLcode res;

 // curl = curl_easy_init();
 // if(curl) {
 //   curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
 //   /* example.com is redirected, so we tell libcurl to follow redirection */
 //   curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

 //   /* Perform the request, res will get the return code */
 //   res = curl_easy_perform(curl);
 //   /* Check for errors */
 //   if(res != CURLE_OK)
 //     fprintf(stderr, "curl_easy_perform() failed: %s\n",
 //             curl_easy_strerror(res));

 //   /* always cleanup */
 //   curl_easy_cleanup(curl);
 // }

	return 0;
}
