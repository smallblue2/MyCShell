/* CMD FILE
 *
 * Contains all internal command implementations
 *
 */

// Local Libraries
#include"constants.h"
#include"common.h"
#include"util.h"
#include"ioredirect.h"
#include"sigerrorhandlers.h"

/* FUNCTION: cd(command *cmd)
 * RETURNS: void
 * DESC:
 *   Changes the current working directory to the path
 *   specified in cmd->args[1].
 *   If no path is specified, the current working directory
 *   is changed to the home directory.
 */
void cd(command *cmd) {
    // If a path was given
    if (cmd->args[1] != NULL) {

        // Try and open dir
        DIR *dir = opendir(cmd->args[1]);
        // Check if succeeded
        if (dir != NULL) {
            chdir(cmd->args[1]);
        } else {
            handleerror(NULL);
            return;
        }
        // Close dir
        closedir(dir);
    } else {
        // No arguments given, just display current dir
        fprintf(stdout, "%s\n", getenv("PWD"));
    }

    // Finally, set PWD env variable
    char *cwd = (char*)malloc(MAX_PATH * sizeof(char));
    if (!cwd) {
        // Call handle error function
        // to display error
        handleerror(NULL);
        return;
    }

    // Store current working directory in cwd
    getcwd(cwd, MAX_PATH);
    // Re-write "PWD" to cwd's value
    setenv("PWD", cwd, 1);
    // Free memory
    free(cwd);

    return;
}

/* FUNCTION: clr(command *cmd)
 * RETURNS: void
 * DESC:
 *   Clears the screen.
 */
void clr(command *cmd) { 
    // e [1;1H] places the cursor in the upper
    // right hand corner.
    // e [2J adds a space to the top of all
    // existing screen characters.
    // Essentially clears screen.
    //
    // Works on all POSIX systems & ANSI terminals
    fputs("\e[1;1H\e[2J", stdout);
    return;
}

/* FUNCTION: dir(command *cmd)
 * RETURNS: void
 * DESC:
 *   Lists the contents of the current directory.
 *   If a path is specified, lists the contents of
 *   that directory.
 */
void dir(command *cmd) {
    // Set input file to NULL
    cmd->input_file = NULL;
    // Create a new process
    pid_t pid;
    switch(pid = fork()) {
        case -1:
            // Call handle error function
            // to display error
            handleerror(NULL);
            // Exit with failure
            exit(EXIT_FAILURE);
        case 0:
            // Set parent env var
            setenv("PARENT", getenv("SHELL"), 1);
            // Check for redirection and apply
            handle_redirection(cmd);

            int exec_status;
            // If a path was given, list it
            if (cmd->args[1] != NULL) {
                exec_status = execlp("ls", "ls", "-al", cmd->args[1], NULL);
            } else {
                // No path given, just list current dir
                exec_status = execlp("ls", "ls", "-al", NULL);
            }
            // If exec failed
            if (exec_status == -1) {
                // Call handle error function
                // to display error
                handleerror(NULL);
                // Exit with failure
                _exit(EXIT_FAILURE);
            }
        default:
            // Parent process waits for child to finish
            waitpid(pid, NULL, 0); 
    }
    return;
}

/* FUNCTION: environ(command *cmd)
 * RETURNS: void
 * DESC:
 *   Lists all environment strings.
 */
void env(command *cmd) {
    // Set input file to NULL
    cmd->input_file = NULL;
    // Store original stdout
    int original_stdout = dup(STDOUT_FILENO);

    // Check for redirection and apply
    handle_redirection(cmd);

    // Create a pointer to the environment
    char **traveler = environ;
    // While the pointer is not NULL
    while (*traveler) {
        // Print the environment string
        fprintf(stdout, "%s\n", *traveler);
        // Increment the pointer
        traveler++;
    }
    // Flush stdout (clear the buffer)
    fflush(stdout);

    // Restore original stdout
    dup2(original_stdout, STDOUT_FILENO);

    return;
}

/* FUNCTION: echo(command *cmd)
 * RETURNS: void
 * DESC:
 *   Prints all arguments to stdout.
 */
