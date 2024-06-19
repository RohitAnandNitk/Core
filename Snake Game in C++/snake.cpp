#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LENGTH 100

// DIRECTION
const char dir_up = 'U';
const char dir_down = 'D';
const char dir_left = 'L';
const char dir_right = 'R';

// window size
int consoleWidth, consoleHeight;

// initialize screen
void initScreen()
{
  // Handles returned by GetStdHandle can be used by applications that need to read from or write to the console
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  /*
  typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
       COORD      dwSize;
       COORD      dwCursorPosition;
       WORD       wAttributes;
       SMALL_RECT srWindow;
       COORD      dwMaximumWindowSize;
  } CONSOLE_SCREEN_BUFFER_INFO;

  */
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

  /*
    srWindow
    A SMALL_RECT structure that contains the console screen buffer coordinates of the upper-left and lower-right corners of the display window.
  */
}

struct Point
{
  int xCoordinate, yCoordinate;

  Point()
  {
  }
  Point(int x, int y)
  {
    xCoordinate = x;
    yCoordinate = y;
  }
};

class Snake
{
private:
  int length;
  char direction;

public:
  Point body[MAX_LENGTH];
  Snake(int x, int y)
  {
    length = 1;
    body[0] = Point(x, y);
    direction = dir_right;
  }
  int getSnakeLength()
  {
    return length;
  }
  void changeDirection(char newDir)
  {
     if( newDir == dir_up && direction != dir_down)  direction = newDir;
     else if( newDir == dir_down && direction != dir_up)  direction = newDir;
     else if( newDir == dir_left && direction != dir_right)  direction = newDir;
     else if( newDir == dir_right && direction != dir_left)  direction = newDir;
  }
  bool move(Point food)
  {
     for(int i =length -1 ;i>0 ;i--) // here we have not set the head of snake because it depend on direction
     {
         body[i] = body[i-1];
     }
     // now set head of snake
     switch (direction)
     {
       int val;
       case dir_up:  val = body[0].yCoordinate;
                     body[0].yCoordinate =val-1;
                     break;
       case dir_down:  val = body[0].yCoordinate;
                     body[0].yCoordinate =val+1;
                     break;
       case dir_left:  val = body[0].xCoordinate;
                     body[0].xCoordinate =val-1;
                     break;
       case dir_right:  val = body[0].xCoordinate;
                     body[0].xCoordinate =val+1;
                     break;
     }
     // snake bits itself.
      for(int  i= 1;i<length;i++)
      {
        if(body[0].xCoordinate == body[i].xCoordinate &&  body[0].yCoordinate == body[i].yCoordinate) return false; 
      }
      // snake feed food
      if(food.xCoordinate == body[0].xCoordinate && food.yCoordinate == body[0].yCoordinate)
      {
         body[length] = Point(body[length-1].xCoordinate,body[length-1].yCoordinate);
         length++;
      }
      // snake is alive it feed the food
      return true;
  }
  ~Snake()
  {
    delete[] body;
  }
};



class Board{
     Snake *snake;
     const char SNAKE = 'O';
     Point food;
     const char FOOD = 'o';
     int score;
   public:
     Board(){
        spawnFood();
        snake = new Snake(10,10);
        score=0;
     }

     void spawnFood(){
          int x = rand()%consoleWidth;
          int y = rand()%consoleHeight;

          food = Point(x,y);

     }
     void gotoXY(int x,int y)
     {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
     }
     int getScore() {
       return score;
     }
     
     void displayCurrentScore()
     {
        gotoXY(consoleWidth/2,0);
        cout<<"Current Score :"<<score;
     }

     void draw()
     {
      system("cls"); // clear screen
      for(int i=0;i<snake->getSnakeLength();i++)
      {
          gotoXY(snake->body[i].xCoordinate,snake->body[i].yCoordinate);
          cout<<SNAKE;
      }
      gotoXY(food.xCoordinate,food.yCoordinate);
      cout<<FOOD;

      displayCurrentScore();
     }
   
    bool update(){
       bool isAlive = snake->move(food);
        if(isAlive == false) return false;

      if(food.xCoordinate == snake->body[0].xCoordinate && food.yCoordinate == snake->body[0].yCoordinate)
      {
         score++;
         spawnFood();  
      }
        else return true;
    }
    void getInput()
    {
       if(kbhit())
       {
          char key = getch();

          if(key=='w' || key == 'W'){
             snake->changeDirection(dir_up);
          } 
          else if(key == 'a' || key == 'A'){
            snake->changeDirection(dir_left);
          }
          else if(key== 's' || key == 'S'){
            snake->changeDirection(dir_down);
          }
          else if(key== 'd' || key == 'D'){
            snake->changeDirection(dir_right);
          }

       }
    }
     ~Board()
     {
        delete snake;
     }
};


int main()
{
  initScreen();
  Board *board = new Board();
  while(board->update())
  {
        board->getInput();
        board->draw();
        Sleep(100); // 1 sec delay
  }
  cout<<"GAME OVER"<<endl;
  cout<<"Final Score is "<<board->getScore();
  return 0;
}
