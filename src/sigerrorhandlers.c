/* SIGNAL ERROR HANDLERS FILE
 *
 * Contains signal and error handling functions.
 *
 */

// Local Header Files
#include"common.h"

/* FUNCTION: siginthandler(int sig)
 * RETURNS: void
 * DESC:
 *   Handles the SIGINT signal.
 *   - Reaps all child processes.
 *   - Ensure that no zombie processes are left.
 *   - Prints a message to stderr.
 */

void siginthandler(int sig) {
    // Initialize child_pid.
    pid_t child_pid;
    
    // Loop until no more child processes are found.
    do {
        // Wait for a child process to terminate without blocking, storing its pid.
        child_pid = waitpid(-1, NULL, WNOHANG);
        
        // If a terminated child process is found (child_pid > 0), 
        // it is reaped by the waitpid function.

    } while (child_pid > 0);
    
    // Print a message to stderr.
    fputs(" \nProcesses Reaped ", stderr);
}

/* FUNCTION: handleerror(char *msg)
 * RETURNS: void
 * DESC:
 *   Handles errors - inspired by perror()!
 *   - Prints a message to stderr.
 *   - If msg is NULL, prints the error message associated with errno.
 *   - If msg is "FATAL", prints the error message wiht "FATAL" prepended.
 *   - Otherwise, prints the error message associated with errno and the message passed to the function.
 */
void handleerror(char *msg) {
    if (msg == NULL) {
        fprintf(stderr, "MyShell: %s\n", strerror(errno));
    } else if (strcmp(msg, "FATAL") == 0) {
        fprintf(stderr, "FATAL: %s\n", strerror(errno));
    } else {
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    }
    return;
}
