#include <SDL2/SDL.h>
#include <iostream>

int main() {
    // 1. Start up SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

// 2. Create a window 
//  SDL_Window* window means that we are declaring a pointer to an SDL_Window structure. This structure represents a window in SDL. 
//  The SDL_CreateWindow function is called to create a new window with the specified parameters, and it returns a pointer to the newly created window. If the window creation fails, SDL_CreateWindow will return nullptr, and we handle that case by printing an error message and returning from the program.
// !why are we declaring a pointer to an SDL_Window structure ? 
// because we need to manage the window's properties and behavior, such as its size, position, and title. By using a pointer, we can easily pass the window around to different functions and manage its lifecycle (creation and destruction) without copying the entire structure. 
// This is more efficient and allows for better control over the window's state throughout the program.
    SDL_Window* window = SDL_CreateWindow(
        "My DOOM",              // title
        SDL_WINDOWPOS_CENTERED, // x position
        SDL_WINDOWPOS_CENTERED, // y position
        640, 480,               // width, height
        0                       // flags
    );

    if (!window) {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

// 3. Create a renderer (this is what actually draws pixels)
// SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0) means that we are declaring a pointer to an SDL_Renderer structure. This structure represents a rendering context in SDL,
// which is responsible for drawing graphics to the window. The SDL_CreateRenderer function is called to create a new renderer associated with the specified window. The parameters passed to SDL_CreateRenderer are as follows: 
// SDL_Renderer structure is a inbuilt structure in SDL that provides an interface for rendering graphics. It allows you to draw shapes, textures, and other graphical elements onto the window. 
// The renderer is responsible for managing the rendering pipeline and handling the actual drawing operations.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 4. Keep it open until the user closes it
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Set draw color (R, G, B, Alpha) — dark blue-ish
        SDL_SetRenderDrawColor(renderer, 20, 20, 60, 255);
        // Clear the whole screen with that color
        SDL_RenderClear(renderer);

        // change draw color to white for rectangle
        // SDL_SetRenderDrawColor arguments are (renderer, red, green, blue, alpha) where each color component is an integer value between 0 and 255. The alpha value represents the opacity of the color, with 255 being fully opaque and 0 being fully transparent.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // now lets draw a rectangle in the middle of the screen :
        // SDL_Rect rect = { 270, 200, 100, 100 }; // x, y, width, height
        // i dont see any rectangle drawn on the screen, why is that ?
        // The reason you don't see the rectangle is that you haven't actually drawn it yet. You need to call SDL_RenderFillRect to fill the rectangle with the current draw color. 

        // SDL_RenderFillRect(renderer, &rect); // This will fill the rectangle with the current draw color (dark blue-ish in this case). Make sure to call this function before SDL_RenderPresent 
        //  Push what we drew onto the actual visible window

        SDL_Rect walls[] = {
            { 270, 200, 100, 100 }, // x, y, width, height
            { 150, 150, 50, 200 },
            { 400, 100, 80, 300 }
        };

        int numWalls = sizeof(walls) / sizeof(walls[0]); // instead of int numWalls = 3; we write this because it is more flexible and allows us to easily change the number of walls in the future without having to manually update the numWalls variable.
    //  this means that we are calculating the number of elements in the walls array by dividing the total size of the array (in bytes) by the size of a single element (in bytes). This gives us the number of rectangles (walls) we have defined in the walls array.
        for (int i = 0; i < numWalls; ++i) {
            SDL_RenderFillRect(renderer, &walls[i]);
        }
    // this for loop iterates through each rectangle in the walls array and calls SDL_RenderFillRect to fill each rectangle with the current draw color (white in this case). The &walls[i] syntax is used to pass the address of the current rectangle to the function, as SDL_RenderFillRect expects a pointer to an SDL_Rect structure.

        SDL_RenderPresent(renderer);
        //  overall working sudo code is : first we set the draw color to dark blue-ish, then we clear the screen with that color, then we change the draw color to white, then we draw a rectangle in the middle of the screen, and finally we push what we drew onto the actual visible window.
    }

    // 5. Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}