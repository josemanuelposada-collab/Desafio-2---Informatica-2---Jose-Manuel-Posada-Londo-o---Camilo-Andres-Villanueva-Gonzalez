#include "jugador.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

Jugador::Jugador() {
    nombre = copiarCadena("");
    apellido = copiarCadena("");
    numeroCamiseta = 0;
    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

Jugador::Jugador(const char* nombre, const char* apellido, int numeroCamiseta) {
    this->nombre = copiarCadena(nombre);
    this->apellido = copiarCadena(apellido);
    this->numeroCamiseta = numeroCamiseta;
    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

Jugador::Jugador(const Jugador& otro) {
    nombre = copiarCadena(otro.nombre);
    apellido = copiarCadena(otro.apellido);
    numeroCamiseta = otro.numeroCamiseta;
    partidosJugados = otro.partidosJugados;
    goles = otro.goles;
    minutosJugados = otro.minutosJugados;
    asistencias = otro.asistencias;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas = otro.tarjetasRojas;
    faltas = otro.faltas;
}

Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

Jugador& Jugador::operator=(const Jugador& otro) {
    if(this != &otro) {
        delete[] nombre;
        delete[] apellido;
        nombre = copiarCadena(otro.nombre);
        apellido = copiarCadena(otro.apellido);
        numeroCamiseta = otro.numeroCamiseta;
        partidosJugados = otro.partidosJugados;
        goles = otro.goles;
        minutosJugados = otro.minutosJugados;
        asistencias = otro.asistencias;
        tarjetasAmarillas = otro.tarjetasAmarillas;
        tarjetasRojas = otro.tarjetasRojas;
        faltas = otro.faltas;
    }
    return *this;
}

const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getApellido() const { return apellido; }
int Jugador::getNumeroCamiseta() const { return numeroCamiseta; }
int Jugador::getPartidosJugados() const { return partidosJugados; }
int Jugador::getGoles() const { return goles; }
int Jugador::getMinutosJugados() const { return minutosJugados; }
int Jugador::getAsistencias() const { return asistencias; }
int Jugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Jugador::getTarjetasRojas() const { return tarjetasRojas; }
int Jugador::getFaltas() const { return faltas; }

void Jugador::imprimir() const {
    cout << "Jugador: " << nombre << " " << apellido
         << " | Camiseta: " << numeroCamiseta << endl;
}