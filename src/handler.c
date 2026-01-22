#include "bouncy.h"

void
key_handler (GLFWwindow *window, int key, int scancode, int action, int mods)
{
  t_scene *scene = (t_scene *)glfwGetWindowUserPointer (window);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose (window, GLFW_TRUE);
    }
  if (scene->balls_iteration && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
      if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP)
        {
          scene->balls_iteration += 1;
        }
      else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN)
        {
          if (scene->balls_iteration > 3)
            scene->balls_iteration -= 1;
        }
    }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
      if (scene->running)
        scene->running = false;
      else
        scene->running = true;
    }
  (void)scancode;
  (void)mods;
}
