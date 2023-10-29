#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Particle.hpp"
#include "Vec2f.hpp"

using namespace std;
using sf::FloatRect;

/* PROTOTYPES */
int renderFrame();
void drawParticleGrid(int numParticles, float particleSpacing);
float smoothingFunction(Vec2f vector, float smoothingRadius);
void handleCollisions();

/* PHYSICS CONSTANTS */
const float COLLISION_DAMPER = 0.90f;
const float INFLUENCE_RADIUS = 60.0f;
const Vec2f GRAVITY_ACCELERATION = Vec2f(0.0, 10000.0f);
const float NEAR_PARTICLE_FORCE = 20000000.0f;
const float PARTICLE_MASS = 1.0f;

/* PARTICLE SETUP - 20 big */
const float PARTICLE_SIZE = 4.0f;
const float PARTICLE_RADIUS = PARTICLE_SIZE / 2.0f;
const int NUM_PARTICLES = 200;

/* SIM STATE */
vector<Particle> particles;
sf::Time deltaFrame;
sf::Rect<float> windowBox;

/* WINDOW */
sf::RenderWindow window(sf::VideoMode(800, 600), "Liquid Sim");
sf::Clock gameClock;
sf::Clock fpsClock;
sf::Clock frameClock;
int numFramesRenderedSinceLastSecond = 0;
int fps = 0;

/* MAIN ENTRY */
int main()
{
    int loopIteration = 0;
 
    cout << "Game starting...\n";
    
    drawParticleGrid(NUM_PARTICLES, PARTICLE_RADIUS * 2.0f);
    
    // Reset clocks
    gameClock.restart();
    fpsClock.restart();
    frameClock.restart();
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    return EXIT_SUCCESS;
                    break;
                    
                case sf::Event::Resized:
                    cout << "Resized to W" << window.getSize().x << " H" << window.getSize().y << endl;
                    break;
                    
                default:
                    break;
            }
        }
 
        // Clear screen
        window.clear(sf::Color(20, 20, 20));
        
        deltaFrame = frameClock.getElapsedTime();
        renderFrame();
        frameClock.restart();
        
        // update FPS
        if(fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fpsClock.restart();
            fps = numFramesRenderedSinceLastSecond;
            numFramesRenderedSinceLastSecond = 0;
            cout << "Game FPS: " << fps << endl;
        }
        numFramesRenderedSinceLastSecond++;
        
        // Update the window
        window.display();
        
        loopIteration++;
    }
    
    cout << "Window closed. Exiting...\n";
 
    return EXIT_SUCCESS;
}

/* FUNCTIONS */
int renderFrame() {
    windowBox = FloatRect(0.0f, 0.0f, window.getSize().x, window.getSize().y);
    
    for(int i = 0; i < particles.size(); i++) {
        particles[i].appliedForce = Vec2f(0.0, 0.0);
    }
    
    // Apply forces from nearby particles by adding acceleration
    for(int i = 0; i < particles.size(); i++) {
        for(int j = 0; j < particles.size(); j++) {
            if(i == j) continue;
            
            Vec2f difference = particles[i].position - particles[j].position;
            float dist = sqrt(pow(difference.x, 2) + pow(difference.y, 2));
            if(dist < INFLUENCE_RADIUS) {
//                Vec2f nearParticleForce = (difference / dist) * (float)(pow(((INFLUENCE_RADIUS - dist) / INFLUENCE_RADIUS), 3) * NEAR_PARTICLE_FORCE * PARTICLE_MASS);
                Vec2f nearParticleForce = (difference / dist) * (smoothingFunction(difference, INFLUENCE_RADIUS) * NEAR_PARTICLE_FORCE * PARTICLE_MASS);
                particles[i].appliedForce += nearParticleForce;
                particles[j].appliedForce += -nearParticleForce;
            }
        }
    }
    
    handleCollisions();
    
    // Draw particles
    for(int i = 0; i < particles.size(); i++) {
        sf::CircleShape shape(PARTICLE_RADIUS);
        float pressureGradient = clamp(particles[i].appliedForce.length() / NEAR_PARTICLE_FORCE, 0.0f, 1.0f);
        if(pressureGradient == 1.0f) {
            shape.setFillColor(sf::Color::White);
        } else {
            shape.setFillColor(sf::Color(50 + (int)(pressureGradient * 200.0f), 50, 50 + (int)((1.0f - pressureGradient) * 200.0f)));
        }
        shape.setPosition(particles[i].position.x - PARTICLE_RADIUS, particles[i].position.y - PARTICLE_RADIUS);
        window.draw(shape);
    }
    
    return 0;
}

void drawParticleGrid(int numParticles, float particleSpacing) {
    float windowCenterX = window.getSize().x / 2;
    float windowCenterY = window.getSize().y / 2;
    int numCols = (int) sqrt(numParticles);
    int numRows = ceil(numParticles / ((float) numCols));
    
//    cout << "Num cols " << numCols << ", num rows " << numRows << endl;
//    cout << "Grid width " << (int)((numCols * PARTICLE_SIZE) + ((numCols - 1) * particleSpacing)) << " grid height " << (int)((numRows * PARTICLE_SIZE) + ((numRows - 1) * particleSpacing)) << endl;
    
    for(int i = 0; i < numParticles; i++) {
        int particleCol = i % numCols;
        int particleRow = i / numCols;
        float particleX = windowCenterX - (((numCols * PARTICLE_SIZE) + ((numCols - 1) * particleSpacing)) * 0.5) + (particleCol * (PARTICLE_SIZE + particleSpacing));
        float particleY = windowCenterY - (((numRows * PARTICLE_SIZE) + ((numRows - 1) * particleSpacing)) * 0.5) + (particleRow * (PARTICLE_SIZE + particleSpacing));
        
        particles.push_back(Particle(particleX, particleY));
    }
}

float smoothingFunction(Vec2f vector, float smoothingRadius) {
//    return pow(((smoothingRadius - vector.length()) / smoothingRadius), 3);
    return (10 * M_1_PI) * pow(smoothingRadius - vector.length(), 3) * (1/pow(smoothingRadius, 5));
}

void handleCollisions() {
    for(int i = 0; i < particles.size(); i++) {
        // TODO: Maybe do some kind of line intersection to find the exact point it collides with a wall
        
        // Apply gravity & forces
        particles[i].acceleration = GRAVITY_ACCELERATION;
        particles[i].acceleration += particles[i].appliedForce;
        
        // Calculate velocity from acceleration
        particles[i].velocity += particles[i].acceleration * (float) deltaFrame.asSeconds();
        particles[i].position += particles[i].velocity * (float) deltaFrame.asSeconds();
        
        if(!windowBox.contains(particles[i].position)) {
            if(particles[i].position.x < 0.0f || particles[i].position.x > windowBox.width) {
                particles[i].position.x = clamp(particles[i].position.x, 0.0f, windowBox.width);
                particles[i].velocity.x *= (-1.0 * COLLISION_DAMPER);
            }
            if(particles[i].position.y < 0.0f || particles[i].position.y > windowBox.height) {
                particles[i].position.y = clamp(particles[i].position.y, 0.0f, windowBox.height);
                particles[i].velocity.y *= (-1.0 * COLLISION_DAMPER);
            }
        }
        
        // cout << "pos: " << particles[i].position.x << "," << particles[i].position.y << " v: " << particles[i].velocity.x << "," << particles[i].velocity.y << " a: " << particles[i].acceleration.x << "," << particles[i].acceleration.y << endl;
    }
}
