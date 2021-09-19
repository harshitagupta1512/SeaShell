# SeaShell - Linux C Shell
***
## Overview
SeaShell is a user-defined interactive shell program that can create and manage new processes.

SeaShell supports the following features :
1. A `prompt` that displays username, systemname and current working directory.
2. **Builtin Commands**: `pwd`, `cd`, `echo`, `ls`, `pinfo`, `history`, `repeat`.
3. All other **System Commands** (with and wihout arguments). 
4. Process management (executing foreground and background) processes.

## Running and exiting the shell
In the directory where the source file resides,
1. Run the `make` command.
2. Run `./SeaShell` to start the shell.
3. Enter `exit` to exit the shell.

## Implementation Details

`Pseudohome` The directory from which the shell is invoked is the home directory of the shell, represented by `~`.

SeaShell supports `;` separated list of commands. SeaShell also handles random multiple `tabs` and `spaces` in the command.

`Prompt` For every next command a prompt of the format `<username@systemname:curr_dir>` is printed.

#### Built-in Commands
1. Apart from basic functionality `cd` command supports multiple flags `.`, `..`, `-`, `~`. More than one command-line arguments are not allowed. `cd` with no flags or arguments cd into home directory.

2. `echo` command displays the line of text/string that is passed as a command line argument.

3. `pwd` command prints the path of the current working directory.

4. `ls` commands with multiple flags `-a`, `-l`, multiple directory/file names. Handles arguments in any order.

5. `pinfo` prints process related information. `pinfo` accepts one command line argument that is PID and prints the process info of the given `PID`. With no command-line arguments, it prints the process info of the `shell program` itself. Process info printed includes, `pid`, `process status`, `virtual memory (in kB)`, `executable path`. `+` in process status signifies that the process is running in `foreground`.  

6. `repeat` command executes the given instruction multiple times. Format of the command will be `repeat n instruction`, where n is a positive integer specifying the number of times `instruction` should be repeated. `Limitation` - `repeat` does not work with `background` processes.

7. `history` command prints the last `10` commands executed by the shell across all sessions. `history` accepts a positive integer (say `n)` as a command line argument and prints the last n commands executed by the shell across all sessions. At `max`, the shell stores `20` commands in history.

#### System Commands - Foreground and Background Processes

Other than the builtin commands, SeaShell executes all other system commands either in foreground or background. It supports the `&` operator which lets a program run in the background after printing the `pid` of the newly created process. Running a process in background implies that the shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. SeaShell can handle multiple background processes at a time. This implies that your shell will spawn that process and doesn't wait for the
process to exit. It will keep taking other user commands. When the background process exits the shell, it display the appropriate message to the user.
## File Wise Code Breakdown

1. `makefile` - Contains the `make` rules for compiling the files.

2. `main.c` - contains code to take command input from user, tokenize it properly and calling corresponding functions. 

3. `prompt.c` - has the function responsible for generating and displaying the shell prompt.

4. `echo.c` - has the function to execute the builtin `echo` command.

5. `pwd.c` - has the function to execute the builtin `pwd` command.

6. `cd.c` - has the function to execute the builtin `cd` command.

7. `ls.c` -h as the function to execute the builtin `ls` command.

8. `system_commands.c` - has the function that identifies whether a command (other than builtin commands) need to run in background or foreground.

9. `bg.c` - has the function responsible for executing processes in background.

10. `fg.c` - has the function responsible for executing system commands in foreground.

11. `signal_handler.c` - has the user defined signal handlers to handle `SIGCHLD` and `SIGINT` signals.

12. `repeat.c` - has the function to execute the builtin `repeat` command.

13. `queue.c` - has the implementation of a `deque` used to maintain history of commands.

14. `history.c` - has the function to execute the builtin `history` command.

15. `pinfo.c`  -has the function to execute the builtin `pinfo` command.

16. `def.h` - contains all the function declarations and macro definitions to be shared between all forementioned source files.

17. `README.md` - The file you are reading now. Briefly describes the SeaShell and which file corresponds to what part of the functionality.

## Assumptions/Limitations

Maximum possible length of command input is `1000`.

Maximum number of background processes that can be run across one session = `1000`.

----