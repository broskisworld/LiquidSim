//
//  Particle.cpp
//  LiquidSim
//
//  Created by Josh Bosley on 10/28/23.
//

#include "Particle.hpp"

Particle::Particle() {
    Particle(0.0, 0.0);
}
    
Particle::Particle(float _x, float _y) {
    position.x = _x;
    position.y = _y;
}
