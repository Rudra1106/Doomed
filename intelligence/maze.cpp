#include <SDL2/SDL.h>
#include <iostream>

const int TILE = 32;

// instead of defining the maze as a 2D array of characters, we can define it as an array of strings. Each string represents a row of the maze, 
// and each character in the string represents a tile in that row. This makes it easier to visualize the maze and modify it if needed.
// initializing the maze as an array of strings allows us to easily define the layout of the maze using a simple and intuitive representation. 
// Each character in the string corresponds to a specific tile type, such as walls, empty spaces, coins, player start position, and exit. 
// This approach makes it easier to read and modify the maze layout without having to deal with complex data structures or nested loops. 
// Additionally, using strings allows for easy visualization of the maze in a text-based format, making it more accessible for debugging and testing purposes.
const char* maze[] =
{
"####################",
"#P......#......C..E#",
"#.####..#.#######..#",
"#......##.........##",
"#.##############...#",
"#.......C..........#",
"#..##############..#",
"#..................#",
"####################"
};

const int ROWS = sizeof(maze)/sizeof(maze[0]); // this means that we are calculating the number of rows in the maze by dividing the total size of the maze array (in bytes) by the size of a single row (in bytes). This gives us the number of strings (rows) we have defined in the maze array.
const int COLS = 20;

struct Player // this struct defines the properties of the player in the game, including their position (x and y coordinates) and size (width and height). The x and y coordinates represent the player's position in the game world, while the width and height define the dimensions of the player's character. This struct allows us to easily manage and manipulate the player's state throughout the game.
{
    float x;
    float y;
    int w = 20;
    int h = 20;
};

