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

    assert(glfwInit());
    
    *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
    

    if (!*window){
        printf("%d", -1);  
        glfwTerminate();
    }

    glfwSetKeyCallback(*window, key_callback);
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
    gladLoadGL();

}

void while_loop_window(GLFWwindow** window){
    glfwSwapBuffers(*window);
    glfwPollEvents();
}

void end_window(GLFWwindow** window){
    glfwDestroyWindow(*window);
}

void end_process(){
    glfwTerminate();
}

// Render loop specific functions

void draw_objects(unsigned int* program, unsigned int* VAO, GLenum mode, GLint start_idx, GLsizei num_vertices){
    glUseProgram(*program);
    glBindVertexArray(*VAO);
    glDrawArrays(mode, start_idx, num_vertices);
}



// Buffers
// (Seems that the way one passes data to buffers is very... type of data dependent (who would've thought) and as such,
//  it will be done in the specific library that needs it (e.g. cpu_erosion))

void setup_VBO(unsigned int* VBO){
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
}

void setup_VAO(unsigned int* VAO){
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
}

void setup_SSB(unsigned int* SSB){
    glGenBuffers(1, SSB);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, *SSB); // No actual idea of how this works
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, *SSB);        // Are these two lines both necessary??? IDK. If it works it works
}



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
    if (strcmp(type, "fragment") == 0){
        *shader = glCreateShader(GL_FRAGMENT_SHADER);
    }else if (strcmp(type, "vertex") == 0){
        *shader = glCreateShader(GL_VERTEX_SHADER);
    }else if (strcmp(type, "compute") == 0){
        *shader = glCreateShader(GL_COMPUTE_SHADER);
    }else {
        printf("wrong type given - setup_fvc_shader: given type was: %s \n", type);
    }
    
    glShaderSource(*shader, 1, shaderSource, NULL);
    glCompileShader(*shader);


}

void setup_fvc_shader_prog(unsigned int* program, unsigned int* fragmentShader, unsigned int* vertexShader, unsigned int* computeShader, const char* type){
    *program = glCreateProgram();
    if (strcmp(type, "compute") == 0){
        glAttachShader(*program, *computeShader);
    }else if (strcmp(type, "main_pipeline") == 0){
        glAttachShader(*program, *fragmentShader);
        glAttachShader(*program, *vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog: given type was: %s \n", type);
    }
    
    glLinkProgram(*program);

    if (strcmp(type, "compute") == 0){
        glDeleteShader(*computeShader);
    }else if (strcmp(type, "main_pipeline") == 0){
        glDeleteShader(*fragmentShader);
        glDeleteShader(*vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog: given type was: %s \n", type);
    }
}


void fvc_shader_comp_error(unsigned int* shader, const char* type){
    GLint status;

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_TRUE){

    }
    else {
        printf("Compile error: %s shader\n", type);
        char infoLog[256];
        glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
        printf("\n");
        printf(infoLog);
        printf("\n");
    }

}

void fvc_shader_prog_link_error(unsigned int* program, const char* type){
    GLint status;
   
    glGetProgramiv(*program, GL_LINK_STATUS, &status);
    if(status == GL_TRUE){
        //glUseProgram(program); // IDK if use here or in while loop
    }
    else {
        printf("Linking error: %s program\n", type);
        printf("\n");
    }
}

// Profiling, hardware, ...

char* get_used_GPU(){
    return glGetString(GL_RENDERER);
}

void print_used_GPU(){
    char* vendor = glGetString(GL_RENDERER);
    printf("GPU in use is: %s \n", vendor);
}
