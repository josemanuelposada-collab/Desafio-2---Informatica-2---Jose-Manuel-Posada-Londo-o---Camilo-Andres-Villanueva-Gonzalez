#include <cmath>
#include "partido.h"
#include "utilidades.h"
#include <iostream>
using namespace std;
Partido::Partido() {
    hora = copiarCadena("00:00");
    sede = copiarCadena("nombreSede");
    etapa = copiarCadena("Grupos");
    arbitros = new char*[3];
    arbitros[0] = copiarCadena("codArbitro1");
    arbitros[1] = copiarCadena("codArbitro2");
    arbitros[2] = copiarCadena("codArbitro3");
    equipo1 = 0;
    equipo2 = 0;
    convocados1 = 0;
    convocados2 = 0;
    cantidadConvocados1 = 0;
    cantidadConvocados2 = 0;
    goles1 = 0;
    goles2 = 0;
    posesion1 = 50;
    posesion2 = 50;
    huboProrroga = false;
}
Partido::Partido(const Partido& otro) {
    fecha = otro.fecha;
    hora = copiarCadena(otro.hora);
    sede = copiarCadena(otro.sede);
    etapa = copiarCadena(otro.etapa);
    arbitros = new char*[3];
    for(int i = 0; i < 3; i++) arbitros[i] = copiarCadena(otro.arbitros[i]);
    equipo1 = otro.equipo1;
    equipo2 = otro.equipo2;
    goles1 = otro.goles1;
    goles2 = otro.goles2;
    posesion1 = otro.posesion1;
    posesion2 = otro.posesion2;
    huboProrroga = otro.huboProrroga;
    cantidadConvocados1 = otro.cantidadConvocados1;
    cantidadConvocados2 = otro.cantidadConvocados2;
    convocados1 = 0;
    convocados2 = 0;
    if(cantidadConvocados1 > 0) {
        convocados1 = new RegistroJugadorPartido[cantidadConvocados1];
        for(int i = 0; i < cantidadConvocados1; i++) convocados1[i] =
                otro.convocados1[i];
    }
    if(cantidadConvocados2 > 0) {
        convocados2 = new RegistroJugadorPartido[cantidadConvocados2];
        for(int i = 0; i < cantidadConvocados2; i++) convocados2[i] =
                otro.convocados2[i];
    }
}
Partido::~Partido() {
    delete[] hora;
    delete[] sede;
    delete[] etapa;
    for(int i = 0; i < 3; i++) delete[] arbitros[i];
    delete[] arbitros;
    delete[] convocados1;
    delete[] convocados2;
}
Partido& Partido::operator=(const Partido& otro) {
    if(this != &otro) {
        delete[] hora;
        delete[] sede;
        delete[] etapa;
        for(int i = 0; i < 3; i++) delete[] arbitros[i];
        delete[] arbitros;
        delete[] convocados1;
        delete[] convocados2;
        fecha = otro.fecha;
        hora = copiarCadena(otro.hora);
        sede = copiarCadena(otro.sede);
        etapa = copiarCadena(otro.etapa);
        arbitros = new char*[3];
        for(int i = 0; i < 3; i++) arbitros[i] = copiarCadena(otro.arbitros[i]);
        equipo1 = otro.equipo1;
        equipo2 = otro.equipo2;
        goles1 = otro.goles1;
        goles2 = otro.goles2;
        posesion1 = otro.posesion1;
        posesion2 = otro.posesion2;
        huboProrroga = otro.huboProrroga;
        cantidadConvocados1 = otro.cantidadConvocados1;
        cantidadConvocados2 = otro.cantidadConvocados2;
        convocados1 = 0;
        convocados2 = 0;
        if(cantidadConvocados1 > 0) {
            convocados1 = new RegistroJugadorPartido[cantidadConvocados1];
            for(int i = 0; i < cantidadConvocados1; i++) convocados1[i] =
                    otro.convocados1[i];
        }
        if(cantidadConvocados2 > 0) {
            convocados2 = new RegistroJugadorPartido[cantidadConvocados2];
            for(int i = 0; i < cantidadConvocados2; i++) convocados2[i] =
                    otro.convocados2[i];
        }
    }
    return *this;
}
void Partido::setFecha(const Fecha& fecha) { this->fecha = fecha; }
void Partido::setHora(const char* hora) {
    delete[] this->hora;
    this->hora = copiarCadena(hora);
}
void Partido::setSede(const char* sede) {
    delete[] this->sede;
    this->sede = copiarCadena(sede);
}
void Partido::setEtapa(const char* etapa) {
    delete[] this->etapa;
    this->etapa = copiarCadena(etapa);
}
void Partido::setEquipo1(Equipo* equipo) { equipo1 = equipo; }
void Partido::setEquipo2(Equipo* equipo) { equipo2 = equipo; }
void Partido::configurarArbitrosFijos() {
    delete[] arbitros[0];
    delete[] arbitros[1];
    delete[] arbitros[2];
    arbitros[0] = copiarCadena("codArbitro1");
    arbitros[1] = copiarCadena("codArbitro2");
    arbitros[2] = copiarCadena("codArbitro3");
}
Fecha Partido::getFecha() const { return fecha; }
Equipo* Partido::getEquipo1() const { return equipo1; }
Equipo* Partido::getEquipo2() const { return equipo2; }
int Partido::getGoles1() const { return goles1; }
int Partido::getGoles2() const { return goles2; }
bool Partido::getHuboProrroga() const { return huboProrroga; }
const char* Partido::getEtapa() const { return etapa; }
void Partido::prepararConvocados() {
    delete[] convocados1;
    delete[] convocados2;
    cantidadConvocados1 = 11;
    cantidadConvocados2 = 11;
    convocados1 = new RegistroJugadorPartido[11];
    convocados2 = new RegistroJugadorPartido[11];
    int* once1 = equipo1->obtenerOnceAleatorio();
    int* once2 = equipo2->obtenerOnceAleatorio();
    for(int i = 0; i < 11; i++) {
        convocados1[i] = RegistroJugadorPartido(&equipo1->getPlantilla()
                                                    [once1[i]]);
        convocados2[i] = RegistroJugadorPartido(&equipo2->getPlantilla()
                                                     [once2[i]]);
    }
    delete[] once1;
    delete[] once2;
}
void Partido::calcularPosesion() {
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();
    int fuerza1 = 250 - r1;
    int fuerza2 = 250 - r2;
    if(fuerza1 < 1) fuerza1 = 1;
    if(fuerza2 < 1) fuerza2 = 1;
    int total = fuerza1 + fuerza2;
    posesion1 = (fuerza1 * 100) / total;
    if(posesion1 < 35) posesion1 = 35;
    if(posesion1 > 65) posesion1 = 65;
    posesion2 = 100 - posesion1;
}
int Partido::calcularGolesEsperados(Equipo* atacante, Equipo* defensor) const {
    int pjAtacante = atacante->getPartidosGanados()
    + atacante->getPartidosEmpatados()
        + atacante->getPartidosPerdidos();

    int pjDefensor = defensor->getPartidosGanados()
                     + defensor->getPartidosEmpatados()
                     + defensor->getPartidosPerdidos();

    if(pjAtacante <= 0) pjAtacante = 1;
    if(pjDefensor <= 0) pjDefensor = 1;

    double GFA = (double)atacante->getGolesFavorHistoricos() / pjAtacante;
    double GCB = (double)defensor->getGolesContraHistoricos() / pjDefensor;

    double mu = 1.35;
    double alfa = 0.6;
    double beta = 0.4;

    double lambda = mu * pow(GFA / mu, alfa) * pow(GCB / mu, beta);

    int goles = (int)lambda;
    double parteDecimal = lambda - goles;

    if(aleatorioEnRango(0, 99) < (int)(parteDecimal * 100.0)) {
        goles++;
    }

    if(goles < 0) goles = 0;
    if(goles > 6) goles = 6;

    return goles;
}
void Partido::simularTarjetasYFaltas(RegistroJugadorPartido* registros, int cantidad, int minutos) {
    for(int i = 0; i < cantidad; i++) {
        registros[i].setMinutosJugados(minutos);
        if(aleatorioEnRango(1, 10000) <= 1300) registros[i].sumarFalta();
        if(aleatorioEnRango(1, 10000) <= 275) registros[i].sumarFalta();
        if(aleatorioEnRango(1, 10000) <= 70) registros[i].sumarFalta();
        bool primeraAmarilla = false;
        if(aleatorioEnRango(1, 10000) <= 600) {
            registros[i].sumarAmarilla();
            primeraAmarilla = true;
        }
        if(primeraAmarilla && aleatorioEnRango(1, 10000) <= 115) {
            registros[i].sumarAmarilla();
            registros[i].sumarRoja();
        }
    }
}
void Partido::repartirGoles(RegistroJugadorPartido* registros, int cantidad,int golesEquipo) {
    int asignados = 0;
    int intentos = 0;
    while(asignados < golesEquipo && intentos < 1000) {
        int idx = aleatorioEnRango(0, cantidad - 1);
        if(aleatorioEnRango(1, 10000) <= 400) {
            registros[idx].sumarGol();
            asignados++;
        }
        intentos++;
    }
    while(asignados < golesEquipo) {
        int idx = aleatorioEnRango(0, cantidad - 1);
        registros[idx].sumarGol();
        asignados++;
    }
}
void Partido::simular(bool permitirEmpate) {
    if(equipo1 == 0 || equipo2 == 0) return;
    goles1 = 0;
    goles2 = 0;
    huboProrroga = false;
    prepararConvocados();
    calcularPosesion();
    goles1 = calcularGolesEsperados(equipo1, equipo2);
    goles2 = calcularGolesEsperados(equipo2, equipo1);
    if(!permitirEmpate && goles1 == goles2) {
        huboProrroga = true;
        int r1 = equipo1->getRankingFIFA();
        int r2 = equipo2->getRankingFIFA();
        int total = r1 + r2;
        if(total <= 0) total = 1;
        int probEquipo1 = (r2 * 100) / total;
        if(aleatorioEnRango(1, 100) <= probEquipo1) goles1++;
        else goles2++;
    }
    int minutos = huboProrroga ? 120 : 90;
    simularTarjetasYFaltas(convocados1, cantidadConvocados1, minutos);
    simularTarjetasYFaltas(convocados2, cantidadConvocados2, minutos);
    repartirGoles(convocados1, cantidadConvocados1, goles1);
    repartirGoles(convocados2, cantidadConvocados2, goles2);
    actualizarHistoricos();
}
void Partido::actualizarHistoricos() {
    if(equipo1 == 0 || equipo2 == 0) return;
    equipo1->setGolesFavorHistoricos(equipo1->getGolesFavorHistoricos() + goles1);
    equipo1->setGolesContraHistoricos(equipo1->getGolesContraHistoricos() + goles2);
    equipo2->setGolesFavorHistoricos(equipo2->getGolesFavorHistoricos() + goles2);
    equipo2->setGolesContraHistoricos(equipo2->getGolesContraHistoricos() +  goles1);
    if(sonIguales(etapa, "Grupos")) {
        equipo1->registrarResultadoGrupo(goles1, goles2);
        equipo2->registrarResultadoGrupo(goles2, goles1);
    }
    if(goles1 > goles2) {
        equipo1->setPartidosGanados(equipo1->getPartidosGanados() + 1);
        equipo2->setPartidosPerdidos(equipo2->getPartidosPerdidos() + 1);
    } else if(goles2 > goles1) {
        equipo2->setPartidosGanados(equipo2->getPartidosGanados() + 1);
        equipo1->setPartidosPerdidos(equipo1->getPartidosPerdidos() + 1);
    } else {
        equipo1->setPartidosEmpatados(equipo1->getPartidosEmpatados() + 1);
        equipo2->setPartidosEmpatados(equipo2->getPartidosEmpatados() + 1);
    }
    for(int i = 0; i < cantidadConvocados1; i++)
        convocados1[i].aplicarAHistorico();
    for(int i = 0; i < cantidadConvocados2; i++)
        convocados2[i].aplicarAHistorico();
}
void Partido::imprimir() const {
    if(equipo1 == 0 || equipo2 == 0) return;

    cout << "Etapa: " << etapa << endl;

    cout << "Fecha: ";
    fecha.imprimir();
    cout << " | Hora: " << hora << " | Sede: " << sede << endl;

    cout << "Arbitros: ";
    if(arbitros != 0) {
        for(int i = 0; i < 3; i++) {
            if(arbitros[i] != 0) {
                cout << arbitros[i];
                if(i < 2) cout << ", ";
            }
        }
    } else {
        cout << "sin asignar";
    }
    cout << endl;

    cout << equipo1->getPais() << " " << goles1 << " - " << goles2 << " "
         << equipo2->getPais();

    if(huboProrroga) cout << " (Prorroga)";

    cout << endl;

    cout << "Posesion: " << equipo1->getPais() << " " << posesion1 << "% - "
         << posesion2 << "% " << equipo2->getPais() << endl;
}
void Partido::imprimirGoleadores() const {
    cout << "Goleadores " << equipo1->getPais() << ": ";
    bool hay = false;
    for(int i = 0; i < cantidadConvocados1; i++) {
        for(int j = 0; j < convocados1[i].getGoles(); j++) {
            cout << convocados1[i].getJugador()->getNumeroCamiseta() << " ";
            hay = true;
        }
    }
    if(!hay) cout << "ninguno";
    cout << endl;
    cout << "Goleadores " << equipo2->getPais() << ": ";
    hay = false;
    for(int i = 0; i < cantidadConvocados2; i++) {
        for(int j = 0; j < convocados2[i].getGoles(); j++) {
            cout << convocados2[i].getJugador()->getNumeroCamiseta() << " ";
            hay = true;
        }
    }
    if(!hay) cout << "ninguno";
    cout << endl;
}

