//
//  Vec2f.cpp
//  LiquidSim
//
//  Created by Josh Bosley on 10/28/23.
//

#include "Vec2f.hpp"

float Vec2f::length() {
    return sqrt((x * x) + (y * y));
}

Vec2f Vec2f::normalized() {
    return Vec2f(*this) / sqrt((x * x) + (y * y));
}
