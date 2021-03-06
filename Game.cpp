#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <utility>
#include <map>
#include <ctime>
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
const float COLLIDE_TIME = 2.0f;

struct Game_object
{
    float x;
    float y;
    float dx;
    float dy;
    int size;
    float angle;
};

vector<Game_object> vector_asteroids;
vector<Game_object> vector_bullets;
vector<Game_object> vector_hearts;
vector<int> vector_score;
Game_object player; 


bool is_dead = false;
bool is_acclerate = false;
int h_points = 0;
float mx[3] = { 0.0f, -2.5f, +2.5f };
float my[3] = { -5.5f, +2.5f, +2.5f };

float sx[3], sy[3];
float old_dt = 0;
float collide_dt = 0;
int score = 0;
int old_score = score;
bool is_collide_flag = false;
map<int, vector<pair<float, float>>> digit_map;

vector<pair<float, float>> vecModelShip;
vector<pair<float, float>> vecModelHeart;
vector<pair<float, float>> vecModelFlame;
vector<pair<float, float>> vecModelAsteroid;
vector<pair<float, float>> vecModelBullet;
vector<pair<float, float>> vecModelDigit0;
vector<pair<float, float>> vecModelDigit1;
vector<pair<float, float>> vecModelDigit2;
vector<pair<float, float>> vecModelDigit3;
vector<pair<float, float>> vecModelDigit4;
vector<pair<float, float>> vecModelDigit5;
vector<pair<float, float>> vecModelDigit6;
vector<pair<float, float>> vecModelDigit7;
vector<pair<float, float>> vecModelDigit8;
vector<pair<float, float>> vecModelDigit9;