void echo(command *cmd) {
    // Set input file to NULL
    cmd->input_file = NULL;
    // Store original stdout
    int original_stdout = dup(STDOUT_FILENO);

    // Check for redirection and apply
    handle_redirection(cmd);

    // Print all arguments
    for (int i = 1; cmd->args[i] != NULL; ++i) {
        printf("%s ", cmd->args[i]);
        if (cmd->args[i + 1] == NULL) {
            printf("\n");
        }
    }
    // Flush stdout (clear the buffer)
    fflush(stdout);
    
    // Restore original stdout
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);

    return;
}

/* FUNCTION: help(command *cmd)
 * RETURNS: void
 * DESC:
 *   Prints the manual page for the shell.
 */
void help(command *cmd) {
    // Set input file to NULL
    pid_t pid;
    
    // Create a new process
    switch(pid = fork()) {
        case -1:
            // Call handle error function
            // to display error
            handleerror(NULL);
            // Exit with failure
            exit(EXIT_FAILURE);
        case 0:
            // Set parent env variable
            setenv("PARENT", getenv("SHELL"), 1);
            // Check for redirection and apply
            handle_redirection(cmd);
            // Redirect stdin to /dev/null
            // This is required with more to prevent
            // it from waiting for user input
            // (which would cause the shell to hang)
            freopen("/dev/null", "r", stdin);
            // Execute more with the manual page
            if (execlp("more", "more", getenv("MANUAL"), NULL) == -1) {
                // Call handle error function
                // to display error
                handleerror(NULL);
                // Exit with failure if execlp failed
                _exit(EXIT_FAILURE);
            }
        default:
            // Parent process waits for child to finish
            waitpid(pid, NULL, 0);
    }
    return;
}

/*  FUNCTION: pause_shell(command *cmd)
 *  RETURNS: void
 *  DESC:
 *    Pauses the shell until the user presses enter.
 */
void pause_shell(command *cmd) {
    /* Some sources I used to learn about termios:
     * The Open Group:
     * https://pubs.opengroup.org/onlinepubs/7908799/xbd/termios.html#tag_008_001_006
     *
     * GNU:
     * https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html
     *
     * Termios MAN pages:
     * https://man7.org/linux/man-pages/man3/termios.3.html
     */

    fputs("Press Enter to continue...", stdout);
    // FLUSH STDOUT (clear the buffer)
    fflush(stdout);

    /* SAVE THE CURRENT TERMINAL SETTINGS
     * (into original_termios)
     *
     * So we can bring back the default after
     * the user has pressed enter.
     */
    struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);

    /* DISABLE CANONICAL MODE AND ECHO
     *
     * Non-canonical mode is where input is processed
     * immediately without line buffering or editing,
     * good for real-time input processing.
     * 
     * Disabling echo prevents us from seeing keyboard input.
     */ 
    struct termios new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios); 
                         // ^ TCSANOW is a constant of termios.h that tells the changes to be applied NOW

    // READ CHARACTERS UNTIL ENTER IS PRESSED
    while (getchar() != '\n');

    // RESTORE THE ORIGINAL TERMINAL SETTINGS
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    fputs("\n", stdout);
}

/* FUNCTION: pwd(command *cmd)
 * RETURNS: void
 * DESC:
 *   Prints the current working directory.
 */
void pwd(command *cmd) {
    // Display current working directory
    fprintf(stdout, "%s\n", getenv("PWD"));
    return;
}

/* FUNCTION: quit(command *cmd)
 * RETURNS: void
 * DESC:
 *   Exits the shell.
 */
void quit(void) {
    exit(0);
}

/* FUNCTION: moo(command *cmd)
 * RETURNS: void
 * DESC:
 *   I've noticed that "mooing" is a popular
 *   thing to do in the Linux community.
 *   So I decided to add a "moo" command to
 *   my shell.
 *
 *   It's not very useful, but it's fun.
 */ 
void moo(command *cmd) {
    fputs("         (__)\n", stdout);
    fputs("         (oo)\n", stdout);
    fputs("   /------\\/ \n", stdout);
    fputs("  / |    ||  \n", stdout);
    fputs("*  /\\---/\\ \n", stdout);
    fputs("   ~~   ~~\n", stdout);
    fputs("....\"Myshell proudly supports mooing\"...\n", stdout);
    return;
}
