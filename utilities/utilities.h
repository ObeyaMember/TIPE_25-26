#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "libraries.h"

// OpenGL window, events...

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setup_window(GLFWwindow** window);

// Buffers

// Shaders

char* get_shader_content(const char* fileName);
void setup_fv_shader(unsigned int* fragmentShader, unsigned int* vertexShader);
void setup_c_shader(unsigned int* computeShader);
void setup_fv_shader_prog(unsigned int* shaderProgram);
void setup_c_shader_prog(unsigned int* computeProgram);

void fvc_shader_comp_error(unsigned int shader, const char* type);
void fvc_shader_prog_link_error(unsigned int shaderProgram, const char* type);



#endif