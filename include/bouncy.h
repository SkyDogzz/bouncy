#ifndef BOUNCY__H
#define BOUNCY__H

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// #define SCENE_ACCELERATION_X 0
// #define SCENE_ACCELERATION_Y 0.981
// #define SCENE_REBOUCE 1 - 0.2
// #define SCENE_FRICTION 1 - 0.001
//
// #define BALLS_COUNT 25
// #define BALLS_ITERATION 32
#define SCENE_ACCELERATION_X 0
#define SCENE_ACCELERATION_Y 0.1
#define SCENE_REBOUCE 1 - 0.1
#define SCENE_FRICTION 1 - 0.0005

#define BALLS_COUNT 10000
#define BALLS_ITERATION 16

extern const float colors[6][3];

typedef struct vec2 {
  float x;
  float y;
} t_vec2;

typedef struct ball {
  t_vec2 position;
  int r;
  t_vec2 speed;
} t_ball;

typedef struct scene {
  bool running;
  float dt;
  double last_fps_time;
  double last_update_time;
  int frames;
  int viewport_width;
  int viewport_height;

  t_ball **balls;
  int balls_count;
  int balls_iteration;
  float rebounce;
  float friction;
  t_vec2 acceleration;
} t_scene;

void key_handler(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

bool init_scene(t_scene *scene);
void draw(t_scene *scene);
void update(t_scene *scene);
void free_scene(t_scene *scene);

void init_balls(t_scene *scene);
void free_balls(t_ball **balls);
void draw_balls(t_scene *scene);
void update_balls(t_scene *scene);

#endif
