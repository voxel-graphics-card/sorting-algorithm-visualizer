#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

// Using std:: namespace for common elements to avoid repetitive std::
using std::cout;
using std::getline;
using std::string;
using std::swap;
using std::vector;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;

const int arrSize = 120;
const int rectSize = 8;

// Using std::vector instead of raw arrays for better memory management
vector<int> arr(arrSize);
vector<int> Barr(arrSize);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool complete = false;
bool isSorting = false;
int sortDelay = 1;
bool isPaused = false;

// Struct to hold visualization colors
struct Colors
{
	SDL_Color BASE_COLOR = {170, 183, 184, 0};
	SDL_Color HIGHLIGHT_COLOR = {165, 105, 189, 0};
	SDL_Color SWAP_COLOR = {100, 180, 100, 0};
	SDL_Color SORTED_COLOR = {100, 180, 100, 0};
	SDL_Color BACKGROUND_COLOR = {0, 0, 0, 0};
};

Colors colors;

// Function prototypes - Declare functions before their first use
void visualize(int x = -1, int y = -1, int z = -1);
void inplaceHeapSort(vector<int> &input, int n);
int partition_array(vector<int> &a, int si, int ei);
void quickSort(vector<int> &a, int si, int ei);
void merge2SortedArrays(vector<int> &a, int si, int ei);
void mergeSort(vector<int> &a, int si, int ei);
void bubbleSort();
void insertionSort();
void selectionSort();
void loadArr();
void randomizeAndSaveArray();
void execute();
bool controls();
void intro();
bool handleInputDuringSort();

bool init()
{
	// Initialize SDL once for the entire program lifetime
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
		success = false;
	}
	else
	{
		window = SDL_CreateWindow("Sorting Visualizer", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (window == NULL)
		{
			cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, NULL);
			if (renderer == NULL)
			{
				cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
				success = false;
			}
		}
	}

	return success;
}

void close()
{
	// Clean up SDL resources once at the end of the program
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

void visualize(int x, int y, int z)
{
	// NO EVENT POLLING HERE. Events are handled in the main execute() loop and sorting functions.

	// Clear the renderer with a black background
	SDL_SetRenderDrawColor(renderer, colors.BACKGROUND_COLOR.r, colors.BACKGROUND_COLOR.g, colors.BACKGROUND_COLOR.b, colors.BACKGROUND_COLOR.a);
	SDL_RenderClear(renderer);

	// Render each bar based on the array values
	for (int j = 0; j < arr.size(); ++j)
	{
		float xoffset = 20.0f;
		// SDL_FRect for float precision and cleaner bar drawing
		SDL_FRect rect = {(float)j * rectSize + xoffset, (float)SCREEN_HEIGHT - arr[j], (float)rectSize, (float)arr[j]};

		if (complete)
		{
			SDL_SetRenderDrawColor(renderer, colors.SORTED_COLOR.r, colors.SORTED_COLOR.g, colors.SORTED_COLOR.b, colors.SORTED_COLOR.a);
			SDL_RenderRect(renderer, &rect);
		}
		else if (j == x || j == z)
		{
			SDL_SetRenderDrawColor(renderer, colors.SWAP_COLOR.r, colors.SWAP_COLOR.g, colors.SWAP_COLOR.b, colors.SWAP_COLOR.a);
			SDL_RenderFillRect(renderer, &rect);
		}
		else if (j == y)
		{
			SDL_SetRenderDrawColor(renderer, colors.HIGHLIGHT_COLOR.r, colors.HIGHLIGHT_COLOR.g, colors.HIGHLIGHT_COLOR.b, colors.HIGHLIGHT_COLOR.a);
			SDL_RenderFillRect(renderer, &rect);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, colors.BASE_COLOR.r, colors.BASE_COLOR.g, colors.BASE_COLOR.b, colors.BASE_COLOR.a);
			SDL_RenderRect(renderer, &rect);
		}
	}
	// Update the screen with the new drawing
	SDL_RenderPresent(renderer);
}

