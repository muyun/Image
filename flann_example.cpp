//#include "stdafx.h"

#include <flann/flann.hpp>
//#include <flann/io/hdf5.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include <windows.h>

using namespace std;
using namespace flann;

void read_points(const Matrix<unsigned char>& points, const string& filename, int rows, int cols)
{
	//int* data;
	//float *p;
	//FILE* fin;
	int i, j;

	/*
	fin = fopen(filename, "r");
	if (!fin) {
		printf("Cannot open input file.\n");
		exit(1);
	}
	*/
	ifstream file(filename);
	if (!file){
		cout << "Cannot open input file.\n";
		return;
	}

	/*
	data = (float*)malloc(rows*cols*sizeof(float));
	if (!data) {
		printf("Cannot allocate memory.\n");
		exit(1);
	}
	p = data;
	*/

	for (i = 0; i<rows; ++i) {
		for (j = 0; j < cols; ++j) {
			file >> points[i][j];
		}
	}

	file.close();

	// verify the input
	/*
	for (i = 0; i<rows; ++i) {
		for (j = 0; j < cols; ++j) {
			cout<< points[i][j] << " " ;
		}
		cout << endl;
	}*/
	

	//fclose(fin);

	//return data;
}

void write_results(vector<vector<size_t>>& points, const string& filename, int rows, int cols)
{
	//FILE* fout;
	//int* p;
	int i, j;

	/*
	fout = fopen(filename, "w");
	if (!fout) {
		printf("Cannot open output file.\n");
		exit(1);
	} */
	ofstream file (filename);

	if (file.is_open()){

		for (i = 0; i < rows; ++i) {
			for (j = 0; j < points[i].size(); j++) {
				file << points[i][j] << " ";
				//cout << points[i][j] << " ";
			}
			//cout << endl;
			file << endl;
			file.flush();
		}
	}

	file.close();
	//fclose(fout);
}

int main(int argc, char** argv)
{
    //int nn = 3;

	int table_number = 4;

	unsigned int nn = table_number * 3000;
	int rows = 10000;
	int cols = 1024;
	int tcount = 100;
	int qrows = 100;
	
	/*
	unsigned int nn = 10;
	int rows = 20;
	int cols = 1024;
	int tcount = 20;
	int qrows = 20;
	*/

    //LSH
	typedef flann::Hamming<unsigned char> Distance;
	typedef Distance::ElementType ElementType;
	typedef Distance::ResultType DistanceType;
	
    //Read the data
	Matrix<unsigned char> dataset(new unsigned char[rows*cols], rows, cols);
	Matrix<unsigned char> query(new unsigned char[qrows*cols], tcount, cols);
	//load_from_file(dataset, "E:\\wenlong\\flann-1.8.4-src\\data\\gallery.dat", "dataset");
	//load_from_file(query, "E:\\wenlong\\flann-1.8.4-src\\data\\probe.dat", "query");
	printf("Reading input data file.\n");
	//read_points(dataset, "E:\wenlong\flann-1.8.4-src\sourcedata\\dataset.dat", rows, cols);
	read_points(dataset, "E:\\wenlong\\flann-1.8.4-src\\data\\gallery.dat", rows, cols);

	printf("Reading test data file.\n");
	//read_points(query, "E:\wenlong\flann-1.8.4-src\sourcedata\\testset.dat", tcount, cols);
	read_points(query, "E:\\wenlong\\flann-1.8.4-src\\data\\probe.dat", tcount, cols);

    //Matrix<size_t> linearIndices(new size_t[query.rows*nn], query.rows, nn);
	//Matrix<size_t> indices(new size_t[query.rows*nn], query.rows, nn);
	//Matrix<DistanceType> linearDists(new DistanceType[query.rows*nn], query.rows, nn);
	//Matrix<DistanceType> dists(new DistanceType[query.rows*nn], query.rows, nn);
	vector<vector<size_t>>	indices(query.rows, vector<size_t>(nn));
	vector<vector<DistanceType>> dists(query.rows, vector<DistanceType>(nn));

	// get the collision count
	//vector<vector<size_t>> cnt(rows, vector<size_t>(table_number));
	
    //// construct an randomized kd-tree index using 4 kd-trees
    //Index<L2<float> > index(dataset, flann::KDTreeIndexParams(4));
	// linear
	//Index<Distance> linearIndex(dataset, flann::LinearIndexParams());

	//	hierarchical
	//Index<Distance> hierIndex(dataset, flann::HierarchicalClusteringIndexParams(60, FLANN_CENTERS_RANDOM,1,17));
	//Index<Distance> hierIndex(dataset, flann::HierarchicalClusteringIndexParams());
	
	//LSH
	Index<Distance> index(dataset, flann::LshIndexParams(table_number, 7, 1));
	//linearIndex.buildIndex();

	//hierIndex.buildIndex();
	index.buildIndex();
    //// do a knn search, using 128 checks
	/*
	DWORD dw1 = GetTickCount();

	linearIndex.knnSearch(query, linearIndices, linearDists, nn, flann::SearchParams(-1));
	DWORD dw2 = GetTickCount();

	DWORD dw = dw2 - dw1;
	cout << "linear Time difference is " << dw << " milliSeconds" << endl;
	*/
	//DWORD dw1_ = GetTickCount();
	index.knnSearch(query, indices, dists, nn, flann::SearchParams(-1));
	//DWORD dw2_ = GetTickCount();
	//DWORD dw_ = dw2_ - dw1_;
	//cout << "Hier Time difference is " << dw_ << " milliSeconds" << endl;
	//index.knnSearch(query, indices, dists, nn, flann::SearchParams(-1));
	//std::cout.precision(5);
	//float speedup = dw_ / dw;
	//cout << "speedup is " << speedup << "" << endl;

	//index.knnSearch(query, indices, dists, nn, flann::SearchParams(-1));

	write_results(indices, "E:\\wenlong\\flann-1.8.4-src\\build\\result\\lsh_hashtable_4-1_keysize_7_3000.dat", tcount, nn);

	//write_results(cnt, "E:\\wenlong\\flann-1.8.4-src\\build\\result\\lsh_multi_probe_level-2.dat", tcount, nn);

    //flann::save_to_file(indices,"result.hdf5","result");

    delete[] dataset.ptr();
    delete[] query.ptr();
    //delete[] indices.ptr();
    //delete[] dists.ptr();

	//
	//delete[] 
    
    return 0;
}
