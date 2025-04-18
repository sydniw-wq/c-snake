#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctype.h>

// Macros variable (HEIGHT, WIDTH)
#define HEIGHT 20
#define WIDTH 40

//store coordinates of the snake
//tail coordinates
int snakeTailX[100], snakeTailY[100];

//store length of snake's tail
int snakeTailLen;

//score and signal flags
int gameover, key, score;

//coordinates of snake head
int x, y;

//coordinates of fruit
int fruitx, fruity;

// Non-blocking keyboard input
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

//generate fruit within game boundary, initial game set up
void setup(){
    //game not over
    gameover = 0;

    //set initial coordinates of the snake
    x = WIDTH / 2;
    y = HEIGHT / 2;

    //set initial coordinates of the fruit
    fruitx = rand() % WIDTH;
    fruity = rand() % HEIGHT;

    //handle if random number chosen in 0, generate new num
    while (fruitx == 0){
        fruitx = rand() % WIDTH;
    }
    while (fruity == 0){
        fruity = rand() % HEIGHT;
    }

    //initialize score
    score = 0;
}

//draw the game field, draw snake and fruit
void draw(){
    system("clear");

    //create top wall, use characters to draw out boundary
    for (int i =0; i< WIDTH + 2; i++){
        printf("-");
    }
    printf("\n");

    //create side walls, snake head, and fruit
    for (int i =0; i< HEIGHT; i++){
        for (int j = 0; j<= WIDTH; j++){
            //side wall, end of width or beginning create side boundary
            if (j==0 || j == WIDTH){
                printf("#");
            }
            //if coordinate matches what snake head set to then display snake head
            if (i == y && j == x){
                printf("0");
            }
            //if coordinate matches generated fruit coordinate then display fruit
            else if (i == fruity && j == fruitx){
                printf("*");
            }
            //will draw the body (otherwise will leave space blank)
            else{
                int printTail = 0;
                //for the entire length of the snake
                for (int k = 0; k < snakeTailLen; k++){
                    //in the case that the tail coordinates match current game board coordinate 
                    if (snakeTailX[k] == j && snakeTailY[k] == i){
                        printf("o");
                        //indicates there is tail to print
                        printTail = 1;
                    }
                }
                //no tail to print in that block so leave space blank for board
                if (!printTail){
                    printf(" ");
                }
            }
        }
        printf("\n"); 
    }
    //create bottom wall
    for (int i = 0; i < WIDTH + 2; i++){
        printf("-");
    }
    printf("\n");

    //print score and instructions at bottom of game board
    printf("score = %d\n", score);
    printf("Press W, A, S,D for movement.\n");
    printf("Press X to quit the game.\n");
}

//function to take user input and set flag accordingly
void input() {
    if (kbhit()){
        switch (tolower(getchar())){
            case 'a':
                if(key!=2){
                    key = 1;
                    break;
                }
            case 'd':
                if (key!=1){
                    key = 2;
                    break;
                }
            case 'w':
                if (key!=4){
                    key = 3;
                    break;
                }
            case 's':
                if (key!=3){
                    key = 4;
                    break;
                }
            case 'x':
                gameover = 1;
                break;
        }
    }
}

//function to keep track of movement logic
void logic(){
    //handle eating fruit, add to tail and increase score
    if (x == fruitx && y == fruity){
        //head collides with fruit so generate new fruit somewhere else
        fruitx = rand() % WIDTH;
        fruity = rand() % HEIGHT;
        
        while (fruitx == 0){
            fruitx = rand() % WIDTH;
        } 
        while (fruity == 0){
            fruity = rand() % HEIGHT;
        }
        score += 10;
        snakeTailLen++;
    }
    //update coordinates to keep snake moving
    int prevX = snakeTailX[0]; //takes earliest coordinate in tail
    int prevY = snakeTailY[0];

    int prev2X, prev2Y;

    snakeTailX[0] = x; //start at the head of the snake
    snakeTailY[0] = y;

    //assign values in tail to prev values in tail, take over prev values to give illusion of movement
    for (int i = 1; i< snakeTailLen; i++){
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];

        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }

    //change direction of snake based on input
    //each char associated with key value, each key assigned with modifying coordinate of snake
    switch (key){
        case 1:
            x--;
            break;
        case 2:
            x++;
            break;
        case 3:
            y--;
            break;
        case 4:
            y++;
            break;
    }

    //handle game over, snake collides with wall or tail
    if (x < 0 || x >= WIDTH || y < 0 || y>= HEIGHT){
        gameover = 1;
    }
    for (int i = 0; i < snakeTailLen; i++){
        if (snakeTailX[i] == x && snakeTailY[i] == y){
            gameover = 1;
        }
    }

}

//main function where we run the fnctions to run the game
int main(){
    //run setup to initialize game board
    setup();

    //start game here, jkeep track of gameover value
    while(!gameover){
        //draw boundary, snake and fruit
        draw();

        //take user input
        input();

        //take logic into account to keep game moving properly
        logic();

        //allows som etime to wait
        usleep(100000);
    }

    return 0;
}