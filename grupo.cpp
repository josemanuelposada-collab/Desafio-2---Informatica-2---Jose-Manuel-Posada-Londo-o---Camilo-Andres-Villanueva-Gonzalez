#include "grupo.h"
#include "utilidades.h"
#include <iostream>
using namespace std;
Grupo::Grupo() {
    letra = 'A';
    cantidadEquipos = 0;
    equipos = new Equipo*[4];
    for(int i = 0; i < 4; i++) equipos[i] = 0;
}
Grupo::Grupo(char letra) {
    this->letra = letra;
    cantidadEquipos = 0;
        equipos = new Equipo*[4];
    for(int i = 0; i < 4; i++) equipos[i] = 0;
}
Grupo::Grupo(const Grupo& otro) {
    letra = otro.letra;
    cantidadEquipos = otro.cantidadEquipos;
    equipos = new Equipo*[4];
    for(int i = 0; i < 4; i++) equipos[i] = otro.equipos[i];
}
Grupo::~Grupo() {
    delete[] equipos;
}
Grupo& Grupo::operator=(const Grupo& otro) {
    if(this != &otro) {
        delete[] equipos;
        letra = otro.letra;
        cantidadEquipos = otro.cantidadEquipos;
        equipos = new Equipo*[4];
        for(int i = 0; i < 4; i++) equipos[i] = otro.equipos[i];
    }
    return *this;
}
void Grupo::setLetra(char letra) { this->letra = letra; }
char Grupo::getLetra() const { return letra; }
int Grupo::getCantidadEquipos() const { return cantidadEquipos; }
Equipo* Grupo::getEquipo(int i) const {
    if(i < 0 || i >= cantidadEquipos) return 0;
    return equipos[i];
}
bool Grupo::puedeAgregarEquipo(Equipo* equipo) const {
    if(cantidadEquipos >= 4 || equipo == 0) return false;
    int cantidadUEFA = 0;
    for(int i = 0; i < cantidadEquipos; i++) {
        if(sonIguales(equipos[i]->getConfederacion(), equipo->getConfederacion())) {
            if(sonIguales(equipo->getConfederacion(), "UEFA")) {
                cantidadUEFA++;
            } else {
                return false;
            }
        }
    }
        if(sonIguales(equipo->getConfederacion(), "UEFA") && cantidadUEFA >= 2)
        return false;
    return true;
}
bool Grupo::agregarEquipo(Equipo* equipo) {
    if(!puedeAgregarEquipo(equipo)) return false;
    equipos[cantidadEquipos++] = equipo;
    return true;
}
void Grupo::imprimir() const {
    cout << "Grupo " << letra << endl;
    for(int i = 0; i < cantidadEquipos; i++) {
        cout << " - " << equipos[i]->getPais() << " (" << equipos[i]->getConfederacion() << ")" << endl;
    }
}
void Grupo::ordenarTabla() {
    for(int i = 0; i < cantidadEquipos - 1; i++) {
        for(int j = 0; j < cantidadEquipos - 1 - i; j++) {
            bool cambiar = false;
            if(equipos[j]->getPuntosGrupo() < equipos[j + 1]->getPuntosGrupo())
            {
                cambiar = true;
            } else if(equipos[j]->getPuntosGrupo() == equipos[j + 1]->getPuntosGrupo()) {
                if(equipos[j]->getDiferenciaGolesGrupo() < equipos[j + 1]->getDiferenciaGolesGrupo()) {
                    cambiar = true;
                } else if(equipos[j]->getDiferenciaGolesGrupo() == equipos[j +
                                                                            1]->getDiferenciaGolesGrupo()) {
                    if(equipos[j]->getGolesFavorGrupo() < equipos[j + 1]->getGolesFavorGrupo()) {
                        cambiar = true;
                    }
                }
            }
            if(cambiar) {
                Equipo* temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}
