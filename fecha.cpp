#include "fecha.h"
#include <iostream>

using namespace std;

Fecha::Fecha() {
    dia = 1;
    mes = 1;
    anio = 2026;
}

Fecha::Fecha(int dia, int mes, int anio) {
    this->dia = dia;
    this->mes = mes;
    this->anio = anio;
}

Fecha::Fecha(const Fecha& otra) {
    dia = otra.dia;
    mes = otra.mes;
    anio = otra.anio;
}

Fecha::~Fecha() {
}

Fecha& Fecha::operator=(const Fecha& otra) {
    if(this != &otra) {
        dia = otra.dia;
        mes = otra.mes;
        anio = otra.anio;
    }
    return *this;
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::operator<(const Fecha& otra) const {
    if(anio != otra.anio) return anio < otra.anio;
    if(mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

void Fecha::setDia(int dia) { this->dia = dia; }
void Fecha::setMes(int mes) { this->mes = mes; }
void Fecha::setAnio(int anio) { this->anio = anio; }

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

bool Fecha::esBisiesto() const {
    if(anio % 400 == 0) return true;
    if(anio % 100 == 0) return false;
    return anio % 4 == 0;
}

int Fecha::diasDelMes() const {
    if(mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) return 31;
    if(mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    if(esBisiesto()) return 29;
    return 28;
}

void Fecha::sumarUnDia() {
    dia++;
    if(dia > diasDelMes()) {
        dia = 1;
        mes++;
        if(mes > 12) {
            mes = 1;
            anio++;
        }
    }
}

void Fecha::sumarDias(int cantidad) {
    for(int i = 0; i < cantidad; i++) {
        sumarUnDia();
    }
}

int Fecha::diferenciaDias(const Fecha& otra) const {
    Fecha menor = *this;
    Fecha mayor = otra;
    int signo = 1;

    if(otra < *this) {
        menor = otra;
        mayor = *this;
        signo = -1;
    }

    int contador = 0;
    while(!(menor == mayor)) {
        menor.sumarUnDia();
        contador++;
    }

    return contador * signo;
}

void Fecha::imprimir() const {
    if(dia < 10) cout << '0';
    cout << dia << '/';
    if(mes < 10) cout << '0';
    cout << mes << '/' << anio;
}