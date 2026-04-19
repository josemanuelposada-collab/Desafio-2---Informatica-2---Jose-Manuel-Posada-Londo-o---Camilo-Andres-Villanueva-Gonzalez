#include "registro_jugador_partido.h"
#include <iostream>

using namespace std;

RegistroJugadorPartido::RegistroJugadorPartido() {
    jugador = 0;
    goles = 0;
    amarillas = 0;
    rojas = 0;
    faltas = 0;
    minutosJugados = 0;
}

RegistroJugadorPartido::RegistroJugadorPartido(Jugador* jugador) {
    this->jugador = jugador;
    goles = 0;
    amarillas = 0;
    rojas = 0;
    faltas = 0;
    minutosJugados = 0;
}

RegistroJugadorPartido::RegistroJugadorPartido(const RegistroJugadorPartido& otro) {
    jugador = otro.jugador;
    goles = otro.goles;
    amarillas = otro.amarillas;
    rojas = otro.rojas;
    faltas = otro.faltas;
    minutosJugados = otro.minutosJugados;
}

RegistroJugadorPartido::~RegistroJugadorPartido() {
}

RegistroJugadorPartido& RegistroJugadorPartido::operator=(const RegistroJugadorPartido& otro) {
    if(this != &otro) {
        jugador = otro.jugador;
        goles = otro.goles;
        amarillas = otro.amarillas;
        rojas = otro.rojas;
        faltas = otro.faltas;
        minutosJugados = otro.minutosJugados;
    }
    return *this;
}

void RegistroJugadorPartido::setJugador(Jugador* jugador) {
    this->jugador = jugador;
}

Jugador* RegistroJugadorPartido::getJugador() const { return jugador; }
int RegistroJugadorPartido::getGoles() const { return goles; }
int RegistroJugadorPartido::getAmarillas() const { return amarillas; }
int RegistroJugadorPartido::getRojas() const { return rojas; }
int RegistroJugadorPartido::getFaltas() const { return faltas; }
int RegistroJugadorPartido::getMinutosJugados() const { return minutosJugados; }

void RegistroJugadorPartido::sumarGol() { goles++; }
void RegistroJugadorPartido::sumarAmarilla() { amarillas++; }
void RegistroJugadorPartido::sumarRoja() { rojas++; }
void RegistroJugadorPartido::sumarFalta() { faltas++; }
void RegistroJugadorPartido::setMinutosJugados(int minutos) { minutosJugados = minutos; }

void RegistroJugadorPartido::aplicarAHistorico() {
    if(jugador == 0) return;

    jugador->sumarPartido();
    jugador->sumarMinutos(minutosJugados);

    for(int i = 0; i < goles; i++) jugador->sumarGol();
    for(int i = 0; i < amarillas; i++) jugador->sumarAmarilla();
    for(int i = 0; i < rojas; i++) jugador->sumarRoja();
    for(int i = 0; i < faltas; i++) jugador->sumarFalta();
}

void RegistroJugadorPartido::imprimir() const {
    if(jugador == 0) return;

    cout << "#" << jugador->getNumeroCamiseta()
         << " G:" << goles
         << " A:" << amarillas
         << " R:" << rojas
         << " F:" << faltas
         << " Min:" << minutosJugados << endl;
}