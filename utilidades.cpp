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
bool sonIguales(const char* a, const char* b) {
    if(a == 0 || b == 0) return false;
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0') {
        if(a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}
void copiarCadenaEnBuffer(char* destino, const char* origen) {
    int i = 0;
    while(origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}
int stringAEntero(const char* cad) {
    int i = 0;
    int signo = 1;
    int numero = 0;
    if(cad[0] == '-') {
        signo = -1;
        i++;
    }
    while(cad[i] != '\0') {
        numero = numero * 10 + (cad[i] - '0');
        i++;
    }
    return numero * signo;
}
void enteroACadena(int n, char* buffer) {
    if(n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    char temp[20];
    int i = 0;
    int signo = 0;
    if(n < 0) {
        signo = 1;
        n = -n;
    }
    while(n > 0) {
        temp[i++] = char('0' + (n % 10));
        n /= 10;
    }
    int j = 0;
    if(signo) buffer[j++] = '-';
    for(int k = i - 1; k >= 0; k--) {
        buffer[j++] = temp[k];
    }
    buffer[j] = '\0';
}
void inicializarAleatorio() {
    srand((unsigned)time(0));
}
int aleatorioEnRango(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}
void intercambiarEnteros(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}
void intercambiarPunteros(void*& a, void*& b) {
    void* t = a;
    a = b;
    b = t;
}
