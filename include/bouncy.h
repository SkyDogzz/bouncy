#ifndef BOUNCY__H
#define BOUNCY__H

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct vec2 {
	float x;
	float y;
} t_vec2;

typedef struct ball {
	t_vec2 position;
	int	   r;
	t_vec2 speed;
	int	   triangle;
	t_vec2 acceleration;
	float  rebouce;
	float  friction;
} t_ball;

#endif
