#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include "cat.hpp"

void cat(char *nombre_archivo) {
  int file = open(nombre_archivo, O_RDWR | O_CREAT, 0666);

  int child = fork();
  if (child == 0) {
    // redireccionar stdout al archivo
    dup2(file, STDOUT_FILENO);
    execlp("cat", "cat", NULL);
  } else {
    // esperar por EOF
    wait(&child);
  }

  // terminar el proceso
  exit(0);
}
