#include "../libraries.h"
#include "../utilities/utilities.h"
#include "../stb_perlin.h"
#include "../stb_image.h"

float* heightmap_to_vertices(const char* heightmap_file, unsigned int** indices_loc, float** tex_coords_loc, int* n_vertices, int* n_indices, int* n_strips, int* n_tris_strips){
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(heightmap_file, &width, &height, &nrChannels, 0);
    if (data)
    {
        printf("Loaded heightmap of size %d * %d \n", height, width);
    }
    else
    {
        printf("Failed to load texture \n");
    }

    float* vertices = malloc(3 * height * width * sizeof(float));
    float* tex_coords = malloc(2 * height * width * sizeof(float));

    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    int rez = 1;
    unsigned bytePerPixel = nrChannels;

    int idx = 0; // keep track of current idx in vertices
    int idx2 = 0; // keep track of current idx in tex_coords

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            //vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
            //vertices.push_back( (int) y * yScale - yShift);   // vy
            //vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
        
            vertices[idx] = -height/2.0f + height*i/(float)height; idx+=1;
            vertices[idx] =  (int) y * yScale - yShift; idx+=1;
            vertices[idx] = -width/2.0f + width*j/(float)width; idx+=1;

            tex_coords[idx2] = (float)j / (float)width; idx2 += 1;
            tex_coords[idx2] = (float)i / (float)height; idx2 += 1;
        
        }
    }

    unsigned int a[6] = { // note that we start from 0!
        0, 1, 3, // first triangle
        0, 2, 3 // second triangle
    };

    unsigned int* indices = malloc(3 * height * width * sizeof(unsigned)); // too big but am dumb

    printf("indices: ");
    idx = 0;
    /* for (int i = 0; i < 6; i+=1){
        indices[i] = a[i]; idx += 1;
        printf("%d, ",indices[i]);
    } */
    
    for(unsigned i = 0; i < height-1; i += rez)
    {
        for(unsigned j = 0; j < width; j += rez)
        {
            for(unsigned k = 0; k < 2; k++)
            {
                indices[idx] = j + width * (i + k*rez); idx += 1;
                //printf("%d, ",indices[0]);
            }
        }
    }
       
    printf("\n"); 
    printf("Loaded %d indices\n", idx);

    const int numStrips = (height-1)/rez;
    const int numTrisPerStrip = (width/rez)*2-2;
    printf("Created lattice of %d strips with %d triangles each \n", numStrips, numTrisPerStrip);
    printf("Created %d triangles total \n", numStrips * numTrisPerStrip);
    //print_vertex_array(vertices, height*width);

    // indices
    *n_tris_strips = numTrisPerStrip;
    *n_strips = numStrips;
    *n_vertices = height * width;
    *n_indices = idx;
    *indices_loc = indices;
    *tex_coords_loc = tex_coords;

    

    return vertices;

}

void one_erosion_step(){

}

void many_erosion_steps(){

}

void full_simulation(){

}

