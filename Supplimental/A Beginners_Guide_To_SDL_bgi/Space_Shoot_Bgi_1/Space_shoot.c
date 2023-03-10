//------------------------------------------------------------------------------
// Name:        Space_shoot.c
// Purpose:     Example Game using SDL-BGI graphics.h
// Title:       "Space Shoot SDL-BGI"
//
// Platform:    Win64, Ubuntu64 (No VirtualBox client)
//
// compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     SDL2-devel, SDL_bgi-3.0.0, WaveBeep.exe
//
// Author:      Axle
// Created:     19/01/2023
// Updated:     18/02/2023
// Version:     0.1.2.0 beta
// Copyright:   (c) Axle 2023
// Licence:     MIT No Attribution (MIT-0)
//------------------------------------------------------------------------------
// NOTES:
// This is a first version attempt at a small shooter game using the library
// SDL_Bgi. I have made every effort toward staying with what is provided
// within the SDL_Bgi API without using any of the underlaying SDL2 functions.
// The Game logic progressed as I built upon it and became a little bit
// difficult to follow so I have tidied some of that up although still in need
// of some work.
// I only have a very modest limiter on the CPU cycles in this version so
// FPS is not really controled or restricted. I have corrected this in V2.x.x.x
// The main point was to see what I could acheive with the old BGI API.
// I created this and offer it up as a demo for others who may be looking
// at making use of the graphics.h library. I would consider also looking at
// Version 2.x.x.x as I have corrected and highlighted many of the limitations
// and issues that I had encountered.
// Version 2 also offers a demonstration of using a game intro and outro :)
//
// This will not run well on a VirtualBox Ubuntu guest.
//
// At this time the keyboard repeat delay is enabled. In previous SDL we could
// Control this with SDL_EnableKeyRepeat(), but it does not exist in SDL2.
// I will have to re-impliment the keyboard input handling routines to test for
// a state of keydown, plus the key. Untill then there will be a slight delay
// when holding down the movement keys.[Fixed]
//
// guido.gonzato@gmail.com
// re-impliment SDL_EnableKeyRepeat(). Removed from SDL2.
//extern DECLSPEC int SDLCALL SDL_EnableKeyRepeat(int delay, int interval);
//extern DECLSPEC void SDLCALL SDL_GetKeyRepeat(int *delay, int *interval);
//
// Implimented a workaround using xkbkit() to reset the event buffer, but
// stiil has some glitches.
//
// Add Audio
// https://gist.github.com/armornick/3447121
//
// I made a version using threads and the CLI tool "WavBeep.exe". A simple
// command live tool to play a wave using system("WavBeep.exe .\filname.wav")
// WavBeep.exe makes use of the Win-API PlaySound().
// In Linux versions I will use the commanline "aplay" tool.
// Other similar tools:
// "webmdshow-1.0.4.1" play webm files (includes video) Uses a playback window.
// https://www.webmproject.org/
// http://downloads.webmproject.org/releases/webm/index.html
// FFMpeg, NirCmd, sounder, sWavPlayer,
//
// TODO:
// Slow down shoot intervals at screen lower. [Unresolved]
// Set some int MAX limits for safety.
// Create Linux (Ubuntu) version. [Partial]
// Tidy up some routines. Use logic FLAGS.
// Tweak game logic speed [Done] Monitoring.
// level period [Done]
// level transition.
// Fix fire logic. stop fire when enemy hit. [Monitoring]
// Occasionally I get a random bullet from an enemy (Invisible?) [Monitoring]
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <SDL2/SDL_keyboard.h>

// Turn off compiler warnings for unused variables between (Windows/Linux etc.)
#define unused(x) (x) = (x)

// Most of the game logic is currently locked to the following dimentions.
// do not change them.
#define SCREEN_W 800  // 0 to 799
#define SCREEN_H 600  // 0 to 599

#define SPRITE_W 78  // 0 to 77
#define SPRITE_H 58  // 0 to 57

// Global declarations
int stop;  // FLAG Global required to end BeepWave thread.

typedef struct coord_xy
    {
    int x;  // x
    int y;  // y
    } Coord_xy;

// Function declarations
int fire_sound(void *arg);
int efire_sound(void *arg);
int explode_sound(void *arg);
int Level_Up_sound(void *arg);
int Bonus_sound(void *arg);
int Warpin_sound(void *arg);
int Warpout_sound(void *arg);
int background_sound(void *arg);
void load_resources(void *Back_n[10], void *Warp_n[10], void *Explode_n[10], void *Enemy_n[10], void *Hero);

