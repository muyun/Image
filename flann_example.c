

#include <flann/flann.h>
//#include <flann/defines.h>

#include <stdio.h>
#include <stdlib.h>


float* read_points(const char* filename, int rows, int cols)
{
	float* data;
	float *p;
	FILE* fin;
	int i,j;

    fin = fopen(filename,"r");
    if (!fin) {
        printf("Cannot open input file.\n");
        exit(1);
    }
    
    data = (float*) malloc(rows*cols*sizeof(float));
    if (!data) {
        printf("Cannot allocate memory.\n");
        exit(1);
    }
    p = data;
    
    for (i=0;i<rows;++i) {
        for (j=0;j<cols;++j) {
            fscanf(fin,"%g ",p);
			//printf("%g ", *p);
            p++;
        }
    }
    
    fclose(fin);
    
    return data;
}

void write_results(const char* filename, int *data, int rows, int cols)
{
	FILE* fout;
	int* p;
	int i,j;

    fout = fopen(filename,"w");
    if (!fout) {
        printf("Cannot open output file.\n");
        exit(1);
    }
    
    p = data;
    for (i=0;i<rows;++i) {
        for (j=0;j<cols;++j) {
            fprintf(fout,"%d ",*p);
			
            p++;
        }
        fprintf(fout,"\n");
    }
    fclose(fout);
}



int main(int argc, char** argv)
{
	float* dataset;
	float* testset;
	int nn;
	int* result;
	float* dists;
	struct FLANNParameters p;
	float speedup;
	flann_index_t index_id;

    //int rows = 9000;
    //int cols = 128;
    //int tcount = 1000;
	int rows = 200;
	int cols = 448;
	int tcount = rows;

    /*
     * The files dataset.dat and testset.dat can be downloaded from:
     * http://people.cs.ubc.ca/~mariusm/uploads/FLANN/datasets/dataset.dat
     * http://people.cs.ubc.ca/~mariusm/uploads/FLANN/datasets/testset.dat
     */
    printf("Reading input data file.\n");
    //dataset = read_points("E:\\wenlong\\flann-1.8.4-src\\sourcedata\\dataset.dat", rows, cols);
	dataset = read_points("E:\\wenlong\\flann-1.8.4-src\\data\\gallery_.dat", rows, cols);
    printf("Reading test data file.\n");
    //testset = read_points("E:\\wenlong\\flann-1.8.4-src\\sourcedata\\testset.dat", tcount, cols);
	testset =read_points("E:\\wenlong\\flann-1.8.4-src\\data\\probe_.dat", tcount, cols);

    nn = 30;
    result = (int*) malloc(tcount*nn*sizeof(int));
    dists = (float*) malloc(tcount*nn*sizeof(float));
    
    p = DEFAULT_FLANN_PARAMETERS;

	/*
    p.algorithm = FLANN_INDEX_KDTREE;
	p.trees = 8;
	p.log_level = FLANN_LOG_INFO;
	p.checks = 64;
	*/ 

	// linear
	//p.algorithm = FLANN_INDEX_LINEAR;
	//p.algorithm = FLANN_INDEX_COMPOSITE;
	
	p.algorithm = FLANN_INDEX_HIERARCHICAL;
	p.branching = 64;
	p.trees = 4;
	p.leaf_max_size = 100;
	
	//p.algorithm = FLANN_INDEX_KMEANS;
	// use the default
	//p.branching = 32;
	//p.iterations = 11;
	
	//p.centers_init = FLANN_CENTERS_RANDOM;
	//p.cb_index = 0.2;
	
	/*
    p.algorithm = FLANN_INDEX_LSH;
	p.table_number_ = 20;
	p.key_size_ = 10;
	p.multi_probe_level_ = 2;
	*/
	p.target_precision = 0.9;
	 
    printf("Computing index.\n");
    //index_id = flann_build_index(dataset, rows, cols, &speedup, &p);
	index_id = flann_build_index_int(dataset, rows, cols, &speedup, &p);
	printf("%d \n", index_id);
    //flann_find_nearest_neighbors_index(index_id, testset, tcount, result, dists, nn, &p);
	flann_find_nearest_neighbors_index(index_id, testset, tcount,result,dists,nn, &p);
    write_results("E:\\wenlong\\flann-1.8.4-src\\build\\result\\hier_results_branching-64.dat",result, tcount, nn);
    
    flann_free_index(index_id, &p);
    free(dataset);
    free(testset);
    free(result);
    free(dists);
    
    return 0;
}