// Helper function to check for quit events during a sort
bool handleInputDuringSort()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
		{
			return true;
		}
		// Handle pause/unpause
		if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE)
		{
			isPaused = !isPaused;
			//(isPaused)? (cout << "PAUSED\n") : (cout << "RESUMED\n");//does this cheap trick even work?(ANS. somewhat just paused prints)
			if(isPaused)
			{
				cout<<"PAUSED\n";
			}
			else if(!isPaused)
			{
				cout<<"RESUMED\n";
			}

		}
		// Handle speed control
		if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_UP)
		{
			sortDelay = (sortDelay > 1) ? sortDelay - 1 : 1;
			cout << "\nSpeed increased. Current delay: " << sortDelay << "ms.\n";
		}
		if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_DOWN)
		{
			sortDelay += 1;
			cout << "\nSpeed increased. Current delay: " << sortDelay << "ms.\n";
		}
	}
	// Keep visualization paused until spacebar is pressed
	while (isPaused)
	{
		SDL_Delay(100);
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE)
			{
				isPaused = false;
			}
			if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
			{
				return true;
			}
		}
	}
	return false;
}

void inplaceHeapSort(vector<int> &input, int n)
{
	for (int i = 1; i < n; i++)
	{
		if (handleInputDuringSort())
			return;
		int childIndex = i;
		int parentIndex = (childIndex - 1) / 2;

		while (childIndex > 0)
		{
			if (input[childIndex] > input[parentIndex])
			{
				swap(input[parentIndex], input[childIndex]);
			}
			else
			{
				break;
			}

			visualize(parentIndex, childIndex);
			SDL_Delay(sortDelay);

			childIndex = parentIndex;
			parentIndex = (childIndex - 1) / 2;
		}
	}

	for (int heapLast = n - 1; heapLast >= 0; heapLast--)
	{
		if (handleInputDuringSort())
			return;
		swap(input[0], input[heapLast]);

		int parentIndex = 0;
		int leftChildIndex = 2 * parentIndex + 1;
		int rightChildIndex = 2 * parentIndex + 2;

		while (leftChildIndex < heapLast)
		{
			int maxIndex = parentIndex;

			if (input[leftChildIndex] > input[maxIndex])
			{
				maxIndex = leftChildIndex;
			}
			if (rightChildIndex < heapLast && input[rightChildIndex] > input[maxIndex])
			{
				maxIndex = rightChildIndex;
			}
			if (maxIndex == parentIndex)
			{
				break;
			}

			swap(input[parentIndex], input[maxIndex]);

			visualize(maxIndex, parentIndex, heapLast);
			SDL_Delay(sortDelay);

			parentIndex = maxIndex;
			leftChildIndex = 2 * parentIndex + 1;
			rightChildIndex = 2 * parentIndex + 2;
		}
	}
}

int partition_array(vector<int> &a, int si, int ei)
{
	int count_small = 0;

	for (int i = (si + 1); i <= ei; i++)
	{
		if (a[i] <= a[si])
		{
			count_small++;
		}
	}
	int c = si + count_small;
	swap(a[c], a[si]);
	visualize(c, si);

	int i = si, j = ei;

	while (i < c && j > c)
	{
		if (a[i] <= a[c])
		{
			i++;
		}
		else if (a[j] > a[c])
		{
			j--;
		}
		else
		{
			swap(a[j], a[i]);

			visualize(i, j);
			SDL_Delay(sortDelay);

			i++;
			j--;
		}
		if (handleInputDuringSort())
			return -1; // Added check for early exit
	}
	return c;
}

void quickSort(vector<int> &a, int si, int ei)
{
	if (si >= ei)
	{
		return;
	}
	if (handleInputDuringSort())
		return;

	int c = partition_array(a, si, ei);
	if (c == -1)
		return; // Check if partition returned early due to quit event
	quickSort(a, si, c - 1);
	quickSort(a, c + 1, ei);
}