void wrap_coordinates(float ix, float iy, float& ox, float& oy);
void draw_point(int x, int y, uint32_t color);
void draw_point2(int x, int y, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void draw_model(const vector<pair<float, float>>& vecModelCoordinates,
    float x, float y, float r = 0.0f, float s = 1.0f, uint32_t color = WHITE);
bool is_collide(float ax, float ay, float a_radius, float b_radius ,float bx, float by);
void reset();
void asteroids_random_spawn(vector<Game_object>& );

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
    vecModelDigit0 =
    {
        {-1.0f,0.0f},
        {-1.0f,-2.0f},
        {0.0f,-2.0f},
        {-1.0f,0.0f},
        {0.0f,0.0f},
        {0.0f,-2.0f}
    };
    vecModelDigit1 = 
    {
        {0.0f,0.0f},
        {0.0f,-2.0f},
        {-1.0f,-1.0f},
        {0.0f,-2.0f}

    };
    vecModelDigit2 =
    {
        {0.0f,0.0f},
        {-1.0f,0.0f},
        {0.0f,-1.5f},
        {0.0f,-2.0f},
        {-0.75f,-2.0f},
        {-1.0f,-1.65f},
        {-0.75f,-2.0f},
        {0.0f,-2.0f},
        {0.0f,-1.5f},
        {-1.0f,0.0f}
    };
    vecModelDigit3 =
    {
        {-1.0f,-2.0f},
        {0.0f,-2.0f},
        {-1.0f,-1.0f},
        {0.0f,-1.0f},
        {-1.0f,0.0f},
        {0.0f,-1.0f},
        {-1.0f,-1.0f},
        {0.0f,-2.0f}
    };
    vecModelDigit4 =
    {
        {-1.0f,-2.0f},
        {-1.0f,-1.0f},
        {0.0f,-1.0f},
        {0.0f,0.0f},
        {0.0,-2.0f},
        {0.0f,-1.0f},
        {-1.0f,-1.0f}
    };
    vecModelDigit5 =
    {
        {-1.0f,-.0f},
        {0.0f,-0.5f},
        {-1.0f,-1.0f},
        {-1.0f,-1.5f},
        {-1.0,-2.0f},
        {0.0f,-2.0f},
        {-1.0,-2.0f},
        {-1.0f,-1.5f},
        {-1.0f,-1.0f},
        {0.0f,-0.5f}
    };
    vecModelDigit6 =
    {
        {-1.0f,-.0f},
        {0.0f,0.0f},
        {0.0f,-1.0f},
        {-1.0f,-1.0f},
        {0.0f,-2.0f},
        {-1.0f,-1.0f}
    };
    vecModelDigit7 =
    {
        {-1.0f,-2.0f},
        {0.0,-2.0f},
        {-1.0f,0.0f},
        {0.0,-2.0f}
    };
    vecModelDigit8 =
    {
        {0.0f,0.0f},
        {0.0,-2.0f},
        {-1.0,-2.0f},
        {-1.0f,0.0f},
        {0.0,0.0f},
        {0.0,-1.0f},
        {-1.0,-1.0f},
        {-1.0,0.0f}
    };
    vecModelDigit9 =
    {
        {-1.0f,0.0f},
        {0.0,-1.0f},
        {-1.0,-1.0f},
        {-1.0f,-2.0f},
        {0.0,-2.0f},
        {0.0,-1.0f},

    };
    vecModelShip =
    {
        { 0.0f, -5.0f},
        {-2.5f, +2.5f},
        {+2.5f, +2.5f}
    };

    digit_map.insert(make_pair(0, vecModelDigit0));
    digit_map.insert(make_pair(1, vecModelDigit1));
    digit_map.insert(make_pair(2, vecModelDigit2));
    digit_map.insert(make_pair(3, vecModelDigit3));
    digit_map.insert(make_pair(4, vecModelDigit4));
    digit_map.insert(make_pair(5, vecModelDigit5));
    digit_map.insert(make_pair(6, vecModelDigit6));
    digit_map.insert(make_pair(7, vecModelDigit7));
    digit_map.insert(make_pair(8, vecModelDigit8));
    digit_map.insert(make_pair(9, vecModelDigit9));

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

    int flame_count = 10;
    pair<float, float> old_point = { -2.25f,2.5f };
    vecModelFlame.push_back(old_point);
    for (int i = 1; i < flame_count*2+1; i++)
    {
        float step = 4.5f / (flame_count * 2);
        old_point.first += step;
        if (i % 2 == 0)
        {
            old_point.second -= 0.5f;
        }
        else
        {
            old_point.second += 0.5f;
        }

        vecModelFlame.push_back(old_point);
    }
    vector_score.push_back(0);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    srand(time(0));
    vector<Game_object> vector_new_asteroids;

    if (is_dead)
    {
        reset();
    }

    asteroids_random_spawn(vector_new_asteroids);
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
        is_acclerate = true;
    }
    else
    {
        is_acclerate = false;
    }

    if (is_key_pressed(VK_SPACE)) {
        if (old_dt > 0.25f && !is_collide_flag)
        {
            vector_bullets.push_back({ player.x,player.y,400.0f * sinf(player.angle),-400.0f * cosf(player.angle),5,0 });
            old_dt = 0;
        }
    }

    player.x += player.dx * dt;
    player.y += player.dy * dt;

    wrap_coordinates(player.x, player.y, player.x, player.y);

    if (collide_dt > COLLIDE_TIME && is_collide_flag == true)
    {
        is_collide_flag = false;
    }
    for (auto& a : vector_asteroids){
        if (is_collide(a.x, a.y, a.size, 2.5f, player.x, player.y) && collide_dt > COLLIDE_TIME)
        {
            
            h_points -= 1;
            if (vector_hearts.size() > 0)
            {
                vector_hearts.pop_back();
            }
            collide_dt = 0;
            is_collide_flag = true;
            if (h_points <= 0)
            {
                is_dead = true;
            }
        }
    }

    for (auto& a : vector_asteroids) {
        a.x += a.dx * dt;
        a.y += a.dy * dt;
        wrap_coordinates(a.x, a.y, a.x, a.y);
    }

    for (auto& b : vector_bullets) 
    {
        b.x += b.dx * dt;
        b.y += b.dy * dt;
        wrap_coordinates(b.x, b.y, b.x, b.y);
        b.angle -= 1.0f * dt;

        for (auto& a : vector_asteroids)
        {
            if (is_collide(a.x, a.y, a.size,b.size, b.x, b.y))
            {
                b.x = -10000;
                if (a.size > 16)
                {
                    float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                    float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;

                    vector_new_asteroids.push_back({ a.x, a.y, 50.0f * cosf(angle1),50.0f * sinf(angle1),(int)a.size / 2, 0.0f });
                    vector_new_asteroids.push_back({ a.x, a.y, 50.0f * cosf(angle2),50.0f * sinf(angle2),(int)a.size / 2, 0.0f });
                }
                a.x = -10000;
                score += 1;
            }
        }  
    }

    if (vector_bullets.size() > 0)
    {
        for (auto itr = vector_bullets.begin(); itr != vector_bullets.end();)
        {
            if (itr->x < 1 || itr->y < 1 || itr->x > SCREEN_WIDTH || itr->y > SCREEN_HEIGHT) {
                if (itr != vector_bullets.end()) {
                    itr = vector_bullets.erase(itr);
                }
            }
            else
            {
                ++itr;
            }
        } 
    }

    if (vector_asteroids.size() > 0)
    {
        for (auto itr = vector_asteroids.begin(); itr != vector_asteroids.end();)
        {
            if (itr->x < -100 || itr->y < -100 || itr->x > SCREEN_WIDTH + 100 || itr->y > SCREEN_HEIGHT + 100) {
                if (itr != vector_asteroids.end()) {
                    itr = vector_asteroids.erase(itr);
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

    wrap_coordinates(player.x, player.y, player.x, player.y);
    
    for (auto& a : vector_new_asteroids)
    {
        vector_asteroids.push_back(a);
    }

    if (old_score != score)
    {
        int temp_score = score;
        vector_score.clear();
        int delimetr = 1;
        while (temp_score != 0)
        {
            temp_score = temp_score / delimetr;
            vector_score.push_back(temp_score % 10);
            delimetr *= 10;

        }
        reverse(vector_score.begin(),vector_score.end());
    }   
    old_score = score;

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  for (auto& a : vector_hearts)
  {
      draw_model(vecModelHeart, a.x, a.y, a.angle, a.size, RED);
  }
  for (auto& a : vector_asteroids) { 
      draw_model(vecModelAsteroid, a.x, a.y, a.angle, a.size, YELLOW);
  }
  if (!is_collide_flag)
  {
      draw_model(vecModelShip, player.x, player.y, player.angle, 5.0f);
  }
  else
  {
      draw_model(vecModelShip, player.x, player.y, player.angle, 5.0f,ORANGE);
  }

  if (is_acclerate)
  {
      draw_model(vecModelFlame, player.x, player.y, player.angle,5.0f, RED);
  }
  for (auto& a : vector_bullets) {
      draw_model(vecModelBullet, a.x, a.y, a.angle, a.size);
  }

  for (int i = 0; i < vector_score.size(); i++)
  {
      draw_model(digit_map[vector_score[i]], 150.0f+20*i, 30, 0.0,10.0f);
  }
}

void reset()
{
    finalize();
    h_points  = 3;
    score = 0;
    old_score = 0;
    vector_score.push_back(0);

    for (int i = 0; i < h_points; i++)
    {
        vector_hearts.push_back({ 20.0f + 40.0f * i, 30.0f, 0.0f, 0.0f, 10, 0.0f });
    }
    is_dead = false;
    is_collide_flag = false;
    //vector_asteroids.push_back({ 20.0f, -50.0f, 40.0f,-20.0f,50,0.0f });
    //vector_asteroids.push_back({ -40.0f, -500.0f, -40.0f,20.0f,50,0.0f });
    player.x = SCREEN_HEIGHT / 2.0f;
    player.y = SCREEN_WIDTH / 2.0f;
    player.dx = 0.0f;
    player.dy = 0.0f;
    player.angle = 0.0f;

}

// free game data in this function
void finalize()
{
    vector_asteroids.clear();
    vector_bullets.clear();
    vector_hearts.clear();
    vector_score.clear();
}


bool is_collide(float ax, float ay, float a_radius, float b_radius, float bx, float by)
{
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) <= b_radius + a_radius;

};

float get_distance(float ax, float ay, float bx, float by)
{
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}


void draw_line(int x1, int y1, int x2, int y2, uint32_t color)
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
        draw_point2(x, y, color);
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
            draw_point2(x, y, color);
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
        draw_point2(x, y, color);
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
            draw_point2(x, y, color);
        }
    }

};

