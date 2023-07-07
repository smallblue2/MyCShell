// Header file for all c files
// Contains all the constants and structs used in the program

// Constants
#define BUFFER_LENGTH 512
#define MAX_ARGS 64
#define MAX_PATH 1024

// Struct
typedef struct {
    char *args[MAX_ARGS];
    char *input_file;
    char *output_file;
    int append_output;
    int background;
} command;

