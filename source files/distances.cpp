#include <string.h>
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "distances.h"
#include "metric_spaces.hpp"
#include "misc_functions.h"

using namespace std;


int hamming_distance(char* point, char* query, int max_bits) //returns the hamming-distance of two points
{
	int counter = 0;
	for(int i=0; i<strlen(point); i++)
	{
		if (point[i] != query[i])
			counter++;
	}
	return counter;
}

double euclidean_distance(double* v1, double* v2, int dim) { //returns the euclidean-distance of two points
	double res=0.0 ;
	
	for (int i=0 ; i<dim ; ++i) {
		res += pow(v1[i]-v2[i], 2) ;
	}
	return sqrt(res) ;
}

double cosine_distance(double* v1, double* v2, int dim) { //returns the cosine distance of two points
	double v1_norm=0.0, v2_norm=0.0 ;
	
	for (int i =0; i<dim; ++i) {
		v1_norm = v1_norm + pow(v1[i], 2) ;
		v2_norm = v2_norm + pow(v2[i], 2) ;
	}
	v1_norm = sqrt(v1_norm) ;
	v2_norm = sqrt(v2_norm) ;
	
	return ( 1 - (dot_product(v1, v2, dim)/(v1_norm*v2_norm)) ) ;
}

