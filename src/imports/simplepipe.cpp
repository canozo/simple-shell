#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include "simplepipe.hpp"

void simplepipe(char **commands) {
  // si contiene el caracter '|' y solo hay 4 argumentos es un pipe simple:
  // write => pipe[1] => data => pipe[0] <= read
  int pipes[2];
  pipe(pipes);

  int primer_comando = fork();
  if (primer_comando == 0) {
    // ponemos nuestro pipe en el lugar de stdout
    dup2(pipes[1], STDOUT_FILENO);
    // cerramos el extremo del pipe que no ocupamos
    close(pipes[0]);
    execlp(commands[0], commands[0], NULL);
  }

  int segundo_comando = fork();
  if (segundo_comando == 0) {
    // ponemos nuestro pipe en el lugar de stdin
    dup2(pipes[0], STDIN_FILENO);
    // cerramos el extremo del pipe que no ocupamos
    close(pipes[1]);
    execlp(commands[2], commands[2], NULL);
  }

  // cerrar los pipes y esperar la ejecucion de los procesos hijos
  close(pipes[0]);
  close(pipes[1]);
  wait(&primer_comando);
  wait(&segundo_comando);

  // terminar el proceso
  exit(0);
}
