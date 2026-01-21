
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <unistd.h>

#include "bouncy.h"

static void key_handler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	int* triangle_count = (int*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (triangle_count && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP) {
			(*triangle_count)++;
		} else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN) {
			if (*triangle_count > 3)
				(*triangle_count)--;
		}
	}
	(void)scancode;
	(void)mods;
}

t_ball* init_balls(t_ball* balls) {
	balls = (t_ball*)malloc(sizeof(t_ball));
	if (!balls)
		return NULL;
	balls->position.x = 0;
	balls->position.y = 0;
	balls->r = 100;
	balls->speed.x = 10;
	balls->speed.y = 10;
	balls->acceleration.x = 0;
	balls->acceleration.y = 1;
	balls->friction = 0.8;
	return balls;
}

void draw_balls(t_ball* balls, int triangle_count) {
	float  fi = (2.0f * M_PI) / triangle_count;
	t_vec2 point[triangle_count + 1];

	for (int i = 0; i <= triangle_count; i++) {
		float a = i * fi;
		point[i].x = balls->r * cosf(a);
		point[i].y = balls->r * sinf(a);
	}

	for (int i = 0; i < triangle_count; i++) {
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex2f(balls->position.x / WINDOW_WIDTH, balls->position.y / WINDOW_HEIGHT);
		glVertex2f((balls->position.x + point[i].x) / WINDOW_WIDTH, (balls->position.y + point[i].y) / WINDOW_HEIGHT);
		glVertex2f((balls->position.x + point[i + 1].x) / WINDOW_WIDTH,
				   (balls->position.y + point[i + 1].y) / WINDOW_HEIGHT);
		glEnd();
	}
}

void update_balls(t_ball* balls) {
	balls->speed.y -= balls->acceleration.y;
	if (balls->speed.x > 0 && (balls->position.x + balls->r) >= WINDOW_WIDTH)
		balls->speed.x = -balls->speed.x * balls->friction;
	else if (balls->speed.x < 0 && (balls->position.x - balls->r) <= -WINDOW_WIDTH)
		balls->speed.x = -balls->speed.x * balls->friction;
	if (balls->speed.y > 0 && (balls->position.y + balls->r) >= WINDOW_HEIGHT)
		balls->speed.y = -balls->speed.y * balls->friction;
	else if (balls->speed.y < 0 && (balls->position.y - balls->r) <= -WINDOW_HEIGHT)
		balls->speed.y = -balls->speed.y * balls->friction;
	balls->position.x += balls->speed.x;
	balls->position.y += balls->speed.y;
}

int main() {
	t_ball* balls = NULL;
	balls = init_balls(balls);
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
	glfwSwapInterval(1);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	glfwSetKeyCallback(window, key_handler);
	double last_fps_time = glfwGetTime();
	int	   frames = 0;
	int	   triangle_count = 32;
	glfwSetWindowUserPointer(window, &triangle_count);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		draw_balls(balls, triangle_count);
		update_balls(balls);

		glfwSwapBuffers(window);
		frames++;
		double now = glfwGetTime();
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
