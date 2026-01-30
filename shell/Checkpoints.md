## Task 1

### Explanation

1. Which process (parent or child) executes the exec call?
    - The child.
2. What happens to the child process’s address space after exec succeeds?
    - It's flushed.
3. Why must the parent call wait()?
    - The parent is the shell so it must wait for the called program to finish executing.
4. What would happen if the parent did not wait?
    - The sh_launch() function will return before the launched program is finished executing meaning its output will show up at some later time when the prompt "utsh$" and the user is able to type. Program output and user input will get mixed up.

### Testing

1. Executing a program with no arguments
    - Command: ```ls```
    - Expected: ```Checkpoints.md  README.md  sh.c  utsh```
    - Actual: ```Checkpoints.md  README.md  sh.c  utsh```
2. Executing a program with arguments
    - Command: ```ls home```
    - Expected: ```will```
    - Actual: ```will```
3. Executing a program using an absolute path
    - Command: ```/bin/ls```
    - Expected: ```Checkpoints.md  README.md  sh.c  utsh```
    - Actual: ```Checkpoints.md  README.md  sh.c  utsh```
4. Attempting to execute a non-existent program
    - Command: ```fake_program```
    - Expected: ```fake_program: No such file or directory```
    - Actual: ```fake_program: No such file or directory```

## Task 2

### Explanation

1. Why close(1); open(“x.txt”, …) causes standard output to go to the file
2. Why this works without calling dup() or dup2()
3. What would happen if close(1) were omitted
4. Why redirection must not be performed in the parent process

### Testing

1. Output redirection creating a new file
    - Command: ``` ```
    - Expected: ``` ```
    - Actual: ``` ```
2. Output redirection overwriting an existing file
    - Command: ``` ```
    - Expected: ``` ```
    - Actual: ``` ```
3. Input redirection from a file
    - Command: ``` ```
    - Expected: ``` ```
    - Actual: ``` ```
4. Redirection with command arguments
    - Command: ``` ```
    - Expected: ``` ```
    - Actual: ``` ```
5. Failure cases (e.g., redirecting input from a missing file)
    - Command: ``` ```
    - Expected: ``` ```
    - Actual: ``` ```

## Task 3

### Explanation

### Testing