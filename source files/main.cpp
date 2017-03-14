#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <istream>
#include <list>
#include <ctime>
#include <math.h>
#include "metric_spaces.hpp"
#include "hashtable.cpp"
#include "misc_functions.h"

using namespace std ;


int main (int argc, char* argv[]) {

	/* Variables */
	int argCounter=1, k=4, L=5 ;
	int item=0, pos, i, j, data_length, total_items=0, total_coords=0, flag_total_coords=0 , *temp_distArray, cnt, ri;
	int bits, position, res, bit, size = pow(2,k), h_p, f_p, g_p, w=4, query_id, row, column, median ;
	int x1_pos, x2_pos, t1, t2, *H ;
	unsigned int M  = (pow(2,32) - 5);
	char inputDataType[10], data[100], temp_name[10], temp_item[4], temp_str[100], c, **namesArray, *nam = new char[20];
	double temp_coordinate, *temp_coordArray, real_time, lsh_time ;
	double *vec, x1, y1, y2, r, tmp, inner_product, t, *query_coords, *line_proj, *sorted_line_proj ;
	fstream inputFile, inputFile_temp1, inputFile_temp2, queryFile, outputFile ;
	EuclideanObj** EuclideanList;
	HammingObj* HammingList ;
	MatrixObj** MatrixList;
	clock_t begin , end;
	srand(time(NULL));

	/* Reading command line parameters */
	if (argc==1 || argc%2==0) {
		cout << "Too few command line arguments" << endl ;
		return 0 ;
	}
	while (argCounter < argc) {
		if (strcmp(argv[argCounter], "-d") == 0) {
			inputFile.open(argv[argCounter+1], ios::in) ;
			inputFile_temp1.open(argv[argCounter+1], ios::in) ;
			inputFile_temp2.open(argv[argCounter+1], ios::in) ;
			if ( !inputFile.is_open() ) {
		    	cout << "Input file (" << argv[argCounter+1] << ") could not be opened for reading" << endl ;
		    	inputFile_temp1.close() ;
		    	inputFile_temp2.close() ;
				queryFile.close() ;
				outputFile.close() ;
				return 0 ;
		    }
		}
		else if (strcmp(argv[argCounter], "-q") == 0) {
			queryFile.open(argv[argCounter+1], ios::in) ;
			if ( !queryFile.is_open() ) {
		    	cout << "Query file (" << argv[argCounter+1] << ") could not be opened for reading" << endl ;
		    	inputFile.close() ;
		    	inputFile_temp1.close() ;
		    	inputFile_temp2.close() ;
				outputFile.close() ;
				return 0 ;
		    }
		}
		else if (strcmp(argv[argCounter], "-k") == 0) {
			k = atoi(argv[argCounter+1]) ;
		}
		else if (strcmp(argv[argCounter], "-L") == 0) {
			L = atoi(argv[argCounter+1]) ;
		}
		else if (strcmp(argv[argCounter], "-o") == 0) {
			outputFile.open(argv[argCounter+1], ios::out|ios::trunc) ;
			if ( !outputFile.is_open() ) {
		    	cout << "Output file (" << argv[argCounter+1] << ") could not be ready for writing" << endl ;
		    	inputFile.close() ;
		    	inputFile_temp1.close() ;
		    	inputFile_temp2.close() ;
		    	queryFile.close() ;
				return 0 ;
		    }
		}
		else {
			cout << "Invalid command line argument (" << argv[argCounter] << ")" << endl ;
			inputFile.close() ;
			inputFile_temp1.close() ;
			inputFile_temp2.close() ;
			queryFile.close() ;
			outputFile.close() ;
			return 0 ;
		}
		argCounter+=2 ;
	}
	/* Checking all the needed files */
	if ( !inputFile.is_open() ) {
		cout << "Input file, at least, is missing" << endl ;
		inputFile_temp1.close() ;
		inputFile_temp2.close() ;
		queryFile.close() ;
		outputFile.close() ;
		return 0 ;
	}
	else if ( !queryFile.is_open() ) {
		cout << "Query file, at least, is missing" << endl ;
		inputFile.close() ;
		inputFile_temp1.close() ;
		inputFile_temp2.close() ;
		outputFile.close() ;
		return 0 ;
	}
	else if ( !outputFile.is_open() ) {
		cout << "Output file is missing" << endl ;
		inputFile.close() ;
		inputFile_temp1.close() ;
		inputFile_temp2.close() ;
		queryFile.close() ;
		return 0 ;
	}
	/* Specifing the input data type */
	inputFile >> data ;
	inputFile >> data ;
	inputFile_temp1 >> data ;
	inputFile_temp1 >> data ;
	inputFile_temp2 >> data ;
	inputFile_temp2 >> data ;
	if (strcmp(data, "vector") == 0) {
		inputFile >> data ;
		inputFile >> data ;
		inputFile_temp1 >> data ;
		inputFile_temp1 >> data ;
		inputFile_temp2 >> data ;
		inputFile_temp2 >> data ;
		if (strcmp(data, "euclidean") == 0) {
			strcpy(inputDataType, data) ;
		}
		else if (strcmp(data, "manhattan") == 0) {
			strcpy(inputDataType, data) ;
		}
		else if (strcmp(data, "cosine") == 0) {
			strcpy(inputDataType, data) ;
		}
	}
	else if (strcmp(data, "hamming") == 0) {
		strcpy(inputDataType, data) ;
	}
	else if (strcmp(data, "matrix") == 0) {
		strcpy(inputDataType, data) ;
	}
	/* Reading data from input file and inserting into local storage space */
	if ( strcmp(inputDataType,"euclidean")==0 || strcmp(inputDataType,"manhattan")==0 || strcmp(inputDataType,"cosine")==0 ) {
		double real_distance = 1000000000.0, dist, radius;
		/*Create Hashtable */
		hash_node<EuclideanObj> hashtable[L][size];
		/* Counting items from the file*/
		inputFile_temp1 >> data ;
		while(!inputFile_temp1.eof()) {
			inputFile_temp1 >> data ;
			strncpy(temp_item, data, 4);
			if (strncmp(temp_item, "item", 4) == 0)
			{
				total_coords = 0;
				total_items++;
			}
			else
			{
				total_coords++;
			}
		}
		total_items++ ;
		total_coords-- ;
		EuclideanList = new EuclideanObj*[total_items] ; // set an array of pointers to euclidean/cosine objects that we 'll store info about the points
		//declare some arrays to use for the calculation
		temp_coordArray = new double[total_coords] ;
		vec = new double[total_coords];
		query_coords = new double[total_coords];
		
		/* Inserting into local storage */
		cout << "NN-search and queries-execution(Euclidean Space) " << endl ;
		inputFile >> data ;
		while(!inputFile.eof()) { // read each item from the file
			EuclideanList[item] = new EuclideanObj(temp_name, total_coords) ; 
			cnt = pos = 0;
			strcpy(temp_name, data) ;
			inputFile >> data ;
			strncpy(temp_item, data, 4) ;
			while (strncmp(temp_item, "item", 4)!=0 && !inputFile.eof()) {
				temp_coordinate = strtod(data, NULL) ;
				EuclideanList[item]->add_coordinate(temp_coordinate, pos) ;
				/* Inserting point to hashtable - Gaussian distribution using Marsaglia */
				if (strcmp(inputDataType, "euclidean") == 0) //euclidean case
				{
					for (int l=0; l<L; l++) //for each hashtable
					{
						f_p = 0;
						for (int q=0; q<k; q++) //find k h(p) hash functions and concatenate them to create hash function f(p)
						{
							for (int ii=0; ii<total_coords; ii++) //create vector v using gaussian distribution for each coordinates
							{
								do
								{
									y1 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
									y2 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
									r = y1*y1 + y2*y2;
								}
								while(r>1.0);

								//Calculate gaussian var
								x1 = y1*sqrt((-2.0)*(log(r)/r));
								vec[ii] = x1; //store coordinate of vector v
							}
							//calculate the dot_product of the two vectors
							inner_product = dot_product(vec, EuclideanList[item]->get_coordinates(), total_coords);  //calculate dot_product of each point and vector v
							t = (double) w*(rand() / (RAND_MAX+1.0)); // pick t uniformly in [0,w)
							h_p = (int) ((inner_product + t)/w);
							ri = rand() + 1; // pick random ri
							f_p = f_p + modulo(ri*h_p, M); // create the ID for each point
						}
						//store point's ID
						EuclideanList[item]->set_ID(f_p); //set ID
						size = pow(2,k);
						f_p = modulo(f_p, size); // f(p) = ID mod size_of_hashtable
						hashtable[l][f_p].insert_data(EuclideanList[item]); //insert point to the hashtable(in the right bucket, a pointer should point to the object-point of the array we declared before)
					}
				}
				else if (strcmp(inputDataType, "cosine") == 0) //cosine case
				{
					for (int l=0; l<L; l++) //for each hashtable
					{
						g_p = 0;
						for (int q=0; q<k; q++) //create k h(p) hash functions and concatenate them to form hash function g(p)
						{
							for (int ii=0; ii<total_coords; ii++) //create vector ri using gaussian distribution and store its coordinates to vec(array)
							{
								do
								{
									y1 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
									y2 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
									r = y1*y1 + y2*y2;
								}
								while(r>1.0);

								//Calculate gaussian var
								x1 = y1*sqrt((-2.0)*(log(r)/r));
								vec[ii] = x1; //store the coordinate
							}
							//calculate the dot_product of the two vectors
							inner_product = dot_product(vec, EuclideanList[item]->get_coordinates(), total_coords); //calculate dot product of point and ri(vec)
							if (inner_product >= 0) //form hash function h(p)
								h_p = 1;
							else
								h_p = 0;
							g_p = g_p + h_p*pow(2, (k-1)-q); //concatenate
						}
						hashtable[l][g_p].insert_data(EuclideanList[item]); //insert point to the hashtable(in the right bucket, a pointer should point to the object-point of the array we declared before)
					}
				}
				pos++ ;				
				inputFile >> data ;
				strncpy(temp_item, data, 4) ;
			}
			item++ ;	
		}
		/*read query file */
		int count = cnt = 0;
		queryFile >> temp_name;
		while(!queryFile.eof()) {
			dist=1000000000.0;
			real_distance = 1000000000.0;
			strcpy(nam, "");
			if (count == 1) // get radius and store it
			{
				radius = strtod(temp_name, NULL);
			}
			else if (count > 1)
			{
				if (strncmp(temp_name, "item", 4) == 0) //read item name
				{
					outputFile << "Query : " << temp_name << endl;
					cnt = 0;
				}
				else //read coordinates of the item and store them
				{
					temp_coordinate = strtod(temp_name, NULL) ;
					query_coords[cnt] = temp_coordinate;
					cnt++;
				}
			}
			if (cnt == total_coords) //if we read all the coordinates of an item
			{
				if (strcmp(inputDataType, "euclidean") == 0) //same as before - find ID and hash function f(p) of query item
				{
					f_p = 0;
					for (int q=0; q<k; q++)
					{
						for (int ii=0; ii<total_coords; ii++)
						{
							do
							{
								y1 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
								y2 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
								r = y1*y1 + y2*y2;
							}
							while(r>1.0);

							//Calculate gaussian var
							x1 = y1*sqrt((-2.0)*(log(r)/r));
							vec[ii] = x1;
						}

						//calculate the dot_product of the two vectors
						inner_product = dot_product(vec, query_coords, total_coords); 
						t = (double) w*(rand() / (RAND_MAX+1.0));
						h_p = (int) ((inner_product + t)/w);
						ri = rand() + 1;
						f_p = f_p + modulo(ri*h_p, M);
					}
					//store query's ID
					query_id = f_p;
					size = pow(2,k);
					f_p = modulo(f_p, size); //store query's f(p) hash function
					
					/*start searching for neighbors */
					if (radius>0)
					{
						outputFile << "R-Near neighbors:" << endl;
					}
					dist = 1000000000.0;
					begin = clock();
					//LSH
					for (int ii=0; ii<L; ii++) //for each hashtable
					{
						hashtable[ii][f_p].euc_search_chain(query_coords, total_coords, query_id, radius, outputFile, &dist, nam, inputDataType); //function to search for neighbors given the position in the hashtable and the ID of the query
					}
					end = clock();
					lsh_time = (double) (end - begin) / CLOCKS_PER_SEC; 
					if (dist == 1000000000.0)
						outputFile << "None" << endl;
					if (strcmp(nam, "") != 0)
					{
						outputFile << "Nearest neighbor: " << nam << endl << "distanceLSH : " << dist << endl;
					}

					//REAL NEAREST NEIGHBOR DISTANCE SEARCH
					begin = clock();
					for (int i=0; i<total_items; i++)
					{
						EuclideanList[i]->set_flag(false);
						vec = EuclideanList[i]->get_coordinates();
						dist = euclidean_distance(vec, query_coords, total_coords); //get real distance
						if (dist < real_distance)
							real_distance = dist;
					}
					end = clock();
					real_time = (double) (end - begin) / CLOCKS_PER_SEC;
					outputFile << "distanceTrue : " << real_distance << endl << "tLSH : " << lsh_time << endl << "tTrue : " << real_time << endl << "--------------" << endl;
				}

				else if (strcmp(inputDataType, "cosine") == 0) //find hash function g(p) for the query item
				{
					g_p = 0;
					for (int q=0; q<k; q++)
					{
						for (int ii=0; ii<total_coords; ii++)
						{
							do
							{
								y1 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
								y2 = -1.0 + (rand() / (RAND_MAX + 1.0))*(1.0-(-1.0));
								r = y1*y1 + y2*y2;
							}
							while(r>1.0);

							//Calculate gaussian var
							x1 = y1*sqrt((-2.0)*(log(r)/r));
							vec[ii] = x1;
						}

						//calculate the dot_product of the two vectors
						inner_product = dot_product(vec, query_coords, total_coords); 
						if (inner_product >= 0)
								h_p = 1;
							else
								h_p = 0;
						g_p = g_p + h_p*pow(2, (k-1)-q); //query's hash function g(p)
					}
					/*start searching for neighbors */
					if (radius>0)
					{
						outputFile << "R-Near neighbors:" << endl;
					}
					dist = 1000000000.0;
					begin = clock();
					//LSH
					for (int ii=0; ii<L; ii++) 
					{
						hashtable[ii][g_p].euc_search_chain(query_coords, total_coords, query_id, radius, outputFile, &dist, nam, inputDataType); //search for neighbors for query(according to its hash function g(p))
					}
					end = clock();
					lsh_time = (double) (end - begin) / CLOCKS_PER_SEC;
					if (dist == 1000000000.0)
						outputFile << "None" << endl;
					if (strcmp(nam, "") != 0)
					{
						outputFile << "Nearest neighbor: " << nam << endl << "distanceLSH : " << dist << endl;
					}

					//REAL NEAREST NEIGHBOR DISTANCE SEARCH
					begin = clock();
					for (int i=0; i<total_items; i++)
					{
						EuclideanList[i]->set_flag(false);
						vec = EuclideanList[i]->get_coordinates();
						dist = cosine_distance(vec, query_coords, total_coords); //get real_distance
						if (dist < real_distance)
							real_distance = dist;
					}
					end = clock();
					real_time = (double) (end - begin) / CLOCKS_PER_SEC;
					outputFile << "distanceTrue : " << real_distance << endl << "tLSH : " << lsh_time << endl << "tTrue : " << real_time << endl << "--------------" << endl;
				}
			}
			count++;
			queryFile >> temp_name;
		}
		cout << "Results to output file" << endl ;
		/* Deallocation */
		for(int z=0 ; z<total_items-1 ; z++) {
			delete EuclideanList[z] ;
		}
		delete[] temp_coordArray ;
		delete[] vec ;
		delete[] query_coords ;
	}
	else if (strcmp(inputDataType, "hamming") == 0) { //hamming case
		/*Create Hashtable */
		hash_node<HammingObj> hashtable[L][size];
		int dist, radius, real_distance = 65; //set it as max distance which is 65
		/* Counting items of the file*/
		inputFile_temp1 >> data ;
		while(!inputFile_temp1.eof()) {
			inputFile_temp1 >> data ;
			total_items++ ;
			inputFile_temp1 >> data ;
		}
		HammingList = new HammingObj[total_items] ; //set an array of objects-points to store point info
		
		/* Inserting into local storage */
		cout << "NN-search and queries-execution(Hamming Space) " << endl ;
		inputFile >> data ;
		while(!inputFile.eof()) { 
			strcpy(temp_name, data) ;
			inputFile >> data ;
			HammingList[item] = HammingObj(temp_name, data) ;
			HammingList[item].set_ID(item);
			bits = strlen(data);
			/*Inserting point to the hashtable */
			for (int ii=0; ii<L; ii++)
			{
				res = 0;
				for (int q=0; q<k; q++) //concatenation of k hash functions(k bits)
				{
					// choose hi=xi
					position = 1 + (rand() / (RAND_MAX + 1.0))*(bits); //select it uniformly
					position--;
					strcpy(temp_str, HammingList[item].get_binaryStr());
					bit = temp_str[position] - '0';
					res = res + bit*pow(2, (k-1)-q); // res = result  of function g
				}
				hashtable[ii][res].insert_data(&HammingList[item]); // insert to hashtable(in the right bucket, a pointer will point to the right object-point of the array)
			}
			item++ ;
			inputFile >> data ;
		}
		
		/* read from query file */
		int count=0;
		queryFile >> temp_name;
		while(!queryFile.eof()) {
			dist=65;
			real_distance = 65;
			strcpy(nam, "");
			if (count == 1) // get radius
			{
				radius = atoi(temp_name);
			}
			else if (count > 1)
			{
				if (strncmp(temp_name, "item", 4) == 0) // read item name
				{
					outputFile << "Query : " << temp_name << endl;
				}
				else //read binary-str
				{
					// LSH
					if (radius>0)
					{
						outputFile << "R-Near neighbors:" << endl;
					}
					bits = strlen(temp_name);
					begin = clock();
					for (int ii=0; ii<L; ii++) //for each hashtable
					{
						res=0;
						for (int q=0; q<k; q++) //concatenate k hash functions to create g(p)
						{
							//chose hi=xi
							position = 1 + (rand() / (RAND_MAX + 1.0))*(bits); 
							position--;
							strcpy(temp_str, HammingList[position].get_binaryStr());
							bit = temp_str[position] - '0';
							res = res + bit*pow(2, (k-1)-q); //res = result  of function g
						}
						hashtable[ii][res].ham_search_chain(temp_name, bits, radius, outputFile, &dist, nam);  //search for nearest neighbors using that query's g-hash function
					}
					end = clock();
					lsh_time = (double) (end - begin) / CLOCKS_PER_SEC;
					if (dist == 65)
						outputFile << "None" << endl;
					if (strcmp(nam, "") != 0)
					{
						outputFile << "Nearest neighbor: " << nam << endl << "distanceLSH : " << dist << endl;
					}

					//REAL NEAREST NEIGHBOR DISTANCE SEARCH
					begin = clock();
					for (int i=0; i<total_items; i++)
					{
						HammingList[i].set_flag(false);
						strcpy(temp_str, HammingList[i].get_binaryStr());
						dist = hamming_distance(temp_str, temp_name, bits); //get real distance
						if (dist < real_distance)
							real_distance = dist;
					}
					end = clock();
					real_time = (double) (end - begin) / CLOCKS_PER_SEC;
					outputFile << "distanceTrue : " << real_distance << endl << "tLSH : " << lsh_time << endl << "tTrue : " << real_time << endl << "--------------" << endl;
				}
			}
			count++;
			queryFile >> temp_name;
		}
		cout << "Results to output file" << endl ;
		
		/* delete */
		delete[] HammingList ;
	}
	else if (strcmp(inputDataType, "matrix") == 0) { //matrix case
		hash_node<MatrixObj> hashtable[L][size];
		/* Counting items */
		while (inputFile_temp1 >> c) {
			if ( c == ',' ) {
				total_items++ ;
			}
		}
		total_items++ ;
		// set an array of pointers to matrix objects that we 'll store info about the points
		MatrixList = new MatrixObj*[total_items] ;
		//declare some arrays to help us in the calculation
		namesArray = new char*[total_items] ;
		temp_distArray = new int[total_items] ;
		/* Inserting into local storage */
		cout << "NN-search and queries-execution via Matrix " << endl ;
		inputFile_temp2 >> data ;
		inputFile_temp2 >> c ;
		for (i=0 ; i<total_items ; ++i) { 
			inputFile_temp2 >> data ;
			data_length=0 ;
			c = data[0] ;
			while ( c!=',' && c!=']' ) {
				data_length++ ;
				c = data[data_length] ;
			}
			strncpy(temp_name, data, data_length);
			namesArray[i] = new char[10] ;
			strncpy(namesArray[i], temp_name, data_length);
		}
		inputFile >> data ;
		for (column=0 ; column<total_items ; ++column) {
			inputFile >> data ;
		}
		for (row=0 ; row<total_items ; ++row) {
			MatrixList[row] = new MatrixObj(namesArray[row], total_items) ;
			MatrixList[row]->set_ID(row) ;
			
			for (column=0 ; column<total_items ; ++column) {
				inputFile >> data ;
				
				MatrixList[row]->add_distance(atoi(data), column) ;
			}
		}
		
		//some declared arrays to help us in the calculation
		line_proj = new double[total_items] ;
		sorted_line_proj = new double[total_items] ;
		H = new int[total_items] ;
		
		/* Line Projection */
		for (row=0 ; row<total_items ; ++row) { //for each item
			//get points x1,x2 uniformly
			x1_pos = (rand() / (RAND_MAX + 1.0))*total_items;
			do {
  				x2_pos = (rand() / (RAND_MAX + 1.0))*total_items;
  			} while ( x1_pos == x2_pos ) ;
  			//calculate matrix projection and store it to an array
			line_proj[row] = matrix_projection(MatrixList[row]->get_distances(), MatrixList[x1_pos]->get_distances(), x1_pos, x2_pos) ;
		}
		/* Sorting Line Projection Array */
		sorted_line_proj = bubble_sort(line_proj, total_items) ;
		/* Getting the Median */
		if (total_items%2 == 0) {
			median = (sorted_line_proj[total_items/2-1] + sorted_line_proj[total_items/2])/2 ;
		}
		else {
			median = sorted_line_proj[total_items/2] ;
		}
		/* Family Hash Function H */
		for (row=0 ; row<total_items ; ++row) {
			if ( line_proj[row] >= median ) {
				H[row] = 1 ;
			}
			else {
				H[row] = 0 ;
			}
		}
		delete[] line_proj;
		for (row=0 ; row<total_items ; ++row) {
			for(j=0 ; j<L ; ++j) {
				res=0 ;
				for (i=0 ; i<k ; ++i) { //concatenation of k hash functions to form hash function g
					pos = rand() % total_items ;
					res += H[pos]*pow(2, (k-1)-i);
				}
				//res = hash function g
				hashtable[j][res].insert_data(MatrixList[row]) ; // insert to hashtable(in the right bucket, a pointer will point to the right object-point of the array)
			}
		}
		
		/* Read from query file */
		int count = cnt = 0, real_distance, dist, radius, cur_dist[total_items];
		queryFile >> temp_name;
		while(!queryFile.eof()) {
			dist = real_distance = 1000000000;
			strcpy(nam, "");
			if (count == 1) // get radius
			{
				radius = atoi(temp_name);
			}
			else if (count > 1)
			{
				if (strncmp(temp_name, "item", 4) == 0) //read item
				{
					outputFile << "Query : " << temp_name << endl;
					cnt = 0;
				}
				else //read distances
				{
					cur_dist[cnt] = atoi(temp_name);
					cnt++;
				}
			}
			if (cnt == total_items) { //if we read all the distances of a query item
				if (radius>0)
				{
					outputFile << "R-Near neighbors:" << endl;
				}
				res=0 ;
				for (i=0 ; i<k ; ++i) { //create hash function g for the query item
					pos = rand() % total_items ;
					res += H[pos]*pow(2, (k-1)-i);
				}
				dist = 1000000000;
				begin = clock();
				//LSH
				for (int ii=0; ii<L; ii++) 
				{
					hashtable[ii][res].matrix_search_chain(cur_dist, total_items, radius, outputFile, &dist, nam); //search for nearest neighbors using query's hash function g(p)
				}
				end = clock();
				lsh_time = (double) (end - begin) / CLOCKS_PER_SEC;
				if (dist == 1000000000)
					outputFile << "None" << endl;
				if (strcmp(nam, "") != 0)
				{
					outputFile << "Nearest neighbor: " << nam << endl << "distanceLSH : " << dist << endl;
				}

				//REAL NEAREST NEIGHBOR DISTANCE SEARCH
				begin = clock();
				for (int i=0; i<total_items; i++)
				{
					MatrixList[i]->set_flag(false);
					dist = cur_dist[i]; //get real distance
					if (dist == 0)
						continue ;
					if (dist < real_distance)
						real_distance = dist;
				}
				end = clock();
				real_time = (double) (end - begin) / CLOCKS_PER_SEC;
				outputFile << "distanceTrue : " << real_distance << endl << "tLSH : " << lsh_time << endl << "tTrue : " << real_time << endl << "--------------" << endl;
			}
			count++ ;
			queryFile >> temp_name;
		}
		cout << "Results to output file" << endl ;
		for(int w=0 ; w<total_items ; ++w) {
			delete MatrixList[w] ;
		}
		for(int w=0 ; w<total_items ; ++w) {
			delete[] namesArray[w] ;
		}
		delete[] temp_distArray ;
		delete[] sorted_line_proj ;
		delete[] H ;
	}
	delete[] nam ;
	inputFile.close() ;
	inputFile_temp1.close() ;
	inputFile_temp2.close() ;
	queryFile.close() ;
	outputFile.close() ;
	
	return 0 ;
}

