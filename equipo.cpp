#include "equipo.h"
#include "utilidades.h"
#include <iostream>
using namespace std;
Equipo::Equipo() {
    pais = copiarCadena("");
    confederacion = copiarCadena("");
    directorTecnico = copiarCadena("DT");
    rankingFIFA = 0;
    golesFavorHistoricos = 0;
    golesContraHistoricos = 0;
    partidosGanados = 0;
    partidosEmpatados = 0;
    partidosPerdidos = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
    plantilla = 0;
    cantidadJugadores = 0;
    puntosGrupo = 0;
    golesFavorGrupo = 0;
    golesContraGrupo = 0;
}
Equipo::Equipo(const char* pais, const char* confederacion, const char*
                                                                directorTecnico, int rankingFIFA,
               int gf, int gc, int pg, int pe, int pp, int ta, int tr, int
                   faltas) {
    this->pais = copiarCadena(pais);
    this->confederacion = copiarCadena(confederacion);
    this->directorTecnico = copiarCadena(directorTecnico);
    this->rankingFIFA = rankingFIFA;
    golesFavorHistoricos = gf;
    golesContraHistoricos = gc;
    partidosGanados = pg;
    partidosEmpatados = pe;
    partidosPerdidos = pp;
    tarjetasAmarillas = ta;
    tarjetasRojas = tr;
    this->faltas = faltas;
    plantilla = 0;
    cantidadJugadores = 0;
    puntosGrupo = 0;
    golesFavorGrupo = 0;
    golesContraGrupo = 0;
}
Equipo::Equipo(const Equipo& otro) {
    pais = copiarCadena(otro.pais);
    confederacion = copiarCadena(otro.confederacion);
    directorTecnico = copiarCadena(otro.directorTecnico);
    rankingFIFA = otro.rankingFIFA;
    golesFavorHistoricos = otro.golesFavorHistoricos;
    golesContraHistoricos = otro.golesContraHistoricos;
    partidosGanados = otro.partidosGanados;
    partidosEmpatados = otro.partidosEmpatados;
    partidosPerdidos = otro.partidosPerdidos;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas = otro.tarjetasRojas;
    faltas = otro.faltas;
    cantidadJugadores = otro.cantidadJugadores;
    puntosGrupo = otro.puntosGrupo;
    golesFavorGrupo = otro.golesFavorGrupo;
    golesContraGrupo = otro.golesContraGrupo;
    if(cantidadJugadores > 0) {
        plantilla = new Jugador[cantidadJugadores];
        for(int i = 0; i < cantidadJugadores; i++) {
            plantilla[i] = otro.plantilla[i];
        }
    } else {
        plantilla = 0;
    }
}
Equipo::~Equipo() {
    delete[] pais;
    delete[] confederacion;
    delete[] directorTecnico;
    delete[] plantilla;
}
Equipo& Equipo::operator=(const Equipo& otro) {
    if(this != &otro) {
        delete[] pais;
        delete[] confederacion;
        delete[] directorTecnico;
        delete[] plantilla;
        pais = copiarCadena(otro.pais);
        confederacion = copiarCadena(otro.confederacion);
        directorTecnico = copiarCadena(otro.directorTecnico);
        rankingFIFA = otro.rankingFIFA;
        golesFavorHistoricos = otro.golesFavorHistoricos;
        golesContraHistoricos = otro.golesContraHistoricos;
        partidosGanados = otro.partidosGanados;
        partidosEmpatados = otro.partidosEmpatados;
        partidosPerdidos = otro.partidosPerdidos;
        tarjetasAmarillas = otro.tarjetasAmarillas;
        tarjetasRojas = otro.tarjetasRojas;
        faltas = otro.faltas;
        cantidadJugadores = otro.cantidadJugadores;
        puntosGrupo = otro.puntosGrupo;
        golesFavorGrupo = otro.golesFavorGrupo;
        golesContraGrupo = otro.golesContraGrupo;
        if(cantidadJugadores > 0) {
            plantilla = new Jugador[cantidadJugadores];
            for(int i = 0; i < cantidadJugadores; i++) {
                plantilla[i] = otro.plantilla[i];
            }
        } else {
            plantilla = 0;
        }
    }
    return *this;
}
bool Equipo::operator<(const Equipo& otro) const {
    return rankingFIFA < otro.rankingFIFA;
}
void Equipo::setPais(const char* pais) {
    delete[] this->pais;
    this->pais = copiarCadena(pais);
}
void Equipo::setConfederacion(const char* confederacion) {
    delete[] this->confederacion;
    this->confederacion = copiarCadena(confederacion);
}
void Equipo::setDirectorTecnico(const char* directorTecnico) {
    delete[] this->directorTecnico;
    this->directorTecnico = copiarCadena(directorTecnico);
}
void Equipo::setRankingFIFA(int ranking) { rankingFIFA = ranking; }
void Equipo::setGolesFavorHistoricos(int valor) { golesFavorHistoricos =
        valor; }