void merge2SortedArrays(vector<int> &a, int si, int ei)
{
	vector<int> output((ei - si) + 1);
	int mid = (si + ei) / 2;
	int i = si, j = mid + 1, k = 0;
	while (i <= mid && j <= ei)
	{
		if (handleInputDuringSort())
			return;
		if (a[i] <= a[j])
		{
			output[k] = a[i];
			visualize(i, j);
			i++;
			k++;
		}
		else
		{
			output[k] = a[j];
			visualize(i, j);
			j++;
			k++;
		}
	}
	while (i <= mid)
	{
		if (handleInputDuringSort())
			return;
		output[k] = a[i];
		visualize(-1, i);
		i++;
		k++;
	}
	while (j <= ei)
	{
		if (handleInputDuringSort())
			return;
		output[k] = a[j];
		visualize(-1, j);
		j++;
		k++;
	}
	int x = 0;
	for (int l = si; l <= ei; l++)
	{
		if (handleInputDuringSort())
			return;
		a[l] = output[x];
		visualize(l);
		SDL_Delay(sortDelay);
		x++;
	}
}

void mergeSort(vector<int> &a, int si, int ei)
{
	if (si >= ei)
	{
		return;
	}
	if (handleInputDuringSort())
		return;

	int mid = (si + ei) / 2;

	mergeSort(a, si, mid);
	mergeSort(a, mid + 1, ei);

	merge2SortedArrays(a, si, ei);
	SDL_Delay(sortDelay);
}

void bubbleSort()
{
	for (int i = 0; i < arrSize - 1; i++)
	{
		if (handleInputDuringSort())
			return;
		for (int j = 0; j < arrSize - 1 - i; j++)
		{
			if (arr[j + 1] < arr[j])
			{
				swap(arr[j], arr[j + 1]);

				visualize(j + 1, j, arrSize - i);
			}
			SDL_Delay(sortDelay);
		}
	}
}

void insertionSort()
{
	for (int i = 1; i < arrSize; i++)
	{
		if (handleInputDuringSort())
			return;
		int j = i - 1;
		int temp = arr[i];
		while (j >= 0 && arr[j] > temp)
		{
			arr[j + 1] = arr[j];
			j--;

			visualize(i, j + 1);
			SDL_Delay(sortDelay);
		}
		arr[j + 1] = temp;
	}
}

void selectionSort()
{
	int minIndex;
	for (int i = 0; i < arrSize - 1; i++)
	{
		if (handleInputDuringSort())
			return;
		minIndex = i;
		for (int j = i + 1; j < arrSize; j++)
		{
			if (arr[j] < arr[minIndex])
			{
				minIndex = j;
				visualize(i, minIndex);
			}
			SDL_Delay(sortDelay);
		}
		swap(arr[i], arr[minIndex]);
	}
}

void loadArr()
{
	arr = Barr;
}

void randomizeAndSaveArray()
{
	// Use std::mt19937 and std::uniform_int_distribution for better randomness
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, SCREEN_HEIGHT);

	for (int i = 0; i < arrSize; i++)
	{
		Barr[i] = distrib(gen);
	}
}

