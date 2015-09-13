#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define TWO_PI 6.28318530718
#define PI 3.14159265359


double SCREEN_WIDTH = 640;
double SCREEN_HEIGHT = 480;


SDL_Window* window = NULL; 
SDL_Renderer* renderer = NULL;

typedef struct {
  double x;
  double y;
  double angle; // radians
  double turning; // radians
  double velocity;
  double max_velocity;
  double acceleration;
} ship_t;

typedef struct {
  double x;
  double y;
} point_t;

ship_t ship;
int running = 1;


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
  double delta_x = ship.velocity * cos(ship.angle);
  double delta_y = ship.velocity * sin(ship.angle);
  ship.x = fmod(ship.x + delta_x + SCREEN_WIDTH, SCREEN_WIDTH);
  ship.y = fmod(ship.y + delta_y + SCREEN_HEIGHT, SCREEN_HEIGHT);
}

void handle_key(SDL_KeyboardEvent key)
{
  switch(key.keysym.sym){

  case SDLK_UP:
    ship.velocity += ship.acceleration;
    if(ship.velocity > ship.max_velocity)
      ship.velocity = ship.max_velocity;
    break;

  case SDLK_DOWN:
    ship.velocity -= ship.acceleration;
    if(ship.velocity < 0)
      ship.velocity = 0;
    break;

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
  ship.x = 100;
  ship.y = 100;
  ship.angle = 0;
  ship.turning = 0.1;
  ship.velocity = 0;
  ship.max_velocity = 0.1;
  ship.acceleration = 0.01;

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


