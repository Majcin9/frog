#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 30000
#define NUMBER_OF_CARS 5

typedef struct {
	WINDOW* window;
	int y,x;
	int rows, cols;
} WIN;
typedef struct {
  int y,x,color;
  char look;
  int maxy,maxx;
}FROG;

typedef struct {
  int y,x, color, speed;
  char look;
}CAR;
//-------------------------------OKNO-------------------------------------------------
WIN* initiate_window(){
  initscr();
  refresh();
  nodelay(stdscr,TRUE);
  noecho();
  curs_set(FALSE);
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
    if (y%2==0){
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
//-------------------------------------------------------------------------------------
FROG* init_frog(WIN* window,CAR* cars){
  FROG* frog = (FROG*)malloc(sizeof(FROG));
  if (frog == NULL){
    free(window);
    free(cars);
    exit(1);
  }
  frog->x = window->cols/2;
  frog->maxy = frog->y = window->rows-2;
  frog->maxx = window->cols-2;
  frog->look = 'F';
  
  return frog;
}

void move_frog(WIN* window, FROG* frog){
  int input = getch();
  if (input!=ERR){
    if (input == 'a' && frog->x>1){
      frog->x--;
    }
    else if (input == 'd' && frog->x<frog->maxx){
      frog->x++;
    }
    else if (input == 'w' && frog->y>1){
      frog->y--;
    }
    else if (input == 's' && frog->y<frog->maxy){
      frog->y++;
    }
  }
}
//-------------------------------------------------------------------------------------
void free_memory(WIN* window,CAR* cars, FROG* frog){
  free(cars);
  free(window);
  free(frog);
}
void move_cars(WIN* window,CAR* cars,FROG* frog){
  while(1){
    wclear(window->window);
    box(window->window,0,0);
    mvwprintw(window->window,0,1,"frogger");
    for(int i = 0;i<NUMBER_OF_CARS;i++){
      if ((cars[i].x)>=(window->cols-1)){
        cars[i].x=1;
      }
    move_frog(window,frog);
    mvwprintw(window->window,frog->y,frog->x,"F");
    mvwprintw(window->window,cars[i].y,cars[i].x,"o");
    (cars[i].x)++;
    }
    wrefresh(window->window);
    usleep(DELAY);
  }
}
int main(int argc, char* argv[]){
  WIN* window = initiate_window();
  CAR* cars = initiate_cars(window);
  FROG* frog = init_frog(window,cars);
  move_cars(window,cars,frog);
  getch();
  free_memory(window,cars,frog);
  endwin();
  return 0;
}