int main(int argc, char *argv[])
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // Import resources into application memory
    //==========================================================================
    unsigned int ImgSize = 0;  // For creating malloc()
    int i = 0;  // counter for array builing

    // Arrays to hold image data.
    void *Back_n[10];
    void *Hero;
    void *Enemy_n[10];
    void *Explode_n[10];
    void *Warp_n[10];

    // Resource file paths.
    char *f_space[10] = {"./assets/Space/Back_0_800x600.bmp",
                         "./assets/Space/Back_1_800x600.bmp",
                         "./assets/Space/Back_2_800x600.bmp",
                         "./assets/Space/Back_3_800x600.bmp",
                         "./assets/Space/Back_4_800x600.bmp",
                         "./assets/Space/Back_5_800x600.bmp",
                         "./assets/Space/Back_6_800x600.bmp",
                         "./assets/Space/Back_7_800x600.bmp",
                         "./assets/Space/Back_8_800x600.bmp",
                         "./assets/Space/Back_9_800x600.bmp",
                        };

    char *f_playerShip[1] = {"./assets/Player/playerShip3_green_78x58.bmp"};

    char *f_enemyShip[10] = {"./assets/Enemy/enemyBlack2_78x58.bmp",
                             "./assets/Enemy/enemyBlue2_78x58.bmp",
                             "./assets/Enemy/enemyBlack1_78x58.bmp",
                             "./assets/Enemy/enemyRed2_78x58.bmp",
                             "./assets/Enemy/enemyGreen2_78x58.bmp",
                             "./assets/Enemy/enemyBlue1_78x58.bmp",
                             "./assets/Enemy/enemyGreen1_78x58.bmp",
                             "./assets/Enemy/enemyRed1_78x58.bmp",
                             "./assets/Enemy/enemyRed4_78x58.bmp",
                             "./assets/Enemy/ufoYellow_78x58.bmp",
                            };

    char *f_explode[10] = {"./assets/Explode/bubble_explo1_78x58.bmp",
                           "./assets/Explode/bubble_explo2_78x58.bmp",
                           "./assets/Explode/bubble_explo3_78x58.bmp",
                           "./assets/Explode/bubble_explo4_78x58.bmp",
                           "./assets/Explode/bubble_explo5_78x58.bmp",
                           "./assets/Explode/bubble_explo6_78x58.bmp",
                           "./assets/Explode/bubble_explo7_78x58.bmp",
                           "./assets/Explode/bubble_explo8_78x58.bmp",
                           "./assets/Explode/bubble_explo9_78x58.bmp",
                           "./assets/Explode/bubble_explo10_78x58.bmp"
                          };

    char *f_warp[10] = {"./assets/Warp/warp0_78x58.bmp",
                        "./assets/Warp/warp1_78x58.bmp",
                        "./assets/Warp/warp2_78x58.bmp",
                        "./assets/Warp/warp3_78x58.bmp",
                        "./assets/Warp/warp4_78x58.bmp",
                        "./assets/Warp/warp5_78x58.bmp",
                        "./assets/Warp/warp6_78x58.bmp",
                        "./assets/Warp/warp7_78x58.bmp",
                        "./assets/Warp/warp8_78x58.bmp",
                        "./assets/Warp/warp9_78x58.bmp"
                       };


    // Get Sprite image to RAM
    // Set the SDL windows options.
    setwinoptions("Load image to RAM 78x58", // char *title
                  SDL_WINDOWPOS_CENTERED, // int x
                  SDL_WINDOWPOS_CENTERED, // int y
                  SDL_WINDOW_HIDDEN);  // -1 | SDL_WINDOW_HIDDEN

    int Win_ID_0 = initwindow(SPRITE_W, SPRITE_H);  // 78x58

    //==========================================================================
    // Reading an image file as a once off outside of a loop is convenient, but
    // best not used inside of a maine loop as it reads and loads from the drive.
    //void readimagefile (char *filename, int x1, int y1, int x2, int y2 );
    //Reads a .bmp file and displays it immediately at (x1, y1 ). If (x2, y2 ) are not 0, the
    //bitmap is stretched to fit the rectangle x1, y1, x2, y2 ; otherwise, the bitmap is clipped
    //as necessary.
    //==========================================================================
    // Create images in RAM
    // This places image files into RAM for faster access rather than loading
    // the BMP file in every cycle of the loop with readimagefile(). Reading the
    // file from a drive into memory is extremely slow and puts excess load on
    // the HDD/SSD.

    //==========================================================================
    // Load hero ship graphic.
    readimagefile(f_playerShip[0], 0, 0, getmaxx(), getmaxy());// x, y, W, H
    ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
    Hero = malloc( ImgSize);
    getimage (0, 0, getmaxx(), getmaxy(), Hero);

    //==========================================================================
    // Load enemy ships graphic.
    // explode images x 10
    for(i = 0; i < 10; i++)
        {
        readimagefile(f_enemyShip[i], 0, 0, getmaxx(), getmaxy());// x, y, W, H
        ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
        Enemy_n[i] = malloc( ImgSize);
        getimage (0, 0, getmaxx(), getmaxy(), Enemy_n[i]);
        }

    // Load explode graphic.
    // explode images x 10
    for(i = 0; i < 10; i++)
        {
        readimagefile(f_explode[i], 0, 0, getmaxx(), getmaxy());// x, y, W, H
        ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
        Explode_n[i] = malloc( ImgSize);
        getimage (0, 0, getmaxx(), getmaxy(), Explode_n[i]);
        }

    // Load warp graphic.
    // warp images x 10
    for(i = 0; i < 10; i++)
        {
        readimagefile(f_warp[i], 0, 0, getmaxx(), getmaxy());// x, y, W, H
        ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
        Warp_n[i] = malloc( ImgSize);
        getimage (0, 0, getmaxx(), getmaxy(), Warp_n[i]);
        }

    closewindow(Win_ID_0);  // Closes the temporary 78x58 window.


    // (Re)Set the SDL windows options.
    // This is our main game window options.
    setwinoptions("Load image to RAM 800x600", // char *title
                  SDL_WINDOWPOS_CENTERED, // int x
                  SDL_WINDOWPOS_CENTERED, // int y
                  SDL_WINDOW_HIDDEN);  // - 1 | SDL WINDOW HIDDEN

    // intiate the graphics driver and main game window size.
    int Win_ID_1 = initwindow(SCREEN_W, SCREEN_H);

    // Get Main screen Background graphic.
    // Background images x 10
    for(i = 0; i < 10; i++)
        {
        readimagefile(f_space[i], 0, 0, getmaxx(), getmaxy());// x, y, W, H
        ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
        Back_n[i] = malloc( ImgSize);
        getimage (0, 0, getmaxx(), getmaxy(), Back_n[i]);
        }

    // NOTE! These window IDs remain active in SDL under the BGI library.
    // A max of ~14 window IDs are allowed in any SDL-BGI session.
    closewindow(Win_ID_1);  // Closes the temporary 800x600 window.

    // END Copy Images to RAM
    //==========================================================================

    // Begin main game window
    // Sets the window title title, the initial position to (x, y ), and SDL2 flags ORed together.
    // x and y can be set to SDL WINDOWPOS CENTERED or SDL WINDOWPOS UNDEFINED.
    // If title is an empty string, the window title is set to the default value SDL bgi.
    // If either x or y is -1, the position parameters are ignored.
    // If flags is -1, the parameter is ignored; otherwise, only the values SDL WINDOW FULL-
    // SCREEN, SDL WINDOW FULLSCREEN DESKTOP, SDL WINDOW SHOWN, SDL WINDOW HIDDEN, SDL WINDOW-
    // BORDERLESS, and SDL WINDOW MINIMIZED can be applied
    setwinoptions("Battlestar Gatica SDL-BGI (800x600)", // char *title
                  SDL_WINDOWPOS_CENTERED, // int x
                  SDL_WINDOWPOS_CENTERED, // int y
                  -1);

    int Win_ID_2 = initwindow(SCREEN_W, SCREEN_H);

    // Set to fast = manual refresh.
    // It defaults to fast, so I don't think this is needed.
    sdlbgifast();  // sdlbgiauto(void)

    int Back_count = 0;  // select game backgrounds.
    putimage (0, 0, Back_n[Back_count], COPY_PUT);  // First backgound image

    // Do intro screen
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 6 );
    setcolor (GREEN);
    outtextxy (50, 100, "BATTLESTAR GATICA" );
    setcolor (LIGHTGREEN);  //LIGHTGREEN
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 2 );
    outtextxy (50, 200, "You are a Viper pilot for the colony fleet," );
    outtextxy (50, 250, "Protect the GATICA from the Cylox Raiders." );
    //settextstyle (DEFAULT_FONT, HORIZ_DIR, 1 );
    setcolor (LIGHTBLUE);
    outtextxy (180, 350, "Use <-- Left and Right -->" );
    outtextxy (185, 400, "To control the Viper ship" );
    outtextxy (230, 450, "Space bar to shoot" );
    outtextxy (270, 500, "'Q' to quit" );

    refresh();  // refresh the screen to display the background and text.

    SDL_Delay(5000);  // Pause to allow time to read. could use getch or kbhit.

    //==========================================================================

    // All Graphics.h, SDL functions must come after the initwindow() or initgraph()
    // Maxx/y uses the array dimensions , not the screen width, aka screen -1.
    int maxx = getmaxx();
    int maxy = getmaxy();
    // Get mid position in x and y -axis
    int midx = maxx / 2;
    //int midy = maxy / 2;  // not currently used.

    // Variables for main()
    stop = 1;  // FLAG required to end BeepWave thread. (1 = TRUE)

    // Intializes random number generator.
    time_t t;  // Seed for random.
    srand((unsigned) time(&t));

    // Set the main loop refresh rate
    int wait = 1;  // Uses an SDL Sleep function to lower the CPU usage.
    //int key_delay = 1;  // Sleep Hack to correct missed keys.

    int Key_Event = 0;  // Retreve key "Events"
    int ev;  // eventtype();  // return (bgi_last_event);
    //int mc;  // mouseclick();

    //Set the initial start possition for hero ship.
    // Top Left corner of image.
    Coord_xy hero_xy = { .x = midx - 39, .y = maxy - 59 };
    //int hero_xy.x = midx - 39;  // half the width/height of the sprite.
    //int hero_xy.y = maxy - 59;

    // Variables for hero fire.
    //int Hero_shoot_flag = 0;
    Coord_xy Hero_shoot = { .x = 0, .y = 0 }; // Variable for Hero fire.


    // Game display stats.
    int Hero_Points = 0;  // Score
    char Score[32] = {'\0'};
    int Hero_life = 5;  // Herolife
    int bonus_life = 1;  // counter for life bonus * 1000
    char Herolife[32] = {'\0'};
    int level = 1; // current level (stops at 10 at the moment)
    char Levelup[32] = {'\0'};
    int Game_Over = 0;

    //==========================================================================

    // Variables for Enemy, Enemy fire.
    // Random x, for Enemy sprite. 78x58px 78+6+6 = 90
    int ernd = 6 + (rand() % (maxx - 90));  // I am using a 6 pixel padding
    // start enemy at a random x location
    // Leaving a 30 px y padding for game info (Text).
    Coord_xy enemy_xy = { .x = ernd, .y = 30 };

    Coord_xy Enemy_shoot = { .x = 0, .y = 0, }; // Variable for Enemy fire.
    int Enemy_shoot_rnd = 0;  // Random number 0 to 999.
    // if Enemy_shoot_rnd above Shoot_freq threshold then shoot.
    int Shoot_freq = 997;  // Enemy shoot threshold (997 recomended)
    int Enemy_shoot_flag = 0;  // FLAG set to 1 when enemy is shooting.
    int sfreq_reduce = 0;  // Flag to reduce shoot frequence neer screen bottom.

    int Enemy_counter = 0;  // Counts total enemy ships. used for level up intervals.

    int Enemy_n_sel = 0;  // rnd ememy ship select from Enemy_n[Enemy_n_sel]

    int e_LR_direction = 0;  // Random choose x left or right direction.
    // randomly sets how far the enemy will move in either direction using e_LR_direction.
    // the random number is biased to keep it moving left or right for a longer
    // number of loop cycles. This stops the sprite from jittering side to side
    // in a narraw space, but still keeps some randomness as to how far it will
    // move left or right.
    int e_LR_dir_delay;  // this is from a random generated number.

    // Uses random between 0 and 99 from e_LR_dir_delay.
    // If e_LR_dir_delay greater than threshold, rand change direction (e_LR_direction).
    // a higher value the wider the LR movevement.
    int e_LR_dir_change_threshhold = 98;  // recomend 98

    // The following control the speed of the enemy ship.
    // Misses 6 CPU cycles befor incrementing enemy_xy.y + step_y.
    int CPU_LOW = 1;  // when off CPU core is alowed 100%
    int downdelay = 6;  // Faster 0 to 6 Slower
    int edelay_y = 0;  // downdelay counter for downdelay
    // Set the number of x,y pixels to move each CPU cycle.
    int step_x = 1;  // Set px movement steps in px( Higher value for more speed)
    int step_y = 1;  // Set px movement steps in px( Higher value for more speed)

    // Controls enemy life + explode animations.
    // Each image is displayed 10 times for each 10 CPU cycles untill all
    // 10 images have been displayed (100 frames).
    int estart = 0;  // FLAG to set warp in for new enemy.
    // Consider changing this to a flag + counter to 100....
    int elife = 100;  // Temp variable for "Is enemy alive", and explosion routine"
    int ewarp = 100;  // Counter for enemy warp animation.

    // Temp array enumeration/counters for image animations.
    int E_explode_image_cnt = 0;  // Enemy explode animate (10 images)
    int warpout_image_cnt = 0;  // Enemy Warp out animate (10 images)
    int warpin_image_cnt = 0;  // Enemy Warp in animate (10 images)
    int H_explode_image_cnt = 0;  // Hero Enemy explode animate (10 images)

    int Hero_explode_flag = 0;  // FLAG
    int hexplode = 0;  // Hero explasion animation count.

    // Set up seperate thread for audio. I may need back music, fire and explode.
    // I may not need to declare and get return ID in this instance.
    //SDL_Thread *Fire_Thread;  // Set up seperate thread for audio.
    //SDL_Thread *EFire_Thread;  // Set up seperate thread for audio.
    //SDL_Thread *Explode_Thread;  // Set up seperate thread for audio.
    //SDL_Thread *Background_Thread;  // Set up seperate thread for audio.

    //Background_Thread = SDL_CreateThread(background_sound, NULL, NULL); //Thread name, data to send
    SDL_CreateThread(background_sound, NULL, NULL);

    //==========================================================================
    // main loop
    // NOTE! SDL_BGI.h #define kbhit k_bhit | kbhit() is in <conio.h> or
    // windows.h as _kbhit()
    // kbhit() is for the console windows, and xkbhit() is for the graphic window.
    // But can occassional produce undefined behavior from both.
    while (stop)// When our QUIT key is pressed, stop flag is set to 0
        {
        // No Aplha channel available.
        // Display our image previously stored in RAM.
        // COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT
        putimage (0, 0, Back_n[Back_count], COPY_PUT);  // Render Background image.
        //Bitmap.bmp only. See: SDL2_Image

        //======================================================================
        // Set Game stats display.
        //void settextstyle (int font, int direction, int charsize );
        settextstyle (DEFAULT_FONT, HORIZ_DIR, 3 );  // Can be moved outside of the loop.
        //Sets the text font ( bitmap font DEFAULT FONT and vector fonts TRIPLEX FONT, SMALL-
        //FONT, SANS SERIF FONT, GOTHIC FONT, SCRIPT FONT, SIMPLEX FONT, TRIPLEX SCR FONT),
        //the text direction (HORIZ DIR, VERT DIR), and the size of the characters.
        //charsize is a scaling factor for the text (max. 10). If charsize is 0, the text will either
        //use the default size, or it will be scaled by the values set with setusercharsize().
        //Experimental feature: if a CHR font is available in the same directory as the running
        //program, it will be loaded and used instead of its internal equivalent.
        //void settextjustify (int horiz, int vert );
        //void gettextsettings (struct textsettingstype *texttypeinfo );
        setcolor (DARKGRAY);
        //setbkcolor (BLACK);
        outtextxy (5, 10, "Score" );
        sprintf(Score, "[%06d]", Hero_Points);
        setcolor (BROWN);
        outtextxy (125, 10, Score );
        setcolor (DARKGRAY);
        outtextxy (340, 10, "Level" );
        sprintf(Levelup, "[%03d]", level);
        setcolor (BROWN);
        outtextxy (455, 10, Levelup );
        setcolor (DARKGRAY);
        outtextxy (605, 10, "Life" );
        sprintf(Herolife, "[%02d]", Hero_life);
        setcolor (BROWN);
        outtextxy (700, 10, Herolife );
        setcolor (WHITE);

        // Just a hack to clear the screen except for
        // background before displaying Game Over text.
        if (Game_Over == 0)
            {

            //==================================================================
            // Do Game logic

            // Do hero ship display and explode.
            // Movement logic is in events tests.
            if(Hero_explode_flag > 0)  // Explode hexplode Hero_explode_flag
                {
                //printf("HERO_EXPLODE!\n");

                if (hexplode < 30)  //((hexplode > 0) && (hexplode < 50))
                    {
                    // Fade out hero ship 5 images (50 frames)
                    putimage (hero_xy.x, hero_xy.y, Hero, OR_PUT);
                    H_explode_image_cnt = (int)(hexplode / 10);
                    putimage (hero_xy.x, hero_xy.y, Explode_n[H_explode_image_cnt], OR_PUT);
                    hexplode++;
                    }
                else if ((hexplode >29) && (hexplode < 100))
                    {
                    // Fade out hero ship 5 images (50 frames)
                    H_explode_image_cnt = (int)(hexplode / 10);
                    putimage (hero_xy.x, hero_xy.y, Explode_n[H_explode_image_cnt], OR_PUT);
                    hexplode++;
                    }
                else
                    {
                    Hero_life--;  // decrement hero life.
                    if (Hero_life == 0)  // if hero life == 0. Dead hero.
                        {
                        // Do game over and clean up
                        Game_Over = 1;
                        }
                    hexplode = 0;  // reset eplode frame count.
                    Hero_explode_flag = 0;  // reset hero is exploding flag.
                    }
                }
            else  // Normal hero ship dislay.
                {
                // Place hero image on the screen at x, y.
                putimage (hero_xy.x, hero_xy.y, Hero, OR_PUT);  // COPY_PUT, 2"XOR_PUT", 1"OR_PUT"
                }


//==============================================================================
            // Enemy sprite x movement logic.
            if ( elife > 99)  // TODO: Maybe change this to a flag and use a seperate counter.
                {
                e_LR_dir_delay = rand() % 100;  // sets wider random movement.
                // if the rnd value is above the threshold, do rnd change direction.
                if (e_LR_dir_delay > e_LR_dir_change_threshhold )
                    {
                    e_LR_direction = rand() % 2;  // returns 0|1
                    }
                else  // continue moving in the previous L R direction until rnd threshold.
                    {
                    if (e_LR_direction > 0)  // 1 = Move Right
                        {
                        if (enemy_xy.x < maxx - 90 - step_x) // R Padding test
                            {
                            enemy_xy.x = enemy_xy.x + step_x;
                            }
                        }
                    else  // 01 = Move Left
                        {
                        if (enemy_xy.x > 6 + step_x) // L Padding test
                            {
                            enemy_xy.x = enemy_xy.x - step_x;
                            }
                        }
                    }


                // Enemy Downward Y movement logic.
                // Test if enemy has made it past hero.
                if (enemy_xy.y < maxy - (58 + step_y))  // 78x58
                    {
                    if (edelay_y >= downdelay)
                        {
                        enemy_xy.y += step_y;  // Downward steps
                        edelay_y = 0;
                        }
                    else
                        {
                        edelay_y += 1;  // increment delay
                        }
                    }
                else  // if enemy success, remove hero points.
                    {
                    // need to create an enemy win strategy.
                    // remove score points from hero if enemy makes it past
                    if (Enemy_n_sel == 9)  // ship 9 is the UFO
                        {
                        Hero_Points -= 50;
                        }
                    else if (Enemy_n_sel == 8)
                        {
                        Hero_Points -= 20;
                        }
                    else if ( (Enemy_n_sel > 3) && (Enemy_n_sel < 8))
                        {
                        Hero_Points -= 10;
                        }
                    else  // all other ships (for now)
                        {
                        Hero_Points -= 5;
                        }

                    SDL_CreateThread(Warpout_sound, NULL, NULL);
                    // TODO: I need to seperate counters and flags elife.
                    elife--;  // set warp out flags
                    ewarp--;  // set warp out flags
                    }
                }  // END Enemy sprite movement logic.
//==============================================================================
            /*
                        // reduce the enemy shoot frequency neer the lower screen.
                        // The closer the enemy gets to the bottom, the faster the shoot
                        // becomes. In harder levels it is difficult to avoid bullets.
                        if (enemy_xy.y == maxy - 150)
                            {
                            Shoot_freq += 20;
                            sfreq_reduce = 1;
                            }
                            else if ((enemy_xy.y == 31) && (sfreq_reduce == 1))
                            {
                                Shoot_freq -= 20;
                                sfreq_reduce = 0;
                            }
            */

// This next routine needs a logic clean up.
            // Put enemy image + warp and explosion. reset new enemy after dies.
            if ( elife > 99)
                {
                if (estart == 1)
                    {
                    SDL_CreateThread(Warpin_sound, NULL, NULL);
                    }

                if(estart > 50)  // Do warp fade in 5 images (50 frames)
                    {
                    // warp and no ship.
                    //printf("WARP_IN!\n");
                    warpin_image_cnt = (int)(estart / 10);  // sets 10 images for 100 frames.
                    putimage (enemy_xy.x, enemy_xy.y, Warp_n[warpin_image_cnt], OR_PUT);
                    estart--;
                    }
                else if ((estart < 51) && (estart > 0))  // Do warp in 5 images.
                    {
                    // Warp + ship
                    //printf("WARP_IN!\n");
                    putimage (enemy_xy.x, enemy_xy.y, Enemy_n[Enemy_n_sel], OR_PUT);
                    warpin_image_cnt = (int)(estart / 10);  // sets 10 images for 100 frames.
                    putimage (enemy_xy.x, enemy_xy.y, Warp_n[warpin_image_cnt], OR_PUT);
                    estart--;
                    }
                else  // do normal enemy ship placement.
                    {
                    // Ship only.
                    putimage (enemy_xy.x, enemy_xy.y, Enemy_n[Enemy_n_sel], OR_PUT);
                    }
                }
            else  // do explosion/ warp out
                {
                if (elife > 0)
                    {
                    if(elife > 50)  // fade out ship on explode or screen bottom
                        {
                        putimage (enemy_xy.x, enemy_xy.y, Enemy_n[Enemy_n_sel], OR_PUT);
                        }
                    // explosion routine
                    elife--;

                    if(ewarp > 99)  // Explode
                        {
                        //printf("EXPLODE!\n");
                        E_explode_image_cnt = (int)(elife / 10);
                        putimage (enemy_xy.x, enemy_xy.y, Explode_n[E_explode_image_cnt], OR_PUT);
                        }
                    else  // Warp out
                        {
                        //printf("WARP_OUT!\n");
                        ewarp--;
                        warpout_image_cnt = (int)(elife / 10);
                        putimage (enemy_xy.x, enemy_xy.y, Warp_n[warpout_image_cnt], OR_PUT);
                        }
                    }
                else  // reset to start next enemy after explosion/warp out.
                    {
                    // reset enemy and start new enemy. Reset logic.
                    ernd = 6 + (rand() % (maxx - 90));  // I am using a 6 pixel padding
                    // temppory x, y for Enemy sprite. (Random) 78x58
                    //int enemy_xy.x = midx - 39;
                    Enemy_n_sel = rand() % 10;
                    if (Enemy_n_sel == 9)
                        {
                        Shoot_freq -= 20;  // Make UFO shoot more often.
                        }

                    enemy_xy.x = ernd;  // start enemy at a random x location
                    enemy_xy.y = 30;  // Leaving a 40 px padding for game info (Text).
                    elife = 100;
                    ewarp = 100;
                    estart = 100;
                    Enemy_counter++;
                    Enemy_shoot_flag = 0;

                    // Check enemy ship count for level up.
                    if (Enemy_counter > level * 20)  // Level up.
                        {
                        level++;
                        SDL_CreateThread(Level_Up_sound, NULL, NULL);
                        //
                        if (Back_count < 10)
                            {
                            Back_count++;
                            }

                        // Increase difficulty on level up.
                        if (downdelay > 0)  // test for cpu cycles max.
                            {
                            downdelay--;  // increase steps per clock (speed).
                            Shoot_freq -= 2;
                            }
                        // else if (downdelay == 0, step_y < 2) Do CPU_LOW + 0;
                        else if ((downdelay < 1) && (step_y < 3))  // if cpu max, increase steps px
                            {
                            // Increase difficulty on level up.
                            //step_x++;  // Increase step size (speed).
                            step_y++;  // speed up downward steps in px
                            Shoot_freq -= 4;
                            }
                        else
                            {
                            Shoot_freq -= 6;
                            }
                        }

                    // !!! May not be required !!!
                    E_explode_image_cnt = 0;  // Reset explosion image count to 0
                    warpout_image_cnt = 0;  // Reset warp out image count to 0

                    }
                }  // END Put enemy image.




            //==================================================================
            // Hero and Enemy shoot.

            // Hero fire logic. The projectile moves a "step" each loop cycle.
            //Note @800x600 we can use a grid of 10x80 columns 10x60 rows.
            //if (( Hero_shoot.y !=0 ) && (elife > 99))  // duplicate (elife > 99)
            if ( Hero_shoot.y !=0 )  // duplicate (elife > 99)
                {
                setlinestyle(SOLID_LINE, 1, 3);  // Can move this outside of the loop.
                line(Hero_shoot.x, Hero_shoot.y, Hero_shoot.x, Hero_shoot.y - 8);  // -5 set line length in px
                if (Hero_shoot.y >= 20)  // Was 10
                    {
                    Hero_shoot.y = Hero_shoot.y -5;  // Fire step in px
                    }
                else
                    {
                    Hero_shoot.y = 0;
                    }
                }  // END Hero fire logic

            //Set enemy shoot random
            if (Enemy_shoot_flag == 0)
                {
                Enemy_shoot_rnd = rand() % 1000;
                }

            // Reduce the enemy fire interval at the lower screen.
            if (enemy_xy.y > maxy - 150)
                {
                Enemy_shoot_rnd = Enemy_shoot_rnd - (997 - Shoot_freq);
                //Enemy_shoot_rnd = Enemy_shoot_rnd - (int)((997 - Shoot_freq ) * 1);
                }

            // set enemy shoot flag (dont use x, y) // int Shoot_freq = 997
            if ((Enemy_shoot_rnd > Shoot_freq) && (Enemy_shoot_flag == 0) && (Hero_explode_flag == 0))  //Hero_explode_flag = 0
                {
                //EFire_Thread = SDL_CreateThread(efire_sound, NULL, NULL); // This is laggy
                SDL_CreateThread(efire_sound, NULL, NULL); // This is laggy
                Enemy_shoot_flag = 1;
                Enemy_shoot.x = enemy_xy.x + SPRITE_W / 2; // Correction: Change enemy_xy.y to enemy_xy.y.y
                Enemy_shoot.y = enemy_xy.y + SPRITE_H + 1;
                }

            //Enemy_shoot
            if ( Enemy_shoot_flag !=0 )  // duplicate (elife > 99)?
                {
                setlinestyle(SOLID_LINE, 1, 3);  // Can move this outside of the loop.
                setcolor (LIGHTRED);
                line(Enemy_shoot.x, Enemy_shoot.y, Enemy_shoot.x, Enemy_shoot.y + 8);  // +5 set line length in px
                setcolor (WHITE);
                if (Enemy_shoot.y <= (maxy - 10))  // close to screen bottom.
                    {
                    Enemy_shoot.y = Enemy_shoot.y + 2;  // Fire step in px
                    }
                else
                    {
                    Enemy_shoot_flag = 0;
                    Enemy_shoot.y = 0; // DEBUG. Looking for stray bullet.
                    }
                }  // END Hero fire logic

            //==========================================================================
            // Collision detection (Hero)
            // Hero fire at enemy.
            // Test the fire x.y against enemy sprite box x,y,W,H
            if ((Hero_shoot.y > 40 + SPRITE_H) &&(Hero_shoot.x > enemy_xy.x) && (Hero_shoot.x < enemy_xy.x + SPRITE_W)
                    && (Hero_shoot.y > enemy_xy.y) && (Hero_shoot.y < enemy_xy.y + SPRITE_H))
                {
                //Explode_Thread = SDL_CreateThread(explode_sound, NULL, NULL); //Thread name, data to send
                SDL_CreateThread(explode_sound, NULL, NULL); //Thread name, data to send

                if (elife > 99)  // Enemy life (Stop double shoots)
                    {
                    if (Enemy_n_sel == 9)  // ship 9 is the UFO
                        {
                        Hero_Points += 50;
                        Shoot_freq += 20;  // restor normal shoot frequency.
                        }
                    else if (Enemy_n_sel == 8)
                        {
                        Hero_Points += 20;
                        }
                    else if ( (Enemy_n_sel > 3) && (Enemy_n_sel < 8))
                        {
                        Hero_Points += 10;
                        }
                    else  // all other ships (for now)
                        {
                        Hero_Points += 5;
                        }
                    }

                if (Hero_Points > bonus_life * 1000)
                    {
                    SDL_CreateThread(Bonus_sound, NULL, NULL);
                    bonus_life++;  // Set a limit on this !
                    Hero_life += 1;  // Bonus life
                    }
                elife--;  // flag to start enemy explosion. hero life
                Hero_shoot.y = 0;  // stop shoot and reset.
                }
            //==================================================================

            // Collision detection (Enemy).
            // enemy fire at hero.
            // Test the fire x.y against enemy sprite box x,y,W,H
            if ((Enemy_shoot.y < maxy - 10) && (Enemy_shoot.x > hero_xy.x) && (Enemy_shoot.x < hero_xy.x + SPRITE_W)
                    && (Enemy_shoot.y > hero_xy.y) && (Enemy_shoot.y < hero_xy.y + SPRITE_H))
                {
                //Explode_Thread = SDL_CreateThread(explode_sound, NULL, NULL); //Thread name, data to send
                SDL_CreateThread(explode_sound, NULL, NULL); //Thread name, data to send

                Hero_explode_flag = 1;
                //Hero_life--;  // flag to start enemy explosion. Move to afetr explosion routine
                Enemy_shoot_flag = 0;  // stop shoot and reset.
                Enemy_shoot.y = 0;
                Enemy_shoot.x = 0;
                }

            //==================================================================
            // We can use double buffering wich is the standard method to create
            // smooth flowing animations without flicker.
            // Use void sdlbgifast (void); Mode + refresh()
            //swapbuffers(); //swapbuffers is the same as the 4 lines below.
            // Use swpapbuffers or getvisualpage() etc
            int olda = getactivepage();
            int oldv = getvisualpage();
            setvisualpage(olda);
            setactivepage(oldv);
            // refresh(), event(), x|kbhit()
            refresh();  // Note kbhit also preforms a refresh!
            //==========================================================================
            }  // Game Over hack
        else  // Game over routine
            {
            // Print Game over
            settextstyle (DEFAULT_FONT, HORIZ_DIR, 8 );
            setcolor (BROWN);
            outtextxy (80, 200, "GAME OVER!!!" );
            setcolor (WHITE);

            int olda = getactivepage();
            int oldv = getvisualpage();
            setvisualpage(olda);
            setactivepage(oldv);
            refresh();
            SDL_Delay(5000);
            stop = 0;
            }  // End Game Over hack

        if (CPU_LOW == 1 )
            {
            SDL_Delay(wait);  // Keep our CPU use low.
            }

        //==========================================================================
        // Main event handlers. Handle keyboard and mouse.
        event();
        //Returns 1 if one of the following events has occurred: SDL KEYDOWN, SDL MOUSEBUT-
        //TONDOWN, SDL MOUSEWHEEL, or SDL QUIT; 0 otherwise.
        ev = eventtype();  // return (bgi_last_event);
        //Returns the type of the last event. Reported events are SDL KEYDOWN, SDL MOUSEMOTION,
        //SDL MOUSEBUTTONDOWN, SDL MOUSEBUTTONUP, SDL MOUSEWHEEL, and SDL QUIT.

        // Check for mouse events.
        if (ev == SDL_MOUSEBUTTONDOWN)  // Not used in this game.
            {
            //mc = mouseclick();
            //printf("SDL_MOUSEBUTTONDOWN\n");
            //mclick = mouseclick ();
            //switch (mclick)
            //    {
            //    case WM_MOUSEMOVE:
            //        ...

            if (ismouseclick (WM_LBUTTONDOWN))  // SDL_BUTTON_LEFT
                {
                //printf("WM_LBUTTONDOWN\n");
                ;
                }
            else if (ismouseclick (WM_MBUTTONDOWN))  // SDL_BUTTON_RIGHT
                {
                //printf("WM_MBUTTONDOWN\n");
                ;
                }
            else if (ismouseclick (WM_RBUTTONDOWN))  // SDL_BUTTON_MIDDLE
                {
                //printf("WM_RBUTTONDOWN\n");
                ;
                }
            else
                {
                ;  // dummy
                }
            }
        else if (ev == WM_MOUSEMOVE)  // SDL_MOUSEMOTION
            {
            // get x,y
            //printf("WM_MOUSEMOVE\n");
            ;  // dummy
            }
        else if (ev == SDL_KEYDOWN)  // Check for keyboard events
            {
            // This section was somewhat difficult to do outside of SDL2 and
            // using only the SDL-BGI library. The key repeat rate is locked so
            // I found a small work around :)
            // SDL Keydown and Key Events can't be cleared or released from
            // SDL-BGI. As a workaround I am using kbhit(). xkbhit() to clear
            // the last event to -1 by reading the keyboard buffer (bgi_last_key_pressed).
            // getch(); // using getch() introduces keyboard repeat delay.
            xkbhit();
            //xkbhit();
            //SDL_Delay(key_delay); // Sometimes xkbhit() skips. Can slow down motion with higher value.

            // SDL routine derived from SDL_bgi.c event() bgi_last_key_pressed = (int) event.key.keysym.sym;
            // and SDLKey -- Keysym definitions.
            // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
            // https://wiki.libsdl.org/SDL2/SDL_Keycode
            Key_Event = lastkey();  // get the last keyboard event.

            switch (Key_Event)
                {
                //case SDLK_UP:
                //    printf("KEY_UP\n");    // DEBUG
                //    break;
                case SDLK_LEFT:
                    //printf("KEY_LEFT\n");  // DEBUG
                    // Hero movement logic.
                    if ( (hero_xy.x > 6) && (hero_xy.x < maxx - SPRITE_W -6))
                        {
                        hero_xy.x = hero_xy.x -5;
                        if ( (hero_xy.x <= 6) || (hero_xy.x >= maxx - SPRITE_W -6))
                            {
                            hero_xy.x = hero_xy.x +5;
                            }
                        }
                    break;
                case SDLK_RIGHT:
                    //("KEY_RIGHT\n");  // DEBUG
                    // Hero movement logic.
                    if ( (hero_xy.x > 6) && (hero_xy.x < maxx - SPRITE_W -6))
                        {
                        hero_xy.x = hero_xy.x +5;
                        if ( (hero_xy.x <= 6) || (hero_xy.x >= maxx - SPRITE_W -6))
                            {
                            hero_xy.x = hero_xy.x -5;
                            }
                        }
                    break;
                case SDLK_q: // 'q'
                    //printf("q\n");  // DEBUG
                    stop = 0;
                    break;
                case SDLK_SPACE: // ' '
                    //printf("SPACE\n");  // DEBUG
                    // Set fire position and fire on flag from Hero position.
                    //if ((Hero_shoot.y == 0) && (elife > 99))  // duplicate (elife > 99)?
                    if (Hero_shoot.y == 0)
                        {
                        // Use SDL Multithreading for sound.
                        //Fire_Thread = SDL_CreateThread(fire_sound, NULL, NULL); //Thread name, data to send
                        SDL_CreateThread(fire_sound, NULL, NULL); //Thread name, data to send
                        Hero_shoot.x = hero_xy.x + SPRITE_W / 2;
                        Hero_shoot.y = hero_xy.y - 1;
                        }
                    break;
                default:  // No case match found
                    //printf("DEFAULT\n");  // DEBUG
                    break;
                }
            }
        else
            {
            if (ev == QUIT)  // SDL_QUIT
                {
                printf("QUIT\n");
                stop = 0;
                //break;
                }
            }
        //ev = 0;  // May not be required, Used as DEBUG to stop key repeats.
        //Key_Event = 0;  // May not be required, Used as DEBUG to stop key repeats.
        }  // END main loop

    //==========================================================================
    // Clean up tasks and shut down.

    // End Audio tasks.
    // Wait for thread to finish. Can't use this :(
    //SDL_WaitThread(Background_Thread, NULL);
    // Windows
    #ifdef _WIN32
    system("Taskkill /IM WavBeep.exe  /F"); // kind of ugly hack here :(
    // Using TaskKill in this instance is safe as I know the child process
    // WavBeep.exe and the wave file will be cleared from the Windows memory.
    // Otherwise using this could leave data fragments orphened in the system
    // memory heap.
    
    #elif __linux__
    // Ubuntu
    system("pkill -9 aplay"); // kind of ugly hack here :( -9 task kill, -15 sigterm
    #else
    #endif
    

    // Always release dynamic memory!
    free(Hero);

    for(i = 0; i < 10; i++)
        {
        free(Enemy_n[i]);
        }

    for(i = 0; i < 10; i++)
        {
        free(Explode_n[i]);
        }

    for(i = 0; i < 10; i++)
        {
        free(Warp_n[i]);
        }

    for(i = 0; i < 10; i++)
        {
        free(Back_n[i]);
        }

    // deallocate memory allocated for graphic screen
    closewindow(Win_ID_2);
    closegraph();

    return 0;
    }



