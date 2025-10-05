Features

The application visualizes the following six classic sorting algorithms:

    Selection Sort

    Insertion Sort

    Bubble Sort

    Merge Sort

    Quick Sort

    Heap Sort

Array elements are drawn as vertical bars, with the bar height representing their value. Specific colors are used to highlight elements currently being compared, swapped, or finalized.

Requirements

To build and run this project, you need:

    A C++ compiler (supporting C++17 or later).

    The SDL3 Development Libraries installed on your system.

Note: This project uses standard console input/output for menus and controls, meaning it does not require the SDL_ttf library.

Build and Run Instructions

1. Compiling the Code

Since this is a single-file project (viz_sort_algo.cpp), you must link it against the SDL3 library during compilation.

Example Compilation Command (using g++):
Bash

g++ viz_sort_algo.cpp -o sort_visualizer `sdl3-config --cflags --libs` -std=c++17

(You may need to adjust the command if sdl3-config is not available or if your SDL3 libraries are in non-standard locations.)

2. Execution

Run the compiled executable from your terminal. The application will first display a welcome message and control menu in the console before opening the graphical visualization window.
Bash

./sort_visualizer

User Controls

Interaction is handled via specific key presses while the visualization window is active:
Key	Algorithm / Action
0	Generate a new, randomized list of elements.
1 - 6	Start the corresponding sorting algorithm (e.g., 1 for Selection Sort, 6 for Heap Sort).
Spacebar	Toggle Pause or Resume the simulation.
Up Arrow	Increase the visualization speed (decrease the delay).
Down Arrow	Decrease the visualization speed (increase the delay).
Q	Quit the SDL window and return to the main console menu.
-1 (in console)	Quit the entire program.

Important Note: The list size is fixed at 120 elements for optimal display on the screen. Delays are artificially added to faster algorithms (like Merge Sort and Quick Sort) so that their complex steps can be properly visualized in real-time.
