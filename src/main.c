#include "bouncy.h"

const float colors[4][3] = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f, 0.f}};

int main() {
	t_scene scene;
	if (!init_scene(&scene)) {
		fprintf(stderr, "Could not initialize scene\n");
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
	scene.last_fps_time = glfwGetTime();
	scene.last_update_time = scene.last_fps_time;
	scene.frames = 0;

	glfwSetWindowUserPointer(window, &scene);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		draw(&scene);
		double now = glfwGetTime();
		scene.dt = (float)(now - scene.last_update_time);
		scene.last_update_time = now;
		if (scene.running)
			update(&scene);

		glfwSwapBuffers(window);
		scene.frames++;
		now = glfwGetTime();
		if (now - scene.last_fps_time >= 1.0) {
			double fps = (double)scene.frames / (now - scene.last_fps_time);
			printf("\rFPS: %.2f", fps);
			fflush(stdout);
			scene.frames = 0;
			scene.last_fps_time = now;
		}
	}
	free_scene(&scene);

	glfwDestroyWindow(window);
	glfwTerminate();
}
