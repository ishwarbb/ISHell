# ISHell

## Overview
ISHell is Linux command-line shell implemented in C. It provides users most of the common features of a Unix-like shell but with a simpler interface and implementation. The shell supports command execution, process management, I/O redirection, piping, signal handling, autocompletion, and built-in commands.

## Key Features

### Built-In Commands
The shell supports a variety of built-in commands implemented in C, including:
- `echo`: Display text.
- `cd`: Change directory.
- `pwd`: Print working directory.
- `exit`: Exit the shell.
- `ls`: List files and directories.
- `pinfo`: Retrieve process information.
- `history`: Show command history.
- `discover`: Search files/directories.
- `jobs`: List background processes.
- `sig`: Send signals to jobs.
- `fg`: Bring jobs to the foreground.
- `bg`: Resume stopped jobs in the background.

### Command Execution 
- **Command System**: Supports both foreground and background processes, allowing users to chain commands using `;` and `&` operators.

### Process Management
- **Comprehensive Tracking**: Background processes initiated with the `&` operator are monitored, enabling users to control them through built-in commands like `jobs`, `fg`, `bg`, and `sig`.
- **Enhanced Multitasking**: Provides a flexible environment for handling multiple processes simultaneously.

### Advanced Features
- **I/O Redirection**: Users can redirect output to files or take input from files seamlessly using `<`, `>`, and `>>`.
- **Piping Support**: Allows the output of one command to be used as the input for another command, supporting complex command sequences.

### Signal Handling
- **Responsive to Signals**: The shell handles common signals such as Ctrl+C (terminate), Ctrl+Z (background), and Ctrl+D (logout) for a smooth user experience.

### Autocompletion
- **Intuitive Suggestions**: Autocompletion enhances user experience by providing file or directory name suggestions based on current input when the tab key is pressed.

## Specifications
The C-Shell is designed to meet several specifications:
1. **Input/Output Redirection**: Supports redirection of both input and output with appropriate error handling.
2. **Command Pipelines**: Allows chaining commands using pipes (`|`) for seamless data flow between commands.
3. **Signal Handling**: Responds appropriately to user signals for managing process states.
4. **Autocompletion**: Implements tab completion for commands and file names.

## How to Use
1. **Build the Project**: Compile the shell using:
   ```bash
   make
   ```
2. **Run the Shell**: Execute the shell with:
   ```bash
   ./shell
   ```

Kindly refer description.md for some notes on the implementation of the shell.

## General Assumptions

1. All commands are limited to a maximum size of 1000 characters.
2. The shell currently differentiates between `(filename)`, '(filename)', and "(filename)", and only expects the first format.
3. The maximum number of `;` or `&` separators in a single command is 10,000.
4. Quotes are not processed in any command except for the `discover` command, where they are used to distinguish a filename from the target directory.
5. The completion of background processes is managed through probing.
6. The flags `-df` and `-fd` can be used as shorthand for `-d -f`.
7. Control sequences such as Ctrl+C and Ctrl+Z are not handled by the shell.
8. The `discover` command accepts a maximum of three arguments.
9. The `ls` command lists all files, directories, and executables in alphabetical order, displaying each argument in the order provided when multiple arguments are given.
10. Command history varies based on the directory from which the shell is spawned; to maintain continuous history, the shell should be started from the same directory.
11. Invalid commands entered as background processes are executed as separate processes, even if their output consists solely of error messages.
12. When multiple processes are executed using `;` or `&`, the time taken by the last foreground process is displayed.

