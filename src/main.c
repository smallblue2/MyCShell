/* MAIN FILE
 *
 * Contains the main shell loop and shell initialization.
 *
 */

// Local Header Files
#include"constants.h"
#include"common.h"
#include"util.h"
#include"cmdexec.h"
#include"sigerrorhandlers.h"

// Function Definitions
int batch_mode(int argc, char **argv);

// Main Function
int main(int argc, char **argv) {
    // Set Environment variables
    setinitenv();

    // Set up signal handling
    signal(SIGINT, siginthandler);

    // Check for a batch file.
    int batchmode = batch_mode(argc, argv);

    // Define prompt
    const char *PROMPT = "%> ";
    
    // Initialize buffer
    char *BUFFER = (char *)malloc(BUFFER_LENGTH * sizeof(char));
    if (!BUFFER) { // Ensure memory was assigned succesfully
        handleerror("FATAL");
        exit(EXIT_FAILURE);
    }

    // Keep accepting input from stdin stream until END OF FILE signal.
    while (1) {
        // Output prompt if not in batch mode
        if (batchmode != 1) {
            fputs("\033[0;32m", stdout);        // Make stdout green with ANSI colour codes
            fputs(getenv("PWD"), stdout);       // Display working directory
            fputs("\033[0;31m", stdout);        // Make stdout red with ANSI colour codes
            fputs(PROMPT, stdout);              // Display shell prompt
            fputs("\033[0m", stdout);           // Reset stdout colour with ANSI colour codes
        }

        // Read input from stdin stream, ensuring that it is not NULL
        // If it is null, it usually means end of file has been reached.
        if (fgets(BUFFER, BUFFER_LENGTH, stdin) == NULL) break;

        // Execute the line
        exec_line(BUFFER);
    }

    // Free all manually allocated memory
    free(BUFFER);
    return 0;
}


/* FUNCTION: batch_mode(int argc, char **argv)
 * RETURNS: 1 if batch mode is enabled, 0 otherwise
 * DESC:
 *   This function checks if the program is being run in batch mode.
 *   If it is, then it will return 1, otherwise it will return 0.
 *
 *   Redirection of stdin to the batchfile is handled in this function.
 */
int batch_mode(int argc, char **argv) {
    // If there are more than 2 arguments, then there is an error.
    if (argc > 2) {
        fprintf(stderr, "Error: Too many arguments.\n");
        return 0;
    }
    // If there is only one argument, then there is no batch file.
    if (argc == 1) return 0;
    // Otherwise there is only two arguments, and the second argument is the batch file.
    FILE *file = fopen(*(argv + 1), "r+"); // Open the file for reading and writing
    if (file == NULL) { // If the file does not exist, then return 0
        return 0;
    }
    // Check if the file is empty
    fseek(file, 0L, SEEK_END);
    if (ftell(file) == 0) { // If the file is empty, then return 0
        fclose(file); // Close the file
        return 0;
    }

    // Reset the file pointer to the beginning of the file
    rewind(file);
    
    // Redirect stdin to the file & check if it was successful
    if (freopen(*(argv + 1), "r+", stdin) == NULL) {
        fclose(file); // Close the file
        return 0;
    } else {
        // Return 1 to indicate that batch mode is enabled
        return 1;
    }
}
