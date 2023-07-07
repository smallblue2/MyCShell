/* I/O REDIRECTION FILE
 *
 * Contains functions that handle I/O redirection.
 *
 */

// Local Header Files
#include"common.h"
#include"constants.h"
#include"sigerrorhandlers.h"

/* FUNCTION: handle_redirection(command *cmd)
 * RETURNS: void
 * DESC:
 *   Handles I/O redirection.
 *   - Opens the input file and redirects stdin to it.
 *   - Opens the output file and redirects stdout to it.
 */
void handle_redirection(command *cmd) {
    // Initialize file descriptor.
    int fd;

    // If the input file is not NULL, open it and redirect stdin to it.
    if (cmd->input_file) {
        // Open the file in read-only mode.
        fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            handleerror(NULL);
            _exit(EXIT_FAILURE);
        }
        // dup2() duplicates the file descriptor fd to the file descriptor STDIN_FILENO.
        dup2(fd, STDIN_FILENO);
        // close() closes the file descriptor fd.
        close(fd);
    }

    // Do the same as above, but for the output file.
    if (cmd->output_file) {
        // If the append_output flag is set, open the file in append mode.
        // Otherwise, open it in write mode.
        fd = open(cmd->output_file, cmd->append_output ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            handleerror(NULL);
            _exit(EXIT_FAILURE);
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}
