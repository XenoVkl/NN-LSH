#include <string.h>
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "misc_functions.h"

using namespace std;


int modulo(int x, int y)
{
	int a;
	if (y<0)
	{
		return modulo(x, -y);
	}
	else
	{
		a = x % y;
		if (a < 0)
			a = a + y;
		return a;
	}
}

double dot_product(double* v1, double* v2, int dim)
{
	double result = 0.0;
	for (int i =0; i<dim; i++)
	{
		result = result + v1[i]*v2[i];
	}
	return result;
}

double matrix_projection(int* x, int* x1, int x1_pos, int x2_pos) {
	
	return ( pow(x[x1_pos], 2) + pow(x[x2_pos], 2) - pow(x1[x2_pos], 2) ) / ( 2*(double)x1[x2_pos] ) ;
	
}

double* bubble_sort(double* array, int n) {
	int swap ;
	
	for (int c = 0 ; c < ( n - 1 ); c++) {
		for (int d = 0 ; d < n - c - 1; d++) {
			if (array[d] > array[d+1])
			{
				swap       = array[d];
				array[d]   = array[d+1];
				array[d+1] = swap;
			}
		}
	}
	
	return array ;
}