void execute()
{
	randomizeAndSaveArray();
	loadArr();

	SDL_Event e;
	SDL_zero(e);
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_EVENT_QUIT)
			{
				quit = true;
				complete = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN)
			{
				// Pause and speed controls are now active at all times in this loop
				if (e.key.key == SDLK_SPACE)
				{
					isPaused = !isPaused;
					cout << (isPaused ? "\nPAUSED." : "\nRESUMED.") << '\n';
				}
				if (e.key.key == SDLK_UP)
				{
					sortDelay = (sortDelay > 1) ? sortDelay - 1 : 1;
					cout << "\nSpeed increased. Current delay: " << sortDelay << "ms.\n";
				}
				if (e.key.key == SDLK_DOWN)
				{
					sortDelay += 1;
					cout << "\nSpeed decreased. Current delay: " << sortDelay << "ms.\n";
				}

				// Only allow new sorts if one isn't already running
				if (isSorting)
				{
					continue;
				}

				switch (e.key.key)
				{
				case (SDLK_Q):
					quit = true;
					complete = false;
					cout << "\nEXITING SORTING VISUALIZER.\n";
					break;
				case (SDLK_0):
					randomizeAndSaveArray();
					complete = false;
					loadArr();
					cout << "\nNEW RANDOM LIST GENERATED.\n";
					break;
				case (SDLK_1):
					isSorting = true;
					loadArr();
					cout << "\nSELECTION SORT STARTED.\n";
					selectionSort();
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nSELECTION SORT COMPLETE.\n";
					break;
				case (SDLK_2):
					isSorting = true;
					loadArr();
					cout << "\nINSERTION SORT STARTED.\n";
					insertionSort();
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nINSERTION SORT COMPLETE.\n";
					break;
				case (SDLK_3):
					isSorting = true;
					loadArr();
					cout << "\nBUBBLE SORT STARTED.\n";
					bubbleSort();
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nBUBBLE SORT COMPLETE.\n";
					break;
				case (SDLK_4):
					isSorting = true;
					loadArr();
					cout << "\nMERGE SORT STARTED.\n";
					mergeSort(arr, 0, arrSize - 1);
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nMERGE SORT COMPLETE.\n";
					break;
				case (SDLK_5):
					isSorting = true;
					loadArr();
					cout << "\nQUICK SORT STARTED.\n";
					quickSort(arr, 0, arrSize - 1);
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nQUICK SORT COMPLETE.\n";
					break;
				case (SDLK_6):
					isSorting = true;
					loadArr();
					cout << "\nHEAP SORT STARTED.\n";
					inplaceHeapSort(arr, arrSize);
					isSorting = false;
					if (!handleInputDuringSort())
					{
						complete = true;
					}
					cout << "\nHEAP SORT COMPLETE.\n";
					break;
				}
			}
		}
		visualize();
	}
}

bool controls()
{
	cout << "WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
		 << "Available Controls inside Sorting Visualizer:-\n"
		 << "    Use 0 to Generate a different randomized list.\n"
		 << "    Use 1 to start Selection Sort Algorithm.\n"
		 << "    Use 2 to start Insertion Sort Algorithm.\n"
		 << "    Use 3 to start Bubble Sort Algorithm.\n"
		 << "    Use 4 to start Merge Sort Algorithm.\n"
		 << "    Use 5 to start Quick Sort Algorithm.\n"
		 << "    Use 6 to start Heap Sort Algorithm.\n"
		 << "    Use Up and Down arrow keys to change speed.\n"
		 << "    Use Spacebar to pause/resume the visualization.\n"
		 << "    Use q to exit out of Sorting Visualizer\n\n"
		 << "PRESS ENTER TO START SORTING VISUALIZER...\n\n"
		 << "Or type -1 and press ENTER to quit the program: ";

	string s;
	getline(std::cin, s);
	if (s == "-1")
	{
		return false;
	}
	return true;
}

void intro()
{
	cout << "\t\t\t\t ==============================Sorting Visualizer============================== \n\n"
		 << "Visualization of different sorting algorithms in C++ with SDL3 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
		 << "In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
		 << "The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort and Heap Sort.\n"
		 << "The list size is fixed to 130 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithms like Merge Sort, etc. have been delayed so that they could be properly visualized.\n\n"
		 << "Press ENTER to show controls...";

	string s;
	getline(std::cin, s);
}

int main(int argc, char *args[])
{
	intro();

	// Initialize SDL once
	if (!init())
	{
		cout << "\nERROR: SDL Initialization Failed. Exiting Program.\n";
		return 1;
	}

	// Main program loop to handle the console menu
	while (true)
	{
		cout << '\n';
		if (controls())
		{
			// The execute() function now handles the SDL window's event loop
			execute();
		}
		else
		{
			// Break from the main loop if the user chooses to quit
			break;
		}
	}

	// Clean up SDL once
	close();

	cout << "\nEXITING PROGRAM.\n";
	return 0;
}
