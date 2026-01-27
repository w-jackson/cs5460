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

### Testing


## Task 3

### Explanation

### Testing