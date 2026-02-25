# PixelFlow
## Principle of Programming - Raylib Particle Simulation

## Project Overview

## Project Overview

PixelFlow is an interactive sandbox-style particle simulation developed in C using the Raylib graphics library. In this game, every particle is represented as a single pixel on a two-dimensional grid. Each pixel acts as an independent unit with its own state and behaviour, allowing realistic material interactions to emerge from simple programmed rules.

Different particle types follow specific physical behaviours such as gravity, fluid flow, combustion, and chemical reactions. Through these rule-based interactions, complex and dynamic patterns naturally emerge within the simulation.

This project was developed as part of the Principle of Programming module to demonstrate structured programming, modular design, state management, abstraction, and real-time simulation techniques.

## AI and External Code Acknowledgement

All use of AI tools and any external assistance is documented in [AI_USAGE.md](AI_USAGE.md), in accordance with the module assessment requirements.

## Build Instructions

The project is compiled to WebAssembly using the POP build environment.

To build the application, the following command is executed:

/opt/pop/bin/build-wasm.sh src/main.c src/grid.c src/particle.c src/physics.c src/render.c src/ui.c

This command generates an out directory containing:

The compiled .wasm file

Supporting JavaScript files

An index.html file required to run the simulation in a web browser

## Initial Setup (First-Time Execution)

Before running a POP WebAssembly application for the first time, a port must be allocated:

/opt/pop/bin/allocate_port.sh

After allocation, a new terminal session may be required for the environment variable to update correctly.

The allocated port number can be verified using:

echo $MY_PORT

This should return a five-digit port number.

## Running the Application

To start the application, the following command is executed:

/opt/pop/bin/run-wasm.sh

This launches a web server that serves the contents of the out directory on the allocated port. The server forwards from the POP remote environment to the local machine.

The simulation can then be accessed in a browser via:

localhost:XXXXX

where XXXXX corresponds to the allocated port number.

## Simulation Controls

The following controls are implemented within the application:

Key	Function
1	Sand
2	Water
3	Stone
4	Fire
5	Smoke
6	Wood
7	Acid
0	Eraser particle
[ / ]	Decrease / Increase brush size
Space	Pause / Resume simulation
C	Clear grid
Left Mouse Button	Paint particles
Right Mouse Button	Erase particles

## Technical Structure

The project follows a modular architecture, separating responsibilities across multiple source files:

main.c - Program entry point and main loop

grid.c - Grid data structure and management

particle.c - Particle definitions and state handling

physics.c - Simulation logic and behaviour rules

render.c - Rendering logic using Raylib

ui.c - User input and interface handling

This modular design improves readability, maintainability, and ease of debugging, which aligns with the learning outcomes of the module.
