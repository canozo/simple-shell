#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include "parampipe.hpp"

using std::vector;

bool isparampipe(vector<char *> comandos) {
  int leftcount = 0;
  int rightcount = 0;
  bool foundpipe = false;

  for (int i = 0; i < comandos.size() - 1; i += 1) {
    if (strcmp(comandos[i], "|") == 0) {
      if (!foundpipe) {
        foundpipe = true;
      } else {
        return false;
      }
    } else if (!foundpipe) {
      leftcount += 1;
    } else if (foundpipe) {
      rightcount += 1;
    }
  }
  return (leftcount >= 2 && rightcount >= 1) || (leftcount >= 1 && rightcount >= 2);
}

void parampipe(vector<char *> commands) {
  vector<char *> before;
  vector<char *> after;
  bool foundpipe = false;

  for (int i = 0; i < commands.size() - 1; i += 1) {
    if (strcmp(commands[i], "|") == 0) {
      foundpipe = true;
      continue;
    }
    if (!foundpipe) {
      before.push_back(commands[i]);
    } else {
      after.push_back(commands[i]);
    }
  }

  before.push_back(NULL);
  after.push_back(NULL);

  int pipes[2];
  pipe(pipes);

  int primer_comando = fork();
  if (primer_comando == 0) {
    // ponemos nuestro pipe en el lugar de stdout
    dup2(pipes[1], STDOUT_FILENO);
    // cerramos el extremo del pipe que no ocupamos
    close(pipes[0]);
    execvp(before[0], &before[0]);
  }

  int segundo_comando = fork();
  if (segundo_comando == 0) {
    // ponemos nuestro pipe en el lugar de stdin
    dup2(pipes[0], STDIN_FILENO);
    // cerramos el extremo del pipe que no ocupamos
    close(pipes[1]);
    execvp(after[0], &after[0]);
  }

  // cerrar los pipes y esperar la ejecucion de los procesos hijos
  close(pipes[0]);
  close(pipes[1]);
  wait(&primer_comando);
  wait(&segundo_comando);

  // terminar el proceso
  exit(0);
}