void draw_model(const vector<pair<float, float>>& vecModelCoordinates, float x, float y,
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
        draw_line(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second,
            vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, color);
    }
};

void wrap_coordinates(float ix, float iy, float& ox, float& oy)
{
    ox = ix;
    oy = iy;
    if (ix < 0.0f)	ox = ix + SCREEN_WIDTH;
    if (ix >= SCREEN_WIDTH)	ox = ix - SCREEN_WIDTH;
    if (iy < 0.0f)	oy = iy + SCREEN_HEIGHT;
    if (iy >= SCREEN_HEIGHT) oy = iy - SCREEN_HEIGHT;
};

void draw_point(int x, int y, uint32_t color)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        buffer[y][x] = color;
    }

};

void draw_point2(int x, int y, uint32_t color)
{
    float fx, fy;
    wrap_coordinates(x, y, fx, fy);
    draw_point(fx, fy, color);

};

void asteroids_random_spawn(vector<Game_object>& vectorNew)
{
    if (vector_asteroids.size() < 4)
    {
        for (int i = 0; i < 6 - vector_asteroids.size(); i++)
        {
            float x = (float)(-800 + rand() % 1200);
            float y = (float)(-800 + rand() % 1200);
            float dx = (float)(-60 + rand() % 120);
            float dy = (float)(-60 + rand() % 120);
            int size = (float)(20 + rand() % 40);
           
            while ((get_distance(x, y, player.x, player.y) - size)< 400)
            {
                x = (float)(-800 + rand() % 1200);
                y = (float)(-800 + rand() % 1200);
                dx = (float)(-60 + rand() % 120);
                dy = (float)(-60 + rand() % 120);
                size = (float)(16 + rand() % 40);
            }
            vectorNew.push_back({ x,y,dx,dy,size});

            

        }
    }
}