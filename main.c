#include "libraries.h"
#include "utilities/utilities.h"
#include "cpu_erosion/cpu_erosion.h"


const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 900;
const unsigned int WORLD_WIDTH = 1000;
const unsigned int WORLD_HEIGHT = 1000;
const char* WIN_TITLE = "Hydraulic Erosion Simulation";

int main(){
    printf("------------------------------------------------------------------\n");
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
    /* setup_fvc_shader(&computeShader, &erosionCompShaderSource, "compute");
    fvc_shader_comp_error(&computeShader, "compute"); */

    //                                                                              DATA

    float vertices[] = {
    // positions      // colors         // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };


    //                                                                              SETUP SHADER PROGRAMS
    unsigned int shaderProgram, computeProgram;
    
    // SETUP MAIN PIPELINE PROGRAM
    setup_fvc_shader_prog(&shaderProgram, &fragmentShader, &vertexShader, NULL, "main_pipeline");
    fvc_shader_prog_link_error(&shaderProgram, "main_pipeline");
    
    // SETUP COMPUTE PROGRAM
    /* setup_fvc_shader_prog(&computeProgram, NULL, NULL, &computeShader, "compute");
    fvc_shader_prog_link_error(&computeProgram, "compute"); */

       //                                                                              BUFFERS / TEXTURES

    // SETUP TEXTURES
    unsigned int texture1, texture2;
    
    
    setup_texture_from_png(&texture1, "assets/textures/heightmaps/hmap1.png", &shaderProgram, "texture1", 0);
    setup_texture_from_png(&texture2, "assets/textures/awesomeface.png", &shaderProgram, "texture2", 1);       // For some unknow reason, there is a conflict between the two, cant show both

    // SETUP BUFFERS
/*     unsigned int VBO, VAO, EBO;
    setup_VAO(&VAO);
    setup_VBO(&VBO);
    setup_EBO(&EBO);

    // PASSING THE DATA

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); */

    //                                                                              CAMERA
    
    vec3 x_axis = {1,0,0};
    vec3 y_axis = {0,1,0};
    vec3 z_axis = {0,0,1};
    mat4 model, view, projection;
    
    glm_mat4_identity(model);
    glm_rotate(model, glm_rad(30.0), x_axis);
    vec3 a, b, c;
    glm_vec3_scale(z_axis, -4000, a);
    glm_mat4_identity(view);
    glm_translate(view, a);

    
    

    float fov = 45.0;
    float aspect = 800.0 / 600.0;
    float near = 0.1;
    float far = 100000.0;
    
    glm_perspective(glm_rad(fov), aspect, near, far, projection);
    
   
    //                                                                              TESTING HEIGHTMAP SETUP
    int n_vertices, n_indices, n_strips, n_tris_strips;
    unsigned int* hmap_indices;
    float* tex_coords;
    float* hmap_vertices = heightmap_to_vertices("assets/textures/heightmaps/hmap1.png", &hmap_indices, &tex_coords, &n_vertices, &n_indices, &n_strips, &n_tris_strips);


    // Buffers
    unsigned int terrainVAO, terrainVBO, terrainIBO, tex_coordsVBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * 3 * sizeof(float), hmap_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texcoord attribute
    glGenBuffers(1, &tex_coordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coordsVBO);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * 2 * sizeof(float), tex_coords, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);


    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned), hmap_indices, GL_STATIC_DRAW);

   
   
    //                                                                              RENDER LOOP
   

    float deltaTime = 0;
    float lastFrame = 0;
    while (!glfwWindowShouldClose(window)){
 /*     glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //glm_rotate(model, glm_rad(currentFrame * deltaTime * 5), x_axis);
        glm_rotate(model, glm_rad(currentFrame * deltaTime * 1), y_axis);
        //glm_rotate(model, glm_rad(currentFrame * deltaTime * 5), z_axis);
       
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
      
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);
        //draw_objects_w_texture(&shaderProgram, &VAO, GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //draw_objects_w_texture(&shaderProgram, &VAO, GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        


        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


        glBindVertexArray(terrainVAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for(unsigned strip = 0; strip < n_strips; strip+=1)
        {
            glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                           n_tris_strips+2,   // number of indices to render
                           GL_UNSIGNED_INT,     // index data type
                           (void*)(sizeof(unsigned) * (n_tris_strips+2) * strip)); // offset to starting index
        }


        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
 
 
        while_loop_window(&window);
    }
    free(hmap_vertices); // dont forget to free
    //                                                                              END PROCESS
    end_window(&window);
    end_process();
}