#include <stdio.h>

#include "API.h"

#define WIDTH 16
#define HEIGHT 16

void log(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}
void updatecoords(int orientation, int* x, int* y) {
    if (orientation == 0) {
        *y = *y + 1;
    } else if (orientation == 1) {
        *x = *x + 1;
    } else if (orientation == 2) {
        *y = *y - 1;
    } else if (orientation == 3) {
        *x = *x - 1;
    }
}

int main(int argc, char* argv[]) {
    int orientation = 0; // 0: up, 1: right, 2: down, 3: left
    int x = 0, y = 0; // keep track of our current position in the maze
    int visited[WIDTH][HEIGHT];
    log("Running...");

    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");

    //we need to go to the center of the maze, 
    //so set the end condition of the while loop to be when we are at the center of the maze
    while (x != WIDTH / 2 || y != HEIGHT / 2) {
        //mark the current cell as visited
        //check if there is anything in front
        if(visited[x][y] == 1) {
            API_setColor(x, y, 'R');
            visited[x][y] == 2; //exclude path

        } else {
            visited[x][y] = 1;
            API_setColor(x, y, 'G');

        }
        
        int doInverse = (orientation >= 2) ? -1 : 1;
        int offsetA = ((orientation + 1) % 2) * doInverse;
        int offsetB = (orientation % 2) * doInverse;        
        int visitedFront = 0;
        int visitedRight = 0;
        int visitedLeft = 0;

        if(x + offsetB < WIDTH && x + offsetB >= 0 &&
            y + offsetA < HEIGHT && y + offsetA >= 0 
        ) {
            int visitedFront = visited[x + offsetB][y + offsetA];
        }
        if(x + offsetA < WIDTH && x + offsetA >= 0 &&
            y + offsetB < HEIGHT && y + offsetB >= 0 
        ) {
            visitedRight = visited[x + offsetA][y + offsetB];

        }        
        if(x - offsetA < WIDTH && x - offsetA >= 0 &&
            y - offsetB < HEIGHT && y - offsetB >= 0
        ) {
            visitedLeft = visited[x - offsetA][y - offsetB];
        }



        /**
         * orientations and offsets
         * how this thing works
         * this is to check the front, left, and right of the "visited" grid to see if it is visited
         * i don't want to do a big if statement so here we go:
         * if the orientations are like this, then i want the subsequent offset to be set to this, otherwise its 0
         * front check:
         * 0: y = 1 
         * 1: x = 1 
         * 2: y=-1 
         * 3: x = -1
         * right check (invert it for left):  
         * 0: x = 1 
         * 1: y = 1 
         * 2: x=-1 
         * 3: y = -1
         * these are all of the respective offsets, we add this to the current coordinate, then check if that value is visited or not 
         * 
         */
        if(!API_wallLeft() && !(visitedLeft == 2)) {
            API_turnLeft();
            orientation = (orientation + 3) % 4;
            API_moveForward();
            updatecoords(orientation, &x, &y);

        } else if(!API_wallRight() && !(visitedRight == 2)) {
            API_turnRight();
            orientation = (orientation + 1) % 4;
            API_moveForward();
            updatecoords(orientation, &x, &y);

        } else if(!API_wallFront()) {
            API_moveForward();
            updatecoords(orientation, &x, &y);
        } else {
            API_setColor(x, y, 'R');
            API_turnLeft();
            API_turnLeft();
            orientation = (orientation + 2) % 4;

        }

        

    }

}



