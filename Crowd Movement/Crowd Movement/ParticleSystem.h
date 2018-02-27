
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include<vector>
#include "BHTree.h"
#include "Particle.h"
#include "Vector3D.h"

class ParticleSystem{

private:
	std::vector<Particle> particles;

public:
	//empty constructor
	ParticleSystem();
	//destructor
	~ParticleSystem();
	//returns size of system
	int size();
	//add a particle
	void addParticle(Particle particle);
	//returns 1 if system contains particle
	bool contains(Particle particle);
	//returns index of particle in particles. If not contained in vector then return -1
	int index(Particle particle);
	//removes the particle from the system if it is in system
	void removeParticle(Particle particle);
	//fuse any particles that collide, only checks its parents child nodes
	void applyCollisions(BHTreeNode BHTree);
	//checks for any out of bounds particles and removes them. Bounds defined by position vector of diagonal opposite corners of cube
	void removeOutOfBounds(Vector3D corner1, Vector3D corner2);
	//checks for any out of bounds particles and wraps them to other side of map
	void wrapOutOfBounds(Vector3D corner1, Vector3D corner2);
	//repels particles from walls
	void repelFromWalls(Vector3D corner1, Vector3D corner2, double dangerZone, double repelMag);
	//fill and empty BHTree with the particles
	void fillBHTree(BHTreeNode *BHTree);
	//applies force of an attractor from mouseclick
	void applyAttractor(Particle attractor);
	//updates all particles in system using BHTree with precision theta and draws to image
	void update(BHTreeNode *BHTree, double theta, double timestep);
	//draws all particles in system onto image
	void draw(Mat img);
};

#endif

