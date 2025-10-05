#Sorting Algorithm Visualizer (C++ / SDL3)

This is a high-performance C++ application that uses the core SDL3 library to provide real-time, step-by-step graphical visualization of fundamental sorting algorithms. It's a powerful educational tool for anyone looking to truly understand the mechanics and efficiency of these algorithms.

#Project Overview

Detail	Description
Core Language	C++17
Graphics Library	SDL3 (Simple DirectMedia Layer)
Interaction	Console-driven menu with in-window controls
Key Function	Renders array elements as bars, using color to highlight comparisons and swaps.
Array Size	Fixed at 120 elements.

Algorithms Included

The application features visualizations for six essential sorting methods:

    Selection Sort

    Insertion Sort

    Bubble Sort

    Merge Sort

    Quick Sort

    Heap Sort

#Build & Requirements

Prerequisites

To successfully compile and run this program, you must have the following dependencies installed:

    A C++ compiler supporting C++17 or newer (e.g., GCC, Clang, MSVC).

    The SDL3 Development Libraries.

#Compilation Instructions

Since this is a single-file project (viz_sort_algo.cpp), you must link directly against the SDL3 library.

Typical Compilation Command (Linux/macOS using g++):
Bash

g++ viz_sort_algo.cpp -o sort_visualizer `sdl3-config --cflags --libs` -std=c++17

Execution

Run the compiled executable from your terminal. The program starts with a console menu for selection and controls the visualization window from there.
Bash

./sort_visualizer

🎮 Controls

The application uses intuitive key bindings to manage the simulation flow while the visualization window is active:
Key	Functionality	Description
0	New Array	Generates a new, fully randomized list of elements.
1-6	Start Sort	Initiates the corresponding sorting algorithm.
Spacebar	Pause/Resume	Toggles the state of the simulation.
Up Arrow	Speed Up	Decreases the delay between steps, accelerating the sort.
Down Arrow	Slow Down	Increases the delay, allowing for closer observation.
Q	Quit Window	Closes the SDL graphics window, returning you to the main console menu.

Performance Note: Faster algorithms (like Merge Sort and Quick Sort) have an artificial delay built-in so that their steps remain observable by the human eye.
