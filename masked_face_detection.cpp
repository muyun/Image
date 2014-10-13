#include "stdafx.h"
#include "masked_face_detection.h"

using namespace cv;
using namespace std;

//int DetectMask(const std::string& nir_image){
//	double a = 0.923100;
//	double b = 17.307700;
//
//	double y = getValue(visible_image );
//	double x = getMean(nir_image);
//
//
//	if ( y >=  a * x)
//		return 1;
//	else
//	    return 0;
//}

void SVMTrain(Image* image_ptr, int nfiles){
	float labels[_MAX_NUM];
	double trainingData[_MAX_NUM][2];

	for(int i = 0; i<nfiles; i++){
		printf("Source:%s\t\t%f\t\t%s\t\t%f\n", image_ptr->nir_file_name, image_ptr->nir_norm_mean,image_ptr->visible_file_name, image_ptr->visible_norm_mean, image_ptr->value);
		
		//set up training data
		labels[i] = image_ptr->value;
		trainingData[i][0] = image_ptr->nir_norm_mean ;
		trainingData[i][1] = image_ptr->visible_norm_mean ;
	}

	Mat trainingDataMat(nfiles, 2, CV_32FC1, trainingData);
	//
	Mat labelsMat(nfiles, 1, CV_32FC1, labels );

	//for(int i = 0; i<nfiles; i++){
	//	printf("Source:%s\t\t%f\t\t%f\t\t%f\n", image_ptr->nir_file_name, labelsMat[i][0],labelsMat[i][1],labels[i] = image_ptr->value);
	//	
	//	//set up training data
	//	labels[i] = image_ptr->value;
	//	labelsMat[i][0] = image_ptr->nir_norm_mean ;
	//	labelsMat[i][1] = image_ptr->visible_norm_mean ;
	//}

	

	// set up svm's parameters
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	////Train the SVM
	CvSVM SVM;
	SVM.train(trainingDataMat,labelsMat, Mat(), Mat(), params);
	 
	Vec3b green(0,255,0), blue(255,0,0);
	//the decision regions from svm
	for(int i = 0; i < nfiles; ++i){
		for(int j = 0; j < nfiles; ++j){
			Mat sampleMat = ((Mat_<float>(1,2)) <<j,i);
			float response = SVM.predict(sampleMat);

			if(response == 1){
			    //mask

			} else if (response == 0) {

			}
     
		}
	}
	
}


/*
  replace str1 with str2
*/
std::string GetVisibleFile(std::string& source, std::string str1, std::string  str2){
	int position = 0;
	
	while((position = source.find(str1)) != -1){
		position = source.find(str1);
		source.replace(position, str2.length(), str2);
	}

	//cout << "source:" << source <<endl;

	return source;
}

void PrintInfo(FILE* result_file,Image* image_ptr, int nfiles){
	int i;
	
	for(i = 0; i < nfiles; i++){
		printf("[DEBUG]:%s\t\t%f\n", image_ptr->nir_file_name, image_ptr->nir_norm_mean);
		printf("[DEBUG]:%s\t\t%f\n", image_ptr->visible_file_name, image_ptr->visible_norm_mean);
		
		//printf("%f\n", file_ptr[i]->norm_measured_mean);
		fprintf(result_file, "%s\t\t%f\t\t%s\t\t%f\t\t%f\n", image_ptr->nir_file_name, image_ptr->nir_norm_mean,image_ptr->visible_file_name, image_ptr->visible_norm_mean, image_ptr->value);
		//fprintf(result_file, "%s\t\t%f\n", image_ptr->visible_file_name, image_ptr->visible_norm_mean);
		
		image_ptr++;
	}
}

void NormalizeAll(Image* image_ptr, int nfiles, double nir_max, double visible_max){
	int i;

	for(i = 0; i < nfiles; i++){
		image_ptr->nir_norm_mean = Normalize(image_ptr->nir_norm_mean, nir_max);
		image_ptr->visible_norm_mean = Normalize(image_ptr->visible_norm_mean, visible_max);

		image_ptr++;
	}
}

double  Normalize(double x, double max){

	return log10(x) / log10(max);
}

