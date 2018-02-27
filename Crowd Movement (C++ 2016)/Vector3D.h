#ifndef VECTOR3D
#define VECTOR3D

//includes
#include<math.h>

//vector3D class for 3D vectors
class Vector3D{

//private x and y components of vector
private:
	double x;
	double y;
	double z;

public:
	//constructors for given x and y or no args
	Vector3D(double x_new, double y_new, double z_new);
	Vector3D();
	//destructor
	~Vector3D();
	
	//functions to return x y and z components
	double getX();
	double getY();
	double getZ();

	//returns copy of vector
	Vector3D copy();
	
	//add or subtract another vector
	void add(Vector3D v);
	void sub(Vector3D v);

	//returns new vector that is sum of the two vectors
	Vector3D sum(Vector3D v);
	//returns new vector that is vector - v
	Vector3D diff(Vector3D v);
	//returns a vector which is the scalar product of argument with vector
	Vector3D scalarProd(double k);
	//returns vector product of vector with another
	Vector3D vectorProd(Vector3D v);
	//returns the angle difference between vector and another
	double angleBetween(Vector3D v);

	//scalar multiplication and division
	void mult(double k);
	void div(double k);

	//return magnitude and normalisation
	double mag();
	void norm();

	//returns true if vector is same as target vector
	bool isEqualTo(Vector3D targetVector);

};
#endif

