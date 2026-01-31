## Task 1

### Explanation

1. Which process (parent or child) executes the exec call?
    - The child. The parent must remain the shell. This is why it happens in the `else if (pid == 0)` block.
2. What happens to the child process’s address space after exec succeeds?
    - It's flushed since it doesn't need access to the parent's memory.
3. Why must the parent call wait()?
    - The parent is the shell so it must wait for the called program to finish executing.
4. What would happen if the parent did not wait?
    - The sh_launch() function will return before the launched program is finished executing meaning its output will show up at some later time when the prompt "utsh$" and the user is able to type. Program output and user input will get mixed up.

### Testing

1. Executing a program with no arguments
    - Command: `ls`
    - Expected: `Checkpoints.md  README.md  sh.c  utsh`
    - Actual: `Checkpoints.md  README.md  sh.c  utsh`
2. Executing a program with arguments
    - Command: `ls home`
    - Expected: `will`
    - Actual: `will`
3. Executing a program using an absolute path
    - Command: `/bin/ls`
    - Expected: `Checkpoints.md  README.md  sh.c  utsh`
    - Actual: `Checkpoints.md  README.md  sh.c  utsh`
4. Attempting to execute a non-existent program
    - Command: `fake_program`
    - Expected: `fake_program: No such file or directory`
    - Actual: `fake_program: No such file or directory`

## Task 2

### Explanation

1. Why close(1); open(“x.txt”, …) causes standard output to go to the file
    - 1 is the standard output and closing it leaves open the spot for open to take. 
2. Why this works without calling dup() or dup2()
    - Since there is an invariant that the first open spot on the fd table is taken, it's safe to assume open() will fill the spot left open by the previous close(). Therefore we don't need an extra fd at the end of the table.
3. What would happen if close(1) were omitted
    - The new file descriptor would be put at the end of the fd table and the output would not be redirected. In fact the new file wouldn't be used at all.
4. Why redirection must not be performed in the parent process
    - The parent process is the shell, so we need to keep stdin and stdout available for shell usablility. i.e. the parent needs to be able to take in commands and print out output consistently.

### Testing

1. Output redirection creating a new file
    - Command: `ls > nonexistant.txt`
    - Expected behavior: New file named `nonexistant.txt` is made in the same directory and contains `Checkpoints.md  README.md  sh.c  test.txt utsh`. This file can be opened by me.
    - Actual behavior: Same as expected.
2. Output redirection overwriting an existing file
    - Command: `ls > test.txt`
    - Expected behavior: The text `test` in `test.txt` is overwritten with the text `Checkpoints.md  README.md  sh.c  test.txt utsh`.
    - Actual behavior: Same as expected.
3. Input redirection from a file
    - Command: `cat < sh.c`
    - Expected behavior: Prints out the contents of sh.c.
    - Actual behavior: Same as expected.
4. Redirection with command arguments
    - Command: `echo "this is a test" > test.txt`
    - Expected behavior: The text `"this is a test"` shows up in the file `test.txt`.
    - Actual behavior: Same as expected.
5. Failure cases (e.g., redirecting input from a missing file)
    - Command: `cat < fake_file.txt`
    - Expected behavior: Error message `utsh: open: No such file or directory` is printed and shell continues to run.
    - Actual behavior: Same as expected.

## Task 3

### Explanation

Diagram: See Diagram.png

1. An explanation of how closing 0 or 1 forces the pipe end to take that descriptor
    - Again we rely on the invariant that dup() and open() always take the first open fd in the table. By calling close() then dup() without anything inbetween we have precise control over the fd table.
2. Why unused pipe ends must be closed in every process
    - While the pipes are open the children will keep waiting for input indefinately. Since the parent is waiting on the children, it will never resume. 
3. What deadlock or blocking behavior could occur if they are not
    - The parent (shell) won't be able to take in new commands since it is waiting on the children, but the children are waiting on each other to finish sending data across the pipes so the whole thing will get stuck. 

### Testing

1. Single pipe
    - Command: `ls | wc`
    - Expected behavior: Same output as zsh in the same directory. `      5       5      44`
    - Actual behavior: Same as expected.
2. Multi-stage pipeline
    - Command: `ls | sort | uniq | wc`
    - Expected behavior: Same output as zsh in the same directory. `      5       5      44`
    - Actual behavior: Same as expected.
3. Pipe with arguments
    - Command: `echo -e "b\na\nb" | sort | uniq`
    - Expected behavior: Same as zsh in the same directory. `a
b`
    - Actual behavior: The quotation marks were not correctly handled so the output was: `a
"b
b"`

What tests are supported?
    - All test but the last was supported. While my shell can handle pipes with multiple arguments, it doesn't correctly parse quotation marks. Because of this the command was interpreted slightly differently than a normal shell would.
