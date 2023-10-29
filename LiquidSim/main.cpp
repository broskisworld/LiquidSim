#include <iostream>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Particle.hpp"

using namespace std;

/* PROTOTYPES */
int renderFrame(sf::Time deltaFrame);
void drawParticleGrid(int numParticles, float particleSpacing);

/* CONFIG */
const float PARTICLE_SIZE = 5.0f;
const int NUM_PARTICLES = 10;

/* SIM STATE */
vector<Particle> particles;

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
        
        renderFrame(frameClock.getElapsedTime());
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
int renderFrame(sf::Time deltaFrame) {
    sf::CircleShape shape(10.f);
    
    shape.setFillColor(sf::Color(100, 100, 200));
    
    window.draw(shape);
    
    
    
    return 0;
}

void drawParticleGrid(int numParticles, float particleSpacing) {
    float windowCenterX = window.getSize().x / 2;
    float windowCenterY = window.getSize().y / 2;
    int numCols = ((int) sqrt(numParticles)) + 1;
    int numRows = ceil(numParticles / ((float) numCols));
    
    for(int i = 0; i < numParticles; i++) {
        int particleCol = i % numCols;
        int particleRow = i / numCols;
        float particleX = windowCenterX - ((numCols * PARTICLE_SIZE) + ((numCols - 1) * particleSpacing)) + (particleCol * (PARTICLE_SIZE + particleSpacing));
        float particleY = windowCenterY - ((numRows * PARTICLE_SIZE) + ((numRows - 1) * particleSpacing)) + (particleRow * (PARTICLE_SIZE + particleSpacing));
        
        particles.push_back(Particle(particleX, particleY));
    }
}


