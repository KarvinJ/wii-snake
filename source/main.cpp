#include <grrlib.h>
#include <iostream>
#include <wiiuse/wpad.h>
#include "BMfont3_png.h"
// #include "alien_png.h"
// #include "test_jpg_jpg.h"

#define BLACK 0x000000FF
#define WHITE 0xFFFFFFFF
#define GREEN 0x008000FF
#define RED 0xFF0000FF
#define BLUE 0x0000FFFF

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool isGamePaused;

typedef struct
{
    float x;
    float y;
    float w;
    float h;
    unsigned int color;
} Rectangle;

Rectangle player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, WHITE};
Rectangle ball = {SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 32, 32, 32, WHITE};

const int PLAYER_SPEED = 10;

int ballVelocityX = 4;
int ballVelocityY = 4;

int score;

bool hasCollision(Rectangle &bounds, Rectangle &ball)
{
    return bounds.x < ball.x + ball.w && bounds.x + bounds.w > ball.x &&
           bounds.y < ball.y + ball.h && bounds.y + bounds.h > ball.y;
}

void update()
{
    int padHeld = WPAD_ButtonsHeld(0);

    if (padHeld & WPAD_BUTTON_LEFT && player.x > 0)
    {
        player.x -= PLAYER_SPEED;
    }

    else if (padHeld & WPAD_BUTTON_RIGHT && player.x < SCREEN_WIDTH - player.w)
    {
        player.x += PLAYER_SPEED;
    }

    else if (padHeld & WPAD_BUTTON_UP && player.y > 0)
    {
        player.y -= PLAYER_SPEED;
    }

    else if (padHeld & WPAD_BUTTON_DOWN && player.y < SCREEN_HEIGHT - player.h)
    {
        player.y += PLAYER_SPEED;
    }

    if (ball.x < 0 || ball.x > SCREEN_WIDTH - ball.w)
	{
		ballVelocityX *= -1;

		ball.color = GREEN;
	}

	else if (ball.y < 0 || ball.y > SCREEN_HEIGHT - ball.h)
	{
		ballVelocityY *= -1;

		ball.color = RED;
	}

	else if (hasCollision(player, ball))
	{
		ballVelocityX *= -1;
		ballVelocityY *= -1;

		ball.color = BLUE;

		score++;
	}

	ball.x += ballVelocityX;
	ball.y += ballVelocityY;
}

int main(int argc, char **argv)
{
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // loading images png
    // It fails, but I don't know why
    // GRRLIB_texImg *alien = GRRLIB_LoadTexturePNG(alien_png);

    // loading fonts
    GRRLIB_texImg *blueFonts = GRRLIB_LoadTexture(BMfont3_png);

    // To indicate the font region to load.
    GRRLIB_InitTileSet(blueFonts, 32, 32, 32);

    // Initialise the Wiimotes
    WPAD_Init();

    // Loop forever
    while (true)
    {
        WPAD_SetVRes(0, SCREEN_WIDTH, SCREEN_HEIGHT);
        WPAD_ScanPads(); // Scan the Wiimotes

        int padDown = WPAD_ButtonsDown(0);

        // If HOME was pressed on the first Wiimote, break out of the loop
        if (padDown & WPAD_BUTTON_HOME)
        {
            break;
        }

        if (padDown & WPAD_BUTTON_A)
        {
            isGamePaused = !isGamePaused;
        }

        if (!isGamePaused)
        {
            update();
        }

        GRRLIB_FillScreen(BLACK); // Clear the screen

        // ---------------------------------------------------------------------
        // Place your drawing code below
        // ---------------------------------------------------------------------

        // Draw a img
        // GRRLIB_DrawImg(10, 50, alien, 0, 1, 1, WHITE);

        GRRLIB_Rectangle(player.x, player.y, player.w, player.h, player.color, 1);
        GRRLIB_Rectangle(ball.x, ball.y, ball.w, ball.h, ball.color, 1);

        // displaying text with the loaded fonts.
        if (isGamePaused)
        {
            // Any text has to be written in uppercase if not it doesn't work.
            GRRLIB_Printf(150, 100, blueFonts, WHITE, 1, "GAME PAUSED");
        }

        std::string scoreString = "SCORE: " + std::to_string(score);

        GRRLIB_Printf(185, 20, blueFonts, WHITE, 1, scoreString.c_str());

        GRRLIB_Render(); // Render the frame buffer to the TV
    }

    GRRLIB_FreeTexture(blueFonts); // Be a good boy, clear the memory allocated by GRRLIB

    GRRLIB_Exit();
    exit(0); // Use exit() to exit a program, do not use 'return' from main()
}
