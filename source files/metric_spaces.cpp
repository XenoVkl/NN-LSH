#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <ctime>
#include "metric_spaces.hpp"

using namespace std ;


/*  Class: HammingObj  */
HammingObj::HammingObj(char* given_name, char* given_binaryStr) {
	strcpy(name, given_name) ;
	included = false;
	strcpy(binaryStr, given_binaryStr) ;
}
void HammingObj::set_ID(int given_ID) {
	ID = given_ID ;
}
char* HammingObj::get_name() {
	return name ;
}
int HammingObj::get_ID() {
	return ID ;
}
char* HammingObj::get_binaryStr() {
	return binaryStr ;
}

bool HammingObj::get_flag(){
	return included;
}

void HammingObj::set_flag(bool flag){
	included = flag;
}

/*  Class: EuclideanObj  */
EuclideanObj::EuclideanObj(char* given_name, int total_coords) {
	strcpy(name, given_name) ;
	
	coordinates = new double[total_coords] ;
}
EuclideanObj::~EuclideanObj() {
	delete[] coordinates ;
}
void EuclideanObj::set_ID(int given_ID) {
	ID = given_ID ;
}
void EuclideanObj::add_coordinate(double given_coordinate, int pos) {
	coordinates[pos] = given_coordinate ;
}
char* EuclideanObj::get_name() {
	return name ;
}
int EuclideanObj::get_ID() {
	return ID ;
}
double* EuclideanObj::get_coordinates() {
	return coordinates ;
}

bool EuclideanObj::get_flag(){
	return included;
}

void EuclideanObj::set_flag(bool flag){
	included = flag;
}

/*  Class: MatrixObj  */
MatrixObj::MatrixObj(char* given_name, int total_items) {
	strcpy(name, given_name) ;
	
	distances = new int[total_items] ;
}
MatrixObj::~MatrixObj() {
	delete[] distances ;
}
void MatrixObj::set_ID(int given_ID) {
	ID = given_ID ;
}
void MatrixObj::add_distance(int given_distance, int pos) {
	distances[pos] = given_distance ;
}
char* MatrixObj::get_name() {
	return name ;
}
int MatrixObj::get_ID() {
	return ID ;
}
int* MatrixObj::get_distances() {
	return distances ;
}
bool MatrixObj::get_flag(){
	return included;
}

void MatrixObj::set_flag(bool flag){
	included = flag;
}

