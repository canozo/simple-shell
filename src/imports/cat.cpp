#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "cat.hpp"

using std::ofstream;
using std::string;
using std::cin;
using std::cerr;

void cat(char *nombre_archivo) {
  // redirecciona la entrada estándar del comando cat al archivo correspondiente
  ofstream archivo_salida(nombre_archivo);

  // si el archivo no se pudo abrir, mostrar error
  if (!archivo_salida.is_open()) {
    cerr << "error al abrir el archivo: " << nombre_archivo << '\n';
    exit(1);
  }

  string linea;
  // leer la entrada de la consola
  while (getline(cin, linea)) {
    // agregar la linea de entrada al buffer
    archivo_salida << linea << '\n';
    // escribir el buffer al archivo
    archivo_salida.flush();
  }

  // terminar el proceso
  exit(0);
}
