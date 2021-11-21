#include <SDL2/SDL.h>
#include <iostream>
#include <limits>
#include <time.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;

const int arrSize = 130;
const int rectSize = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {i, 0, rectSize, arr[j]};
        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 170, 110, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 170, 110, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void merge2SortedArrays(int a[], int start, int end)
{
    int size_output = (end - start) + 1;
    int *out = new int[size_output];

    int middle = (start + end) / 2;
    int i = start, j = middle + 1, k = 0;
    while (i <= middle && j <= end)
    {
        if (a[i] <= a[j])
        {
            out[k] = a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            out[k] = a[j];
            visualize(i, j);
            j++;
            k++;
        }
    }
    while (i <= middle)
    {
        out[k] = a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while (j <= end)
    {
        out[k] = a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x = 0;
    for (int l = start; l <= end; l++)
    {
        a[l] = out[x];
        visualize(l);
        SDL_Delay(20);
        x++;
    }
    delete[] out;
}

void mergeSort(int a[], int start, int end)
{
    if (start >= end)
    {
        return;
    }
    int middle = (start + end) / 2;

    mergeSort(a, start, middle);
    mergeSort(a, middle + 1, end);

    merge2SortedArrays(a, start, end);
}

void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % (SCREEN_HEIGHT);
        Barr[i] = random;
    }
}

void execute()
{
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case (SDLK_q):
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
                        loadArr();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        mergeSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                        break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout << "WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
         << "Available Controls inside Sorting Visualizer:-\n"
         << "    Use 0 to Generate a different randomized list.\n"
         << "    Use 1 to start Merge Sort Algorithm.\n"
         << "    Use q to exit out of Sorting Visualizer\n\n"
         << "PRESS ENTER TO START SORTING VISUALIZER...\n\n"
         << "Or type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if (s == "-1")
    {
        return false;
    }

    return true;
}

void intro_mess()
{
    cout << "==============================Sorting Visualizer==============================\n\n"
         << "Visualization of Merge sort algorithm in C++ with SDL2 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
         << "In this implementation of sorting visualizer, we'll be looking at Merge sort algorithms and visually comprehend its working.\n"
         << "The list size is fixed to 130 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithm like Merge Sort have been delayed so that they could be properly visualized.\n\n"
         << "Press ENTER to show controls...";
    string s;
    getline(cin, s);
    if (s == "\n")
    {
        return;
    }
}

int main(int argc, char *args[])
{
    intro_mess();
    char n;
    while (1)
    {
        controls();
        execute();
    }

    return 0;
}