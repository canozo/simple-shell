#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include "./imports/cat.hpp"
#include "./imports/multiplepipe.hpp"
#include "./imports/parampipe.hpp"
#include "./imports/simplepipe.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;

vector<char *> split(string);

int main() {
  int child;
  string cmd;

  while (true) {
    cout << "> ";
    getline(cin, cmd);

    if (cmd == "exit") {
      break;
    }

    vector<char *> vcmds = split(cmd);
    char **cmds = &vcmds[0];

    child = fork();
    if (child == 0) {
      if (vcmds.size() == 4 && strcmp(cmds[1], "|") == 0) {
        simplepipe(cmds);
      } else if (strcmp(cmds[0], "cat") == 0 && strcmp(cmds[1], ">") == 0 && vcmds.size() == 4) {
        cat(cmds[2]);
      } else if (ismultiplepipe(vcmds)) {
        multiplepipe(vcmds);
      } else if (isparampipe(vcmds)) {
        parampipe(vcmds);
      } else {
        execvp(cmds[0], cmds);
        // si llega a este punto hubo un error
        cerr << "error: comando \"" << cmds[0] << "\" no se pudo ejecutar\n";
        exit(1);
      }
    } else {
      wait(&child);
      // liberar memoria
      for (int i = 0; i < vcmds.size() - 1; i += 1) {
        delete vcmds[i];
      }
    }
  }

  return 0;
}

vector<char *> split(string cmd) {
  // dividir string por cada espacio
  vector<char *> args;
  int pos = 0;
  string palabra = "";

  while (cmd[pos]) {
    if (cmd[pos] == ' ') {
      if (palabra != "") {
        char *tempchar = new char();
        strcpy(tempchar, palabra.c_str());
        args.push_back(tempchar);
        palabra = "";
      }
      pos += 1;
      continue;
    }

    if (cmd[pos] == '|') {
      if (palabra != "") {
        char *tempchar = new char();
        strcpy(tempchar, palabra.c_str());
        args.push_back(tempchar);
      }
      char *temppipe = new char();
      strcpy(temppipe, "|");
      args.push_back(temppipe);
      palabra = "";
    } else {
      palabra += cmd[pos];
    }
    pos += 1;
  }

  // guardar la ultima palabra
  if (palabra != "") {
    char *tempchar = new char();
    strcpy(tempchar, palabra.c_str());
    args.push_back(tempchar);
  }
  args.push_back(NULL);

  return args;
}
