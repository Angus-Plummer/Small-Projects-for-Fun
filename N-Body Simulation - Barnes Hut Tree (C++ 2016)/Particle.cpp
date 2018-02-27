//class to define particles for particle system

#include "Particle.h"
double gravitationalConstant = 0.01;


//contructor takes position vector argument and creates a stationary particle at the position vector
Particle::Particle(Vector3D position_new, double mass_new, double radius_new){
	//set new position to position vector input
	position = position_new;
	mass = mass_new;
	radius = radius_new;
}
//destructor
Particle::~Particle(){
}

//returns position of particle
Vector3D Particle::getPosition(){
	return position;
}

//returns velocity of particle
Vector3D Particle::getVelocity(){
	return velocity;
}

//returns acceleration of particle
Vector3D Particle::getAcceleration(){
	return acceleration;
}

//sets position to input vector
void Particle::setPosition(Vector3D position_new){
	position = position_new;
}

//sets the velocity to input vector
void Particle::setVelocity(Vector3D velocity_new){
	velocity = velocity_new;
}

//returns mass of particle
double Particle::getMass(){
	return mass;
}

//returns charge of particle
double Particle::getCharge(){
	return charge;
}

//returns radius of particle
double Particle::getRadius(){
	return radius;
}

//appy force to particle by updating acceleration given particle mass
void Particle::applyForce(Vector3D force){
	// does not apply if mass is 0
	if (mass != 0){
		//create duplicate vector of force
		Vector3D tempForce = force.copy();
		//divide by particle mass to get acceleration
		tempForce.div(mass);
		//add acceleration from force
		acceleration.add(tempForce);
		//destroy temporary force variable
		tempForce.~Vector3D();
	}
}

//apply newtonian gravity force on particle given a barnes hut tree of particles using a precision of theta
void Particle::applyNewtonianGravity(BHTreeNode* BHTree, double theta){
	//get vector from the particle to the CoM of node
	Vector3D posNode = BHTree->getCentreOfMass().copy();
	posNode.sub(position);
	//distance between node and CoM of node
	double distance = posNode.mag();
	//if node holds more than current particle
	if (distance > 2 * radius){
		//std::cout << BHTree->getisExternal() << "\n";
		//if node is internal and it either contains the particle or is too close for its size given the theta factor, split into its octants and call function on them
		if (!(BHTree->getisExternal()) && ((BHTree->contains(position) || (BHTree->getDimensions() / distance > theta)))){
			//std::cout << "tumbling down the rabbit hole\n";
			for (int i = 0; i < 8; i++){
				if (!((BHTree->getChild(i))->getIsEmpty())){
					applyNewtonianGravity(BHTree->getChild(i), theta);
				}
			}
		}
		else{
			//magnitude of grav force
			double forceMag = 0.01 * mass * BHTree->getMass() * pow(distance, -2.);//- 0.1 * pow(distance, -4.);
			//std::cout << "distance " << distance << " node mass " << BHTree->getMass() << " mag " << forceMag << "\n";
			//normalise vector between the particles and * by mag of force
			posNode.norm();
			posNode.mult(forceMag);
			//apply force to particle 
			applyForce(posNode);
		}
	}
}

//fuse particle with another particle (only updates current particle)
void Particle::fuse(Particle otherParticle){
	
	//change position to centre of mass of both particles
	Vector3D vecTo = otherParticle.position.copy();
	vecTo.sub(position);
	vecTo.mult(otherParticle.mass / (mass + otherParticle.mass));
	position.add(vecTo);
	
	//change velocity to conserve momentum
	velocity.mult(mass / (mass + otherParticle.mass));
	otherParticle.velocity.mult(otherParticle.mass / (mass + otherParticle.mass));
	velocity.add(otherParticle.velocity);
	
	//change acceleration so total acceleration * mass is constant
	acceleration.mult(mass / (mass + otherParticle.mass));
	otherParticle.acceleration.mult(otherParticle.mass / (mass + otherParticle.mass));
	acceleration.add(otherParticle.acceleration);

	//update radius assuming partices have uniform density
	radius *= pow((1 + (otherParticle.mass / mass)), 1./3.);
	
	//add masses together
	mass += otherParticle.mass;
	
	std::cout << " fused\n";
}

//returns a new particle which has position at centre of mass of the two particles and their combined mass
Particle Particle::centreOfMass(Particle otherParticle){
	
	//change position to centre of mass of both particles
	Vector3D vecTo = otherParticle.position.copy();
	vecTo.sub(position);
	vecTo.mult(otherParticle.mass / (mass + otherParticle.mass));
	vecTo.add(position);
	Particle p(vecTo, mass + otherParticle.mass, 1);
	return p;

}

//updates particle position and motion given its attributes
void Particle::update(double timestep) {
	//std::cout << "x " << acceleration.getX() << " y " << acceleration.getY()<< " z "<< acceleration.getZ() << "\n";
	//update velocity given acceleration
	velocity.add(acceleration.scalarProd(timestep));
	//update position given velocity
	position.add(velocity.scalarProd(timestep));
	//set acceleration to 0
	acceleration.sub(acceleration);
}

//draws particle to image
void Particle::draw(Mat img){
	//maximum particle radius
	double maxRadius = 4;
	//create point struct of position
	Point loc(position.getX(), position.getY());
	//only draw if point is on the image
	if (loc.x > 0 && loc.x < img.size().width && loc.y > 0 && loc.y < img.size().height){
		//scale radius with z axis to simulate depth
		double circleRadius = maxRadius * position.getZ() / img.size().width;

		//radius takes integer value so if less than 1 just set to 1
		if (circleRadius < 0.1){
			circleRadius = 0.1;
		}
		//if over max circle radius then set to be maximum
		if (circleRadius > maxRadius){
			circleRadius = maxRadius;
		}
		//draw circle

		double colourFactor = 255 * sin(position.getZ() / img.size().width * 3.14159);
		Scalar particleColour(colourFactor, 255/5, 255/3);

		//filled
		circle(img, loc, circleRadius, particleColour, -1, 8);

		//notfilled
		//circle(img, loc, circleRadius, Scalar(150, 0, 0), 1, 1);

		//img.at<Vec3b>(loc) = (0, 0, 0);
	}
}

//returns true if particles have same attributes
bool Particle::isEqualTo(Particle targetParticle){
	if (position.isEqualTo(targetParticle.position) && velocity.isEqualTo(targetParticle.velocity) && acceleration.isEqualTo(targetParticle.acceleration) 
		&& mass == targetParticle.getMass() && charge == targetParticle.getCharge() && radius == targetParticle.getRadius()){
		return 1;
	}
	else{
		return 0;
	}

}

