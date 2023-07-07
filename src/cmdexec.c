/* CMD EXEC FILE
 *
 * Contains functions that parse lines and execute commands.
 *
 */

// Local libraries
#include"constants.h"
#include"common.h"
#include"util.h"
#include"cmds.h"
#include"ioredirect.h"
#include"sigerrorhandlers.h"

/* FUNCTION: parse (char *buffer)
 * RETURNS: 
 *      On Success -> A double pointer array, containing
 *                    the buffer tokenized
 *      On Failure -> NULL
 * DESC:
 *   parse takes in the input buffer, and parses it into
 *   a double char pointer (string array), returning it.
 */
void parse(char *buffer, command *cmd) {
    // Initialize variables
    int isbackground = 0;
    int i = 0;
    // Begin tokenizing the buffer
    char *token = strtok(buffer, " \t\n");
    // While there are still tokens to be found
    while (token != NULL) {
        // If the token is "&"
        if (strcmp(token, "&") == 0) {
            // Set the background flag to 1
            token = strtok(NULL, " \t\n");
            cmd->background = 1;
            isbackground = 1;
        // If the token is "<"
        } else if (strcmp(token, "<") == 0) {
            // Set the input file to the next token
            token = strtok(NULL, " \t\n");
            cmd->input_file = token;
        // If the token is ">"
        } else if (strcmp(token, ">") == 0) {
            // Set the output file to the next token
            token = strtok(NULL, " \t\n");
            cmd->output_file = token;
            // Not appending
            cmd->append_output = 0;
        // If the token is ">>"
        } else if (strcmp(token, ">>") == 0) {
            // Set the output file to the next token
            token = strtok(NULL, " \t\n");
            cmd->output_file = token;
            // We are appending
            cmd->append_output = 1;
        } else {
            // Otherwise, add the token to the args array
            cmd->args[i++] = token;
        }
        // Get the next token
        token = strtok(NULL, " \t\n");
    }
    // If we didn't find a "&" token, set background to 0
    if (!isbackground) cmd->background = 0;
}

/* FUNCTION: exec_line(char *input)
 * RETURNS: void
 * DESC:
 *   Takes in a line of input, and executes it.
 *   - If the line is empty, return.
 *   - If the line is an internal command, execute it.
 *   - If the line is not an internal command, fork and exec it.
 */
void exec_line(char *input) {
    // Initialize command struct
    command cmd = {0};

    // Parse the input
    parse(input, &cmd);

    // If the first argument is NULL, return
    if (cmd.args[0] == NULL) return;
    // Create empty function pointer
    void (*run)(command *);
    // Array of internal cmd names
    const char *cmds_strings[] = {
        "cd",
        "clr",
        "dir",
        "environ",
        "echo",
        "help",
        "pause",
        "quit",
        "pwd",
        "moo",
    };
    // Array of function addresses
    // SAME ORDER OF *CMD_STRINGS[]
    const void (*cmds[])(command *) = {
        &cd,
        &clr,
        &dir,
        &env,
        &echo,
        &help,
        &pause_shell,
        &quit,
        &pwd,
        &moo,
    };
    // Iterate over cmd_strings until a match,
    // set empty function pointer 'cmd' to
    // correct function address.
    for (int i = 0; i < sizeof(cmds_strings) / sizeof(char*); i++) {
        if (strcmp(*(cmds_strings + i), cmd.args[0]) == 0) {
            run = *(cmds + i);
            run(&cmd);
            return;
        }
    }

    // If we get here, we have an external command
    pid_t pid;
    // Fork and assign pid
    switch(pid = fork()) {
        // If pid is -1, fork failed
        case -1:
            // Call handle error function
            // to display error
            handleerror(NULL);
            // Exit with failure
            exit(EXIT_FAILURE);
        // If pid is 0, we are in the child process
        case 0:
            // Set parent environment variable
            setenv("PARENT", getenv("SHELL"), 1);
            // Check and handle redirection
            handle_redirection(&cmd);
            if (execvp(cmd.args[0], cmd.args) == -1) {
                // Call handle error function to
                // display error
                handleerror(cmd.args[0]);
                // If execvp fails, exit child process
                _exit(EXIT_FAILURE);
            }
        default:
            // Parent process checks if command is background
            // or foreground, and waits accordingly.
            if (!cmd.background) {
                int status;
                waitpid(pid, &status, 0);
            }
    }
}

