#ifndef SNAKE_H
#define SNAKE_H

//include statements
#include <stdio.h> 
#include <stdlib.h>
#include <conio.h> //for performing console I/O
#include <ctype.h> //checks the type of char
#include <unistd.h>
#include <console.h> //for interacting with the console

//game boundaries
#define HEIGHT 20
#define WIDTH 40

//gloabal variables
extern int snakeTailX[100], snakeTailY[100];
extern int snakeTailLen;
extern int gameover, key, score;
extern int x, y, fruitx, fruity;

//function prototypes
//function to gnerate fruit within the game boundary
void setup();

//function to draw the game field, snake, and fruit
void draw();

//function to take reak time input and set the flag value
void input();

//check for the movement logic that checks eat, move, collisions
void logic();

//function to run the game
void main();

#endif //SNAKE_H
