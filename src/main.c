#include "bouncy.h"

const float colors[6][3] = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f},
                            {1.f, 0.f, 1.f}, {0.f, 1.f, 1.f}, {1.f, 1.f, 0.f}};

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  t_scene *scene = (t_scene *)glfwGetWindowUserPointer(window);
  if (width <= 0 || height <= 0)
    return;
  glViewport(0, 0, width, height);
  if (scene) {
    scene->viewport_width = width;
    scene->viewport_height = height;
  }
}

int main() {
  t_scene scene;
  if (!glfwInit()) {
    fprintf(stderr, "Could not initialize glfw\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_SAMPLES, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  float scale_x = 1.0f;
  float scale_y = 1.0f;
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (monitor)
    glfwGetMonitorContentScale(monitor, &scale_x, &scale_y);
  if (scale_x <= 0.0f)
    scale_x = 1.0f;
  if (scale_y <= 0.0f)
    scale_y = 1.0f;
  int window_width = (int)(WINDOW_WIDTH / scale_x + 0.5f);
  int window_height = (int)(WINDOW_HEIGHT / scale_y + 0.5f);
  if (window_width <= 0)
    window_width = WINDOW_WIDTH;
  if (window_height <= 0)
    window_height = WINDOW_HEIGHT;
  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "GLFW OpenGL", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Could not create window");
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  int fb_width = 0;
  int fb_height = 0;
  glfwGetFramebufferSize(window, &fb_width, &fb_height);
  if (fb_width <= 0)
    fb_width = WINDOW_WIDTH;
  if (fb_height <= 0)
    fb_height = WINDOW_HEIGHT;
  scene.viewport_width = fb_width;
  scene.viewport_height = fb_height;
  glViewport(0, 0, fb_width, fb_height);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  if (!init_scene(&scene)) {
    fprintf(stderr, "Could not initialize scene\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSetWindowUserPointer(window, &scene);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_handler);
  scene.last_fps_time = glfwGetTime();
  scene.last_update_time = scene.last_fps_time;
  scene.frames = 0;

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
