//Vector3D class for using 3D vectors

//includes
#include "Vector3D.h"

//constructor with x, y and z component arguments
Vector3D::Vector3D(double x_new, double y_new, double z_new){
	x = x_new;
	y = y_new;
	z = z_new;
}
//constructor with no arguments creates a zero vector
Vector3D::Vector3D(){
	x = 0;
	y = 0;
	z = 0;
}

//destructor
Vector3D::~Vector3D(){
}

//functions to return x and y components of vector
double Vector3D::getX(){
	return x;
}
double Vector3D::getY(){
	return y;
}
double Vector3D::getZ(){
	return z;
}

//returns copy of vector
Vector3D Vector3D::copy(){
	Vector3D copy(x, y, z);
	return copy;
}

//add or subtract another vector v
void Vector3D::add(Vector3D v){
	x += v.x;
	y += v.y;
	z += v.z;
}
void Vector3D::sub(Vector3D v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

//returns new vector that is sum of the two vectors
Vector3D Vector3D::sum(Vector3D v){
	Vector3D vSum = copy();
	vSum.add(v);
	return vSum;
}
//returns a vector which is the scalar product of argument with vector
Vector3D Vector3D::scalarProd(double k){
	Vector3D vProd = copy();
	vProd.mult(k);
	return vProd;
}

//returns vector product of vector with another
Vector3D Vector3D::vectorProd(Vector3D v){
	Vector3D product;
	product.x = y * v.z - z * v.y;
	product.y = z * v.x - x * v.z;
	product.z = x * v.y - y * v.x;
	return product;
}	

//scalar multiplication and division
void Vector3D::mult(double k) {
	x = x * k;
	y = y * k;
	z = z * k;
}
void Vector3D::div(double k) {
	x = x / k;
	y = y / k;
	z = z / k;
}

//functions to return vector magnitude and to normalise vector
double Vector3D::mag(){
	return pow(x*x + y*y + z*z, 0.5);
}
void Vector3D::norm(){
	double m = mag();
	//only normalises if magnitude is not zero
	if (m != 0) {
		div(m);
	}
}

//comparator function, returns true if vector has the same componenet values
bool Vector3D::isEqualTo(Vector3D targetVector){
	if (targetVector.x == x && targetVector.y == y && targetVector.z == z){
		return 1;
	}
	else{
		return 0;
	}
}
