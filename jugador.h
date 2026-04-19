#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador {
private:
    char* nombre;
    char* apellido;
    int numeroCamiseta;
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    Jugador();
    Jugador(const char* nombre, const char* apellido, int numeroCamiseta);
    Jugador(const Jugador& otro);
    ~Jugador();

    Jugador& operator=(const Jugador& otro);

    const char* getNombre() const;
    const char* getApellido() const;
    int getNumeroCamiseta() const;
    int getPartidosJugados() const;
    int getGoles() const;
    int getMinutosJugados() const;
    int getAsistencias() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;

    void sumarGol();
    void sumarPartido();
    void sumarMinutos(int minutos);
    void sumarAsistencia();
    void sumarAmarilla();
    void sumarRoja();
    void sumarFalta();

    void imprimir() const;
};

#endif