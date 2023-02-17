/**
   The Red Pants Game Engine

Okay so I'm gonna level with you, as I write this I am very new to C.
I have a lot of the core functionality of SDL understood through the lense of Pygame.
I am leaning heavily on this video: https://www.youtube.com/watch?v=yFLa3ln16w0

I am starting off with the basics, trying to implement this method i wrote and have been using for a long time in pygame.

this is going to need a lot,
I will be using uthash.h as my dictionary implementation

 */
# include "inputs.c"
# include <string.h>
# include <stdio.h>

# include <SDL2/SDL.h>
# include <SDL2/SDL_timer.h>
# include <SDL2/SDL_image.h>

# define W 1152
# define H 640

SDL_Texture* get_sprite(const char* name);
void spritesheet_load(SDL_Renderer* rend);
void add_input_state(const char*, SDL_Joystick* joy);

int main (void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
      printf("Error initializing SDL2: %s\n", SDL_GetError());
      return 1;
    }

  SDL_Window* screen = SDL_CreateWindow("Long way to the top, if you wanna make a game engine",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					W,
					H,
					0);
  if (!screen)
    {
      printf("error creating window: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
    }

  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(screen, -1, render_flags);
  
  if (!rend)
    {
      printf("error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(screen);
      SDL_Quit();
      return 1;
    }

  spritesheet_load(rend);
  add_input_state("TEST", NULL);
  InputState* testInputs;
  testInputs = get_input_state("TEST");
  
  SDL_Rect dest;

  SDL_Texture* sprite = NULL;
  while (input_update() != -1) {
    SDL_RenderClear(rend);

    if (testInputs->A) {
      sprite = get_sprite("redpantsstand");
      SDL_QueryTexture(sprite, NULL, NULL, &dest.w, &dest.h);
    } else if (testInputs->B) {
      sprite = get_sprite("redpantsrollout");
      SDL_QueryTexture(sprite, NULL, NULL, &dest.w, &dest.h);
    } else {
      sprite = NULL;
    }
    
    SDL_RenderCopy(rend, sprite, NULL, NULL);
    SDL_RenderPresent(rend);
    SDL_Delay(1000/120);
  }

  
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(screen);
  SDL_Quit();
}

