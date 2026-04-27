#include "medidor_recursos.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

MedidorRecursos::MedidorRecursos() {
    iteraciones = 0;
}

MedidorRecursos::~MedidorRecursos() {
}

void MedidorRecursos::reiniciar() {
    iteraciones = 0;
}

void MedidorRecursos::sumarIteracion() {
    iteraciones++;
}

void MedidorRecursos::sumarIteraciones(long long cantidad) {
    iteraciones += cantidad;
}

long long MedidorRecursos::getIteraciones() const {
    return iteraciones;
}

long long MedidorRecursos::memoriaJugador(const Jugador& jugador) const {
    long long total = sizeof(Jugador);

    total += longitudCadena(jugador.getNombre()) + 1;
    total += longitudCadena(jugador.getApellido()) + 1;

    return total;
}

long long MedidorRecursos::memoriaEquipo(const Equipo& equipo) const {
    long long total = sizeof(Equipo);

    total += longitudCadena(equipo.getPais()) + 1;
    total += longitudCadena(equipo.getConfederacion()) + 1;
    total += longitudCadena(equipo.getDirectorTecnico()) + 1;

    Jugador* plantilla = equipo.getPlantilla();

    if(plantilla != 0) {
        for(int i = 0; i < equipo.getCantidadJugadores(); i++) {
            total += memoriaJugador(plantilla[i]);
        }
    }

    return total;
}

long long MedidorRecursos::memoriaGrupo(const Grupo& grupo) const {
    (void)grupo;

    long long total = sizeof(Grupo);

    total += 4 * sizeof(Equipo*);

    return total;
}

long long MedidorRecursos::memoriaPartido(const Partido& partido) const {
    (void)partido;

    long long total = sizeof(Partido);

    total += 20;                         // hora
    total += 30;                         // sede
    total += 30;                         // etapa
    total += 3 * 30;                     // arbitros
    total += 22 * sizeof(RegistroJugadorPartido); // 11 + 11 convocados aproximados

    return total;
}

long long MedidorRecursos::memoriaEquipos(Equipo* equipos, int cantidadEquipos) const {
    if(equipos == 0 || cantidadEquipos <= 0) return 0;

    long long total = 0;

    for(int i = 0; i < cantidadEquipos; i++) {
        total += memoriaEquipo(equipos[i]);
    }

    return total;
}

long long MedidorRecursos::memoriaGrupos(Grupo* grupos, int cantidadGrupos) const {
    if(grupos == 0 || cantidadGrupos <= 0) return 0;

    long long total = 0;

    for(int i = 0; i < cantidadGrupos; i++) {
        total += memoriaGrupo(grupos[i]);
    }

    return total;
}

long long MedidorRecursos::memoriaPartidos(Partido* partidos, int cantidadPartidos) const {
    if(partidos == 0 || cantidadPartidos <= 0) return 0;

    long long total = 0;

    for(int i = 0; i < cantidadPartidos; i++) {
        total += memoriaPartido(partidos[i]);
    }

    return total;
}

long long MedidorRecursos::memoriaPunterosEquipo(Equipo** lista, int cantidad) const {
    if(lista == 0 || cantidad <= 0) return 0;

    return cantidad * sizeof(Equipo*);
}

void MedidorRecursos::imprimirResumen(const char* funcionalidad, long long memoria) const {
    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: " << funcionalidad << endl;
    cout << "Iteraciones estimadas: " << iteraciones << endl;
    cout << "Memoria estimada: " << memoria << " bytes" << endl;
}