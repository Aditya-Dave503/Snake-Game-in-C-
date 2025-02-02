#include<bits/stdc++.h>
#include<conio.h>       // whenever user press any key, an event is triggerred by this library by kbhit() function, if true, then key is pressed
#include<windows.h>     // to give (x,y) coordinates on the board/terminal

using namespace std;

#define MAX_LENGTH 1000     // defining macros so that snake length does not exceed 1000 characters

const char DIR_UP = 'U';    // now defining directions in the subsequent 4 lines, variables have become read-only, can't be updated now
const char DIR_DOWN = 'D';
const char DIR_RIGHT = 'R';
const char DIR_LEFT = 'L';

int consoleWidth, consoleHeight;    // defining variables to store width and height of the console

void initScreen(){
    // get the width and height of the console

    HANDLE hConsole  = GetStdHandle(STD_OUTPUT_HANDLE);   //helps us in getting console handle from windows.h, GetStdHandle is a method in this library

    CONSOLE_SCREEN_BUFFER_INFO csbi;   // _CONSOLE_SCREEN_BUFFER_INFO is a structure in windows.h library, csbi is a variable of that type
    GetConsoleScreenBufferInfo(hConsole, & csbi);   // GetConsoleScreenBufferInfo is a function in windows.h library, it return buffer info of screen

    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point{
    int xCoord;
    int yCoord;

    Point(){

    }

    Point(int x, int y){
        xCoord = x;
        yCoord = y;
    }
};

class Snake{
    int length;
    char direction;

    public:
    Point body[MAX_LENGTH];     // we are making body of the snake with MAX_LENGTH elements, of type Point, it is public, as we nned to use that in our array

    Snake(int x, int y){            // creating constructor of snake class
        length = 1;               // starting length of the snake is 1
        body[0] = Point(x,y);     // assigning the snake the starting point of our game
        direction = DIR_RIGHT;    // by default, our snake moves right
    }

    // ~Snake(){
    //     // destructor of snake class
    //     delete[] body;             // deleting the array of points, as it is dynamically allocated
    // }

    int getLength(){
        return length;          // returns the length of the snake as it is a private variable
    }

    void changeDirection(char newDirection){

        if(newDirection == DIR_UP && direction != DIR_DOWN){
            direction = newDirection;
        }
        else if(newDirection == DIR_DOWN && direction != DIR_UP){
            direction = newDirection;
        }
        else if(newDirection == DIR_LEFT && direction != DIR_RIGHT){
            direction = newDirection;
        }
        else if(newDirection == DIR_RIGHT && direction != DIR_LEFT){
            direction = newDirection;
        }
    }

    bool move(Point food){
        for(int i=length-1;i>0;i--){
            body[i]=body[i-1];
        }

        switch(direction){
            int val;

            case DIR_UP:
            val=body[0].yCoord;
            body[0].yCoord=val-1;
            break;

            case DIR_DOWN:
            val=body[0].yCoord;
            body[0].yCoord=val+1;
            break;

            case DIR_LEFT:
            val=body[0].xCoord;
            body[0].xCoord=val-1;
            break;

            case DIR_RIGHT:
            val=body[0].xCoord;
            body[0].xCoord=val+1;
            break;

        }

        // snake bites itself

        for(int i=1;i<length;i++){
            if(body[0].xCoord==body[i].xCoord && body[0].yCoord==body[i].yCoord){
                return false;
            }
        }

        // snake eats food
        if(food.xCoord== body[0].xCoord && food.yCoord==body[0].yCoord){
            body[length]=Point(body[length-1].xCoord, body[length-1].yCoord);
            length++;
        }
        return true;
    }
};

class Board{
        Snake *snake;    // our snake object
        const char SNAKE_BODY = 'O';    // snake body is represented by 'O'
        Point food;     // food on our board, it will be at a point on the board
        const char FOOD = 'o';  // food is represented by 'o' on the board
        int score;      // user score

        public:
        Board(){
            spawnFood();    // as soon as board is instantiated, spanfood(), that is, display food will be called
            snake= new Snake(10,10);     // snake is initialized dynamically, see the constructor in the snake class
            score=0;                // initially score will be 0
        }

        int getScore(){
            return score;         // returns the score of the user, getter method
        }

        void displayCurrentScore(){
            gotoxy(consoleWidth/2,0);
            cout << "Current Score is: " << score;
        }

        void gotoxy(int x, int y){
            COORD coord;            // by default, COORD class is in windows.h, having x and y coordinates as public variables
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);    // takes our cursor to that position
        }

        ~Board(){
            delete snake;   // snake is dynamically created, so we ourselves need to delete it
        }

        void spawnFood(){
            int x = rand() % consoleWidth; // random x coordinate between 0 and 99
            int y = rand() % consoleHeight; // random y coordinate between 0 and 99
            food = Point(x,y); // food is at a random point on the board and is of the type point
        }

        void draw(){
            system("cls");  // clears screen before drawing anything on the screen
            for(int i=0;i<snake->getLength();i++){     // prints our snake on screen

                gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);   // takes cursor to x,y coordinate
                cout << SNAKE_BODY;     // prints our snake and gives effect that snake is walking
            }

            gotoxy(food.xCoord,food.yCoord);    // gives food on our board and prints it
            cout << FOOD;

            displayCurrentScore();
        }

        bool update(){
            bool isAlive=snake->move(food);
            if(isAlive== false)
            return false;

            if(food.xCoord== snake->body[0].xCoord && food.yCoord==snake->body[0].yCoord){

                score++;
                spawnFood();
            }
            return true;
        }

            void getInput(){

                if(kbhit()){            // in conio.h, kbhit() is used to check if any key is pressed
                        int key=getch();
                        if(key=='u' || key=='U'){
                            snake->changeDirection(DIR_UP);
                        }
                        else if(key=='l' || key=='L'){
                            snake->changeDirection(DIR_LEFT);
                        }
                        else if(key=='d' || key=='D'){
                            snake->changeDirection(DIR_DOWN);
                        }
                        else if(key=='r' || key=='R'){
                            snake->changeDirection(DIR_RIGHT);
                        }
                }
            }
        
        
};

int main(){
    srand(time(0));

    initScreen();
    Board *board= new Board();

    while(board->update()){
        board->getInput();
        board->draw();
        Sleep(300);
    }

    cout << "Game over" << endl;
    cout << "Final Score is: " << board->getScore() << endl;
    return 0;
}