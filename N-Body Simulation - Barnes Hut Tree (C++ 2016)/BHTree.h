#ifndef BH_TREE_H
#define BH_TREE_H

#include "Vector3D.h"
#include<vector>

class BHTreeNode{

private:

	//position vector of corner of node closest to origin
	Vector3D nodeOrigin;
	//length of sides of node
	double dimensions;
	//position vector of centre of mass of node
	Vector3D centreOfMass;
	//does node contain any particles
	bool isEmpty;
	//total mass of node
	double mass;
	//is node external
	bool isExternal;

	//each of the 8 child nodes
	BHTreeNode *children[8];


public:
	
	BHTreeNode(Vector3D nodeOrigin_new, double dimensions_new);
	~BHTreeNode();

	//returns centre of mass of node
	Vector3D getCentreOfMass();
	//returns mass of node
	double getMass();
	//returns dimensions of node
	double getDimensions();
	//returns isEmpty
	bool getIsEmpty();
	//return true if node is external
	bool getisExternal();
	//return the ith child node
	BHTreeNode* getChild(int i);
	//adds a particle to the node, if there already is one then recursively propograte down until they are in separate nodes
	void addParticle(Vector3D particlePos, double particleMass);
	//checks if a particle is within the nodes volume
	bool contains(Vector3D position);
	//resets the tree to just one empty node
	void wipe();
};

#endif