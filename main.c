#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 30000
#define NUMBER_OF_CARS 5

typedef struct {
	WINDOW* window;
	int x, y;
	int rows, cols;
} WIN;

typedef struct {
  int x,y, color, speed;
  char look;
}CAR;
//-------------------------------OKNO-------------------------------------------------
WIN* initiate_window(){
  srand(time(NULL));
  WIN* window = (WIN*)malloc(sizeof(WIN));
  if (window== NULL){
    exit(1);
  }
  getmaxyx(stdscr,window->rows,window->cols);
  if (window->rows<=0 || window->cols <=0)
    exit(1);
  window->cols/=2;
  window->window = newwin(window->rows,window->cols,0,1+(window->cols/2));
  return window;
}
//--------------------------------AUTA------------------------------------------------
int set_position(WIN* window){
    int y = (rand()%(window->rows))+1;
    if (y%2!=1){
      if (y == 0)
        y++;
      else
        y--;
  }
  return y;
}
void position_cars(WIN* window, CAR* cars){
  for (int i = 0;i<NUMBER_OF_CARS;i++){
    cars[i].x = 1;
    cars[i].y = set_position(window);
    while(1){
      int original_y = 1;
      for (int j = 0;j<i;j++){
        if (cars[i].y == cars[j].y)
          original_y = 0;
      }
      if (original_y == 1)
        break;
      else{
        cars[i].y =  set_position(window);
      }
    }
  }
}
CAR* initiate_cars(WIN* window){
  CAR* cars = (CAR*)malloc(sizeof(CAR)*NUMBER_OF_CARS);
  if (cars == NULL){
    free(window);
    exit(1);
  }
  position_cars(window, cars);
  return cars;
}
void move_cars(WIN* window,CAR* cars){
  while(1){
    wclear(window->window);
    box(window->window,0,0);
    mvwprintw(window->window,0,1,"frogger");
    for(int i = 0;i<NUMBER_OF_CARS;i++){
      if ((cars[i].x)>=(window->cols-1)){
        cars[i].x=1;
      }
    mvwprintw(window->window,cars[i].y,cars[i].x,"o");
    (cars[i].x)++;
    }
    wrefresh(window->window);
    usleep(DELAY);
  }
}
//-------------------------------------------------------------------------------------
void free_memory(WIN* window,CAR* cars){
  free(cars);
  free(window);
}
int main(int argc, char* argv[]){
  initscr();
  WIN* window = initiate_window();
  CAR* cars = initiate_cars(window);
  refresh();
  noecho();
  curs_set(FALSE);
  move_cars(window,cars);
  getch();
  free_memory(window,cars);
  endwin();
  return 0;
}
