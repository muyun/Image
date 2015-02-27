#ifndef EIGENFACES_H
#define EIGENFACES_H
//
// core - this is a compact module defining basic data structures, including the dense multi-dimensional array Mat and basic functions used by all other modules
#include "opencv2/core/core.hpp"
// contrib module - extra modules, contributed functionality
#include "opencv2/contrib/contrib.hpp"
// highgui - an easy-to-use interface to video capturing, image and video codecs, as well as simple UI capabilities
#include "opencv2/highgui/highgui.hpp"

// -- Main Global Parameters here
// the sample number for each subject
const int NUM_SUBJECT_DATASET = 12;
// holds the num of subject
const int SELECTED_SUBJECTS = 40;
// size of the training dataset
const int TRAINING_DATASET = 6;
// size of the test dataset
const int TEST_DATASET = 6;
//
//The number of components(read: Eigenfaces) kept for this Prinicpal Component Analysis.
//As a hint : There¡¯s no rule how many components(read : Eigenfaces) should be kept for good reconstruction capabilities.
//It is based on your input data, so experiment with the number.
//Keeping 80 components should almost always be sufficient.
const int NUM_COMPONENTS = 20;
// threshold
const double THRESHOLD = 0.5;

// -- Main struct definitions
using namespace cv;
using namespace std;
// namespace fs = boost::filesystem;

//typedef vector<vector<Mat> > MatMatrix ;
//typedef vector<vector<int> > IntMatrix ;

Mat norm_0_255(InputArray _src);

vector<string> split(const char *str, char c );

string get_dir(const string& filename, const string& outpath);

void write_file(const string& output_folder, const string& str);

void read_csv(vector<Mat>& trainingSamples, vector<int>& trainingLabels, vector<Mat>& testSamples, vector<int>& testLabels, ofstream& logstream, const string& filename, int& height, char separator);

int get_result(vector<Mat>& trainingSamples, vector<int>& trainingLabels, vector<Mat>& testSamples, vector<int>& testLabels, ofstream& logstream, int height, const string& output_folder);

#endif	/*EIGENFACES_H*/
