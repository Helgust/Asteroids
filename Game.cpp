#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <utility>
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

const uint32_t RED = 0x00FF0000;
const uint32_t GREEN = 0x0000FF00;
const uint32_t BLUE = 0x000000FF;
const uint32_t WHITE = 0xFFFFFFFF;
const uint32_t YELLOW = 0x00FFFF00;
const uint32_t ORANGE = 0x00FFA500;
const float collide_time = 2.0f;

struct GameObject
{
    float x;
    float y;
    float dx;
    float dy;
    int size;
    float angle;
};

vector<GameObject> vecAsteroids;
vector<GameObject> vecBullets;
vector<GameObject> vecHearts;
GameObject player; 


bool is_dead = false;
int h_points = 0;
float mx[3] = { 0.0f, -2.5f, +2.5f };
float my[3] = { -5.5f, +2.5f, +2.5f };

float sx[3], sy[3];
float old_dt = 0;
float collide_dt = 0;
int score = 0;
bool is_collide_flag = false;

vector<pair<float, float>> vecModelShip;
vector<pair<float, float>> vecModelHeart;
vector<pair<float, float>> vecModelAsteroid;
vector<pair<float, float>> vecModelBullet;

void WrapCoordinates(float ix, float iy, float& ox, float& oy);
void drawPoint(int x, int y, uint32_t color);
void drawPoint2(int x, int y, uint32_t color);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
void drawModel(const vector<pair<float, float>>& vecModelCoordinates,
    float x, float y, float r = 0.0f, float s = 1.0f, uint32_t color = WHITE);
bool IsCollide(float ax, float ay, float a_radius, float b_radius ,float bx, float by);
void reset();

// initialize game data in this function
void initialize()
{
    reset();

    vecModelHeart =
    {
        {+1.0f, -2.5f},
        {+1.5f, -2.0f},
        {+1.5f, -1.0f},
        {0.0f, 0.0f},
        {-1.5f, -1.0f},
        {-1.5f, -2.0f},
        {-1.0f, -2.5f},
        {0.0f, -2.0f}
    };
    vecModelShip =
    {
        { 0.0f, -5.0f},
        {-2.5f, +2.5f},
        {+2.5f, +2.5f}
    };

    int bullet_verts = 5;
    for (int i = 0; i < bullet_verts; i++)
    {
        float radius =1.0f;
        float a = ((float)i / (float)bullet_verts) * 6.28318f;
        vecModelBullet.push_back(make_pair(radius * sinf(a), radius * cosf(a)));
    };
    

    int verts = 20;
    for (int i = 0; i < verts; i++)
    {
        float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
        float a = ((float)i / (float)verts) * 6.28318f;
        vecModelAsteroid.push_back(make_pair(radius * sinf(a), radius * cosf(a)));
    }
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (is_dead)
    {
        reset();
    }
    old_dt += dt;
    collide_dt += dt;
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
        player.dx += sin(player.angle) * 100.0f * dt;
        player.dy += -cos(player.angle) * 100.0f * dt;
    }

    if (is_key_pressed(VK_SPACE)) {
        if (old_dt > 0.25f && !is_collide_flag)
        {
            vecBullets.push_back({ player.x,player.y,200.0f * sinf(player.angle),-200.0f * cosf(player.angle),10,0 });
            old_dt = 0;
        }
    }

    player.x += player.dx * dt;
    player.y += player.dy * dt;

    WrapCoordinates(player.x, player.y, player.x, player.y);

    if (collide_dt > collide_time && is_collide_flag == true)
    {
        is_collide_flag = false;
    }
    for (auto& a : vecAsteroids){
        if (IsCollide(a.x, a.y, a.size, 0, player.x, player.y) && collide_dt > collide_time)
        {
            
            h_points -= 1;
            if (vecHearts.size() > 0)
            {
                vecHearts.pop_back();
            }
            collide_dt = 0;
            is_collide_flag = true;
            if (h_points <= 0)
            {
                is_dead = true;
            }
        }
    }

    for (auto& a : vecAsteroids) {
        a.x += a.dx * dt;
        a.y += a.dy * dt;
        WrapCoordinates(a.x, a.y, a.x, a.y);
    }

    vector<GameObject> vecNewAsteroids;
    for (auto& b : vecBullets) 
    {
        b.x += b.dx * dt;
        b.y += b.dy * dt;
        WrapCoordinates(b.x, b.y, b.x, b.y);
        b.angle -= 1.0f * dt;

        for (auto& a : vecAsteroids)
        {
            if (IsCollide(a.x, a.y, a.size,b.size, b.x, b.y))
            {
                b.x = -10000;
                if (a.size > 8)
                {
                    float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                    float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;

                    vecNewAsteroids.push_back({ a.x, a.y, 50.0f * cosf(angle1),50.0f * sinf(angle1),(int)a.size / 2, 0.0f });
                    vecNewAsteroids.push_back({ a.x, a.y, 50.0f * cosf(angle2),50.0f * sinf(angle2),(int)a.size / 2, 0.0f });
                }
                a.x = -10000;
                score += 1;
            }
        }  
    }

    if (vecBullets.size() > 0)
    {
        for (auto itr = vecBullets.begin(); itr != vecBullets.end();)
        {
            if (itr->x < 1 || itr->y < 1 || itr->x > SCREEN_WIDTH || itr->y > SCREEN_HEIGHT) {
                if (itr != vecBullets.end()) {
                    itr = vecBullets.erase(itr);
                }
            }
            else
            {
                ++itr;
            }
        } 
    }

    if (vecAsteroids.size() > 0)
    {
        for (auto itr = vecAsteroids.begin(); itr != vecAsteroids.end();)
        {
            if (itr->x < 1 || itr->y < 1 || itr->x > SCREEN_WIDTH || itr->y > SCREEN_HEIGHT) {
                if (itr != vecAsteroids.end()) {
                    itr = vecAsteroids.erase(itr);
                }
            }
            else
            {
                ++itr;
            }
        }
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
    
    for (auto& a : vecNewAsteroids)
    {
        vecAsteroids.push_back(a);
    }

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  for (auto& a : vecHearts)
  {
      drawModel(vecModelHeart, a.x, a.y, a.angle, a.size, RED);
  }
  for (auto& a : vecAsteroids) { 
      drawModel(vecModelAsteroid, a.x, a.y, a.angle, a.size, YELLOW);
  }
  if (!is_collide_flag)
  {
      drawModel(vecModelShip, player.x, player.y, player.angle, 5.0f);
  }
  else
  {
      drawModel(vecModelShip, player.x, player.y, player.angle, 5.0f,ORANGE);
  }

  for (auto& a : vecBullets) {
      drawModel(vecModelBullet, a.x, a.y, a.angle, a.size);
  }


}

