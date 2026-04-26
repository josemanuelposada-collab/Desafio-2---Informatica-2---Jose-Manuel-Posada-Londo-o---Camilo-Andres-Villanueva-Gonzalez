#ifndef TORNEO_H
#define TORNEO_H

#include "equipo.h"
#include "grupo.h"
#include "partido.h"
#include "medidor_recursos.h"

class Torneo {
private:
    Equipo* equipos;
    int cantidadEquipos;

    Grupo* grupos;
    int cantidadGrupos;

    Equipo** bombos[4];
    int cantidadesBombos[4];

    Partido* partidosGrupos;
    int cantidadPartidosGrupos;

    Equipo** clasificadosR16;
    int cantidadClasificadosR16;

    Partido* partidosR16;
    int cantidadPartidosR16;

    Partido* partidosR8;
    int cantidadPartidosR8;

    Partido* partidosQF;
    int cantidadPartidosQF;

    Partido* partidosSF;
    int cantidadPartidosSF;

    Partido* partidosFinales;
    int cantidadPartidosFinales;

    Equipo** top4;

    MedidorRecursos medidor;
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

    void generarPartidosDeGrupos();
    void simularFaseDeGrupos();
    void mostrarTablaDeGrupos();

    void clasificarADieciseisavos();
    void mostrarClasificadosR16() const;

    void simularFasesFinales();
    void mostrarEstadisticasFinales();

    int getCantidadEquipos() const;
    Equipo* getEquipo(int i) const;

    long long calcularMemoriaTotal() const;
    void mostrarRecursos(const char* funcionalidad);
};

#endif