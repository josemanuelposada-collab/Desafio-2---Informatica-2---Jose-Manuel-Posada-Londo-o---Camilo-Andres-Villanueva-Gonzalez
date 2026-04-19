#ifndef FECHA_H
#define FECHA_H

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha();
    Fecha(int dia, int mes, int anio);
    Fecha(const Fecha& otra);
    ~Fecha();

    Fecha& operator=(const Fecha& otra);
    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;

    void setDia(int dia);
    void setMes(int mes);
    void setAnio(int anio);

    int getDia() const;
    int getMes() const;
    int getAnio() const;

    bool esBisiesto() const;
    int diasDelMes() const;
    void sumarUnDia();
    void sumarDias(int cantidad);
    int diferenciaDias(const Fecha& otra) const;
    void imprimir() const;
};

#endif