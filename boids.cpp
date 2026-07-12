// #include <stdio.h>
// #include <SDL2/SDL.h>
// #include <iostream>

// using namespace std;

// int main(int argc, char* argv[])
// {
//     // Initialize SDL
//     if (SDL_Init(SDL_INIT_VIDEO) != 0)
//     {
//         std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // Create a window
//     SDL_Window* window = SDL_CreateWindow(
//         "Boids Simulation",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         800, 600,
//         0
//     );

//     if (!window)
//     {
//         std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // Create a renderer
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
//     if (!renderer)
//     {
//         std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     float x = 400.0f; // Initial x position of the boid
//     float y = 300.0f; // Initial y position of the boid

//     float vx = 100.0f; // Velocity in x direction (pixels per second)
//     float vy = 75.0f;  // Velocity in y direction (pixels per

//     // now we have to measure the time dt between frames to update the position of the boid correctly. We will use SDL_GetTicks() to get the current time in milliseconds and calculate the delta time (dt) for each frame.
//     // It always means the previous frame's time i,e; lastTick becomes previous currentTick
//     Uint32 lastTick = SDL_GetTicks(); // this means that we are declaring a variable named lastTick of type uint32 and initializing it with the current time in milliseconds obtained from the SDL_GetTicks() function. The SDL_GetTicks() function returns the number of milliseconds since the SDL library was initialized, which allows us to measure time intervals between frames in our application. 
//     // By storing this value in lastTick, we can later calculate the delta time (dt) for each frame by comparing it with the current time in subsequent iterations of the main loop.
    
//     // Main loop
//     bool running = true;
//     while (running)
//     {
//         Uint32 currentTick = SDL_GetTicks(); // this means that we are declaring a variable named currentTick of type uint32 and initializing it with the current time in milliseconds obtained from the SDL_GetTicks() function. Similar to lastTick, this variable will be used to store the current time at each iteration of the main loop, allowing us to calculate the delta time (dt) between frames by comparing it with lastTick.
//         // mathematically, we will calculate dt as follows: dt = (currentTick - lastTick) / 1000.0f; This gives us the time elapsed between the current frame and the previous frame in seconds, which we can then use to update the position of the boid based on its velocity. 
//         float dt = (currentTick - lastTick) / 1000.0f; // this means that we are calculating the delta time (dt) in seconds by subtracting lastTick from currentTick to get the elapsed time in milliseconds, and then dividing it by 1000.0f to convert it to seconds. The result is stored in the variable dt, which represents the time elapsed between the current frame and the previous frame. This value will be used to update the position of the boid based on its velocity.
//         lastTick = currentTick; // this means that we are updating the value of lastTick to be equal to currentTick. This is done to prepare for the next iteration of the main loop, where we will again calculate the delta time (dt) by comparing the new currentTick with the updated lastTick. By doing this, we ensure that lastTick always holds the time of the previous frame, allowing us to accurately measure the time elapsed between frames.
//         // now to explain all this time concept in simple terms, we are measuring the time between frames to ensure that the boid's movement is smooth and consistent regardless of the frame rate. By calculating the delta time (dt), we can update the boid's position based on its velocity and the elapsed time, allowing for frame-independent movement. This means that even if the frame rate fluctuates, the boid will move at a consistent speed in pixels per second.
    
//         SDL_Event event;
//         while (SDL_PollEvent(&event))
//         {
//             if (event.type == SDL_QUIT)
//             {
//                 running = false;
//             }
//         }

//         x += vx * dt; // this means that we are updating the x position of the boid by adding the product of its velocity in the x direction (vx) and the delta time (dt) to its current x position. This allows us to move the boid smoothly based on its speed and the time elapsed since the last frame.
//         y += vy * dt; // this means that we are updating the y position of the boid by adding the product of its velocity in the y direction (vy) and the delta time (dt) to its current y position. Similar to the x position update, this allows us to move the boid smoothly based on its speed and the time elapsed since the last frame.

//         // Clear the screen with a dark blue color
//         SDL_SetRenderDrawColor(renderer, 20, 20, 60, 255);
//         SDL_RenderClear(renderer);

//         // lets render a single white rectangle in the middle of the screen :
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // this means that we are setting the color used for drawing operations in the renderer to white. The parameters passed to SDL_SetRenderDrawColor are as follows: renderer (the SDL_Renderer pointer representing the rendering context), 255 (the red component of the color), 255 (the green component of the color), 255 (the blue component of the color), and 255 (the alpha component of the color, which represents opacity). In this case, we are setting the draw color to white with full opacity.
//         SDL_Rect rect {(int)x, (int)y, 10, 10}; // x, y, width, height
//         // why did we convert x and y to int ? because SDL_Rect structure requires integer values for its x and y coordinates, as well as its width and height. The SDL_Rect structure is defined with integer members, so we need to cast the float values of x and y to int when creating the rect object. This ensures that the rectangle is positioned correctly on the screen based on the current floating-point position of the boid.
//         // if its not int then the rectangle will not be drawn correctly on the screen, as the SDL rendering functions expect integer values for pixel positions. Using floating-point values directly could lead to unexpected behavior or visual artifacts, as the rendering system may not handle non-integer coordinates properly.
//         SDL_RenderFillRect(renderer, &rect);
//        cout << "Rendering boid at (" << x << ", " << y << ")" << endl;

