#include "ParticleSystem.h"
#include "BHTree.h"
#include<iostream>
#include<ctime>

//Problems with constructor and destructors. Memory leaks.


//world parameters
int initialNumberOfParticles = 2000;
int iterations;
int maxIterations = 10000000;
double worldDimensions = 1000;
double borderSize = 100;
double precisionFactor = 0.8;
double timestep = 1;

//particle parameters
double defaultMass = 1.;
double defaultRadius = 10;
double wFactor = 10;

//function to generate a random number between an upper and lower limit in divisions of range/n
double randomNumber(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

//vector to hold attractors
std::vector<Particle> attractors;

//function to use for mousecallback
static void callBackFunc(int event, int x, int y, int flags, void* param){
	if (event == EVENT_LBUTTONDOWN){
		Particle attractor(Vector3D(x, y, worldDimensions / 2), defaultMass * 10000000, defaultRadius);
		attractors.push_back(attractor);
	}
	if (event == EVENT_LBUTTONUP){
		attractors.pop_back();
	}
	if (event == EVENT_RBUTTONDOWN){
		Particle repeller(Vector3D(x, y, worldDimensions / 2), defaultMass * -10000000, defaultRadius);
		attractors.push_back(repeller);
	}
	if (event == EVENT_RBUTTONUP){
		attractors.pop_back();
	}
	if (event == CV_EVENT_MOUSEMOVE){
		if (attractors.size() == 1){
			attractors[0].setPosition(Vector3D(x, y, worldDimensions / 2));
		}
	}
}

VideoWriter outputVideo("Flocking.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, Size(worldDimensions, worldDimensions));

int main(){
	char particleSystemWindow[] = "Plot: Flocking Behaviour";
	Mat particleSystemImage = Mat::zeros(worldDimensions, worldDimensions, CV_8UC3);
	
	//initialise random number function with current time as seed
	srand(time(NULL));

	ParticleSystem pSystem;
	
	//initialise particles with random positions (no initial accelerations or velocities)
	
	for (int i = 0; i < initialNumberOfParticles; i++){
		double pos_x;
		double pos_y;
		double pos_z;

		//spherical distribution
		//*

		//decreasing density
		//double pDistance = randomNumber(worldDimensions / 2 - borderSize, 0, 1000);

		//uniform density
		double pDistance = pow( (randomNumber( pow((worldDimensions / 2 - borderSize), 3) , 0, 1000)) , 1.0/3.0);


		double pTheta = randomNumber(3.14159, 0, 1000);
		double pPhi = randomNumber(2 * 3.14159, 0, 1000);

		pos_x = pDistance * sin(pTheta) * cos(pPhi);
		pos_y = pDistance * sin(pTheta) * sin(pPhi);
		pos_z = pDistance * cos(pTheta);
		//*/

		//disc distribution
		/*
		double pDistance = pow( randomNumber( pow( (worldDimensions / 2 - borderSize), 2), 0, 1000), 0.5);
		pDistance = -(worldDimensions / 2 - borderSize) * log(1 - pDistance / (2*(worldDimensions / 2 - borderSize)));
		double pPhi = randomNumber(2* 3.14159, 0, 1000);
		double pZ = randomNumber((worldDimensions / 40)* pow(1.01, -pDistance), (-worldDimensions / 40) * pow(1.01, -pDistance), 1000);

		pos_x = pDistance * cos(pPhi);
		pos_y = pDistance * sin(pPhi);
		pos_z = pZ;

		// */

		//top down
		//*
		Vector3D pos(pos_x, pos_y, pos_z);
		Vector3D axis(0, 0, 1);
		// */

		//side on
		/*
		Vector3D pos(pos_y, pos_z, pos_x);
		Vector3D axis(0,1,0);
		//*/

		//velocity rotational as v = (r x axis vector) * angular velocity factor
		Vector3D vel = pos.vectorProd(axis);
		vel.norm();
		vel.mult(wFactor * pDistance );


		//move origin to centre of image
		pos.add(Vector3D(worldDimensions / 2, worldDimensions / 2, worldDimensions / 2));

		double randTheta = randomNumber(3.14159, 0, 1000);
		double randPhi = randomNumber(2 * 3.14159, 3.14159, 1000);

		Vector3D randomVel(sin(randTheta) * cos(randPhi), sin(randTheta) * sin(randPhi), cos(randTheta));
		randomVel.mult(15);

		Particle newParticle(pos, defaultMass, defaultRadius);
		newParticle.setVelocity(randomVel);
		//newParticle.setVelocity(vel);
		pSystem.addParticle(newParticle);

	}

	//create new BHTree pointer
	//BHTreeNode BHTree(Vector3D(0, 0, 0), worldDimensions);
	BHTreeNode *BHTreePointer = new BHTreeNode(Vector3D(0,0,0), worldDimensions);

	//main program loop
	for (iterations = 0; iterations < maxIterations; iterations++){
		//set image to be all white
		particleSystemImage = cv::Scalar(0, 0, 0);
		//adds particles to BHTree
		pSystem.fillBHTree(BHTreePointer);
		//applies attractor
		if (attractors.size() > 0){
			pSystem.applyAttractor(attractors[0]);
		}
		//update particle positions
		pSystem.update(BHTreePointer, precisionFactor, timestep);

		// wraps particles around space to contain in bounds
		pSystem.wrapOutOfBounds(Vector3D(0, 0, 0), Vector3D(worldDimensions, worldDimensions, worldDimensions));
		//repels any particles from the simulation bounds
		//pSystem.repelFromWalls(Vector3D(0, 0, 0), Vector3D(worldDimensions, worldDimensions, worldDimensions), 3*borderSize, 1.005);


		//draw particles onto image
		pSystem.draw(particleSystemImage);
		//set the callback function for any mouse event
		setMouseCallback(particleSystemWindow, callBackFunc, NULL);
		//show imge
		imshow(particleSystemWindow, particleSystemImage);
		//save frame to video 30 times per second
		double fps_double = 1 / timestep;
		int fps = std::round(fps_double);
		if (iterations % fps == 0){
			outputVideo.write(particleSystemImage);
		}
		//give time for imshow to display
		if (waitKey(5) >= 0){
			outputVideo.release();
			break;
		}
		//BHTreePointer->~BHTreeNode();
		BHTreePointer->wipe();
		//BHTree.~BHTreeNode();
		//delete BHTreePointer;
		//std::cout << iterations << "\n";
	}
	return 0;
}
