#include <iostream>
#include <cstring>
#include "hashtable.h"
#include "distances.h"

using namespace std;


template < class T3 >
list_node<T3> :: list_node(T3* d) //set data(T3)-pointer of a node(bucket)
{
	data = d;
}

template < class T3 >
list_node<T3> :: ~list_node() //destructor
{

}

template < class T3 >
void list_node<T3> :: set_next(list_node* n1) //set next pointer of a list_node(bucket)
{
	next = n1;
}

template < class T3 >
list_node<T3>* list_node<T3> :: get_next() //get next pointer of a list_node(bucket)
{
	return next;
}

template < class T3 >
T3* list_node<T3> :: get_data() //get data(T3)-pointer of a node(bucket)
{
	return data;
}



template < class T2 >
hash_bucket_list<T2> :: hash_bucket_list() // list constructor that sets head,tail and current node-pointer to NULL
{
	head = NULL;
	tail = NULL;
	current = NULL;
}

template < class T2 >
hash_bucket_list<T2> :: ~hash_bucket_list() //list destructor
{
	while (head != NULL)
	{
		current = head->get_next();
		delete head;
		head = current;
	}
}

template < class T2 >
void hash_bucket_list<T2> :: add_node(T2* dat) //add bucket to the overflow-bucket list
{
	list_node<T2>* node = new list_node<T2>(dat);
	if (head == NULL) // empty bucket list
	{
		head = tail = node;
	}
	else
	{
		tail->set_next(node);
		tail = node;
	}
}


template < class T2 >
void hash_bucket_list<T2> :: matrix_search_list(int* cur_dist, int total_items, int range, fstream& output, int* dis, char* nam) 
{
	T2 *d;
	int temp ;
	current = head;
	int distance;
	while (current != NULL) //search the linked bucket list
	{
		d = current->get_data(); //get the pointer to a point p from the node
		temp = distance ;
		distance = cur_dist[d->get_ID()]; //get distance
		if(distance == 0) { // do not compare same points(which means distance 0)
			current = current->get_next(); // move to the next bucket
			distance = temp ;
			continue ;
		}
		if (distance <= range || range == 0)// search for nearest neighbors and also find the minimum distance
		{
			if ((d->get_flag()) == false) //if a neighbor is not flagged as included to the output file
			{
				if (range != 0)
					output << d->get_name() << " " << distance << endl;
				if (distance < *dis)
				{
					strcpy(nam, d->get_name());
					*dis = distance;
				}
				d->set_flag(true); //set neighbor as included
			}
		}
		current = current->get_next(); // move to the next bucket

	}
}


//nearest neighbor range search(euclidean/cosine case)
template < class T2 >
void hash_bucket_list<T2> :: euc_search_list(double* vec, int total_coords, int q_id, double range, fstream& output, double* dis, char* nam, char* type) 
{
	T2 *d;
	current = head;
	double distance;
	while (current != NULL) //search the linked bucket list
	{
		d = current->get_data(); //get the pointer to point p from the node
		if (strcmp(type, "euclidean") == 0) //euclidean case
		{
			if (d->get_ID() != q_id) //if query item and current item don't have the same ID ,then go to the next bucket
			{
				current = current->get_next();
				continue;
			}
			distance = euclidean_distance(vec, d->get_coordinates(), total_coords); //get the euclidean distance
			if (distance <= range || range == 0) //find nearest neighbors and also the minimum euclidean distance of the two points
			{
				if ((d->get_flag()) == false) //if a neighbor is not flagged as included to the output file
				{
					if (range != 0)
						output << d->get_name() << " " << distance << endl;
					if (distance < *dis)
					{
						strcpy(nam, d->get_name());
						*dis = distance;
					}
					d->set_flag(true); //set neighbor as included
				}
			}
			current = current->get_next(); // move to the next bucket
		}
		else if (strcmp(type, "cosine") == 0) //cosine case
		{
			distance = cosine_distance(vec, d->get_coordinates(), total_coords);
			if (distance <= range || range == 0) //find nearest neighbors and also the minimum cosine distance of the two points
			{
				if ((d->get_flag()) == false) //if a neighbor is not flagged as included to the output file
				{
					if (range != 0)
						output << d->get_name() << " " << distance << endl;
					if (distance < *dis)
					{
						strcpy(nam, d->get_name());
						*dis = distance;
					}
					d->set_flag(true); //set neighbor as included
				}
			}
			current = current->get_next(); //move to the next bucket
		}
	}
}

//nearest neighbor range search(hamming case)
template < class T2 >
void hash_bucket_list<T2> :: ham_search_list(char* query, int max_bits, int range, fstream& output, int* dis, char* nam) 
{
	T2 *d;
	current = head;
	int distance;
	while (current != NULL) //search the linked bucket list
	{
		//get the pointer that points to a point(item) p from the node
		d = current->get_data(); 
		//get hamming distance
		distance = hamming_distance(d->get_binaryStr(), query, max_bits); 
		// search for nearest neighbors in range and also find the minimum hamming distance of the two points
		if (distance <= range || range == 0) 
		{
			if ((d->get_flag()) == false) //if a neighbor is not flagged as included to the output file
			{
				if (range != 0)
					output << d->get_name() << " " << distance << endl;
				if (distance < *dis)
				{
					strcpy(nam, d->get_name());
					*dis = distance;
				}
				d->set_flag(true); //set neighbor as included
			}
		}
		current = current->get_next(); // move to the next bucket
	}
}


template < class T >
hash_node<T> :: hash_node() //creates an empty linked list for the hash-node
{
	bucketlist = new hash_bucket_list<T>();
}

template < class T >
hash_node<T> :: ~hash_node() //destroys the linked list
{
	delete bucketlist;
}

template < class T >
void hash_node<T> :: insert_data(T* data) //adds bucket to the list
{
	bucketlist->add_node(data);
}



//function to search for nearest neighbors and calculate minimum distance(hamming case)
template < class T >
void hash_node<T> :: ham_search_chain(char* query, int max_bits, int range, fstream& output, int* d, char* n) 
{
	bucketlist->ham_search_list(query, max_bits, range, output, d, n);
}

 //function to search for nearest neighbors and calculate minimum distance(euclidean/cosine case)
template < class T >
void hash_node<T> :: euc_search_chain(double* vec, int total_coords, int q_id, double range, fstream& output, double* d, char* n, char* type)
{
	bucketlist->euc_search_list(vec, total_coords, q_id, range, output, d, n, type);
}

//function to search for nearest neighbors and calculate minimum distance(matrix case)
template < class T >
void hash_node<T> :: matrix_search_chain(int* cur_dist, int total_items, int range, fstream& output, int* d, char* n)  
{
	bucketlist->matrix_search_list(cur_dist, total_items, range, output, d, n);
}