void Equipo::setGolesContraHistoricos(int valor) { golesContraHistoricos =
        valor; }
void Equipo::setPartidosGanados(int valor) { partidosGanados = valor; }
void Equipo::setPartidosEmpatados(int valor) { partidosEmpatados = valor; }
void Equipo::setPartidosPerdidos(int valor) { partidosPerdidos = valor; }
void Equipo::setTarjetasAmarillas(int valor) { tarjetasAmarillas = valor; }
void Equipo::setTarjetasRojas(int valor) { tarjetasRojas = valor; }
void Equipo::setFaltas(int valor) { faltas = valor; }
const char* Equipo::getPais() const { return pais; }
const char* Equipo::getConfederacion() const { return confederacion; }
const char* Equipo::getDirectorTecnico() const { return directorTecnico; }
int Equipo::getRankingFIFA() const { return rankingFIFA; }
int Equipo::getGolesFavorHistoricos() const { return golesFavorHistoricos; }
int Equipo::getGolesContraHistoricos() const { return golesContraHistoricos; }
int Equipo::getPartidosGanados() const { return partidosGanados; }
int Equipo::getPartidosEmpatados() const { return partidosEmpatados; }
int Equipo::getPartidosPerdidos() const { return partidosPerdidos; }
int Equipo::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Equipo::getTarjetasRojas() const { return tarjetasRojas; }
int Equipo::getFaltas() const { return faltas; }
int Equipo::getCantidadJugadores() const { return cantidadJugadores; }
Jugador* Equipo::getPlantilla() const { return plantilla; }
int Equipo::getPuntosGrupo() const { return puntosGrupo; }
int Equipo::getGolesFavorGrupo() const { return golesFavorGrupo; }
int Equipo::getGolesContraGrupo() const { return golesContraGrupo; }
int Equipo::getDiferenciaGolesGrupo() const { return golesFavorGrupo -
           golesContraGrupo; }
void Equipo::crearPlantillaArtificial() {
    if(plantilla != 0) delete[] plantilla;
    cantidadJugadores = 26;
    plantilla = new Jugador[cantidadJugadores];
    for(int i = 0; i < cantidadJugadores; i++) {
        char nombre[20];
        char apellido[20];
        char numero[10];
        enteroACadena(i + 1, numero);
        copiarCadenaEnBuffer(nombre, "nombre");
        int lenNom = longitudCadena(nombre);
        int lenNum = longitudCadena(numero);
        for(int j = 0; j < lenNum; j++) nombre[lenNom + j] = numero[j];
        nombre[lenNom + lenNum] = '\0';
        copiarCadenaEnBuffer(apellido, "apellido");
        int lenApe = longitudCadena(apellido);
        for(int j = 0; j < lenNum; j++) apellido[lenApe + j] = numero[j];
        apellido[lenApe + lenNum] = '\0';
        plantilla[i] = Jugador(nombre, apellido, i + 1);
    }
    repartirGolesHistoricos();
}
void Equipo::repartirGolesHistoricos() {
    if(plantilla == 0 || cantidadJugadores == 0) return;
    int golesRestantes = golesFavorHistoricos;
    int i = 0;
    while(golesRestantes > 0) {
        plantilla[i].sumarGol();
        golesRestantes--;
        i++;
        if(i == cantidadJugadores) i = 0;
    }
}
void Equipo::imprimirPlantilla() const {
    cout << "Plantilla de " << pais << endl;
    for(int i = 0; i < cantidadJugadores; i++) {
        plantilla[i].imprimir();
    }
}
void Equipo::imprimirResumen() const {
    cout << "Pais: " << pais
         << " | Confederacion: " << confederacion
         << " | Ranking FIFA: " << rankingFIFA
         << " | GF: " << golesFavorHistoricos
         << " | GC: " << golesContraHistoricos
         << " | PG: " << partidosGanados
         << " | PE: " << partidosEmpatados
         << " | PP: " << partidosPerdidos << endl;
}
void Equipo::reiniciarDatosGrupo() {
    puntosGrupo = 0;
    golesFavorGrupo = 0;
    golesContraGrupo = 0;
}
void Equipo::registrarResultadoGrupo(int gf, int gc) {
    golesFavorGrupo += gf;
    golesContraGrupo += gc;
    if(gf > gc) puntosGrupo += 3;
    else if(gf == gc) puntosGrupo += 1;
}
int* Equipo::obtenerOnceAleatorio() const {
    if(cantidadJugadores < 11) return 0;
    int* indices = new int[cantidadJugadores];
    for(int i = 0; i < cantidadJugadores; i++) indices[i] = i;
    for(int i = 0; i < cantidadJugadores; i++) {
        int j = aleatorioEnRango(0, cantidadJugadores - 1);
        intercambiarEnteros(indices[i], indices[j]);
    }
    int* once = new int[11];
    for(int i = 0; i < 11; i++) once[i] = indices[i];
    delete[] indices;
    return once;
}