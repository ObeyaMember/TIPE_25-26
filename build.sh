gcc -c utilities.o utilities/utilities.c
gcc -c cpu_erosion.o cpu_erosion/cpu_erosion.c
gcc -o exec.exe main.c glad.c utilities.o cpu_erosion.o -lglfw -lm
./exec.exe
