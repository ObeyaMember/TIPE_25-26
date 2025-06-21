#include "utilities.h"

// OpenGL window, events...

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){

    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}

void setup_window(GLFWwindow** window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char* WIN_TITLE){



    glfwSetErrorCallback(error_callback);

    if (!glfwInit()){
        return 0;
    }
    
    *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
    

    if (!window){
        printf("%d", -1);  
        glfwTerminate();
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGL();

}

void while_loop_window(GLFWwindow** window){
    glfwSwapBuffers(*window);
    glfwPollEvents();
}

void end_window(GLFWwindow** window){
    glfwDestroyWindow(window);
}

void end_process(){
    glfwTerminate();
}

// Buffers

// Shaders

char* get_shader_content(const char* fileName){
     FILE *fp;
    long size = 0;
    char* shaderContent;
    
    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if(fp == NULL) {
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);

    return shaderContent;
}

void setup_fvc_shader(unsigned int* shader, const char** shaderSource, const char* type){
    if (type == "fragment"){
        *shader = glCreateShader(GL_FRAGMENT_SHADER);
    }else if (type == "vertex"){
        *shader = glCreateShader(GL_VERTEX_SHADER);
    }else if (type == "compute"){
        *shader = glCreateShader(GL_COMPUTE_SHADER);
    }else {
        printf("wrong type given - setup_fvc_shader");
    }
    
    glShaderSource(shader, 1, shaderSource, NULL);
    glCompileShader(shader);


}

void setup_fvc_shader_prog(unsigned int* program, unsigned int* fragmentShader, unsigned int* vertexShader, unsigned int* computeShader, const char* type){
    *program = glCreateProgram();
    if (type == "compute"){
        glAttachShader(*program, *computeShader);
    }else if (type == "main_pipeline"){
        glAttachShader(*program, *fragmentShader);
        glAttachShader(*program, *vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog");
    }
    
    glLinkProgram(*program);

    if (type == "compute"){
        glDeleteShader(*computeShader);
    }else if (type == "main_pipeline"){
        glDeleteShader(*fragmentShader);
        glDeleteShader(*vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog");
    }
}


void fvc_shader_comp_error(unsigned int* shader, const char* type){
    GLint status;

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_TRUE){

    }
    else {
        printf("Compile error: %c shader\n", type);
        char infoLog[256];
        glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
        printf("\n");
        printf(infoLog);
        printf("\n");
    }

}
void fvc_shader_prog_link_error(unsigned int* program, const char* type){
    GLint status;
   
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_TRUE){
        //glUseProgram(program); // IDK if use here or in while loop
    }
    else {
        printf("Linking error: %c program", type);
        printf("\n");
    }
}

// Profiling, hardware, ...

char* get_used_GPU(){
    return glGetString(GL_RENDERER);
}

void print_used_GPU(){
    char* vendor = glGetString(GL_RENDERER);
    printf(vendor);
    printf("\n");
}
