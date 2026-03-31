#include <stdio.h>
#include <stdbool.h>

#include "API.h"

#define WIDTH 16
#define HEIGHT 16

static const int A_OFFSETS[4] = {0, 1, 0, -1};
static const int B_OFFSETS[4] = {1, 0, -1, 0};

// front: {a, b} right: {b, a}, left: {-b, -a}
/**
 * orientations and offsets
 * how this thing works
 * this is to check the front, left, and right of the "visited" grid to see if it is visited
 * i don't want to do a big if statement so here we go:
 * if the orientations are like this, then i want the subsequent offset to be set to this, otherwise its 0
 * front check, relative to orientation:
 * 0: y = 1
 * 1: x = 1
 * 2: y =-1
 * 3: x = -1
 * right check (invert it for left):
 * 0: x = 1
 * 1: y = -1
 * 2: x = -1
 * 3: y = 1
 * these are all of the respective offsets, we add this to the current coordinate, then check if that value is visited or not
 *
 */
void logText(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
void updateOrientation(int *orientationPointer, bool isLeft)
{
    if (isLeft)
    {
        *orientationPointer = (*orientationPointer + 3) % 4;
        logText("turned Left");
    }
    else
    {
        *orientationPointer = (*orientationPointer + 1) % 4;
        logText("turned Right");
    }
}
int isValid(int x, int y)
{
    return x < WIDTH && x >= 0 && y < HEIGHT && y >= 0;
}
void updateForwardMove(int orientation, int *x, int *y)
{
    switch (orientation)
    {
    case 0:
        *y = *y + 1;
        break;
    case 1:
        *x = *x + 1;
        break;
    case 2:
        *y = *y - 1;
        break;
    case 3:
        *x = *x - 1;
        break;
    }
}
void mainLoop(bool visited[WIDTH][HEIGHT], int *orientationPointer, int *xPointer, int *yPointer)
{
    logText("___________________________________________________");
    int x = *xPointer;
    int y = *yPointer;
    int orientation = *orientationPointer;

    bool visitedNext[3] = {false, false, false}; // 0, left, 1, front, 2 right
    // mark the current cell as visited
    // check if there is anything in front

    int offsetA = A_OFFSETS[orientation];
    int offsetB = B_OFFSETS[orientation];

    if (isValid(x - offsetB, y - offsetA)) // left
    {
        visitedNext[0] = visited[x - offsetB][y + offsetA];
        API_setText(x - offsetB, y - offsetA, "Left");
    }
    if (isValid(x + offsetA, y + offsetB)) // front
    {
        visitedNext[1] = visited[x + offsetA][y + offsetB];
        API_setText(x + offsetA, y + offsetB, "Front");
    }
    if (isValid(x + offsetB, y + offsetA)) // right
    {
        visitedNext[2] = visited[x + offsetB][y - offsetA];
        API_setText(x + offsetB, y + offsetA, "Right");
    }

    logText("__directions:__");
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            if (visitedNext[i])
                logText("visitedLeft");
            else
                logText("clearLeft");
            break;
        case 1:
            if (visitedNext[i])
                logText("visitedFront");
            else
                logText("clearFront");
            break;
        case 2:
            if (visitedNext[i])
                logText("visitedRight");
            else
                logText("clearRight");
            break;
        }
    }
    logText("__movement:__");

    if (!visitedNext[0] && !API_wallLeft())
    {
        logText("turning Left");
        API_turnLeft();
        updateOrientation(orientationPointer, true);
        API_moveForward();
        updateForwardMove(*orientationPointer, xPointer, yPointer);
    }
    else if (!visitedNext[1] && !API_wallFront())
    {
        logText("not turning");
        API_moveForward();
        updateForwardMove(orientation, xPointer, yPointer);
    }
    else if (!visitedNext[2] && !API_wallRight())
    {
        logText("turning Right");
        API_turnRight();
        updateOrientation(orientationPointer, false);
        API_moveForward();
        updateForwardMove(*orientationPointer, xPointer, yPointer);
    }
    else if (!API_wallRight())
    {
        logText("turning Right");
        API_turnRight();
        updateOrientation(orientationPointer, false);
        API_moveForward();
        updateForwardMove(*orientationPointer, xPointer, yPointer);
    }
    else if (!API_wallFront())
    {
        logText("not turning");
        API_moveForward();
        updateForwardMove(orientation, xPointer, yPointer);
    }
    else if (!API_wallLeft())
    {
        logText("turning Left");
        API_turnLeft();
        updateOrientation(orientationPointer, true);
        API_moveForward();
        updateForwardMove(*orientationPointer, xPointer, yPointer);
    }

    else
    {
        API_turnRight();
        API_turnRight();
        updateOrientation(orientationPointer, false);
        updateOrientation(orientationPointer, false);
        API_setColor(x, y, 'B');
        logText("Test");
        logText("wallA");
    }

    if (visited[x][y])
    {
        API_setColor(x, y, 'R');
    }
    else
    {
        visited[x][y] = true;
        API_setColor(x, y, 'G');
    }

    // forward front -> right -> left
    // back forward -> right -> left
}

int main(int argc, char *argv[])
{
    int orientation = 0; // 0: up, 1: right, 2: down, 3: left
    int x = 0, y = 0;    // keep track of our current position in the maze
    bool visited[WIDTH][HEIGHT] = {false};
    logText("Running...");

    API_setText(0, 0, "start");

    // we need to go to the center of the maze,
    // so set the end condition of the while loop to be when we are at the center of the maze
    while (x != WIDTH / 2 || y != HEIGHT / 2)
    {
        mainLoop(visited, &orientation, &x, &y);
    }
}
