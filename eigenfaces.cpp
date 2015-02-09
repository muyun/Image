// eigenfaces.cpp : eigenfaces alg for the face recognition
//                  -  Some code is from OpenCV src
//
// @author wenlong 
//  
// The PCA finds a linear combination of features that maximizes the total variance in data;
// It doesn't consider any classes and so a lot of discriminative information may be lost when throwing components away.
//
#include "stdafx.h"
#include "eigenfaces.h"

//typedef vector<vector<Mat> > MatMatrix ;
//typedef vector<vector<int> > IntMatrix ;

using namespace cv;
using namespace std;

//namespace fs = boost::filesystem;

// dataset in each subject
const int SUBJECT_DATA = 12;


//Preparing the data
//These two functions are from Face Recognition src in OpenCV doc
static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}


static vector<string> split(const char *str, char c = '\\' ) 
{
	vector<string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}

static string get_dir(const string& filename, const string& outpath)
{
	//TODO: using the Boost for this function
	/*
	//int length = path.length();
	//boost::regex replaceRegex("\\\\");

	//string result = boost::regex_replace(path, replaceRegex, "/", boost::match_default | boost::format_all);

	//result = result.substr(0, result.rfind("/"));

	boost::filesystem::path p = path;
	boost::filesystem::path p1 = p.parent_path();
	boost::filesystem::path p2 = p1.filename();
	cout << p2 << endl;

	cout << p2.string() << endl;
	
	fs::path file = filename;
	string dir = file.parent_path().filename().string();
	cout << outpath + dir << endl;
	*/

	//e:\wenlong\documents\code\opencv\eigenfaces\x64\Debug\input_\crop_00005\crop_00005fa010_930831-new.jpg
	vector<string> s = split(filename.c_str());
	//cout << s[9] << endl;

	//get the full output name
	string output = outpath + s[9] + "\\";

	return output;
}

static void read_csv_(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			//Mat im = imread(path, 0);
			//images.push_back(imread(path, 0));
			//Mat image = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

			images.push_back(imread(path, CV_LOAD_IMAGE_GRAYSCALE));
            /*
			Mat im1 = im.clone();
			if ( !im1.isContinuous())
			{
			// .bmp files sometimes are 'padded' to multiples of 4
			// * convert the images to png, pbm or
			// * use im.clone() - the deep copy will force continuity
			//
			//Mat im = im.clone();
			cout <<"out" << endl;
			}
			*/

			labels.push_back(atoi(classlabel.c_str()));
		}
	}

}

