#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <list>

using namespace std ;


class HammingObj {
	private:
		char name[10] ;
		int ID ;
		char binaryStr[100] ;
		bool included;
	public:
		HammingObj() {} ;
		HammingObj(char* given_name, char* given_binaryStr) ;
		~HammingObj() {} ;
		void set_ID(int given_ID) ;
		char* get_name() ;
		int get_ID() ;
		char* get_binaryStr() ;
		void set_flag(bool) ;
		bool get_flag() ;
} ;

class EuclideanObj {
	private:
		char name[10] ;
		int ID ;
		double* coordinates ;
		bool included;
	public:
		EuclideanObj() {} ;
		EuclideanObj(char* given_name, int total_coords) ;
		~EuclideanObj() ;
		void set_ID(int given_ID) ;
		void add_coordinate(double given_coordinate, int pos) ;
		char* get_name() ;
		int get_ID() ;
		double* get_coordinates() ;
		void set_flag(bool) ;
		bool get_flag() ;
} ;

class MatrixObj {
	private:
		char name[10] ;
		int ID ;
		int* distances ;
		bool included;
	public:
		MatrixObj() {} ;
		MatrixObj(char* given_name, int total_items) ;
		~MatrixObj() ;
		void set_ID(int given_ID) ;
		void add_distance(int given_distance, int pos) ;
		char* get_name() ;
		int get_ID() ;
		int* get_distances() ;
		bool get_flag();
		void set_flag(bool flag);
} ;

#endif
