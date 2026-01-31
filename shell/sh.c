#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"

enum redirect_type {
  OUTPUT,
  INPUT,
  NONE
};

struct pipe_args {
  int num_commands;
  char ***split_commands;
};

struct complex_args {
  enum redirect_type type;
  char *filename;
  char **cleaned_args;
};

int verify_fd(int fd, char *direction) 
{
  if (fd < 0)
  {
    perror("utsh: open");
    exit(EXIT_FAILURE);
  }
  else if (fd >= 3)
  {
    close(fd);
    fprintf(stderr, "utsh: error redirecting %s\n", direction);
    exit(EXIT_FAILURE);
  }

  return 1;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int sh_launch(struct complex_args complex_args) 
{
  int pid = fork();
  if (pid < 0) {
    perror("utsh: fork");
  }
  else if (pid == 0) {
    // Child
    if (complex_args.type == OUTPUT) 
    {
      if (complex_args.filename == NULL) 
      {
        fprintf(stderr, "utsh: missing file to redirect to\n");
        exit(EXIT_FAILURE);
      }
      close(1);
      int fd = open(complex_args.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      verify_fd(fd, "output");
    } else if (complex_args.type == INPUT) 
    {
      if (complex_args.filename == NULL) 
      {
        fprintf(stderr, "utsh: missing file to redirect from\n");
        exit(EXIT_FAILURE);
      }
      close(0);
      int fd = open(complex_args.filename, O_RDONLY);
      verify_fd(fd, "input");
    }

    int status = execvp(complex_args.cleaned_args[0], complex_args.cleaned_args);
    if (status < 0) 
    {
      perror(complex_args.cleaned_args[0]);
      exit(EXIT_FAILURE);
    }
  }
  else {
    // Parent
    waitpid(pid, NULL, 0);
  }

  return 1;
}

/**
 * @brief Check for redirection and divide args accordingly.
 * @param args Null terminated list of arguments.
 * @return 
 */
struct complex_args handle_redirection(char **args) 
{
  struct complex_args result;
  result.type = NONE;
  result.filename = NULL;
  result.cleaned_args = args;

  char *token;
  for (int i = 0; args[i] != NULL; i++) 
  {
    token = args[i];
    
    if (strcmp(token, ">") == 0) 
    {
      result.type = OUTPUT;
      if (args[i+1] != NULL) { result.filename = args[i+1]; }
      args[i] = NULL;
      break;
    } else if (strcmp(token, "<") == 0)
    {
      result.type = INPUT;
      if (args[i+1] != NULL) { result.filename = args[i+1]; }
      args[i] = NULL;
      break;
    }
  }

  return result;
}

int count_commands(char **args) 
{
  int count = 0;
  for (int i = 0; args[i] != NULL; i++)
  {
    if (strcmp(args[i], "|") == 0)
    {
      count++;
    }
  }

  return ++count; // There is one more command than there are pipes.
}

struct pipe_args handle_pipes(char **args) 
{
  int count = count_commands(args);

  struct pipe_args result;
  result.num_commands = count;

  char ***split_commands = malloc((count + 1) * sizeof(char**));
  int curr_command_i = 0;
  int start_i = 0;
  char *token;
  int i = 0;
  while(1)
  {
    token = args[i];

    if (token == NULL || strcmp(token, "|") == 0)
    {
      int num_tokens = i - start_i;
      char **curr_command = malloc((num_tokens + 1) * sizeof(char*));
      for (int j = 0; j < num_tokens; j++)
      {
        curr_command[j] = args[start_i + j];
      }
      curr_command[num_tokens] = NULL;
      split_commands[curr_command_i] = curr_command;
      curr_command_i++;
      start_i = i+1;
    }

    if (token == NULL) break;
    i++;
  }

  split_commands[curr_command_i] = NULL;
  result.split_commands = split_commands;

  return result;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int sh_execute(char **args)
{
  if (args[0] == NULL) {
    return 1;  // An empty command was entered.
  }

  struct pipe_args split_args = handle_pipes(args);
  struct complex_args cleaned_args = handle_redirection(args);

  return sh_launch(cleaned_args);   // launch
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *sh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0;  // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } else {
      perror("utsh: readline");
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **sh_split_line(char *line)
{
  int bufsize = SH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "utsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "utsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void sh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("utsh$ ");
    line = sh_read_line();
    args = sh_split_line(line);
    status = sh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Run command loop.
  sh_loop();
  return EXIT_SUCCESS;
}
