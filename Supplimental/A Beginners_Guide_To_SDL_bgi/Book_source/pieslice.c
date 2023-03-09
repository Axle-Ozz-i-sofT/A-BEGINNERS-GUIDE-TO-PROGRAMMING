/* pieslice example */
/* SDL_bgi-3.0.0\test\ pieslice.c */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> // Comment this line out!

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  int stangle = 45, endangle = 135, radius = 100;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* set fill style and draw a pie slice */
  setfillstyle(EMPTY_FILL, getmaxcolor());
  pieslice(midx, midy, stangle, endangle, radius);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
