#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include "multiplepipe.hpp"

using std::vector;

void multiplepipe(vector<char *> commands) {
  int size = (int) commands.size() / 2;
  char *cmds[size];

  // crear arreglo con los comandos (sin caracteres de pipe)
  int count = 0;
  for (int i = 0; i < commands.size() - 1; i += 1) {
    if (strcmp(commands[i], "|") != 0) {
      cmds[count] = commands[i];
      count += 1;
    }
  }

  // iniciar el piping the procesos
  int child = fork();
  if (child == 0) {
    exec_command(cmds, size);
  }
  wait(&child);

  // terminar el proceso
  exit(0);
}

void exec_command(char **cmds, int current_cmd) {
  // si hay mas de un proceso habra que conectar sus pipes
  if (current_cmd > 1) {
    int pipe_in[2];
    pipe(pipe_in);

    int child = fork();
    if (child == 0) {
      pipe_command(cmds, current_cmd - 1, pipe_in);
    }

    // ponemos nuestro pipe en el lugar de stdin
    dup2(pipe_in[0], STDIN_FILENO);
    // cerramos el extremo del pipe que no ocupamos
    close(pipe_in[1]);
    wait(&child);
  }

  // ejecutar el comando actual
  execlp(cmds[current_cmd - 1], cmds[current_cmd - 1], NULL);
}

void pipe_command(char **cmds, int current_cmd, int *pipe_out) {
  // ponemos nuestro pipe en el lugar de stdout
  dup2(pipe_out[1], STDOUT_FILENO);
  // cerramos el extremo del pipe que no ocupamos
  close(pipe_out[0]);

  // continuar con el siguiente comando
  exec_command(cmds, current_cmd);
}

bool ismultiplepipe(vector<char *> commands) {
  // dados los commands revisar si es un pipe multiple
  // comando 1 | comando 2 | comando 3 | ... | comando n
  bool ispipe = false;
  bool haspipe = false;

  for (int i = 0; i < commands.size() - 1; i += 1, ispipe = !ispipe) {
    if (strcmp(commands[i], "|") == 0) {
      haspipe = true;
    }
    if (strcmp(commands[i], "|") == 0 && i == 0) {
      return false;
    } else if (strcmp(commands[i], "|") == 0 && i == commands.size() - 2) {
      return false;
    } else if (ispipe && strcmp(commands[i], "|") != 0) {
      return false;
    } else if (!ispipe && strcmp(commands[i], "|") == 0) {
      return false;
    }
  }

  return haspipe;
}
