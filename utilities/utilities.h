#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "../libraries.h"


// OpenGL window, events, ...

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setup_window(GLFWwindow** window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char* WIN_TITLE);
void while_loop_window(GLFWwindow** window);
void end_window(GLFWwindow** window);
void end_process();

// Render loop specific functions

void draw_objects(unsigned int* program, unsigned int* VAO, GLenum mode, GLint start_idx, GLsizei num_vertices);
void draw_objects_w_texture(unsigned int* program, unsigned int* VAO, GLenum mode, GLsizei count, GLenum type, const void* indices);


// Buffers

void setup_VBO(unsigned int* VBO);
void setup_EBO(unsigned int* EBO);
void setup_VAO(unsigned int* VAO);
void setup_SSB(unsigned int* SSB);

// Data

void setup_texture_from_jpg(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0);
void setup_texture_from_png(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0);

// Shaders

char* get_shader_content(const char* fileName);
void setup_fvc_shader(unsigned int* shader, const char** shaderSource, const char* type);
void setup_fvc_shader_prog(unsigned int* program, unsigned int* fragShader, unsigned int* vertexShader, unsigned int* computeShader, const char* type);
void fvc_shader_comp_error(unsigned int* shader, const char* type);
void fvc_shader_prog_link_error(unsigned int* shaderProgram, const char* type);

// Profiling, hardware, ...

char* get_used_GPU();
void print_used_GPU();

#endif