double GetMean(const std::string& image){
	//store the result

	double k = 1.645;

	Mat gray_image;
	//get gray scale image
    gray_image = imread(image, CV_LOAD_IMAGE_GRAYSCALE); // Read the file

    if(! gray_image.data ) // Check for invalid input
    {
        printf("Could not open or find the image \n");
        return -1;
    }

    //namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
   // imshow( "source image", image ); // Show our image inside it.
	//convert the image from BGR to Grayscale format
	//Mat gray_image;
	//cvtColor(image, gray_image, CV_BGR2GRAY);
	//imshow("gray image", gray_image);
	
	//get mean and standard deviation
	//Scalar is a four-double-type-element array
	Scalar mean, stddev; 
	meanStdDev(gray_image, mean, stddev);

	uchar mean_pxl = mean.val[0];
	uchar stddev_pxl = stddev.val[0];

	//cout << "mean value: " <<mean_pxl <<endl;
	//cout << "Standard deviation: " << stddev_pxl <<endl;
	printf("mean value: %u; Standard deviation: %u\n", mean_pxl, stddev_pxl);
	//printf("Standard deviation: %u\n", stddev_pxl);

	int rows = gray_image.rows;
	int cols = gray_image.cols;
	//measured_image store the image which removes the outliers (about 10% of the radiance are outliers)
	//printf("gray_image.type: %s\n", gray_image.type());
	Mat measured_image(rows, cols, gray_image.type());
	measured_image.setTo(0);
	//
	/*Mat convert_image;
	gray_image.convertTo(convert_image, CV_32FC1);*/
	//

	//double std_dev = (double)stddev_pxl;
	//printf("std_dev: %f\n", std_dev);
	//printf("mean: %d\n", gray_image.at<uchar>(0,0));
	int num = 0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++)
		{
			//Mat::at returns a reference to the specified array element
			double outlier_value = fabs(gray_image.at<uchar>(i,j) - (double)mean_pxl);
//			printf("ri_value: %f", ri_value);
			if( outlier_value <= k * (double)stddev_pxl){
				//take the pixel
			    measured_image.at<uchar>(i,j) = gray_image.at<uchar>(i,j);
				num ++ ;
			} else{
				//outlier
			     measured_image.at<uchar>(i,j) = 0;
			}
		}
	}

	//imshow("forhead region", measured_image);

    //Scalar measured_mean,measured_stddev; 
	//meanStdDev(measured_image, measured_mean, measured_stddev);
	//Scalar measured_mean = mean(measured_image);
	//uchar measured_mean_pxl = measured_mean.val[0];
	double sum = 0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			sum = sum + measured_image.at<uchar>(i,j);
		}
	}

	//printf("measured mean : %f \n", measured_mean.val[0] );

	/*strcpy(image.name, file_info.name);
	image.measured_mean = (double)measured_mean.val[0];*/
	//std::cout<<image<<std::endl;
	//fprintf(result, "%s\t\t%f\n", image.c_str(), measured_mean.val[0]);
	//fprintf(result_file, "%f\n", measured_mean.val[0]);

	//if((double)measured_mean.val[0] >= max_mean){
	//	max_mean = (double)measured_mean.val[0];

	//}

	//if((double)measured_mean.val[0] <= min_mean){
	//	min_mean = (double)measured_mean.val[0];
	//}


	//free memory
	gray_image.release();
	measured_image.release(); 

	return sum / num;
}

