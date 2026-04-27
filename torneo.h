#ifndef TORNEO_H
#define TORNEO_H

#include "equipo.h"
#include "grupo.h"
#include "partido.h"
#include "medidor_recursos.h"

class Torneo {
private:
    Equipo* equipos;
    Grupo* grupos;
    Partido* partidosGrupos;
    Equipo** clasificadosR16;
    Partido* partidosR16;
    Partido* partidosR8;
    Partido* partidosQF;
    Partido* partidosSF;
    Partido* partidosFinales;
    Equipo** top4;

    Equipo** bombos[4];

    MedidorRecursos medidor;

    int cantidadEquipos;
    int cantidadGrupos;
    int cantidadPartidosGrupos;
    int cantidadClasificadosR16;
    int cantidadPartidosR16;
    int cantidadPartidosR8;
    int cantidadPartidosQF;
    int cantidadPartidosSF;
    int cantidadPartidosFinales;
    int cantidadesBombos[4];

    unsigned char estado;

    static const unsigned char EQUIPOS_CARGADOS = 1 << 0;
    static const unsigned char JUGADORES_CREADOS = 1 << 1;
    static const unsigned char BOMBOS_FORMADOS = 1 << 2;
    static const unsigned char GRUPOS_SORTEADOS = 1 << 3;
    static const unsigned char PARTIDOS_GRUPOS_GENERADOS = 1 << 4;
    static const unsigned char FASE_GRUPOS_SIMULADA = 1 << 5;
    static const unsigned char R16_GENERADO = 1 << 6;
    static const unsigned char FINALES_SIMULADAS = 1 << 7;

    void limpiarDatosSimulacion();
    bool tieneEstado(unsigned char mascara) const;
    void activarEstado(unsigned char mascara);
    void desactivarEstadosDesde(unsigned char mascara);

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

    void guardarHistoricos(const char* nombreArchivo);

    int getCantidadEquipos() const;
    Equipo* getEquipo(int i) const;

    long long calcularMemoriaTotal() const;
    void mostrarRecursos(const char* funcionalidad);
};

#endif