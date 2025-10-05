Sorting Algorithm Visualizer (C++ with SDL3)
This is a graphical application built in C++ using the SDL3 library to visualize the steps of various fundamental sorting algorithms in real-time.
It's a great tool for understanding how different algorithms compare and swap data.
Features
The application visualizes the following six sorting 
algorithms:
Selection 
SortInsertion 
SortBubble 
SortMerge 
SortQuick 
SortHeap 
SortArray 

elements are drawn as vertical bars,
with the bar height representing the value.
Colors are used to highlight elements currently involved in comparisons or swaps.
RequirementsTo build and run this project,
you need:A C++ compiler (supporting C++17 or later).The SDL3 Development Libraries installed on your system.

Note:
This project uses the console for menus and input and does not require the SDL_ttf library.
Build and Run Instructions:

1. Compiling the Code Since this is a single-file project (viz_sort_algo.cpp), you must link it against the SDL3 library when compiling.
Example Compilation Command (using g++):g++ viz_sort_algo.cpp -o sort_visualizer `sdl3-config --cflags --libs` -std=c++17

2.(You may need to adjust the command if sdl3-config is not available on your system, or if your SDL3 libraries are in non-standard paths.)

3. ExecutionRun the compiled executable from your terminal:
./sort_visualizer
The console will display an introductory message and a menu of controls before the visualization window opens.
ControlsUse the following keys while the visualization window is active:KeyAction0Generate a new, randomized list of elements.

1Start Selection Sort.
2Start Insertion Sort.
3Start Bubble Sort.
4Start Merge Sort.
5Start Quick Sort.
6Start Heap Sort.
SpacebarToggle Pause or Resume the simulation.
Up ArrowIncrease the visualization speed (decrease the delay).
Down ArrowDecrease the visualization speed (increase the delay).
Q Quit the visualization window and return to the main console menu.
-1 (in console)Quit the entire program.

Important Note: 
The size of the list is fixed at 120 elements. 
Delays have been added to faster algorithms 
(like Merge and Quick Sort) to ensure their steps can be properly visualized.
