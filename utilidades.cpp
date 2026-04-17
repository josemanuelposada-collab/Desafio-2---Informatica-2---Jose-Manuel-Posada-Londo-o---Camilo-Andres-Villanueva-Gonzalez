#include "utilidades.h"
#include <cstdlib>
#include <ctime>

int longitudCadena(const char* cad) {
    if(cad == 0) return 0;
    int i = 0;
    while(cad[i] != '\0') i++;
    return i;
}

char* copiarCadena(const char* origen) {
    if(origen == 0) return 0;
    int n = longitudCadena(origen);
    char* nueva = new char[n + 1];
    for(int i = 0; i < n; i++) nueva[i] = origen[i];
    nueva[n] = '\0';
    return nueva;
}

