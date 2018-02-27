#ifndef PARTICLE_H
#define PARTICLE_H

//Paricle class for particle systems

//includes
#include "BHTree.h"
#include "Vector3D.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

class Particle {

private:
	//private variables describing it position and motion
	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;
	double mass;
	double charge;
	double radius;

public:
	//contructor which takes position vector argument and creates motionless particle at that position
	Particle(Vector3D position_new, double mass_new, double radius_new);
	//destructor
	~Particle();
	//return position
	Vector3D getPosition();
	//return velocity
	Vector3D getVelocity();
	//return acceleration
	Vector3D getAcceleration();
	//sets the position as position vector
	void setPosition(Vector3D pos_new);
	//sets the velocity to input vector
	void setVelocity(Vector3D velocity_new);
	//return mass of particle
	double getMass();
	//return charge of particle
	double getCharge();
	//return radius of particle
	double getRadius();
	//appy force to particle to update acceleration given particle mass
	void applyForce(Vector3D force);
	//apply newtonian gravity force on particle given a barnes hut tree of particles using a precision of theta
	void applyNewtonianGravity(BHTreeNode *BHTree, double theta);
	//applies the force on a particle due to the long range attraction and short range repulsion
	//using octant tree with Barnes Hut approximation
	void applyFlocking(BHTreeNode* BHTree, double theta);
	//fuse particle with another particle (only updates current particle)
	void fuse(Particle otherParticle);
	//returns a new particle which has position at centre of mass of the two particles and their combined mass
	Particle centreOfMass(Particle otherParticle);
	//applies a random angular deviation
	void Particle::randomDeviation();
	//function which updates particle's position and velocity
	void update(double timestep);
	//returns true if argument particle is identical to particle
	bool isEqualTo(Particle targetParticle);
	//draws particle onto screen as circle
	void draw(Mat img);
};

#endif