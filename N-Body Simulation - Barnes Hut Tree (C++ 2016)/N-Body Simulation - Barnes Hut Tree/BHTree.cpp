#include "BHTree.h"
#include<iostream>

//constructor
BHTreeNode::BHTreeNode(Vector3D nodeOrigin_new, double dimensions_new){
	nodeOrigin = nodeOrigin_new;
	dimensions = dimensions_new;
	isExternal = 1;
	isEmpty = 1;
	mass = 0;
	for (int i = 0; i < 8; i++){
		children[i] = nullptr;
	}
}

//destructor also calls destructor of child nodes
BHTreeNode::~BHTreeNode(){
}

//returns centre of mass of node
Vector3D BHTreeNode::getCentreOfMass(){
	return centreOfMass;
}

//returns mass of node
double BHTreeNode::getMass(){
	return mass;
}

//returns dimensions of node
double BHTreeNode::getDimensions(){
	return dimensions;
}

//returns isEmpty
bool BHTreeNode::getIsEmpty(){
	return isEmpty;
}

//return true if node is external
bool BHTreeNode::getisExternal(){
	return isExternal;
}

//return the ith child node
BHTreeNode* BHTreeNode::getChild(int i){
	return children[i];
}

//adds a particle to the node, if there already is one then recursively propograte down until they are in separate nodes
void BHTreeNode::addParticle(Vector3D particlePos, double particleMass){
	//std::cout << "x " << nodeOrigin.getX() << "y " << nodeOrigin.getY() << "z " << nodeOrigin.getZ() << "\n";
	//if node already contains particles
	if (!isEmpty){
		//if node is external
		if (isExternal){
			//std::cout << "external node\n";
			//initialise the child nodes
			children[0] = new BHTreeNode(nodeOrigin, dimensions / 2.);
			children[1] = new BHTreeNode(nodeOrigin.sum(Vector3D(dimensions / 2, 0, 0)), dimensions / 2);
			children[2] = new BHTreeNode(nodeOrigin.sum(Vector3D(0, dimensions / 2, 0)), dimensions / 2);
			children[3] = new BHTreeNode(nodeOrigin.sum(Vector3D(dimensions / 2, dimensions / 2, 0)), dimensions / 2);
			children[4] = new BHTreeNode(nodeOrigin.sum(Vector3D(0, 0, dimensions / 2)), dimensions / 2.);
			children[5] = new BHTreeNode(nodeOrigin.sum(Vector3D(dimensions / 2, 0, dimensions / 2)), dimensions / 2);
			children[6] = new BHTreeNode(nodeOrigin.sum(Vector3D(0, dimensions / 2, dimensions / 2)), dimensions / 2);
			children[7] = new BHTreeNode(nodeOrigin.sum(Vector3D(dimensions / 2, dimensions / 2, dimensions / 2)), dimensions / 2);
			isExternal = 0;
			//iterate over the nodes and add the particles if their location is within the node
			for (int i = 0; i < 8; i++){
				if (children[i]->contains(particlePos)){
					children[i]->addParticle(particlePos, particleMass);
				}
				if (children[i]->contains(centreOfMass)){
					children[i]->addParticle(centreOfMass, mass);
				}
			}
		}
		//if it is an internal node
		else{
			//std::cout << "internal node\n";
			//recursively go down tree trying to add particle
			for (int i = 0; i < 8; i++){
				if (children[i]->contains(particlePos)){
					children[i]->addParticle(particlePos, particleMass);
				}
			}
		}
		//update centre of mass
		Vector3D vecTo = particlePos.copy();
		vecTo.sub(centreOfMass);
		vecTo.mult(particleMass / (mass + particleMass));
		centreOfMass.add(vecTo);
		//add new mass
		//std::cout << "node mass: " << mass << "\n";
		mass += particleMass;
		
	}
	//if the node is empty then add the particle
	else{
		centreOfMass = particlePos;
		mass = particleMass;
		isEmpty = 0;
	}
}

//checks if a location is within the nodes volume
bool BHTreeNode::contains(Vector3D position){
	Vector3D oppDiagonal = nodeOrigin.copy();
	oppDiagonal.add(Vector3D(dimensions, dimensions, dimensions));
	if (position.getX() > nodeOrigin.getX() && position.getX() < oppDiagonal.getX() &&
		position.getY() > nodeOrigin.getY() && position.getY() < oppDiagonal.getY() &&
		position.getZ() > nodeOrigin.getZ() && position.getZ() < oppDiagonal.getZ()){
		return 1;
	}
	else{
		return 0;
	}
}

//wipes the tree
void BHTreeNode::wipe(){
	if (!isExternal){
		for (int i = 0; i < 8; i++){
			children[i]->wipe();
			delete children[i];
		}
	}
	mass = 0;
	centreOfMass = Vector3D(0, 0, 0);
	isEmpty = 1;
	isExternal = 1;
}