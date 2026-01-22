#include "bouncy.h"

static GLuint
compile_shader (GLenum type, const char *source)
{
  GLuint shader = glCreateShader (type);
  GLint status = GL_FALSE;
  GLint log_length = 0;

  glShaderSource (shader, 1, &source, NULL);
  glCompileShader (shader);
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE)
    return shader;

  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 1)
    {
      char *log = (char *)malloc ((size_t)log_length);
      if (log)
        {
          glGetShaderInfoLog (shader, log_length, NULL, log);
          fprintf (stderr, "Shader compile error: %s\n", log);
          free (log);
        }
    }
  glDeleteShader (shader);
  return 0;
}

static GLuint
link_program (GLuint vs, GLuint fs)
{
  GLuint program = glCreateProgram ();
  GLint status = GL_FALSE;
  GLint log_length = 0;

  glAttachShader (program, vs);
  glAttachShader (program, fs);
  glLinkProgram (program);
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_TRUE)
    return program;

  glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 1)
    {
      char *log = (char *)malloc ((size_t)log_length);
      if (log)
        {
          glGetProgramInfoLog (program, log_length, NULL, log);
          fprintf (stderr, "Program link error: %s\n", log);
          free (log);
        }
    }
  glDeleteProgram (program);
  return 0;
}

bool
init_renderer (t_scene *scene)
{
  const char *vs_source = "#version 330 core\n"
                          "layout(location = 0) in vec2 in_pos;\n"
                          "layout(location = 1) in vec3 in_color;\n"
                          "out vec3 v_color;\n"
                          "void main() {\n"
                          "  v_color = in_color;\n"
                          "  gl_Position = vec4(in_pos, 0.0, 1.0);\n"
                          "}\n";

  const char *fs_source = "#version 330 core\n"
                          "in vec3 v_color;\n"
                          "out vec4 out_color;\n"
                          "void main() {\n"
                          "  out_color = vec4(v_color, 1.0);\n"
                          "}\n";

  scene->program = 0;
  scene->vao = 0;
  scene->vbo = 0;
  scene->vertices = NULL;
  scene->vertex_capacity = 0;

  GLuint vs = compile_shader (GL_VERTEX_SHADER, vs_source);
  GLuint fs = compile_shader (GL_FRAGMENT_SHADER, fs_source);
  if (!vs || !fs)
    {
      if (vs)
        glDeleteShader (vs);
      if (fs)
        glDeleteShader (fs);
      return false;
    }

  scene->program = link_program (vs, fs);
  glDeleteShader (vs);
  glDeleteShader (fs);
  if (!scene->program)
    return false;

  glGenVertexArrays (1, &scene->vao);
  glGenBuffers (1, &scene->vbo);
  glBindVertexArray (scene->vao);
  glBindBuffer (GL_ARRAY_BUFFER, scene->vbo);
  scene->vertex_capacity = (size_t)scene->balls_count * scene->balls_iteration * 3u * 5u;
  scene->vertices = (float *)malloc (sizeof (float) * scene->vertex_capacity);
  if (!scene->vertices)
    {
      glDeleteBuffers (1, &scene->vbo);
      glDeleteVertexArrays (1, &scene->vao);
      glDeleteProgram (scene->program);
      scene->program = 0;
      scene->vao = 0;
      scene->vbo = 0;
      return false;
    }
  glBufferData (GL_ARRAY_BUFFER, scene->vertex_capacity * sizeof (float), NULL, GL_DYNAMIC_DRAW);
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void *)0);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void *)(2 * sizeof (float)));
  glEnableVertexAttribArray (1);
  glBindVertexArray (0);
  return true;
}

void
free_renderer (t_scene *scene)
{
  if (scene->program)
    glDeleteProgram (scene->program);
  if (scene->vbo)
    glDeleteBuffers (1, &scene->vbo);
  if (scene->vao)
    glDeleteVertexArrays (1, &scene->vao);
  free (scene->vertices);
  scene->program = 0;
  scene->vao = 0;
  scene->vbo = 0;
  scene->vertices = NULL;
  scene->vertex_capacity = 0;
}
