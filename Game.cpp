#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <vector>
using namespace std;
//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

const uint32_t RED = 0x000000FF;
const uint32_t GREEN = 0x0000FF00;
const uint32_t BLUE = 0x00FF0000;
const uint32_t WHITE = 0xFFFFFFFF;

struct GameObject
{
    float x;
    float y;
    float dx;
    float dy;
    int size;
    float angle;
};

vector<GameObject> vecGameObjects;
GameObject player; 

float mx[3] = { 0.0f, -2.5f, +2.5f };
float my[3] = { -5.5f, +2.5f, +2.5f };

float sx[3], sy[3];
void WrapCoordinates(float ix, float iy, float& ox, float& oy);
void drawPoint(int x, int y, uint32_t color);
void drawPoint2(int x, int y, uint32_t color);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);



void WrapCoordinates(float ix, float iy, float& ox, float& oy)
{
    ox = ix;
    oy = iy;
    if (ix < 0.0f)	ox = ix + SCREEN_WIDTH;
    if (ix >= SCREEN_WIDTH)	ox = ix - SCREEN_WIDTH;
    if (iy < 0.0f)	oy = iy + SCREEN_HEIGHT;
    if (iy >= SCREEN_HEIGHT) oy = iy - SCREEN_HEIGHT;
}

void drawPoint(int x, int y, uint32_t color)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        buffer[y][x] = color;
    }
    
}

void drawPoint2(int x, int y, uint32_t color)
{
    float fx, fy;
    WrapCoordinates(x, y, fx, fy);
    drawPoint(fx, fy, color);

}


// initialize game data in this function
void initialize()
{
    vecGameObjects.push_back({20.0f, 20.0f, 40.0f, -20.0f,40,0.0f});
    player.x = SCREEN_HEIGHT / 2.0f;
    player.y = SCREEN_WIDTH / 2.0f;
    player.dx = 0.0f;
    player.dy = 0.0f;
    player.angle = 0.0f;
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE)){
        schedule_quit_game();
    }
    
    if (is_key_pressed(VK_LEFT)){
        player.angle -= 5.0f * dt;
    }
    if (is_key_pressed(VK_RIGHT)) {
        player.angle += 5.0f * dt;
    }

    if (is_key_pressed(VK_UP)) {
        player.dx += sin(player.angle) * 20.0f * dt;
        player.dy += -cos(player.angle) * 20.0f * dt;
    }

    player.x += player.dx * dt;
    player.y += player.dy * dt;

    

    for (auto& a : vecGameObjects){
        a.x += a.dx * dt;
        a.y += a.dy * dt;
        WrapCoordinates(a.x, a.y, a.x, a.y);
    }

    //Rotate
    for (int i = 0; i < 3; i++)
    {
        sx[i] = mx[i] * cosf(player.angle) - my[i] * sinf(player.angle);
        sy[i] = mx[i] * sinf(player.angle) + my[i] * cosf(player.angle);
    }

    //Translate
    for (int i = 0; i < 3; i++)
    {
        sx[i] = sx[i] + player.x;
        sy[i] = sy[i] + player.y;
    }

    WrapCoordinates(player.x, player.y, player.x, player.y);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));


  for (auto& a : vecGameObjects)
  {
      for (int x = 0; x < a.size; x++)
      {
          for (int y = 0; y < a.size; y++)
          {
              drawPoint2(a.x + x, a.y + y, WHITE);
          }
      }
  }

  for (int i = 0; i < 4; i++)
  {
      int j = (i + 1);
      drawLine(sx[i%3],sy[i%3],sx[j%3],sy[j%3],WHITE);
  }
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        drawPoint2(x, y, WHITE);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
                px = px + 2 * dy1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    y = y + 1;
                else
                    y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            drawPoint2(x, y, WHITE);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        drawPoint2(x, y,WHITE);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    x = x + 1;
                else
                    x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            drawPoint2(x, y, WHITE);
        }
    }

}

// free game data in this function
void finalize()
{
}

