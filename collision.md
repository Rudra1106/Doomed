# Collision Detection from First Principles (AABB)

## Goal

Understand **why** AABB (Axis-Aligned Bounding Box) collision detection
works instead of memorizing the code.

------------------------------------------------------------------------

# 1. A rectangle is just two 1D intervals

A rectangle can be viewed as:

-   an interval on the **X axis**
-   an interval on the **Y axis**

```{=html}
<!-- -->
```
    left ---------------- right

    top ---------------- bottom

For SDL:

``` cpp
SDL_Rect r = {x, y, w, h};
```

Edges are:

``` text
left   = x
right  = x + w

top    = y
bottom = y + h
```

Example:

``` cpp
SDL_Rect r = {100, 50, 30, 20};
```

    left   = 100
    right  = 130

    top    = 50
    bottom = 70

------------------------------------------------------------------------

# 2. Interval overlap in 1D

Imagine two line segments.

    A

    0----------------10

    B

          8----------------18

Shared region:

    8------10

They overlap.

The conditions are

    A.left < B.right

    AND

    A.right > B.left

Now a non-overlap example.

    A

    0-----------10

    B

                  15---------25

No overlap because

    15 > 10

------------------------------------------------------------------------

# 3. Extend to rectangles

Rectangle A

    left = 20
    right = 50

    top = 10
    bottom = 40

Rectangle B

    left = 35
    right = 70

    top = 15
    bottom = 45

Horizontal:

    20-----------50
          35-----------70

    20 < 70

    AND

    50 > 35

Vertical:

    10----------40
        15-----------45

    10 < 45

    AND

    40 > 15

Since BOTH axes overlap, rectangles collide.

------------------------------------------------------------------------

# 4. Why AND?

Collision means occupying the same area.

Cases:

Horizontal YES, Vertical NO

    ####
    ####


          ####
          ####

No collision.

Horizontal NO, Vertical YES

    ####

                ####

No collision.

Horizontal YES AND Vertical YES

    #######
    ##XXXX##
    #######

Collision.

------------------------------------------------------------------------

# 5. Deriving the SDL code

``` cpp
leftA = a.x;
rightA = a.x + a.w;

leftB = b.x;
rightB = b.x + b.w;
```

Overlap on X:

``` text
leftA < rightB
AND
rightA > leftB
```

Substitute:

``` cpp
a.x < b.x + b.w
a.x + a.w > b.x
```

Repeat for Y:

``` cpp
a.y < b.y + b.h
a.y + a.h > b.y
```

Combine:

``` cpp
bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    return (a.x < b.x + b.w) &&
           (a.x + a.w > b.x) &&
           (a.y < b.y + b.h) &&
           (a.y + a.h > b.y);
}
```

------------------------------------------------------------------------

# 6. Thinking backwards (No Collision)

There are only four ways rectangles do NOT touch.

## A is left of B

    ####

             ####

    rightA <= leftB

## A is right of B

             ####

    ####

    leftA >= rightB

## A is above B

    ####


         ####

    bottomA <= topB

## A is below B

         ####


    ####

    topA >= bottomB

Therefore

    No Collision =
    left OR right OR above OR below

Using De Morgan's Law:

    Collision = NOT(No Collision)

becomes

``` cpp
!(
    a.x + a.w <= b.x ||
    a.x >= b.x + b.w ||
    a.y + a.h <= b.y ||
    a.y >= b.y + b.h
)
```

which simplifies to the familiar AABB test.

------------------------------------------------------------------------

# 7. Bounce from walls

Window width = 800

Rectangle width = 10

Right edge:

    right = x + width

Collision with right wall:

``` cpp
if (x + width >= 800)
    vx = -vx;
```

Left wall:

``` cpp
if (x <= 0)
    vx = -vx;
```

Top:

``` cpp
if (y <= 0)
    vy = -vy;
```

Bottom:

``` cpp
if (y + height >= windowHeight)
    vy = -vy;
```

Only velocity changes.

------------------------------------------------------------------------

# 8. Why check X and Y separately?

Bad approach:

Move diagonally in one step.

    P  ↗

    ██████

Combined move overlaps wall.

Player freezes.

Better:

## Test X

``` cpp
SDL_Rect tryX = {newX, playerY, size, size};
```

If clear:

``` cpp
playerX = newX;
```

## Test Y

``` cpp
SDL_Rect tryY = {playerX, newY, size, size};
```

If clear:

``` cpp
playerY = newY;
```

Result:

    ██████
       □

The player slides naturally along the wall.

------------------------------------------------------------------------

# 9. Mental Model

Never memorize the formula.

Instead remember:

1.  Every rectangle has four edges.
2.  Rectangles are intervals on X and Y.
3.  Intervals overlap if:
    -   left \< other right
    -   right \> other left
4.  Rectangles collide only if X overlap AND Y overlap.
5.  Testing X and Y independently produces wall sliding.

------------------------------------------------------------------------

# 10. Complexity

Checking one pair:

    O(1)

Checking N walls:

    O(N)

Games improve this later using QuadTrees, Uniform Grids, BSP trees, BVH,
etc.

------------------------------------------------------------------------

# 11. Common beginner mistakes

-   Comparing x positions instead of edges.
-   Forgetting width/height.
-   Moving both axes together.
-   Using rectangle centers instead of edges.
-   Memorizing instead of deriving.

Once you understand interval overlap, the AABB algorithm becomes simple
geometry rather than a magic formula.
