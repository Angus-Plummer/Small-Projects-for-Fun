import math;
import random;
from matplotlib import pyplot as plt;
import matplotlib.animation as animation;

class Vector2:
    #class for 2D vectors
    def __init__(self, x, y):
        #vector has callable x and y components
        self.x = x;
        self.y = y;
    def getMagnitude(self):
        #returns the magnitude of the vector
        magnitude = math.pow( (math.pow(self.x,2) + math.pow(self.y,2) ), 0.5);
        return magnitude;
    def addVector(self, other_vector):
        #adds a vector to the vector
        self.x += other_vector.x;
        self.y += other_vector.y;
    def subVector(self, other_vector):
        #subtracts a vector from the vector
        self.x -= other_vector.x;
        self.y -= other_vector.y;
    def scalarMult(self, scalar):
        #multiplies vector by a scalar
        self.x *= scalar;
        self.y *= scalar;
    def dotProd(self, other_vector):
        #returns the dot product of the vector and another vector
        dotProd = self.x * other_vector.x + self.y + other_vector.y;
        return dotProd;
    def normalise(self):
        #normalises the vector to unit length
        magnitude = self.getMagnitude();
        self.x /= magnitude;
        self.y /= magnitude;
    def getAngle(self):
        #returns angle of vector in degrees
        angle = math.degrees(math.atan2(self.y, self.x))
        return angle;
    def copy(self):
        #returns a copy of the vector
        return Vector2(self.x,self.y)
    def print(self):
        #prints out the x and y components as a string
        print("%fi + %fj" %(self.x, self.y));

    
        
class Particle:
    #class for describing particle properties
    def __init__(self, position_vector, velocity, mass, charge):
        #particle position vector, mass and charge defined
        self.position_vector = position_vector;
        self.velocity = velocity;
        self.mass = mass;
        self.charge = charge;
        
    def setPositionVector(self, new_position_vector):
        #set the position vector to a new input
        self.position_vector = new_position_vector;
    
    def setVelocity(self, new_velocity):
        #sets the velocity to new velocity vector
        self.velocity = new_velocity;
        
    def vectorTo(self, target_position_vector):
        #returns the vector from the particle's position vector to another position vector
         target_position_vector_copy = target_position_vector.copy()
         target_position_vector_copy.subVector(self.position_vector); 
         return (target_position_vector_copy);
        
def newtonsLaw(focus_particle, particle_array):
    #actually uses 1/r instead of 1/r^2
    #gives force per unit mass on a particle from all other particles
    #modifiable gravitational constant
    gravitational_constant = 10000;
    
    vector = Vector2(0,0); #create null vector
    
    #iterate over all particles with superposition
    for target_particle in particle_array:
        
        r = focus_particle.vectorTo(target_particle.position_vector);
        
        #if you are on the particle then no force to avoid infinities;
        if r.getMagnitude() > 0:
            #calculate magnitude of force
            force_magnitude = gravitational_constant * target_particle.mass * math.pow(r.getMagnitude(),-1);
            #converts r into unit vector then multiply by force magnitude
            r.normalise();            
            r.scalarMult(force_magnitude);
            #add r to vector and re
            vector.addVector(r);
            
    return vector;
    
number_of_particles =30;
width = 1000;
height = 1000;

particle_mass = 1;
particle_charge = 0;
particle_max_initial_velocity = 0;
fuse_distance = 20;


total_run_time = 100;
t = 0; #time passed
dt = 1/300; #timestep



#set up plot
fig = plt.figure();
fig.subplots_adjust(left=0, right=1, bottom=0, top=1)
ax = fig.add_subplot(111, aspect='equal', autoscale_on=False, xlim=(0, width), ylim=(0, height))
    
particle_array = []; #creates empty particle array
particles, = ax.plot([], [], 'bo', ms=6);    
particles_x = [];
particles_y = [];
    
for i in range(number_of_particles):
    #creates the specified number of particles adds them to the array of particles
       # particles.set_data([], []);
        #particle position generated randomly
        new_particle_x = width/3 + random.random() * width/3;
        new_particle_y = height/3 + random.random() * height/3;
        new_particle_position_vector = Vector2(new_particle_x, new_particle_y);
        #initial particle velocity
        random_angle = random.random() * 2 * math.pi;
        new_particle_velocity = Vector2(particle_max_initial_velocity * math.cos(random_angle) ,
                                        particle_max_initial_velocity * math.sin(random_angle));
        #particle mass and charge as entered above
        new_particle_mass = particle_mass;
        new_particle_charge = particle_charge;
        #create the particle with the given attributes and add to particle array
        new_particle = Particle(new_particle_position_vector, new_particle_velocity, new_particle_mass, new_particle_charge);
        particle_array.append(new_particle);
        particles_x.append(new_particle_x);
        particles_y.append(new_particle_y);

        
def updateParticles(particle_array, particles_x, particles_y, dt):
    
    particles_x.clear();
    particles_y.clear();    
    
    particle_array_copy = [];
    
    for particle in particle_array:
        particle_array_copy.append(particle);
        
    for focus_particle in particle_array:
        for target_particle in particle_array:
            r = focus_particle.vectorTo(target_particle.position_vector);
            if r.getMagnitude() > 0 and r.getMagnitude() < fuse_distance:
                #fuse particles if within fuse distance
            
                r_mass_fraction = r.copy();
                r_mass_fraction.scalarMult(target_particle.mass/ (target_particle.mass + focus_particle.mass));
                
            
                focus_particle.position_vector.addVector(r_mass_fraction);
                
                
                target_particle_momentum = target_particle.velocity.copy();
                target_particle_momentum.scalarMult(target_particle.mass);
                
                focus_particle_momentum = focus_particle.velocity.copy();
                focus_particle_momentum.scalarMult(focus_particle.mass);
                
                focus_particle_momentum.addVector(target_particle_momentum);
                focus_particle_momentum.scalarMult(1/(focus_particle.mass + target_particle.mass));
                
                focus_particle.velocty = focus_particle_momentum;
                focus_particle.mass += target_particle.mass;
                
                particle_array.remove(target_particle);

    for particle in particle_array:
       # particle.position_vector.print();
        old_velocity = particle.velocity.copy();
        #calculate the resultant force on the paticle
        force_per_mass = newtonsLaw(particle, particle_array_copy);
        #update the particle velocity
        force_per_mass.scalarMult(particle.mass * dt);
        particle.velocity.addVector(force_per_mass);
        #update the particle position using velocity
        old_velocity.scalarMult(dt);
        particle.position_vector.addVector(old_velocity);
        
        particles_x.append(particle.position_vector.x);
        particles_y.append(particle.position_vector.y);
        
updateParticles(particle_array, particles_x, particles_y, dt);
updateParticles(particle_array, particles_x, particles_y, dt);
    
def animate(i):
    global dt, ax, fig, particle_array, particles_x, particles_y;    

    if i > 0:
        updateParticles(particle_array, particles_x, particles_y, dt);        
        particles.set_data(particles_x, particles_y);
    return particles,;
    
ani = animation.FuncAnimation(fig, animate, interval=1, blit=True);

plt.show();