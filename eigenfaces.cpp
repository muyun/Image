// eigenfaces.cpp : eigenfaces alg for the face recognition
//                  -  Some code is from OpenCV src
//                  -  PCA && NN (Neareast Neighbor)
//
// @author wenlong 
//  
// The PCA finds a linear combination of features that maximizes the total variance in data;
// It doesn't consider any classes and so a lot of discriminative information may be lost when throwing components away.
//
//  
#include "stdafx.h"
#include "eigenfaces.h"

//Preparing the data
//The function is from Face Recognition src in OpenCV doc
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
	//TODO: using the Boost lib for this function
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

			//The labels corresponding to the images
			labels.push_back(atoi(classlabel.c_str()));
		}
	}

}

static void write_file(const string& output_folder, const string& str)
{
	ofstream file;
	file.open(output_folder + "rate.txt");
	
	file << str << endl;

	file.close();
}

static int get_result(vector<Mat>& trainingSamples, vector<int>& trainingLabels, vector<Mat>& testSamples, vector<int>& testLabels, ofstream& logstream, int height, const string& output_folder)
{
    // quit if there are not enough images (at least 2 images to work)
	if (trainingSamples.size() <= 1)
	{
		string error_message = " Needs more training images to data set ...";
		CV_Error(CV_StsError, error_message);
	}

	//mkdir 
	//string dir = get_dir(path, output_folder);
	//_mkdir(dir.c_str());
	_mkdir(output_folder.c_str());

	// get the height from the first image for the output
	//int height = trainingSamples[0].rows;

	/*  -----  use PCA to reduce the dimensionality of the space	 ----- */
	cout << "Use PCA to reduce the dimensionality of the space " << endl;
	logstream << "Use PCA to reduce the dimensionality of the space " << endl;

    // create an Eigenfaces model for face recognition and train it with the images and labels
	// class FaceRecognizer : Algorithm
	// TODO: the FaceRecognizer class, and the derived classes
	//
	// The threshold applied in the prediction. 
	// If the distance to the nearest neighbor is larger than the threshold, this method returns -1
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer(NUM_COMPONENTS, THRESHOLD);
	//Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	model->train(trainingSamples, trainingLabels);

	
	// get the eigenvalues of this Eigenfaces model	in ordered descending
	Mat eigenvalues = model->getMat("eigenvalues");
	// the eigenvectors ordered by the eigenvalues
	Mat W = model->getMat("eigenvectors");
	// the sample mean from the training data
	Mat mean = model->getMat("mean");

	// display the mean	picture
	//imwrite(format("%s\\mean.bmp", output_folder.c_str()), norm_0_255(mean.reshape(1, height)));

	/*
	// display the Eigenfaces
	for (int i = 0; i < W.cols; i++)
	{
	    string msg = format("Eigenvalue #%d = %.5f .", i, eigenvalues.at<double>(i));
	    cout << msg << endl;
	    logstream << msg << endl;
	    
	    // get eigenvector (Eigenfaces) #i
	    Mat ev = W.col(i).clone();
	    string eigenface_msg = "Eigenface #" + to_string(i) + " = ";
	    logstream << eigenface_msg << endl;
	    logstream << ev << endl;
	    
	    // reshape to original size & normalize to [0...255] for imshow
	    //ev = ev.reshape(1, height);
	    Mat grayscale = norm_0_255(ev.reshape(1, height));
	    // show the image & apply a Jet colormap for better sensing
	    //Mat cgrayscale;
	    //applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
	    
	    // display or save
	    //if (argc == 2)
	    //{
	    //	imshow(format("eigenface_%d", i), cgrayscale);
	    //} else {
		imwrite(format("%s\\eigenface_%d.bmp", output_folder.c_str(), i), norm_0_255(grayscale));
	    //}
	}
	*/

	// Get the components
	logstream << "" << endl;
    string projection_msg = "Projection = ";
	logstream << projection_msg << endl;

	vector<Mat> projections;
	for (int i = 0; i < trainingSamples.size(); i++)
	{
		
		//ev = ev.reshape(1, height);
		//This is the components of the observed Samples
		Mat p = subspaceProject(W, mean, trainingSamples[i]);
		projections.push_back(p);

		logstream << p << endl;

		/* This is the reconstruciton code
		Mat reconstruction = subspaceReconstruct(ev, mean, p);
		// Normalize
		reconstruction = norm_0_255(reconstruction.reshape(1, height));
		// save
		imwrite(format("%s\\eigenface_reconstruction_%d.bmp", output_folder.c_str(), num_components), reconstruction);
		*/

	}

	/*

    //  -----   The test	 ----- 
	logstream << "" << endl;
	cout << "Test the model " << endl;
	logstream << "Test the model " << endl;
	int num = 0;
	// test each image
	for (int i = 0; i < testSamples.size(); ++i)
	{
		// predicts the label of a given test image;
		// -1 as label, which indicates this face is unknown
		int predictedLabel = -1;
		// Associated confidence (e.g. distance) for the predicted label.
		double confidence = 0.0;
		model->predict(testSamples[i], predictedLabel, confidence);
		//int predictedLabel = model->predict(testSample);

		// the confidence
		string result_message = format("Predicted class = %d / Actual class = %d .", predictedLabel, testLabels[i]);
		//cout << result_message << endl;
		logstream << result_message << endl;

		if ( predictedLabel == testLabels[i])
		{
			num++;
		}
    }

	//cout << endl;
	logstream << "" << endl;

	double confidence_rate = num / (double)(TEST_DATASET * SELECTED_SUBJECTS);
	string confidence_rate_message = format("%.2f", confidence_rate);
	cout << "Confidence rate = " + confidence_rate_message + " ." << endl;
	logstream << "Confidence rate = " + confidence_rate_message + " ." << endl;

	cout << "----------------------" << endl;
	logstream << "----------------------" << endl;
	
    // write the file
	//ofstream file;
	//file.open(output_folder + "rate.txt");
	string write_content = to_string(testLabels[0]) + " : " + confidence_rate_message;
	//file << write_content << endl;
	//file.close();

	*/

	return 0;
}