static void get_result(vector<Mat>& images, vector<int>& labels, const string& output_folder)
{
    // quit if there are not enough images (at least 2 images to work)
	if (images.size() <= 1)
	{
		string error_message = " Needs more images to data set ...";
		CV_Error(CV_StsError, error_message);
	}

	// get the height from the first image for the output
	int height = images[0].rows;

	// For eigen and fisherfaces, the images have to flatten it to one row; the lbp one doesn't
	for (int index = 0; index < images.size(); ++index)
		images[index] = images[index].reshape(1, 1);

	// the last image in each dataset is the test data
	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	// create an Eigenfaces model for face recognition and train it with the images and labels
	// class FaceRecognizer : Algorithm
	// TODO: the FaceRecognizer class, and the derived classes 
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	model->train(images, labels);

	// predicts the label of a given test image
	//int predictedLabel = 0;
	//double confidence = 0.0;
	//model->predict(testSample, predictedLabel, confidence);
	int predictedLabel = model->predict(testSample);

	// the confidence
	string result_message = format("Predicted class = %d / Actual class = %d .", predictedLabel, testLabel);
	cout << result_message << endl;

	// get the eigenvalues of this Eigenfaces model
	Mat eigenvalues = model->getMat("eigenvalues");
	// the Eigenvectors
	Mat W = model->getMat("eigenvectors");
	// the sample mean from the training data
	Mat mean = model->getMat("mean");

	// display the mean	picture
	//if (argc == 2)
	//{
	//	imshow("mean", norm_0_255(mean.reshape(1, height)));
	//} else {
	imwrite(format("%s\\mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, height)));
	//}

	// display the Eigenfaces
	for (int i = 0; i < min(10, W.cols); i++)
	{
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout << msg << endl;

		// get eigenvector #i
		Mat ev = W.col(i).clone();

		// reshape to original size & normalize to [0...255] for imshow
		//ev = ev.reshape(1, height);
		Mat grayscale = norm_0_255(ev.reshape(1, height));
		// show the image & apply a Jet colormap for better sensing
		Mat cgrayscale;
		applyColorMap(grayscale, cgrayscale, COLORMAP_JET);

		// display or save
		//if (argc == 2)
		//{
		//	imshow(format("eigenface_%d", i), cgrayscale);		
		//} else {
		imwrite(format("%s\\eigenface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
		//}
	}

}


static void read_csv(const string& filename, const string& output_folder, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	// The vectors hold the images and corresponding labels based on the csv format
	vector<Mat> images;
	vector<int> labels;
	//store the subjects
	//vector<vector<Mat> > mat_matrix;
	//vector<vector<int> > label_matrix;
	
	int num = 1;
	//the index of subject
	int index = 1;

	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			//Mat im = imread(path, 0);
			//images.push_back(imread(path, 0));
			Mat image = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

			images.push_back(image);

			/*
			Mat im1 = im.clone();
			if ( !im1.isContinuous())
			{
			  .bmp files sometimes are 'padded' to multiples of 4
			  * convert the images to png, pbm or
			  * use im.clone() - the deep copy will force continuity
			
			Mat im = im.clone();
			cout <<"out" << endl;
			}
			*/

			labels.push_back(atoi(classlabel.c_str()));

			// for each subject
			if ((num % SUBJECT_DATA) == 0)
			{
				//mat_matrix.push_back(images);
				//label_matrix.push_back(labels);

				cout << "For the subject num: " << index << endl;
				
				//mkdir 
				string dir = get_dir(path, output_folder);
				_mkdir(dir.c_str());

				// get the calculation results 
				get_result(images, labels, dir);

				//
				images.clear();
				labels.clear();

				cout << "-----------------------------"<< endl;

				//next suject
				index++;
			}

			num++;
		}
	}

	/*
	//print out
	for (int i = 0; i < mat_matrix.size(); i++)
	{
		for (int j = 0; j < mat_matrix[i].size(); i++)
			cout << mat_matrix[i][j] <<":"<< label_matrix[i][j] << " ";

		cout << endl;
	}
	*/

}

int main(int argc, const char* argv[])
{
	/*
	if (argc < 2)
	{
		cout << "usage: " << argv[0] << " <csv.ext> <output_folder>" << endl;
		exit(1);
	}
	*/
	
	argv[1] = "e:\\wenlong\\documents\\code\\opencv\\eigenfaces\\x64\\Debug\\input_.txt";
	argv[2] = "E:\\wenlong\\documents\\code\\opencv\\eigenfaces\\x64\\Debug\\output\\";

	//string output_folder = ".";
	string output_folder = argv[2];
	
	/*
	if (argc == 3)
	{
		output_folder = string(argv[2]);
	}
	*/

	// get the path to the csv
	string fn_csv = string(argv[1]);

	// The vectors hold the images and corresponding labels based on the csv format
	//vector<Mat> images;
	//store the subjects
	//vector<vector<Mat> > mat_matrix;
	//vector<vector<int> > label_matrix;
	
	// read the csv file
	try {
		read_csv(fn_csv, output_folder);

	} catch (cv::Exception& e){
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	// display if we are not writing to an output folder
	//if (argc == 2)
	//{
		waitKey(0);
	//}

	return 0;
}
