#ifndef GRUPO_H
#define GRUPO_H
#include "equipo.h"
class Grupo {
    private:
              char letra;
    Equipo** equipos;
    int cantidadEquipos;
public:
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);
    ~Grupo();
    Grupo& operator=(const Grupo& otro);
    void setLetra(char letra);
    char getLetra() const;
    int getCantidadEquipos() const;
    Equipo* getEquipo(int i) const;
    bool agregarEquipo(Equipo* equipo);
    bool puedeAgregarEquipo(Equipo* equipo) const;
    void imprimir() const;
    void ordenarTabla();
};
#endif