/* 
   read_csv - 
            - Load the dataset
*/

static void read_csv(vector<Mat>& trainingSamples, vector<int>& trainingLabels, vector<Mat>& testSamples, vector<int>& testLabels, ofstream& logstream, const string& filename, int& height, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	// open the file
	//ofstream fstream;
	//fstream.open(output_folder + "rate.txt");

	cout << "Get the training Samples and test Samples: " << endl;

	// the training set and test set
	vector<Mat> images;
	vector<int> labels;

	// the image height of the sample
	//int height;

	int num = 1;
	//the index of subject
	int index = 0;

	string line, path, classlabel;
	while ((index < SELECTED_SUBJECTS) && (getline(file, line))) {
		 stringstream liness(line);
		 getline(liness, path, separator);
		 getline(liness, classlabel);
		 if (!path.empty() && !classlabel.empty()) {
			//Mat im = imread(path, 0);
			//images.push_back(imread(path, 0));
			Mat image = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

			images.push_back(image);

			if (num == 1)
			{
				height = image.rows;
			}

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
			if ((num % NUM_SUBJECT_DATASET) == 0)
			{
				cout << "The samples in the subject: " << index << endl;
				logstream << "The samples in the subject: " + to_string(index) << endl;

				for (int i = 0; i < images.size(); ++i)
				{
					//THE EIGENFACES METHOD MAKES THE ASSUMPTION, THAT THE TRAINING AND TEST IMAGES ARE OF EQUAL SIZE
					// For eigen and fisherfaces, the images have to flatten it to one row; the lbp one doesn't
					//images[i] = images[i].reshape(1, 1);

					// the traing dataset
					// TODO: should be randomly selection
					if (i < TRAINING_DATASET)
					{
						trainingSamples.push_back(images[i].reshape(1, 1));
						trainingLabels.push_back(labels[i]);
					}

					// the test dataset
					if (i >= TEST_DATASET)
					{
						testSamples.push_back(images[i].reshape(1, 1));
						testLabels.push_back(labels[i]);
					}

				}

				//
				images.clear();
				labels.clear();

				//cout << "-----------------------------" << endl;
				//logstream << "[DEBUG]-----------------------------"  << endl;

				//next suject
				index++;
			}

			num++;
      }
	}
    
	cout << "-----------------------------" << endl;
	logstream << "-----------------------------"  << endl;

	// get the calculation results 
	//get_result(logstream, trainingSamples, trainingLabels, testSamples, testLabels, height);

	//fstream << str << endl;

	//fstream.close();
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

	// the output dir
	string output_folder = argv[2];
	
	// get the path to the csv
	string fn_csv = string(argv[1]);

	// open the log file
	ofstream logstream;
	logstream.open(output_folder + "log.txt");

	// The vectors hold the images and corresponding labels based on the csv format
	//vector<Mat> images;
	//vector<int> labels;
	// the training set and test set
	vector<Mat> trainingSamples, testSamples;
	vector<int> trainingLabels, testLabels;

	// the image height of the sample
	int height;
	
	// read the csv file
	try {
		read_csv(trainingSamples, trainingLabels, testSamples, testLabels, logstream, fn_csv, height);

		get_result(trainingSamples, trainingLabels, testSamples, testLabels, logstream, height, output_folder);

	} catch (cv::Exception& e){
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	waitKey(0);

    logstream.close();

	return 0;
}
