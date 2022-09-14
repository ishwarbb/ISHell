# ISHell

## ASSUMPTIONS
1) All commands are less than 1000 size
2) As of now. My shell treats (filename) different form '(filename)' and "(filename)". As of now I only expect the first kind.
3) Maximum no of ' ; ' seperated or ' & ' in a given command is 10000
4) Quotes have not been handled in any command excpet discover - where quotes are used to distinguisd a filename from the target directory
5) Background Processes' completion have been handled through probing 
6) -df / -fd flags can be used as a short hand notation for -d -f 
7) Control + C or Control + Z have not been handled
8) I expect atmost 3 arguements for the discover command
9) ls displays list of all files and dirctories and executables in alphabetical order. When multiple arguements are given to ls, it prints for each of the argument in given order.
10) history gives different history if the shell is spawned from different locations. If you want history to be continous and full keep spawning the shell from the same directory. 
11) invalid commands when entered as a background process are still considered as a seperate process and executed. even though their output is just an error message.
12) When multiple processes are executed by ; or &. the time taken by last foreground processes is printed.



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

## How to use
Enter make to run the makefile.
Run ./shell to spawn the shell