//==========================================================================

//#ifdef _WIN32
//TerminateThread(SDL_GetThreadID(t), 0);
//#elif __linux__
//pthread_kill(SDL_GetThreadID(t), 0);
//#else
//#endif


// https://gist.github.com/ghedo/963382/
// Linux: (Built in aplay)
// aplay [flags] [filename [filename]] ...
// aplay a.wav
// aplay /home/Axle1/Music/krank_sounds/magnet_action.wav
// see also popen()
// or see mpg123
//int fire_sound(void *arg)
int fire_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Fire Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/laser8.wav");
    // Linux
    #elif __linux__
    system("aplay --quiet ./assets/Sound/laser8.wav");
    #else
    #endif
    return 0;
    }

int efire_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("EFire Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/laser4.wav");
    //Linux
    #elif __linux__
    system("aplay --quiet ./assets/Sound/laser4.wav");
    #else
    #endif
    return 0;
    }

int explode_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Explode Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/explosionCrunch_000.wav");
    #elif __linux__
    system("aplay --quiet ./assets/Sound/explosionCrunch_000.wav");
    #else
    #endif
    return 0;
    }

int Level_Up_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Explode Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/tone1.wav");
    #elif __linux__
    system("aplay --quiet ./assets/Sound/tone1.wav");
    #else
    #endif
    return 0;
    }

