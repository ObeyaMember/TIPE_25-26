 - commands to force use of discrete grapihcs card:

export __NV_PRIME_RENDER_OFFLOAD=1
export __GLX_VENDOR_LIBRARY_NAME=nvidia
export __VK_LAYER_NV_optimus=NVIDIA_only

 - Command for compiling: 
 
gcc -o exec main.c glad.c -lglfw