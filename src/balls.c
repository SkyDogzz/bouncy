#include "bouncy.h"

void
free_balls (t_ball **balls)
{
  int i = 0;
  while (balls[i])
    {
      free (balls[i]);
      i++;
    }
  free (balls);
}

void
init_balls (t_scene *scene)
{
  t_ball **balls = scene->balls;
  balls = (t_ball **)malloc (sizeof (t_ball *) * (scene->balls_count + 1));
  if (!balls)
    return;
  scene->balls = balls;
  balls[scene->balls_count] = NULL;
  srand (time (NULL));
  for (int i = 0; i < scene->balls_count; i++)
    {
      balls[i] = (t_ball *)malloc (sizeof (t_ball));
      if (!balls[i])
        {
          free_balls (balls);
          scene->balls = NULL;
          return;
        }
      int min_r = 20;
      int max_r = 100;
      balls[i]->r = min_r + (rand () % (max_r - min_r + 1));
      int max_x = WINDOW_WIDTH - balls[i]->r;
      int max_y = WINDOW_HEIGHT - balls[i]->r;
      balls[i]->position.x = (float)((rand () % (max_x * 2 + 1)) - max_x);
      balls[i]->position.y = (float)((rand () % (max_y * 2 + 1)) - max_y);
      int min_speed = -40;
      int max_speed = 40;
      balls[i]->speed.x = min_speed + (rand () % (max_speed - min_speed + 1));
      balls[i]->speed.y = min_speed + (rand () % (max_speed - min_speed + 1));
    }
}

void
draw_balls (t_scene *scene)
{
  float fi = (2.0f * M_PI) / scene->balls_iteration;
  t_vec2 point[scene->balls_iteration + 1];
  t_ball **balls = scene->balls;
  t_ball *ball;
  int ball_nbr = 0;
  size_t idx = 0;

  if (!scene->program || !scene->vao || !scene->vbo || !scene->vertices)
    return;

  size_t required = (size_t)scene->balls_count * scene->balls_iteration * 3u * 5u;
  if (required > scene->vertex_capacity)
    {
      float *resized = (float *)realloc (scene->vertices, sizeof (float) * required);
      if (!resized)
        return;
      scene->vertices = resized;
      scene->vertex_capacity = required;
      glBindBuffer (GL_ARRAY_BUFFER, scene->vbo);
      glBufferData (GL_ARRAY_BUFFER, scene->vertex_capacity * sizeof (float), NULL, GL_DYNAMIC_DRAW);
    }

  while (balls[ball_nbr])
    {
      ball = balls[ball_nbr];
      for (int i = 0; i <= scene->balls_iteration; i++)
        {
          float a = i * fi;
          point[i].x = ball->r * cosf (a);
          point[i].y = ball->r * sinf (a);
        }

      for (int i = 0; i < scene->balls_iteration; i++)
        {
          const float r = colors[ball_nbr % 6][0];
          const float g = colors[ball_nbr % 6][1];
          const float b = colors[ball_nbr % 6][2];
          float cx = ball->position.x / WINDOW_WIDTH;
          float cy = ball->position.y / WINDOW_HEIGHT;
          float x1 = (ball->position.x + point[i].x) / WINDOW_WIDTH;
          float y1 = (ball->position.y + point[i].y) / WINDOW_HEIGHT;
          float x2 = (ball->position.x + point[i + 1].x) / WINDOW_WIDTH;
          float y2 = (ball->position.y + point[i + 1].y) / WINDOW_HEIGHT;

          scene->vertices[idx++] = cx;
          scene->vertices[idx++] = cy;
          scene->vertices[idx++] = r;
          scene->vertices[idx++] = g;
          scene->vertices[idx++] = b;
          scene->vertices[idx++] = x1;
          scene->vertices[idx++] = y1;
          scene->vertices[idx++] = r;
          scene->vertices[idx++] = g;
          scene->vertices[idx++] = b;
          scene->vertices[idx++] = x2;
          scene->vertices[idx++] = y2;
          scene->vertices[idx++] = r;
          scene->vertices[idx++] = g;
          scene->vertices[idx++] = b;
        }
      ball_nbr++;
    }

  glUseProgram (scene->program);
  glBindVertexArray (scene->vao);
  glBindBuffer (GL_ARRAY_BUFFER, scene->vbo);
  glBufferData (GL_ARRAY_BUFFER, idx * sizeof (float), scene->vertices, GL_DYNAMIC_DRAW);
  glDrawArrays (GL_TRIANGLES, 0, (GLsizei)(idx / 5));
  glBindVertexArray (0);
  glUseProgram (0);
}

void
update_balls (t_scene *scene)
{
  if (scene->dt > 0.05f)
    scene->dt = 0.05f;
  float time_scale = scene->dt * 60.0f;
  t_ball **balls = scene->balls;
  t_ball *ball;

  while (*balls)
    {
      ball = *balls;
      ball->speed.x -= scene->acceleration.x * time_scale;
      ball->speed.y -= scene->acceleration.y * time_scale;
      float next_x = ball->position.x + ball->speed.x * time_scale;
      float next_y = ball->position.y + ball->speed.y * time_scale;

      if (next_x + ball->r > WINDOW_WIDTH)
        {
          next_x = WINDOW_WIDTH - ball->r;
          ball->speed.x = -ball->speed.x * scene->rebounce;
        }
      else if (next_x - ball->r < -WINDOW_WIDTH)
        {
          next_x = -WINDOW_WIDTH + ball->r;
          ball->speed.x = -ball->speed.x * scene->rebounce;
        }

      if (next_y + ball->r > WINDOW_HEIGHT)
        {
          next_y = WINDOW_HEIGHT - ball->r;
          ball->speed.y = -ball->speed.y * scene->rebounce;
        }
      else if (next_y - ball->r < -WINDOW_HEIGHT)
        {
          next_y = -WINDOW_HEIGHT + ball->r;
          ball->speed.y = -ball->speed.y * scene->rebounce;
          ball->speed.x = ball->speed.x * scene->friction;
        }

      ball->position.x = next_x;
      ball->position.y = next_y;
      balls++;
    }
}