int Bonus_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Explode Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/doorOpen_002.wav");
    #elif __linux__
    system("aplay --quiet ./assets/Sound/doorOpen_002.wav");
    #else
    #endif
    return 0;
    }

int Warpin_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Fire Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/Warpin.wav");
    #elif __linux__
    system("aplay --quiet ./assets/Sound/Warpin.wav");
    #else
    #endif
    return 0;
    }

int Warpout_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("Fire Audio Thread\n");  // DEBUG
    // Windows
    #ifdef _WIN32
    system("WavBeep.exe ./assets/Sound/Warpout.wav");
    #elif __linux__
    system("aplay --quiet ./assets/Sound/Warpout.wav");
    #else
    #endif
    return 0;
    }

int background_sound(void *arg)
    {
    unused(arg);  // Turns off the compiler warning for unused argc, argv
    //printf("background Audio Thread\n");  // DEBUG

    while(stop)  // Unfortunately this plays untill the song finishes at close of program.
        {
        // Windows
        #ifdef _WIN32
        system("WavBeep.exe ./assets/Sound/Ring_Leader_(loop).wav");
        #elif __linux__
        system("aplay --quiet ./assets/Sound/Ring_Leader_loop.wav");
        #else
    #endif
        }
    return 0;
    }

//==========================================================================
