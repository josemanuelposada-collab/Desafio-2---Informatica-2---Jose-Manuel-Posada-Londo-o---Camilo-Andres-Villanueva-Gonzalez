#ifndef REGISTRO_JUGADOR_PARTIDO_H
#define REGISTRO_JUGADOR_PARTIDO_H

#include "jugador.h"

class RegistroJugadorPartido {
private:
    Jugador* jugador;
    int goles;
    int amarillas;
    int rojas;
    int faltas;
    int minutosJugados;

public:
    RegistroJugadorPartido();
    RegistroJugadorPartido(Jugador* jugador);
    RegistroJugadorPartido(const RegistroJugadorPartido& otro);
    ~RegistroJugadorPartido();

    RegistroJugadorPartido& operator=(const RegistroJugadorPartido& otro);

    void setJugador(Jugador* jugador);
    Jugador* getJugador() const;
    int getGoles() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;
    int getMinutosJugados() const;

    void sumarGol();
    void sumarAmarilla();
    void sumarRoja();
    void sumarFalta();
    void setMinutosJugados(int minutos);

    void aplicarAHistorico();
    void imprimir() const;
};

#endif