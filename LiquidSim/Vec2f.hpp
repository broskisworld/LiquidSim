//
//  Vec2f.hpp
//  LiquidSim
//
//  Created by Josh Bosley on 10/28/23.
//

#ifndef Vec2f_hpp
#define Vec2f_hpp

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using sf::Vector2;

class Vec2f : public Vector2<float> {
public:
    using Vector2::Vector2;
    Vec2f(const Vector2<float>& other) : Vector2<float>(other) {};
    float length();
};

#endif /* Vec2f_hpp */
