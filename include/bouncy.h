#ifndef BOUNCY__H
#define BOUNCY__H

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define SCENE_ACCELERATION_X 0
#define SCENE_ACCELERATION_Y 0.981
#define SCENE_REBOUCE 1 - 0.2
#define SCENE_FRICTION 1 - 0.001

#define BALLS_COUNT 25
#define BALLS_ITERATION 32

static float colors[4][3] = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f, 0.f}};

typedef struct vec2 {
	float x;
	float y;
} t_vec2;

typedef struct ball {
	t_vec2 position;
	int	   r;
	t_vec2 speed;
} t_ball;

typedef struct scene {
	float  dt;
	double last_fps_time;
	double last_update_time;
	int	   frames;

	t_ball** balls;
	int		 balls_count;
	int		 balls_iteration;
	float	 rebounce;
	float	 friction;
	t_vec2	 acceleration;
} t_scene;

bool init_scene(t_scene* scene);
void draw(t_scene* scene);
void update(t_scene* scene);
void free_scene(t_scene* scene);

void init_balls(t_scene* scene);
void free_balls(t_ball** balls);
void draw_balls(t_scene* scene);
void update_balls(t_scene* scene);

#endif
