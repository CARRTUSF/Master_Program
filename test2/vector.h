/*
 * vector.h
 *
 *  Created on: Aug 28, 2009
 *      Author: USF WMRA
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

using namespace std;

class vector {
	public:
		vector(); //this is the constructor, to create an empty vector
		vector(double x, double y, double z); //this is the constructor used to create a vector with the spec'd values
		double x; //x-coordinate of vector
		double y; //y-coordinate of vector
		double z; //z-coordinate of vector
};

	vector crossProduct(vector a, vector b); //this takes 2 vectors and finds the cross product
	

#endif /* VECTOR_H_ */
