/* UTILITIES FILE
 * 
 * Contains miscellanious utility functions that are 
 * used by the shell to perform various tasks.
 *
 */


// Local Header Files
#include"util.h"
#include"constants.h"
#include"common.h"
#include"sigerrorhandlers.h"

/* FUNCTION: setinitenv(void)
 * RETURNS: void
 * DESC:
 *   Sets the program's initial environment variables;
 *   - SHELL
 *   - MANUAL
 */
void setinitenv(void) {
    // set shell env var
    char *shell_path = (char*)malloc(MAX_PATH * sizeof(char));
    if (!shell_path) {
        handleerror("FATAL");
        exit(EXIT_FAILURE);
    }

    // realpath() places the absolute path of the current directory
    // in shell_path.
    realpath("./", shell_path);
    // strcat() appends the string "/myshell" to the end of shell_path.
    strcat(shell_path, "/myshell");
    // setenv() sets the environment variable "SHELL" to the value of
    setenv("SHELL", shell_path, 1);
    free(shell_path);
    
    // set manual env var
    char *man_path = (char*)malloc(MAX_PATH * sizeof(char));
    if (!man_path) {
        handleerror("FATAL");
        exit(EXIT_FAILURE);
    }
    // Same as above, but for the manual file. 
    realpath("./", man_path);
    strcat(man_path, "/../manual/readme.md");
    setenv("MANUAL", man_path, 0);
    free(man_path);

    return;
}
