#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define TWO_PI 6.28318530718
#define PI 3.14159265359


double SCREEN_WIDTH = 1200;
double SCREEN_HEIGHT = 800;


SDL_Window* window = NULL; 
SDL_Renderer* renderer = NULL;

typedef struct {
  double x;
  double y;
  double vel_x;
  double vel_y;
  double angle; // radians
  double turning; // radians
  double max_velocity;
  double acceleration;
} ship_t;

typedef struct {
  double x;
  double y;
} point_t;

ship_t ship;
int running = 1;


double rand2()
{
  return (double)rand() / (double)RAND_MAX ;
}

ship_t random_ship(void)
{
  ship_t new_ship;
  new_ship.x = rand2() * SCREEN_WIDTH;
  new_ship.y = rand2() * SCREEN_HEIGHT;
  new_ship.angle = rand2() * TWO_PI;
  new_ship.turning = 0.1 + rand2() * 0.4;
  new_ship.vel_x = 0;
  new_ship.vel_y = 0;
  new_ship.max_velocity = 0.06 + rand2() * 0.1;
  new_ship.acceleration = 0.005 + rand2() * 0.01;

  return new_ship;
}

void display_ship(void)
{
  SDL_RenderClear(renderer); // Fill render with color
  //Draw a line for now
  point_t tip;
  tip.x = ship.x + 20 * cos(ship.angle);
  tip.y = ship.y + 20 * sin(ship.angle);

  point_t back;
  back.x = ship.x + 5 * cos(ship.angle + PI);
  back.y = ship.y + 5 * sin(ship.angle + PI);

  
  point_t lwing;
  lwing.x = ship.x + 10 * cos(ship.angle + PI - 0.8);
  lwing.y = ship.y + 10 * sin(ship.angle + PI - 0.8);

  point_t rwing;
  rwing.x = ship.x + 10 * cos(ship.angle + PI + 0.8);
  rwing.y = ship.y + 10 * sin(ship.angle + PI + 0.8);


  
  SDL_RenderDrawLine(renderer,
		     tip.x, tip.y,
		     lwing.x, lwing.y);
  SDL_RenderDrawLine(renderer,
		     lwing.x, lwing.y,
		     back.x, back.y);
  SDL_RenderDrawLine(renderer,
		     back.x, back.y,
		     rwing.x, rwing.y);
  SDL_RenderDrawLine(renderer,
		     rwing.x, rwing.y,
		     tip.x, tip.y);


  SDL_RenderPresent(renderer); 

}

void update_state(void)
{
  // Update ship
  ship.x = fmod(ship.x + ship.vel_x + SCREEN_WIDTH, SCREEN_WIDTH);
  ship.y = fmod(ship.y + ship.vel_y + SCREEN_HEIGHT, SCREEN_HEIGHT);
}

void handle_key(SDL_KeyboardEvent key)
{
  switch(key.keysym.sym){

  case SDLK_UP:
    ship.vel_x += ship.acceleration * cos(ship.angle);
    ship.vel_y += ship.acceleration * sin(ship.angle);
    double speed = sqrt(ship.vel_x * ship.vel_x + ship.vel_y * ship.vel_y);
    if(speed > ship.max_velocity){
      ship.vel_x = ship.vel_x / speed *  ship.max_velocity;
      ship.vel_y = ship.vel_y / speed *  ship.max_velocity;
    }
    break;

  /* case SDLK_DOWN: */
  /*   ship.velocity -= ship.acceleration; */
  /*   if(ship.velocity < 0) */
  /*     ship.velocity = 0; */
  /*   break; */

  case SDLK_LEFT:
    ship.angle = fmod(ship.angle - ship.turning + TWO_PI, TWO_PI);
    break;

  case SDLK_RIGHT:
    ship.angle = fmod(ship.angle + ship.turning + TWO_PI, TWO_PI);
    break;

  case SDLK_SPACE:
    printf("pew\n");
    break;

  default:
    break;
  }
}


void handle_events(void)
{
  SDL_Event event;
  /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
  /* more events on the event queue, our while loop will exit when */
  /* that occurs.                                                  */


  while( SDL_PollEvent( &event ) ){
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){
      
    case SDL_KEYDOWN:
      handle_key(event.key);
      break;

    case SDL_QUIT:
      running = 0;
      break;
      
    default:
      break;
    }
  }
}


void game_loop(void)
{
  handle_events();
  update_state();
  display_ship();
}


int main(int argc, char **argv)
{
  srand(time(NULL));
  ship = random_ship();

  //Initialize SDL 
  SDL_Init( SDL_INIT_VIDEO );
  window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  //Wait two seconds 
  /* SDL_Delay( 2000 );	 */
  
  while(running) {
      game_loop();
  }
  
  SDL_Quit();
  return 0;
}


