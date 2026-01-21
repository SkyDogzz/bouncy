
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "bouncy.h"

static void key_handler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	int* triangle_count = (int*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (triangle_count && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP) {
			(*triangle_count) += 1;
		} else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN) {
			if (*triangle_count > 3)
				(*triangle_count) -= 1;
		}
	}
	(void)scancode;
	(void)mods;
}

void free_balls(t_ball** balls) {
	int i = 0;
	while (balls[i]) {
		free(balls[i]);
		i++;
	}
	free(balls);
}

t_ball** init_balls(t_ball** balls, int n) {
	balls = (t_ball**)malloc(sizeof(t_ball*) * (n + 1));
	srand(time(NULL));
	if (!balls)
		return NULL;
	balls[n] = NULL;
	for (int i = 0; i < n; i++) {
		balls[i] = (t_ball*)malloc(sizeof(t_ball));
		if (!balls[i]) {
			free_balls(balls);
			return NULL;
		}
		int min_r = 40;
		int max_r = 80;
		balls[i]->r = min_r + (rand() % (max_r - min_r + 1));
		int max_x = WINDOW_WIDTH - balls[i]->r;
		int max_y = WINDOW_HEIGHT - balls[i]->r;
		balls[i]->position.x = (float)((rand() % (max_x * 2 + 1)) - max_x);
		balls[i]->position.y = (float)((rand() % (max_y * 2 + 1)) - max_y);
		int min_speed = -40;
		int max_speed = 40;
		balls[i]->speed.x = min_speed + (rand() % (max_speed - min_speed + 1));
		balls[i]->speed.y = min_speed + (rand() % (max_speed - min_speed + 1));
		balls[i]->acceleration.x = 0;
		balls[i]->acceleration.y = 0.981;
		balls[i]->rebouce = 1 - 0.2;
		balls[i]->friction = 1 - 0.001;
	}
	return balls;
}

float colors[4][3] = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f, 0.f}};

void draw_balls(t_ball** balls, int triangle_count) {
	float	fi = (2.0f * M_PI) / triangle_count;
	t_vec2	point[triangle_count + 1];
	t_ball* ball;
	int		ball_nbr = 0;

	while (balls[ball_nbr]) {
		ball = balls[ball_nbr];
		for (int i = 0; i <= triangle_count; i++) {
			float a = i * fi;
			point[i].x = ball->r * cosf(a);
			point[i].y = ball->r * sinf(a);
		}

		for (int i = 0; i < triangle_count; i++) {
			glBegin(GL_TRIANGLES);
			glColor3f(colors[ball_nbr % 3][0], colors[ball_nbr % 3][1], colors[ball_nbr % 3][2]);
			glVertex2f(ball->position.x / WINDOW_WIDTH, ball->position.y / WINDOW_HEIGHT);
			glVertex2f((ball->position.x + point[i].x) / WINDOW_WIDTH, (ball->position.y + point[i].y) / WINDOW_HEIGHT);
			glVertex2f((ball->position.x + point[i + 1].x) / WINDOW_WIDTH,
					   (ball->position.y + point[i + 1].y) / WINDOW_HEIGHT);
			glEnd();
		}
		ball_nbr++;
	}
}

void update_balls(t_ball** balls, float dt) {
	if (dt > 0.05f)
		dt = 0.05f;
	float time_scale = dt * 60.0f;

	t_ball* ball;

	while (*balls) {
		ball = *balls;
		ball->speed.x -= ball->acceleration.x * time_scale;
		ball->speed.y -= ball->acceleration.y * time_scale;
		float next_x = ball->position.x + ball->speed.x * time_scale;
		float next_y = ball->position.y + ball->speed.y * time_scale;

		if (next_x + ball->r > WINDOW_WIDTH) {
			next_x = WINDOW_WIDTH - ball->r;
			ball->speed.x = -ball->speed.x * ball->rebouce;
		} else if (next_x - ball->r < -WINDOW_WIDTH) {
			next_x = -WINDOW_WIDTH + ball->r;
			ball->speed.x = -ball->speed.x * ball->rebouce;
		}

		if (next_y + ball->r > WINDOW_HEIGHT) {
			next_y = WINDOW_HEIGHT - ball->r;
			ball->speed.y = -ball->speed.y * ball->rebouce;
		} else if (next_y - ball->r < -WINDOW_HEIGHT) {
			next_y = -WINDOW_HEIGHT + ball->r;
			ball->speed.y = -ball->speed.y * ball->rebouce;
			ball->speed.x = ball->speed.x * ball->friction;
		}

		ball->position.x = next_x;
		ball->position.y = next_y;
		balls++;
	}
}

int main() {
	t_ball** balls = NULL;
	balls = init_balls(balls, 100);
	if (!balls) {
		fprintf(stderr, "Could not initialize balls\n");
		return EXIT_FAILURE;
	}

	if (!glfwInit()) {
		fprintf(stderr, "Could not initialize glfw\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLFW OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Could not create window");
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	glfwSetKeyCallback(window, key_handler);
	double last_fps_time = glfwGetTime();
	double last_update_time = last_fps_time;
	int	   frames = 0;
	int	   triangle_count = 32;
	glfwSetWindowUserPointer(window, &triangle_count);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		draw_balls(balls, triangle_count);
		double now = glfwGetTime();
		float  dt = (float)(now - last_update_time);
		last_update_time = now;
		update_balls(balls, dt);

		glfwSwapBuffers(window);
		frames++;
		now = glfwGetTime();
		if (now - last_fps_time >= 1.0) {
			double fps = (double)frames / (now - last_fps_time);
			printf("\rFPS: %.2f", fps);
			fflush(stdout);
			frames = 0;
			last_fps_time = now;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
