#ifndef PARTIDO_H
#define PARTIDO_H
#include "equipo.h"
#include "fecha.h"
#include "registro_jugador_partido.h"
class Partido {
private:
    Fecha fecha;
    char* hora;
    char* sede;
    char** arbitros;
    char* etapa;
    Equipo* equipo1;
    Equipo* equipo2;
    RegistroJugadorPartido* convocados1;
    RegistroJugadorPartido* convocados2;
    int cantidadConvocados1;
    int cantidadConvocados2;
    int goles1;
    int goles2;
    int posesion1;
    int posesion2;
    bool huboProrroga;
public:
    Partido();
    Partido(const Partido& otro);
    ~Partido();
    Partido& operator=(const Partido& otro);
    void setFecha(const Fecha& fecha);
    void setHora(const char* hora);
    void setSede(const char* sede);
    void setEtapa(const char* etapa);
    void setEquipo1(Equipo* equipo);
    void setEquipo2(Equipo* equipo);
    void configurarArbitrosFijos();
    Fecha getFecha() const;
    Equipo* getEquipo1() const;
    Equipo* getEquipo2() const;
    int getGoles1() const;
    int getGoles2() const;
    bool getHuboProrroga() const;
    const char* getEtapa() const;
    void prepararConvocados();
    void calcularPosesion();
    int calcularGolesEsperados(Equipo* atacante, Equipo* defensor) const;
    void simularTarjetasYFaltas(RegistroJugadorPartido* registros, int cantidad, int minutos);
    void repartirGoles(RegistroJugadorPartido* registros, int cantidad, int golesEquipo);
    void simular(bool permitirEmpate);
    void actualizarHistoricos();
    void imprimir() const;
    void imprimirGoleadores() const;
    void imprimirResumenDisciplinario() const;
};
#endif