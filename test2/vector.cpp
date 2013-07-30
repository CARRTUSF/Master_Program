/*
 * vector.cpp
 *
 *  Created on: Aug 28, 2009
 *%%%%% Developed by: Garret Pence%%%%%%%%%%
 */

#include <iostream>
#include "vector.h"

using namespace std;

vector::vector()
{
	x = 0;
	y = 0;
	z = 0;
}

vector::vector(double m, double n, double o)
{
	x = m;
	y = n;
	z = o;
}

vector crossProduct(vector a, vector b)
{
	double i, j, k;
	vector c;

	i = (a.y * b.z - a.z * b.y);
	j = (a.x * b.z - a.z * b.x) * -1;
	k = (a.x * b.y - a.y * b.x);

	c.x = i;
	c.y = j;
	c.z = k;

	return c;
}