void Partido::imprimirResumenDisciplinario() const {
    if(equipo1 == 0 || equipo2 == 0 || convocados1 == 0 || convocados2 == 0) {
        return;
    }

    cout << "Convocados " << equipo1->getPais() << ": ";

    for(int i = 0; i < cantidadConvocados1; i++) {
        Jugador* j = convocados1[i].getJugador();

        if(j != 0) {
            cout << "#" << j->getNumeroCamiseta()
            << "(G:" << convocados1[i].getGoles()
            << ",A:" << convocados1[i].getAmarillas()
            << ",R:" << convocados1[i].getRojas()
            << ",F:" << convocados1[i].getFaltas()
            << ",Min:" << convocados1[i].getMinutosJugados()
            << ") ";
        }
    }

    cout << endl;

    cout << "Convocados " << equipo2->getPais() << ": ";

    for(int i = 0; i < cantidadConvocados2; i++) {
        Jugador* j = convocados2[i].getJugador();

        if(j != 0) {
            cout << "#" << j->getNumeroCamiseta()
            << "(G:" << convocados2[i].getGoles()
            << ",A:" << convocados2[i].getAmarillas()
            << ",R:" << convocados2[i].getRojas()
            << ",F:" << convocados2[i].getFaltas()
            << ",Min:" << convocados2[i].getMinutosJugados()
            << ") ";
        }
    }

    cout << endl;
}