void reset()
{
    finalize();
    h_points  = 3;
    for (int i = 0; i < h_points; i++)
    {
        vecHearts.push_back({ 20.0f + 40.0f * i, 30.0f, 0.0f, 0.0f, 10, 0.0f });
    }
    is_dead = false;
    is_collide_flag = false;
    vecAsteroids.push_back({ 20.0f, -50.0f, 40.0f,-20.0f,50,0.0f });
    vecAsteroids.push_back({ -40.0f, -500.0f, -40.0f,20.0f,50,0.0f });
    player.x = SCREEN_HEIGHT / 2.0f;
    player.y = SCREEN_WIDTH / 2.0f;
    player.dx = 0.0f;
    player.dy = 0.0f;
    player.angle = 0.0f;

}

// free game data in this function
void finalize()
{
    vecAsteroids.clear();
    vecBullets.clear();
}


bool IsCollide(float ax, float ay, float a_radius, float b_radius, float bx, float by)
{
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) <= b_radius + a_radius;

};


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
        drawPoint2(x, y, color);
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
            drawPoint2(x, y, color);
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
        drawPoint2(x, y, color);
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
            drawPoint2(x, y, color);
        }
    }

};

void drawModel(const vector<pair<float, float>>& vecModelCoordinates, float x, float y,
    float r, float s, uint32_t color)
{
    // pair.first = x coordinate
    // pair.second = y coordinate

    // Create translated model vector of coordinate pairs
    vector<pair<float, float>> vecTransformedCoordinates;
    int verts = vecModelCoordinates.size();
    vecTransformedCoordinates.resize(verts);

    // Rotate
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
        vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
    }

    // Scale
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
    }

    // Translate
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
    }

    // Draw Closed Polygon
    for (int i = 0; i < verts + 1; i++)
    {
        int j = (i + 1);
        drawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second,
            vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, color);
    }
};

void WrapCoordinates(float ix, float iy, float& ox, float& oy)
{
    ox = ix;
    oy = iy;
    if (ix < 0.0f)	ox = ix + SCREEN_WIDTH;
    if (ix >= SCREEN_WIDTH)	ox = ix - SCREEN_WIDTH;
    if (iy < 0.0f)	oy = iy + SCREEN_HEIGHT;
    if (iy >= SCREEN_HEIGHT) oy = iy - SCREEN_HEIGHT;
};

void drawPoint(int x, int y, uint32_t color)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        buffer[y][x] = color;
    }

};

void drawPoint2(int x, int y, uint32_t color)
{
    float fx, fy;
    WrapCoordinates(x, y, fx, fy);
    drawPoint(fx, fy, color);

};