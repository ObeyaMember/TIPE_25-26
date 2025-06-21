#include "libraries.h"
#include "utilities/utilities.h"


const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 900;
const unsigned int WORLD_WIDTH = 1000;
const unsigned int WORLD_HEIGHT = 1000;
const char* WIN_TITLE = "Hydraulic Erosion Simulation";

int main(){
    
    const char* vertexShaderSource = get_shader_content("main_pipeline_shaders/vertexShader.txt");
    const char* fragmentShaderSource = get_shader_content("main_pipeline_shaders/fragmentShader.txt");
    const char* erosionCompShaderSource = get_shader_content("gpu_erosion/erosionCompShader.txt");

    //                                                                              SETUP WINDOW
    GLFWwindow* window;
    setup_window(&window, SCR_WIDTH, SCR_HEIGHT, WIN_TITLE);

    //                                                                              CHECK HARDWARE
    print_used_GPU();

    //                                                                              SETUP SHADERS
    unsigned int vertexShader, fragmentShader, computeShader;
    
    // SETUP VERTEX SHADER
    setup_fvc_shader(&vertexShader, &vertexShaderSource, "vertex");
    fvc_shader_comp_error(&vertexShader, "vertex");
    
    // SETUP FRAGMENT SHADER
    setup_fvc_shader(&fragmentShader, &fragmentShaderSource, "fragment");
    fvc_shader_comp_error(&fragmentShader, "fragment");
    
    // SETUP COMPUTE SHADER
    setup_fvc_shader(&computeShader, &erosionCompShaderSource, "compute");
    fvc_shader_comp_error(&computeShader, "compute");

    //                                                                              SETUP SHADER PROGRAMS
    unsigned int shaderProgram, computeProgram;
    
    // SETUP MAIN PIPELINE PROGRAM
    setup_fvc_shader_prog(&shaderProgram, &fragmentShader, &vertexShader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&shaderProgram, "main_pipeline");
    
    // SETUP COMPUTE PROGRAM
    setup_fvc_shader_prog(&computeProgram, NULL, NULL, &computeShader, "compute");
    fvc_shader_prog_link_error(&computeProgram, "compute");

    //                                                                              WHILE LOOP
    while (!glfwWindowShouldClose(window)){
        
        
        
        while_loop_window(&window);
    }

    //                                                                              END PROCESS
    end_window(&window);
    end_process();
}