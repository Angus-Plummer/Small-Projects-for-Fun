#include "ParticleSystem.h"

//empty constructor
ParticleSystem::ParticleSystem(){
	std::vector<Particle> temp;
	particles = temp;
}

//destructor
ParticleSystem::~ParticleSystem(){
}

//returns size of system
int ParticleSystem::size(){
	return particles.size();
}

//add a particle
void ParticleSystem::addParticle(Particle particle){
	particles.push_back(particle);
}

//returns 1 if system contains particle
bool ParticleSystem::contains(Particle particle){
	//iterates over particles vector until particle is found
	for (int i = 0; i < particles.size(); i++){
		if (particle.isEqualTo(particles[i])){
			return 1;
		}
	}
	//if not found return 0
	return 0;
}

//returns index of particle in particles. If not contained in vector then return -1
int ParticleSystem::index(Particle particle){
	//iterates over particles vector
	for (int i = 0; i < particles.size(); i++){
		//if particle is found return index
		if (particle.isEqualTo(particles[i])){
				return i;
		}
	}
	//if not found return -1
	return -1;
}

//removes the particle from the system if it is in system
void ParticleSystem::removeParticle(Particle particle){
	//test if particle is in system
	if (contains(particle)){
		//replace particle to remove with last element
		particles[index(particle)] = particles.back();
		//remove last element
		particles.pop_back();
		//destroy removed particle
		particle.~Particle();
	}
}

//fuse any particles that collide, only checks its parents child nodes
void ParticleSystem::applyCollisions(BHTreeNode BHTree){
	//variable for current number of particles as cba to learn iterators
	int numParticles = particles.size();
	//iterate over each possible pair of particles
	for (int i = 0; i < numParticles; i++){
		for (int j = i + 1; j < numParticles; j++){
			//get bector between the two particles
			Vector3D pos1 = particles[i].getPosition().copy();
			pos1.sub(particles[j].getPosition());
			double distance = pos1.mag();
			//if distance is less than sum of particle radii then collide
			if (distance < (particles[i].getRadius() + particles[j].getRadius())){
				//fuse the particles
				particles[i].fuse(particles[j]);
				//remove the second of the pair as now fused
				removeParticle(particles[j]);
				//reduce number of particles to iterate over
				numParticles -= 1;
			}
		}
	}
}

//checks for any out of bounds particles and removes them
void ParticleSystem::removeOutOfBounds(Vector3D corner1, Vector3D corner2){
	//iterate over all particles
	for (int i = 0; i < particles.size(); i++){
		//obtain location information
		double x = particles[i].getPosition().getX();
		double y = particles[i].getPosition().getY();
		double z = particles[i].getPosition().getZ();
		//if particle out of specified bounds then remove from system
		if (x > corner2.getX() || x < corner1.getX() || y > corner2.getY() || y < corner1.getY() || z > corner2.getZ() || z < corner1.getZ()){
			removeParticle(particles[i]);
			//std::cout << "particle removed\n";

		}
	}
}

//fill and empty BHTree with the particles
void ParticleSystem::fillBHTree(BHTreeNode *BHTree){
	for (int i = 0; i < particles.size(); i++){
		BHTree->addParticle(particles[i].getPosition(), particles[i].getMass());
	}
}

//applies force from attractor created by mouseclick
void ParticleSystem::applyAttractor(Particle attractor){
	for (int i = 0; i < particles.size(); i++){
		Vector3D vecTo = attractor.getPosition();
		vecTo.sub(particles[i].getPosition());
		double distance = vecTo.mag();
		vecTo.norm();
		vecTo.mult(0.01 * particles[i].getMass() * attractor.getMass() * pow(distance, -2));
		particles[i].applyForce( vecTo );
	}
}

//updates all particles in system using BHTree with precision theta and draws to image
void ParticleSystem::update(BHTreeNode* BHTree, double theta, double timestep){
	//std::cout <<"number of particles: "<< particles.size() << "\n";
	//iterate over all particles
	for (int i = 0; i < particles.size(); i++){
		//apply gravitational forces
		particles[i].applyNewtonianGravity(BHTree, theta);
		//update each particle
		particles[i].update(timestep);
	}
	//applyCollisions(BHTree);
}

// draws al particles to image
void ParticleSystem::draw(Mat img){
	//iterate over all particles
	for (int i = 0; i < particles.size(); i++){
		//draw particle on image
		particles[i].draw(img);
	}
}

