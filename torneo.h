#ifndef TORNEO_H
#define TORNEO_H

#include "equipo.h"
#include "grupo.h"

class Torneo {
private:
    Equipo* equipos;
    int cantidadEquipos;

    Grupo* grupos;
    int cantidadGrupos;

    Equipo** bombos[4];
    int cantidadesBombos[4];

public:
    Torneo();
    ~Torneo();

    void cargarEquiposDesdeCSV(const char* nombreArchivo);
    void crearJugadoresDeTodosLosEquipos();
    void mostrarEquipos() const;

    void ordenarEquiposPorRanking();
    void formarBombos();
    void mostrarBombos() const;

    void sortearGrupos();
    void mostrarGrupos() const;

    int getCantidadEquipos() const;
    Equipo* getEquipo(int i) const;
};

#endif