int main( int argc, char* argv[] )
{
	//if( argc != 2)
 //   {
 //    cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
 //    return -1;
 //   }

//	int num = 0;
	//double k = 1.645;
    //double result[50];
	//normazation
	double nir_max_mean = 0;
	double visible_max_mean = 0;
	//double min_mean = 256;
//	double value = 1;
//
	//store the result
	FILE* result_file;
	//char re[256];
//
//	//the image dir
    string dir = "D:\\wenlong\\workspace\\proj\\MaskedfaceDetection\\MaskedfaceDetection\\trainings\\";
//	//string dir_name = "D:\\wenlong\\workspace\\proj\\FaceLivenessDetection\\FaceLivenessDetection\\forehead\\";
    string files_name = dir + "*_NIR.jpg";
//
//	//find the related image files
	struct _finddata_t file_info; 
	long handle = _findfirst(files_name.c_str(), &file_info);

	if(handle == -1) { 
		printf("Cannot read file info!\n"); 
		return -1; 
	}

	//printf("First image name: %s\n",file_info.name);

		//store the result
	result_file = fopen("result.txt", "a+");
	//fprintf(result_file, "Image_Name ");
	fprintf(result_file, "%s\t\t%s\t\t%s\t\t%s\t\t%s\n", "NIR_Image_Name", "nir_norm_mean", "visible_Image_name", "visible_norm_mean", "value");

//	string file_name = dir + file_info.name ;

	//string file = file_info.name;
    //printf("The image name: %s \n", file_info.name );

	int nfiles = 0; //files num
	Image image[_MAX_NUM];

while(_findnext(handle, &file_info) == 0) {
	string nir_file_name_ = dir + file_info.name;
	printf("The Nir image: %s \n", file_info.name);
//
//	struct Image image;
	//string dir = "D:\\wenlong\\workspace\\proj\\MaskedfaceDetection\\MaskedfaceDetection\\forehead\\";
	//string image1 = dir + "processed_head_2014-9-17-11-30-19_NIR.jpg";
	//string image2 = dir + "processed_head_2014-9-17-11-30-19_visible.jpg";
	//int result = DetectMask(image1, image2);

	double nir_mean = GetMean(nir_file_name_);
	//fprintf(result_file, "%s\t\t%f\n", file_info.name, mean);
	//printf("Info:%s\t\t%f\n", file_info.name, mean);
	if(nir_mean > nir_max_mean)
		nir_max_mean = nir_mean;

	//visible 
	string nir_name = file_info.name;
	string visible_file_name_ = GetVisibleFile(nir_name, "NIR", "visible" ) + ".jpg";
	cout<< "The visible file:" << visible_file_name_ <<endl;
	double visible_mean = GetMean(dir + visible_file_name_);
	//visible_max_mean

	if(visible_mean > visible_max_mean)
		visible_max_mean = visible_mean;

	//if(nir_mean < min_mean)
	//	min_mean = nir_mean;

	//image_ptr = (Image*)malloc(sizeof(Image));
	//memset copies '\0' to the first sizeof characters of the string pointed to by the argument
	int num = nfiles++;
	//char* file_name_ptr = image[num].file_name;
	memset(image[num].nir_file_name, '\0', sizeof(char) * _MAX_FNAME);
	strcpy(image[num].nir_file_name, file_info.name);
	//printf("image:%s\n", image[nfiles++].name);
	memset(image[num].visible_file_name, '\0', sizeof(char) * _MAX_FNAME);
	strcpy(image[num].visible_file_name, visible_file_name_.c_str());

	image[num].nir_norm_mean = nir_mean;
	image[num].visible_norm_mean = visible_mean;
	//printf("image:%s\t\t%f\n", image[num].file_name, image[num].norm_measured_mean);
	//fprintf(result_file, "%s\t\t%f\n", image[num].file_name, image[num].norm_measured_mean);
	/*file_ptr[nfiles++] = &image;

	printf("file_ptr:%s\t\t%f\n", file_ptr[nfiles]->name, file_ptr[nfiles]->norm_measured_mean);*/
	//cout<< "result: "<< result <<endl;

	// label the data
	if((nir_name.find("processed") != string::npos) || (visible_file_name_.find("processed") != string::npos) ){
		//find -- real
		//printf("real\n:", nir_name);
		//printf("real\n:", visible_file_name_);
		image[num].value = 0;
	} else {
		//masked
		//printf("Masked\n:", nir_name);
		//printf("Masked\n:", visible_file_name_);
	    image[num].value = 1;
	}
	

}
    
    //printf("max_mean: %f\n", max_mean);
	
    // normalize
    NormalizeAll(image, nfiles, nir_max_mean, visible_max_mean);
	
	PrintInfo( result_file, image, nfiles);
    //value = max_mean - min_mean;

    fprintf(result_file, "end.\n");

    _findclose(handle); // close

    //waitKey(0); // Wait for a keystroke in the window

	fclose(result_file);

	//training
	SVMTrain(image, nfiles);

    return 0;
}