//class to define particles for particle system

#include "Particle.h"
#include <random>
#include <chrono>

//max velocity
double velMag = 10;
//
double pRadius = 5;

//strength of flocking
double forceFactor = pow(10,9); 
//strengths of distance and direction flocking
double proximityFactor = 1; //0->1
double directionFactor = 0.01; //0->1

// std dev of random angle deviation of velocity
double randomAngleDeviation = 3.14159 / 160.;
//angle of vision for particles
double forwardBlindAngle = 2 * 3.14159 * 0.1;
double backBlindAngle = 2 * 3.14159 * 0.25;

//function to generate a random number between an upper and lower limit in divisions of range/n
double randomNumberGaussian(double mean, double var) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<double> normDistribution(mean, var);
	double r = normDistribution(generator);
	return r;
}

//contructor takes position vector argument and creates a stationary particle at the position vector
Particle::Particle(Vector3D position_new, double mass_new, double radius_new){
	//set new position to position vector input
	position = position_new;
	mass = mass_new;
	radius = pRadius;
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

void Particle::setPosition(Vector3D pos_new){
	position = pos_new;
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

/*
//apply newtonian gravity force on particle given a barnes hut tree of particles using a precision of theta
void Particle::applyNewtonianGravity(BHTreeNode* BHTree, double theta){
	//scales the force
	double gravitationalConstant = 0.01;
	//get vector from the particle to the CoM of node
	Vector3D posNode = BHTree->getCentreOfMass().copy();
	posNode.sub(position);
	//distance between node and CoM of node
	double distance = posNode.mag();
	//if node holds more than current particle
	if (distance > 2 * radius){
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
			double forceMag = gravitationalConstant * mass * BHTree->getMass() * pow(distance, -2.);//- 0.1 * pow(distance, -4.);
			//std::cout << "distance " << distance << " node mass " << BHTree->getMass() << " mag " << forceMag << "\n";
			//normalise vector between the particles and * by mag of force
			posNode.norm();
			posNode.mult(forceMag);
			//apply force to particle 
			applyForce(posNode);
		}
	}
}
*/

//applies the force on a particle due to the long range attraction and short range repulsion
//using octant tree with Barnes Hut approximation
void Particle::applyFlocking(BHTreeNode* BHTree, double theta){
	//get vector from the particle to the CoM of node
	Vector3D vecToNode = BHTree->getCentreOfMass().diff(position);
	//distance between node and CoM of node
	double distance = vecToNode.mag();
	//std::cout << "distance = " << distance << ", vecTo = (" << vecToNode.getX() << ", " << vecToNode.getY() << ", " << vecToNode.getZ() << ")\n";
	//std::cout << BHTree->getisExternal() << "\n";
	if (!(BHTree->getisExternal()) && ((BHTree->contains(position) || (BHTree->getDimensions() / distance > theta)))){
		//std::cout << (BHTree->getDimensions() / distance) << "\n";
		for (int i = 0; i < 8; i++){
			if (!((BHTree->getChild(i))->getIsEmpty())){
				applyFlocking(BHTree->getChild(i), theta);
			}
		}
	}
	else{
			//std::cout << "checking flocking\n";
			//std::cout << "angleBetween= " << velocity.angleBetween(vecToNode) << "\n";
		if (velocity.angleBetween(vecToNode) > forwardBlindAngle/2. && velocity.angleBetween(vecToNode) < backBlindAngle/2.){
			//std::cout << "flocking\n";

			double massNode = BHTree->getMass();

			//mag of force due to separation
			vecToNode.norm();

			// V =  k/x^2 - 1/x^4 where k is 1/r^2 so that equilibrium is point is r
			//-> F = 4x^-5 - 2kx^-3
			double proxMag = - 2 * pow(distance, -5) * pow(radius, -2) * pow(distance, -3);

			vecToNode.mult(proximityFactor * forceFactor * massNode * proxMag);
			applyForce(vecToNode);

			//mag of force due to direction
			Vector3D velNode = BHTree->getVelocitySum();

			//force scale as (sin(2*dTheta) ^ 2) / (r ^ 2)
			double dirMag = pow(sin(2 * velocity.angleBetween(velNode)), 2) * pow(distance, -3) * pow(radius, -2) ;
			velNode.mult(directionFactor * forceFactor * massNode * dirMag);

			applyForce(velNode);
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

void Particle::randomDeviation(){
	//create random angle between +- angle deviation
	double randTheta = randomNumberGaussian(0, pow(randomAngleDeviation,2));
	double randPhi = randomNumberGaussian(0, pow(randomAngleDeviation, 2));

	//std::cout << "dtheta= " << randTheta << ", dPhi= " << randPhi << "\n";

	//convert particle velocity into spherical polar coords and add the angle deviation
	double vR = velocity.mag();
	double vTheta = acos(velocity.getZ() / velocity.mag()) + randTheta;
	double vPhi = atan2(velocity.getY(), velocity.getX()) + randPhi;

	//create vector with angle deviation and subtract the velocity to get vector difference (effect of random)
	Vector3D velRand = Vector3D(sin(vTheta) * cos(vPhi), sin(vTheta) * sin(vPhi), cos(vTheta)).scalarProd(vR).diff(velocity);

	//update velocity
	velocity.add(velRand);
}

//updates particle position and motion given its attributes
void Particle::update(double timestep) {
	//std::cout <<  "velocity = " << velocity.mag() <<", a*dt = " <<acceleration.scalarProd(timestep).mag() << "\n";
	//velocity vector turns towards acceleration vector by dTheta/(theta1 - theta2) = |adt|/(|adt| + |v|)
	double dTheta = velocity.angleBetween(acceleration) * acceleration.scalarProd(timestep).mag()
		/ (velocity.mag() + acceleration.scalarProd(timestep).mag());

	//creates orthogonal vector to velocity in ~ direction of a to rotate velocity vector around
	if (velocity.angleBetween(acceleration) != 3.14159 && velocity.angleBetween(acceleration) != 0){
		Vector3D accTick = velocity.vectorProd(acceleration).vectorProd(velocity);
		accTick.norm();
		Vector3D unitVel = velocity.copy();
		unitVel.norm();

		//rotates vector by dTheta towards acceleration
		double velocityMagTemp = velocity.mag();
		velocity = unitVel.scalarProd(cos(dTheta)).sum(accTick.scalarProd(sin(dTheta)));
		//maintain original magnitude of velocity
		velocity.norm();
		velocity.mult(velocityMagTemp);
	}

	//add random deviation to velocity
	//randomDeviation();

	//limit velocity magnitude to velMag
	if (velocity.mag() > velMag){
		velocity.norm();
		velocity.mult(velMag);
	}

	//update position given velocity
	position.add(velocity.scalarProd(timestep));
	//set acceleration to 0
	acceleration.sub(acceleration);
}

//draws particle to image
void Particle::draw(Mat img){
	//maximum particle radius
	double maxRadius = 3;
	//create point struct of position
	Point loc(position.getX(), position.getY());
	//only draw if point is on the image
	if (loc.x > maxRadius && loc.x < img.size().width - maxRadius && 
		loc.y > maxRadius && loc.y < img.size().height - maxRadius){
		//scale radius with z axis to simulate depth
		double circleRadius = maxRadius * position.getZ()/img.size().width;

		//radius takes integer value so if less than 1 just set to 1
		if (circleRadius < 0.5){
			circleRadius = 0.5;
		}
		//if over max circle radius then set to be maximum
		if (circleRadius > maxRadius){
			circleRadius = maxRadius;
		}
		//draw circle

		double colourFactor = 255 * sin(position.getZ() / img.size().width * 3.14159);

		//filled
		circle(img, loc, 1, Scalar(colourFactor, 255, 255/3), -1, 8);

		//notfilled
		//circle(img, loc, 1, Scalar(150, colourFactor, 0), 1, 1);

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