bool checkCollision(SDL_Rect a, SDL_Rect b) // this means that we are declaring a function named checkCollision that takes two SDL_Rect structures (a and b) as input parameters. The function returns a boolean value (true or false) indicating whether the two rectangles are colliding (overlapping) or not. The SDL_Rect structure is used to represent rectangular areas in SDL, and it contains four members: x (the x-coordinate of the top-left corner), y (the y-coordinate of the top-left corner), w (the width of the rectangle), and h (the height of the rectangle). The checkCollision function checks if the two rectangles intersect by comparing their positions and dimensions.
{
    return
    (
        a.x < b.x+b.w && // this means that we are checking if the left edge of rectangle a is to the left of the right edge of rectangle b. If this condition is true, it indicates that rectangle a is not completely to the right of rectangle b, which is one of the conditions for a collision.
        a.x+a.w > b.x && // this means that we are checking if the right edge of rectangle a is to the right of the left edge of rectangle b. If this condition is true, it indicates that rectangle a is not completely to the left of rectangle b, which is another condition for a collision.
        a.y < b.y+b.h && // this means that we are checking if the top edge of rectangle a is above the bottom edge of rectangle b. If this condition is true, it indicates that rectangle a is not completely below rectangle b, which is another condition for a collision.
        a.y+a.h > b.y    // this means that we are checking if the bottom edge of rectangle a is below the top edge of rectangle b. If this condition is true, it indicates that rectangle a is not completely above rectangle b, which is the final condition for a collision.
    );
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO); // this means that we are initializing the SDL library with the SDL_INIT_VIDEO flag, which indicates that we want to use the video subsystem of SDL. This function sets up the necessary resources and prepares SDL for creating windows, rendering graphics, and handling user input related to video. If the initialization is successful, it returns 0; otherwise, it returns a negative error code.

    SDL_Window* window =
    SDL_CreateWindow(
        "Maze Escape",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        COLS*TILE,
        ROWS*TILE,
        0);

    SDL_Renderer* renderer =
    SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED); // this means that we are creating a renderer for the specified window using the SDL_CreateRenderer function. The parameters passed to this function are as follows: window (the SDL_Window pointer representing the window we created earlier), -1 (which indicates that we want to use the first available rendering driver), and SDL_RENDERER_ACCELERATED (which specifies that we want to use hardware acceleration for rendering).
    //  The function returns a pointer to an SDL_Renderer structure, which we can use to draw graphics onto the window. If the renderer creation fails, it returns nullptr.

    Player player;

    int coins = 0;
    bool won = false;

    char map[ROWS][COLS+1]; // this means that we are declaring a 2D array named map with dimensions ROWS (number of rows) and COLS+1 (number of columns plus one). The +1 is added to accommodate the null terminator character ('\0') at the end of each string, which is necessary for proper string handling in C++. 
    // Each element of the array is of type char, which means it can hold a single character. This array will be used to store the current state of the maze, including walls, empty spaces, coins, player position, and exit.

    // this for loop iterates through each row of the maze and copies the corresponding string from the maze array into the map array. The strcpy function is used to perform the string copy operation.
    for(int r=0;r<ROWS;r++)
    {
        strcpy(map[r],maze[r]); // this means that we are copying the string from maze[r] (the r-th row of the maze) into map[r] (the r-th row of the map). The strcpy function takes two arguments: the destination string (map[r]) and the source string (maze[r]). It copies the characters from the source string to the destination string, including the null terminator at the end. This effectively initializes the map array with the same layout as the maze array.

        for(int c=0;c<COLS;c++) // this means that we are iterating through each column of the current row (r) in the map array. The variable c represents the current column index, and it ranges from 0 to COLS-1. This nested loop allows us to access each individual tile in the current row of the map.
        {
            if(map[r][c]=='P') // this means that we are checking if the character at the current position (row r, column c) in the map array is equal to 'P', which represents the player's starting position in the maze. If this condition is true, it indicates that we have found the tile where the player should be placed at the beginning of the game.
            {
                player.x=c*TILE+6;
                player.y=r*TILE+6;
                map[r][c]='.';
            }
        }
    }

    bool running=true;
    SDL_Event event; // this means that we are declaring a variable named event of type SDL_Event. The SDL_Event structure is used to represent various types of events that can occur in an SDL application, such as keyboard input, mouse input, window events, and more. By declaring this variable, we can use it to store and process events that are polled from the SDL event queue during the game loop.

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT)
                running=false;
        }

        const Uint8* key=SDL_GetKeyboardState(NULL); // this means that we are declaring a pointer named key of type const Uint8* and assigning it the result of the SDL_GetKeyboardState function. 
        // The SDL_GetKeyboardState function returns a pointer to an internal array that represents the current state of the keyboard. Each element in this array corresponds to a specific key on the keyboard, and its value indicates whether that key is currently pressed (1) or not pressed (0). 
        // The NULL argument passed to the function indicates that we do not need to retrieve the length of the array, as we are only interested in accessing the state of the keys. By using this pointer, we can easily check the state of specific keys during the game loop.

        float speed=2.5f;

        float dx=0;
        float dy=0;
        // overall concept of this code snippet is that we are checking the state of specific keys on the keyboard (W, A, S, D, and arrow keys) to determine the direction in which the player should move.
        // arthematically, we are using the state of the keys to calculate the change in the player's position (dx and dy) based on the defined speed. If a key is pressed, we adjust the corresponding dx or dy value accordingly, which will later be used to update the player's position in the game world.
        if(key[SDL_SCANCODE_W]||key[SDL_SCANCODE_UP]) dy=-speed;
        if(key[SDL_SCANCODE_S]||key[SDL_SCANCODE_DOWN]) dy=speed;
        if(key[SDL_SCANCODE_A]||key[SDL_SCANCODE_LEFT]) dx=-speed;
        if(key[SDL_SCANCODE_D]||key[SDL_SCANCODE_RIGHT]) dx=speed;

        // ---------- X collision ----------

        float newX=player.x+dx;

        SDL_Rect tryX={(int)newX,(int)player.y,player.w,player.h}; // 

        bool blocked=false;
        // this for loop iterates through each row (r) and column (c) of the maze to check for collisions between the player's attempted new position (tryX) and any walls in the maze.
        for(int r=0;r<ROWS;r++) 
        {
            for(int c=0;c<COLS;c++)
            {
                if(map[r][c]=='#')
                {
                    SDL_Rect wall=
                    {
                        c*TILE,
                        r*TILE,
                        TILE,
                        TILE
                    };

                    if(checkCollision(tryX,wall))
                        blocked=true;
                }
            }
        }

        if(!blocked)
            player.x=newX;

        // ---------- Y collision ----------

        float newY=player.y+dy;

        SDL_Rect tryY={(int)player.x,(int)newY,player.w,player.h}; // this means that we are creating an SDL_Rect structure named tryY to represent the player's attempted new position after applying the vertical movement (dy). The x-coordinate of tryY is set to the current x-coordinate of the player (player.x), while the y-coordinate is set to the new y-coordinate (newY) calculated by adding dy to the current y-coordinate (player.y). The width and height of tryY are set to the player's width (player.w) and height (player.h), respectively. This rectangle will be used to check for collisions with walls in the maze when the player attempts to move vertically.

        blocked=false;

        for(int r=0;r<ROWS;r++)
        {
            for(int c=0;c<COLS;c++)
            {
                if(map[r][c]=='#')
                {
                    SDL_Rect wall=
                    {
                        c*TILE,
                        r*TILE,
                        TILE,
                        TILE
                    };

                    if(checkCollision(tryY,wall))
                        blocked=true;
                }
            }
        }

        if(!blocked)
            player.y=newY;

        SDL_Rect playerRect=
        {
            (int)player.x,
            (int)player.y,
            player.w,
            player.h
        };

        for(int r=0;r<ROWS;r++)
        {
            for(int c=0;c<COLS;c++)
            {
                SDL_Rect tile=
                {
                    c*TILE,
                    r*TILE,
                    TILE,
                    TILE
                };

                if(map[r][c]=='C')
                {
                    SDL_Rect coin=
                    {
                        c*TILE+10,
                        r*TILE+10,
                        12,
                        12
                    };

                    if(checkCollision(playerRect,coin))
                    {
                        map[r][c]='.';
                        coins++;
                    }
                }

                if(map[r][c]=='E')
                {
                    if(checkCollision(playerRect,tile))
                    {
                        won=true;
                    }
                }
            }
        }

        if(won)
        {
            SDL_SetWindowTitle(window,"YOU ESCAPED!");
        }
        else
        {
            std::string title="Coins : "+std::to_string(coins);
            SDL_SetWindowTitle(window,title.c_str());
        }

        // ---------------- DRAW ----------------

        SDL_SetRenderDrawColor(renderer,18,18,18,255);
        SDL_RenderClear(renderer);

        for(int r=0;r<ROWS;r++)
        {
            for(int c=0;c<COLS;c++)
            {
                SDL_Rect tile=
                {
                    c*TILE,
                    r*TILE,
                    TILE,
                    TILE
                };

                // floor

                SDL_SetRenderDrawColor(renderer,30,30,35,255);
                SDL_RenderFillRect(renderer,&tile);

                // grid

                SDL_SetRenderDrawColor(renderer,40,40,45,255);
                SDL_RenderDrawRect(renderer,&tile);

                if(map[r][c]=='#')
                {
                    SDL_SetRenderDrawColor(renderer,65,65,80,255);
                    SDL_RenderFillRect(renderer,&tile);
                }

                if(map[r][c]=='C')
                {
                    SDL_SetRenderDrawColor(renderer,255,220,0,255);

                    SDL_Rect coin=
                    {
                        c*TILE+10,
                        r*TILE+10,
                        12,
                        12
                    };

                    SDL_RenderFillRect(renderer,&coin);
                }

                if(map[r][c]=='E')
                {
                    SDL_SetRenderDrawColor(renderer,0,180,255,255);

                    SDL_Rect exit=
                    {
                        c*TILE+4,
                        r*TILE+4,
                        24,
                        24
                    };

                    SDL_RenderFillRect(renderer,&exit);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,50,220,120,255);

        SDL_RenderFillRect(renderer,&playerRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}