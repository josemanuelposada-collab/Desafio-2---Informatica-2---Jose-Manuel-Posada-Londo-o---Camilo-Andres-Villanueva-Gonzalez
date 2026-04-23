#ifndef UTILIDADES_H
#define UTILIDADES_H
char* copiarCadena(const char* origen);
int longitudCadena(const char* cad);
bool sonIguales(const char* a, const char* b);
void copiarCadenaEnBuffer(char* destino, const char* origen);
int stringAEntero(const char* cad);
void enteroACadena(int n, char* buffer);
void inicializarAleatorio();
int aleatorioEnRango(int minimo, int maximo);
void intercambiarEnteros(int& a, int& b);
void intercambiarPunteros(void*& a, void*& b);
#endif