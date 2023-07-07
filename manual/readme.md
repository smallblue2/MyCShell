MANUAL
    This manual is intended to explain certain operating system concepts to
    a beginner to the Unix system(s) and Unix shells.


MYSHELL
    myshell is a command line interpreter, or shell, to allow you to access
    and use your operating system.


WHAT IS A SHELL
   A shell, in terms of Unix, is a command-line interface that allows a user
   to interact with their operating system, and control their computer. This
   is done by entering commands as text. A shell serves as the bridge between
   the user and the operating system, allowing users to execute programs, manage
   files, and perform various tasks by typing commands and recieving textual
   feedback.

   In basic terms, a shell is like a command prompt or terminal window where
   you can type and enter in commands, and see the results. There are many
   different shells out there, including this one. Some examples are 
   Bourne-Again SHell(BASH), Korn SHell(KSH), Friendly-Interactive SHell(FISH),
   Z SHell(ZSH), and many more, all with their own syntax and features.


OPERATING
    The operation of myshell is very simple! Simply type a command, press enter,
    and see the result outputted right infront of you. Some basic commands include
    navigating the Unix file system like "ls", "cd", and "pwd", creating and editing
    files using touch and nano/vim, and running programs using their name or their
    filepath.

    myshell also includes two modes;

    INTERACTIVE MODE
        Interactive mode is where the user actively enters in and executes commands,
        one-by-one. This is the mode most users will be familiar with, and is often
        used for day-to-day tasks.

        Simply run the executable with no arguments to enter interactive mode, e.g.
        "./myshell"

    BATCH MODE
        Batch mode is where the user provides the shell a group of commands or jobs
        to execute. Useful for making automation scripts - If you find yourself often
        repeating many of the same steps - just enter them into a 'batch file', and
        run it at your convenience.

        A batch file is essentially any file that contains a list of commands to be
        executed.

        To run a batch file, simply execute the shell with the batch file as an argument.
        E.g. "./myshell batchfile".

INTERNAL COMMANDS
    An internal command is a command that comes packaged with myshell.
    It's an action that takes place within the shell, and is performed by myshell.

    cd <path> - change the current default directory. If the argument is not present, 
                report the current directory.

    clr - clear the screen.

    dir <directory> - list the contents of the directory. If there is no directory
                      provided, list the contents of the current directory.

    environ - list all the environment strings.

    echo <message> - echo on the display followed by a new line.

    help - display the user manual.

    pause - pause the operation of the shell until enter is pressed.

    quit - quit the shell.

EXTERNAL COMMANDS
    In Unix systems, an external command refers to a program or utility that is 
    not built into myshell, but instead is a separate executable file. These 
    commands are usually stored in specific directories, such as /bin or /usr/bin, 
    and can be run by typing their name in the terminal. Directories searched when
    an external command is entered, are included in your PATH environment variable.
    Some examples of external commands include ls, grep, and cat.


SHELL ENVIRONMENT
    In a shell, the environment refers to the collection of variables, settings, 
    and other information that the shell uses to configure itself and its behavior.

    Some common environment concepts in a shell include:

        Environment variables: These are variables that are set by the shell or the 
        user and are used to store information that can be accessed by programs and 
        scripts. Examples include the PATH variable, which tells the shell where to 
        find executable files, and the HOME variable, which specifies the user's 
        home directory.

        Shell configuration files: These are files that are run when the shell starts 
        up and are used to set environment variables, define functions, and configure 
        shell behavior. Examples include the .bashrc and .bash_profile files in the 
        Bash shell.

        Aliases: These are shortcuts that allow users to create their own command names 
        or to modify the behavior of existing commands. For example, an alias can be 
        created to make the ls command always include the -l flag.

    Be aware, myshell currently only supports environmet variables.


I/O REDIRECTION
    In myshell, input/output (I/O) redirection is a feature that allows users to 
    control where the input or output of a command or process goes.

    Here are some examples to further explain the concept:

        Redirecting output to a file: The output of a command can be redirected 
        to a file by using the ">" symbol. For example, the command "ls > file.txt" 
        will write the output of the "ls" command to a file called "file.txt" 
        instead of printing it to the terminal.

        Redirecting input from a file: The input to a command can be redirected from 
        a file by using the "<" symbol. For example, the command "sort < file.txt" 
        will sort the contents of the file "file.txt" instead of waiting for user input.

        Combining input/output redirection: It's also possible to combine input/output 
        redirection to create more complex pipelines of commands. For example, the 
        command "sort < file.txt > sorted.txt" will sort the contents of "file.txt" 
        and write the sorted output to a file called "sorted.txt".

    In terms of processes, I/O redirection is a feature that is implemented at the 
    operating system level. When a process is started, it has three standard 
    streams associated with it: standard input (stdin), standard output (stdout), 
    and standard error (stderr). By default, these streams are connected to the 
    terminal, but with I/O redirection, they can be redirected to files, devices, 
    or other processes.

    SUPPORTED I/O REDIRECTION
        "<" - Input; redirects the standard input for a command to a file specified.

        ">" - Output; redirects the standard output for a command to a file specified.

        ">>" - Append; redirects the standard output for a command to the end of a file
                       specified.


PROCCESSES
    In Unix, a process is an instance of a running program, consisting of a set of 
    instructions, data, and resources like memory and file handles. The operating 
    system (OS) manages processes by assigning them unique process identification 
    numbers (PIDs) and allocating the necessary resources.

    Foreground and background execution refer to how processes interact with the user's 
    terminal and how they are scheduled by the OS.

FOREGROUND EXECUTION
    When a process runs in the foreground, it takes control of the terminal and 
    occupies the user's input and output streams (stdin, stdout, and stderr). The 
    terminal becomes unavailable for new commands until the foreground process 
    completes its execution or is stopped.
    
    Foreground processes receive signals from the terminal, like SIGINT (generated 
    by Ctrl+C) or SIGTSTP (generated by Ctrl+Z), allowing users to directly control 
    their execution.

BACKGROUND EXECUTION
    Background processes, on the other hand, run independently of the terminal, 
    allowing the user to continue entering commands and interacting with the 
    system.
    
    To start a process in the background, append an ampersand (&) to the command. 
    The OS will assign the process a unique PID, and the process will run independently 
    of the terminal.
    
    Background processes do not receive signals from the terminal, and their input 
    and output streams are typically redirected to files or other processes.
    
    Users can monitor, manage, and control background processes using external 
    commands like 'jobs', 'bg', and 'fg'.

SOURCES:
    ENVIRONMENT CONCEPTS
        Environment Variables: Linux Documentation Project. (n.d.). Chapter 9.1 
        Variables and Parameters. Retrieved March 19, 2023, from 
        https://tldp.org/LDP/abs/html/internalvariables.html

    I/O REDIRECTION
        Input/output Redirection: Linux Documentation Project. (n.d.). Chapter 20. I/O 
        Redirection. Retrieved March 19, 2023, from 
        https://tldp.org/LDP/abs/html/io-redirection.html  

    PROCESSES
        Unix Processes: GNU C Library documentation. (n.d.). Retrieved March 19, 2023, 
        from 
        https://www.gnu.org/software/libc/manual/html_node/Processes.html

        Linux Processes Guide: Baeldung. (n.d.). Linux Processes Guide.
        Retrieved March 19, 2023, from
        https://www.baeldung.com/linux/linux-processes-guide
