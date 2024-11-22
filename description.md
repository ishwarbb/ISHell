
## Description
1) I get the username and host name with the uname function and the structure utsname.
2) I have an infinite while loop that displays the shell prompt and reads commands untill 'quit' is entered.
3) The commands entered are tokenised and executed seperately using the help of the function strtok
4) The functions cd, pwd are implemented with the help of chdir and getcwd while simple tokenisation is enough for implementing echo.
5) My helper functions like Tildify, DeTildify and executeCommand help me put everything together
6) The ls command is implemented through functions ls, lsdir, lsfile and helper-functions lscmp, lscolourX
7) pinfo fetches all the required cases from /proc and checks if the current process is foreground on comparing processgroupid and pid.
8) discover is implemented along with the functions ddir and dfile. Discover basically iterates through all the files and directories and prints them according to the arguements. It recursively enters into valid regularly directories and performs the same functions. 
9) history is implemented by having a hidden file in the directory from where the shell is being spawned.
10) history stores every successful and unsuccessful command entered.
11) It however doesnt store duplicate commands
12) Background processes are handled using the setpgid function and an array to store the details of spawned background processes including an allocated serial number.
13) The status of completed background processes is checked before the prompt is being printed through the method of probing
14) All background prcesses are killed before quiting
15) Foreground and Background processes are executed using execvp after forking a child process.
16) Time taken is printed for foreground processes that take more than 1 second.
