#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <fstream>
#include "metric_spaces.hpp"


template < class T3 >
class list_node // node of a bucket overflow list
{
private:
	T3* data;
	list_node* next;
public:
	list_node(T3*);
	~list_node();
	void set_next(list_node*);
	list_node* get_next();
	T3* get_data();
};

template < class T2 >
class hash_bucket_list // overflow list for each position of the hashtable
{
	public:
	list_node<T2>* head;
	list_node<T2>* tail;
	list_node<T2>* current;

	hash_bucket_list();
	~hash_bucket_list();
	void add_node(T2*);
	void ham_search_list(char*, int, int, fstream&, int*, char*);
	void euc_search_list(double*, int, int, double, fstream&, double*, char*, char*);
	void matrix_search_list(int*, int, int, fstream&, int*, char*);
};

template < class T >
class hash_node // position of a hashtable(bucket)
{
private:
	hash_bucket_list<T>* bucketlist;
public:
	void insert_data(T*);
	void ham_search_chain(char*, int, int, fstream&, int*, char*);
	void euc_search_chain(double*, int, int, double, fstream&, double*, char*, char*);
	void matrix_search_chain(int*, int, int, fstream&, int*, char*);
	hash_node();
	~hash_node();
};

#endif
