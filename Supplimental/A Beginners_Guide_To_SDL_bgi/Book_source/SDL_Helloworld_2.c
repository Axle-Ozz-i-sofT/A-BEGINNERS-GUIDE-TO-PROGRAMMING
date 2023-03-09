#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
//using namespace std;  // C++

// -lmingw32 -lSDL2main -lSDL2

int main(int argc, char** argv)
    {
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Surface* screenSurface= SDL_GetWindowSurface(window);
//	SDL_Surface* img =SDL_LoadBMP("");
//	SDL_BlitSurface(img,NULL,screenSurface,NULL);
    bool exit=false;
    SDL_Event e;
    SDL_FillRect(screenSurface,NULL, SDL_MapRGB(screenSurface->format,128,0,120));
    SDL_UpdateWindowSurface(window);
    while(!exit)
        {
        while(SDL_PollEvent(&e)!=0)
            {
            if(e.type == SDL_QUIT)
                {
                exit=true;
                }
            if(e.type == SDL_KEYDOWN)
                {
                switch(e.key.keysym.sym)
                    {
                    case SDLK_UP:
                        exit=true;
                        break;
                    }
                }
            }


        }
    //SDL:FreeSurface(img);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    }
