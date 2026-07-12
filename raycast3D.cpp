#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// SCREEN + MAP SETUP
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAP_WIDTH = 8;
const int MAP_HEIGHT = 8;

// 0 = empty floor, 1 = wall. This replaces the old rect-list walls[]
// entirely -- raycasting needs instant "is there a wall HERE" lookups,
// which a grid gives you for free.
int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,0,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}};

// Same idea as before: treat anything outside the map as a wall so a
// ray (or the player) can never march off the edge into undefined space.
bool isWall(int gridX, int gridY)
{
    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT)
        return true;
    return worldMap[gridY][gridX] == 1;
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "My DOOM - Raycaster",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0);
    if (!window)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // PLAYER STATE
    // Position is now a floating-point point INSIDE the grid
    // (e.g. 3.5, 3.5 means "centre of the cell at column 3, row 3"),
    // not a pixel coordinate.
    double posX = 3.5, posY = 3.5;

    // Direction the player is facing (a unit-length vector).
    // Starting value (-1, 0) means "facing left" along the grid.
    double dirX = -1.0, dirY = 0.0;

    // Camera plane: perpendicular to dirX/dirY. Its LENGTH sets your
    // field of view -- 0.66 gives ~66 degrees. Bigger plane = wider
    // FOV = more fisheye-looking distortion at the edges.
    double planeX = 0.0, planeY = 0.66;

    Uint32 oldTime = SDL_GetTicks();

    bool running = true;
    SDL_Event event;
    while (running)
    {
        // ------------------------------------------------------------
        // FRAME TIMING: measure how long the last frame took, so
        // movement/rotation speed stays the same regardless of
        // frame rate. Without this, a faster PC would make the
        // player move faster -- not what you want.
        // ------------------------------------------------------------
        Uint32 currentTime = SDL_GetTicks();
        double frameTime = (currentTime - oldTime) / 1000.0; // seconds
        oldTime = currentTime;

        // ------------------------------------------------------------
        // EVENT LOOP: only discrete, one-time events live here.
        // (Same lesson as before -- continuous input must NOT go here.)
        // ------------------------------------------------------------
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // ------------------------------------------------------------
        // CONTINUOUS INPUT: read once per frame, outside the event loop.
        // ------------------------------------------------------------
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        double moveSpeed = frameTime * 3.0; // grid cells per second
        double rotSpeed = frameTime * 2.5;  // radians per second

        // --- Move forward / backward along the direction vector ---
        if (keystate[SDL_SCANCODE_UP])
        {
            double newX = posX + dirX * moveSpeed;
            double newY = posY + dirY * moveSpeed;
            // Same sliding trick as your 2D game: check X and Y
            // separately so grazing a wall at an angle still lets you
            // slide along it instead of stopping dead.
            if (!isWall((int)newX, (int)posY)) posX = newX;
            if (!isWall((int)posX, (int)newY)) posY = newY;
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            double newX = posX - dirX * moveSpeed;
            double newY = posY - dirY * moveSpeed;
            if (!isWall((int)newX, (int)posY)) posX = newX;
            if (!isWall((int)posX, (int)newY)) posY = newY;
        }

        // --- Rotate the direction vector AND the camera plane ---
        // Both must rotate together, by the same angle, or the camera
        // plane stops being perpendicular to your facing direction and
        // the view distorts.
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }

        // ------------------------------------------------------------
        // RENDER: ceiling + floor as flat colours first...
        // ------------------------------------------------------------
        SDL_SetRenderDrawColor(renderer, 50, 50, 80, 255); // ceiling
        SDL_Rect ceilingRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
        SDL_RenderFillRect(renderer, &ceilingRect);

        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255); // floor
        SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
        SDL_RenderFillRect(renderer, &floorRect);

        // ------------------------------------------------------------
        // ...then one ray PER SCREEN COLUMN. This is the heart of
        // raycasting: instead of rendering a full 3D world, we solve a
        // much simpler 2D problem 640 times -- "how far until this one
        // ray hits a wall?" -- and turn that distance into a wall
        // height. Closer wall = taller line on screen.
        // ------------------------------------------------------------
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            // cameraX maps screen column x to a value from -1 (left
            // edge of screen) to +1 (right edge), 0 = dead centre.
            double cameraX = 2.0 * x / (double)SCREEN_WIDTH - 1.0;

            // Blend the camera plane into the direction vector using
            // cameraX. At cameraX=0 this is just dirX/dirY (straight
            // ahead). At the edges it leans fully into the plane
            // vector -- that's what fans the rays out into a cone.
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // Which grid cell the ray currently starts in.
            int mapX = (int)posX;
            int mapY = (int)posY;

            // ----- DDA setup -----
            // deltaDist = how far (in ray-length units) the ray must
            // travel to cross one full grid cell on that axis.
            // A very steep ray crosses X cells quickly but Y cells
            // slowly (or vice versa) -- deltaDist captures that.
            double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1.0 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1.0 / rayDirY);

            int stepX, stepY;      // which direction to step in the grid (+1 or -1)
            double sideDistX, sideDistY; // distance from start to the FIRST grid line crossing

            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            // ----- DDA main loop -----
            // This is the "jump from grid line to grid line" you saw
            // in the diagram. Whichever axis has the SMALLER
            // accumulated distance gets stepped next -- that's always
            // the next grid line the ray actually crosses.
            bool hit = false;
            int side = 0; // 0 = we hit a vertical (x-facing) wall side, 1 = horizontal
            while (!hit)
            {
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (isWall(mapX, mapY))
                    hit = true;
            }

            // ----- Fisheye fix -----
            // The straight-line distance from player to hit point
            // would make walls bulge outward at the screen edges
            // (fisheye effect), because edge rays travel further to
            // cross the same forward distance. Using the PERPENDICULAR
            // distance (measured along the player's own forward axis,
            // not along the ray itself) removes that distortion.
            double perpWallDist;
            if (side == 0)
                perpWallDist = sideDistX - deltaDistX;
            else
                perpWallDist = sideDistY - deltaDistY;

            // Closer wall (smaller perpWallDist) -> taller line.
            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            // Cheap fake lighting: darken walls hit on their Y-side so
            // the two wall orientations read as visually distinct,
            // like two faces of a box catching light differently.
            Uint8 brightness = (side == 1) ? 140 : 200;
            SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
            SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}