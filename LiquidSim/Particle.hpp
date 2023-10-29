//
//  Particle.hpp
//  LiquidSim
//
//  Created by Josh Bosley on 10/28/23.
//

#ifndef Particle_hpp
#define Particle_hpp

#include "Vec2f.hpp"

class Particle {
    
public:
    float x;
    float y;
    Vec2f position;
    Vec2f velocity;
    Vec2f acceleration;
    Vec2f appliedForce;
    Vec2f gradient;
    float density;
    float pressure;
    
    Particle();
    Particle(float _x, float _y);
};

#endif /* Particle_hpp */