//         // Render boids and other elements here...

//         // Present the rendered frame
//         SDL_RenderPresent(renderer);
//     }

//     // Clean up and quit
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }

// ! More cleaner version : 

#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

// ------------------------------------------------------------
// A Vec2 simply represents a point (x,y) or a direction (x,y).
//
// Mathematically:
//
// Position = (x,y)
//
// Velocity = (vx,vy)
//
// Instead of storing x and y separately, we group them together
// because they belong to the same mathematical object.
//
// This idea is used everywhere:
// • Physics
// • Graphics
// • Robotics
// • AI
// • Games
// ------------------------------------------------------------
struct Vec2
{
    float x;
    float y;
};

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "SDL_Init failed: " << SDL_GetError() << endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Boids Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    if (!window)
    {
        cout << "Window creation failed: " << SDL_GetError() << endl;
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        cout << "Renderer creation failed: " << SDL_GetError() << endl;
        return 1;
    }

    //----------------------------------------------------------
    // Screen information
    //----------------------------------------------------------

    const int screenWidth = 800;
    const int screenHeight = 600;

    const int rectWidth = 10;
    const int rectHeight = 10;

    //----------------------------------------------------------
    // Position of our boid
    //
    // Mathematically:
    //
    // P = (400,300)
    //----------------------------------------------------------

    Vec2 position = {400.0f, 300.0f};

    //----------------------------------------------------------
    // Velocity
    //
    // Velocity is also a vector.
    //
    // vx = horizontal speed
    // vy = vertical speed
    //
    // Since BOTH are non-zero, the rectangle naturally moves
    // diagonally.
    //----------------------------------------------------------

    Vec2 velocity = {180.0f, 140.0f};

    //----------------------------------------------------------
    // Time measurement
    //----------------------------------------------------------

    Uint32 lastTick = SDL_GetTicks();

    bool running = true;

    while (running)
    {
        //------------------------------------------------------
        // Delta Time (seconds)
        //------------------------------------------------------

        Uint32 currentTick = SDL_GetTicks();

        float dt = (currentTick - lastTick) / 1000.0f;

        lastTick = currentTick;

        //------------------------------------------------------
        // Handle Events
        //------------------------------------------------------

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        //------------------------------------------------------
        // Update Position
        //
        // Physics Equation
        //
        // New Position =
        //
        // Old Position + Velocity × Time
        //
        // x = x + vx * dt
        // y = y + vy * dt
        //
        // Because vx and vy are BOTH non-zero,
        // the rectangle moves diagonally.
        //------------------------------------------------------

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        //------------------------------------------------------
        // Bounce from Left Wall
        //
        // If x becomes less than zero,
        // the rectangle has crossed the wall.
        //
        // We place it exactly back on the wall and reverse
        // the x velocity.
        //
        // vx = -vx
        //
        // Positive becomes negative.
        //------------------------------------------------------

        if (position.x < 0)
        {
            position.x = 0;
            velocity.x = -velocity.x;
        }

        //------------------------------------------------------
        // Bounce from Right Wall
        //
        // The right side of the rectangle is
        //
        // x + width
        //
        // If this becomes greater than the screen width,
        // the rectangle has crossed the boundary.
        //------------------------------------------------------

        if (position.x + rectWidth > screenWidth)
        {
            position.x = screenWidth - rectWidth;

            velocity.x = -velocity.x;
        }

        //------------------------------------------------------
        // Bounce from Top Wall
        //------------------------------------------------------

        if (position.y < 0)
        {
            position.y = 0;

            velocity.y = -velocity.y;
        }

        //------------------------------------------------------
        // Bounce from Bottom Wall
        //
        // Bottom edge =
        //
        // y + height
        //------------------------------------------------------

        if (position.y + rectHeight > screenHeight)
        {
            position.y = screenHeight - rectHeight;

            velocity.y = -velocity.y;
        }

        //------------------------------------------------------
        // Rendering
        //------------------------------------------------------

        SDL_SetRenderDrawColor(renderer, 20, 20, 60, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //------------------------------------------------------
        // SDL_Rect requires integer coordinates.
        //
        // Our physics uses floating-point numbers because
        // movement is smoother.
        //
        // Before drawing, we convert the float position into
        // pixel coordinates.
        //------------------------------------------------------

        SDL_Rect rect =
        {
            (int)position.x,
            (int)position.y,
            rectWidth,
            rectHeight
        };

        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        cout << "Position : ("
             << position.x
             << ", "
             << position.y
             << ")"
             << endl;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}