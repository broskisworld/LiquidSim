Just a fun afternoon project playing around with some liquid simulation concepts! Based on Smoothed Particle Hydrodynamics rather than grid based simulation.

What it does now:
- Draws some arbitrary number of liquid particles based on config
- Particles calculate a local density for themselves using a cubic kernel function for "smearing" to estimate true density based on nearby particles
- For visualization, a grid of density is shown as well using the same method
- Calculated the necessary derivative and wrote the code for, but have not fully implemented, a gradient of the density kernel function so I can set an "ideal pressure" for fluids and implement more accurate incompressability
- Particles push away from eachother in a near-particle force sort of way

Future goals:
- Implement ideal pressure physics / incompressability using the gradient function
- Improve the config values to make it more interactible
- Add interaction with the mouse
- Detect the window moving around and maybe allow shaking the window to shake the fluid particles inside
- Draw blob-like water with smooth edges

Fluid dynamics: pressure kernel functions (Desmos)
https://www.desmos.com/calculator/t2pijc0py8
