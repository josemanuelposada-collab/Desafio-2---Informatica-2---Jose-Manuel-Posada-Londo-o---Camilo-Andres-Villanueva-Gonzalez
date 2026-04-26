#include "torneo.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>

using namespace std;

bool esNumeroCSV(const char* cad) {
    if(cad == 0) return false;

    int i = 0;

    while(cad[i] == ' ') i++;

    if(cad[i] == '\0' || cad[i] == '\r') return false;

    while(cad[i] != '\0' && cad[i] != '\r') {
        if(cad[i] < '0' || cad[i] > '9') {
            return false;
        }
        i++;
    }

    return true;
}

int indiceConfederacion(const char* confederacion) {
    if(sonIguales(confederacion, "UEFA")) return 0;
    if(sonIguales(confederacion, "CONMEBOL")) return 1;
    if(sonIguales(confederacion, "CONCACAF") ||
        sonIguales(confederacion, "Concacaf")) return 2;
    if(sonIguales(confederacion, "CAF")) return 3;
    if(sonIguales(confederacion, "AFC")) return 4;
    if(sonIguales(confederacion, "OFC")) return 5;
    return 6;
}
const char* nombreConfederacionPorIndice(int indice) {
    if(indice == 0) return "UEFA";
    if(indice == 1) return "CONMEBOL";
    if(indice == 2) return "CONCACAF";
    if(indice == 3) return "CAF";
    if(indice == 4) return "AFC";
    if(indice == 5) return "OFC";
    return "OTRA";
}

void imprimirConfederacionMayor(const char* etapa, Equipo** lista, int cantidad) {
    int conteos[7];

    for(int i = 0; i < 7; i++) {
        conteos[i] = 0;
    }

    for(int i = 0; i < cantidad; i++) {
        if(lista[i] != 0) {
            int indice = indiceConfederacion(lista[i]->getConfederacion());
            conteos[indice]++;
        }
    }

    int mayor = 0;

    for(int i = 1; i < 7; i++) {
        if(conteos[i] > conteos[mayor]) {
            mayor = i;
        }
    }

    cout << "Confederacion con mayor presencia en "
         << etapa
         << ": "
         << nombreConfederacionPorIndice(mayor)
         << " con "
         << conteos[mayor]
         << " equipos"
         << endl;
}


bool esMejorEnTabla(Equipo* a, Equipo* b) {
    if(a == 0 && b == 0) return false;
    if(a != 0 && b == 0) return true;
    if(a == 0 && b != 0) return false;

    if(a->getPuntosGrupo() != b->getPuntosGrupo())
        return a->getPuntosGrupo() > b->getPuntosGrupo();

    if(a->getDiferenciaGolesGrupo() != b->getDiferenciaGolesGrupo())
        return a->getDiferenciaGolesGrupo() > b->getDiferenciaGolesGrupo();

    if(a->getGolesFavorGrupo() != b->getGolesFavorGrupo())
        return a->getGolesFavorGrupo() > b->getGolesFavorGrupo();

    return aleatorioEnRango(0, 1) == 1;
}

bool esPeorEnTabla(Equipo* a, Equipo* b) {
    if(a == 0 && b == 0) return false;
    if(a != 0 && b == 0) return true;
    if(a == 0 && b != 0) return false;

    if(a->getPuntosGrupo() != b->getPuntosGrupo())
        return a->getPuntosGrupo() < b->getPuntosGrupo();

    if(a->getDiferenciaGolesGrupo() != b->getDiferenciaGolesGrupo())
        return a->getDiferenciaGolesGrupo() < b->getDiferenciaGolesGrupo();

    if(a->getGolesFavorGrupo() != b->getGolesFavorGrupo())
        return a->getGolesFavorGrupo() < b->getGolesFavorGrupo();

    return aleatorioEnRango(0, 1) == 1;
}

void intercambiarEquipoYGrupo(Equipo** lista, char* grupos, int i, int j) {
    Equipo* tempEquipo = lista[i];
    lista[i] = lista[j];
    lista[j] = tempEquipo;

    char tempGrupo = grupos[i];
    grupos[i] = grupos[j];
    grupos[j] = tempGrupo;
}

Torneo::Torneo() {
    equipos = 0;
    cantidadEquipos = 0;

    grupos = 0;
    cantidadGrupos = 12;

    partidosGrupos = 0;
    cantidadPartidosGrupos = 0;

    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    partidosQF = 0;
    cantidadPartidosQF = 4;

    partidosSF = 0;
    cantidadPartidosSF = 2;

    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    top4 = new Equipo*[4];
    for(int i = 0; i < 4; i++) {
        top4[i] = 0;
    }

    for(int i = 0; i < 4; i++) {
        bombos[i] = 0;
        cantidadesBombos[i] = 0;
    }

    estado = 0;

    inicializarAleatorio();
}

Torneo::~Torneo() {
    delete[] equipos;
    equipos = 0;

    delete[] grupos;
    grupos = 0;

    delete[] partidosGrupos;
    partidosGrupos = 0;

    delete[] clasificadosR16;
    clasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;

    delete[] partidosR8;
    partidosR8 = 0;

    delete[] partidosQF;
    partidosQF = 0;

    delete[] partidosSF;
    partidosSF = 0;

    delete[] partidosFinales;
    partidosFinales = 0;

    delete[] top4;
    top4 = 0;

    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
        bombos[i] = 0;
        cantidadesBombos[i] = 0;
    }

    cantidadEquipos = 0;
    cantidadClasificadosR16 = 0;
    cantidadPartidosGrupos = 0;
    estado = 0;
}

bool Torneo::tieneEstado(unsigned char mascara) const {
    return (estado & mascara) != 0;
}

void Torneo::activarEstado(unsigned char mascara) {
    estado = estado | mascara;
}

void Torneo::desactivarEstadosDesde(unsigned char mascara) {
    estado = estado & (~mascara);
}

void Torneo::limpiarDatosSimulacion() {
    delete[] grupos;
    grupos = 0;

    delete[] partidosGrupos;
    partidosGrupos = 0;
    cantidadPartidosGrupos = 0;

    delete[] clasificadosR16;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
        bombos[i] = 0;
        cantidadesBombos[i] = 0;
    }

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    estado = estado & EQUIPOS_CARGADOS;
}

void Torneo::cargarEquiposDesdeCSV(const char* nombreArchivo) {
    medidor.reiniciar();
    limpiarDatosSimulacion();


    if(equipos != 0) {
        delete[] equipos;
        equipos = 0;
        cantidadEquipos = 0;
    }

    estado = 0;

    ifstream archivo(nombreArchivo);

    if(!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        mostrarRecursos("Carga de equipos desde CSV");
        return;
    }

    equipos = new Equipo[48];
    cantidadEquipos = 0;

    char linea[512];

    while(archivo.getline(linea, 512) && cantidadEquipos < 48) {
        medidor.sumarIteracion();

        char rankingStr[20] = "0";
        char pais[100] = "";
        char director[100] = "DT";
        char federacion[120] = "";
        char confederacion[50] = "";

        char gfStr[20] = "0";
        char gcStr[20] = "0";
        char pgStr[20] = "0";
        char peStr[20] = "0";
        char ppStr[20] = "0";

        int campo = 0;
        int pos = 0;
        int b = 0;
        char buffer[150];

        while(true) {
            medidor.sumarIteracion();

            char c = linea[pos];

            if(c == ';' || c == '\0') {
                buffer[b] = '\0';

                if(campo == 0) copiarCadenaEnBuffer(rankingStr, buffer);
                else if(campo == 1) copiarCadenaEnBuffer(pais, buffer);
                else if(campo == 2) copiarCadenaEnBuffer(director, buffer);
                else if(campo == 3) copiarCadenaEnBuffer(federacion, buffer);
                else if(campo == 4) copiarCadenaEnBuffer(confederacion, buffer);
                else if(campo == 5) copiarCadenaEnBuffer(gfStr, buffer);
                else if(campo == 6) copiarCadenaEnBuffer(gcStr, buffer);
                else if(campo == 7) copiarCadenaEnBuffer(pgStr, buffer);
                else if(campo == 8) copiarCadenaEnBuffer(peStr, buffer);
                else if(campo == 9) copiarCadenaEnBuffer(ppStr, buffer);

                campo++;
                b = 0;

                if(c == '\0') break;
            } else {
                buffer[b] = c;
                b++;
            }

            pos++;
        }

        if(!esNumeroCSV(rankingStr)) {
            continue;
        }

        equipos[cantidadEquipos] = Equipo(
            pais,
            confederacion,
            director,
            stringAEntero(rankingStr),
            stringAEntero(gfStr),
            stringAEntero(gcStr),
            stringAEntero(pgStr),
            stringAEntero(peStr),
            stringAEntero(ppStr),
            0,
            0,
            0
            );

        cantidadEquipos++;
    }

    archivo.close();

    cout << "Equipos cargados correctamente." << endl;
    cout << "Cantidad de equipos leidos: " << cantidadEquipos << endl;

    activarEstado(EQUIPOS_CARGADOS);

    mostrarRecursos("Carga de equipos desde CSV");
}

void Torneo::crearJugadoresDeTodosLosEquipos() {
    medidor.reiniciar();

    if(!tieneEstado(EQUIPOS_CARGADOS) || equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        mostrarRecursos("Creacion de jugadores artificiales");
        return;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        equipos[i].crearPlantillaArtificial();
        medidor.sumarIteracion();
    }

    cout << "Jugadores artificiales creados correctamente." << endl;

    activarEstado(JUGADORES_CREADOS);

    mostrarRecursos("Creacion de jugadores artificiales");
}

void Torneo::mostrarEquipos() const {
    if(equipos == 0) {
        cout << "No hay equipos cargados." << endl;
        return;
    }

    cout << "\n===== EQUIPOS CARGADOS =====\n";

    for(int i = 0; i < cantidadEquipos; i++) {
        cout << "[" << i + 1 << "] ";
        equipos[i].imprimirResumen();
    }

    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: Mostrar equipos cargados" << endl;
    cout << "Iteraciones estimadas: " << cantidadEquipos << endl;
    cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;
}

void Torneo::ordenarEquiposPorRanking() {
    if(equipos == 0) return;

    for(int i = 0; i < cantidadEquipos - 1; i++) {
        for(int j = 0; j < cantidadEquipos - 1 - i; j++) {
            medidor.sumarIteracion();

            if(equipos[j + 1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

void Torneo::formarBombos() {
    medidor.reiniciar();

    if(!tieneEstado(EQUIPOS_CARGADOS) || equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        mostrarRecursos("Formacion de bombos");
        return;
    }

    ordenarEquiposPorRanking();

    for(int i = 0; i < 4; i++) {
        medidor.sumarIteracion();

        delete[] bombos[i];
        bombos[i] = 0;
        bombos[i] = new Equipo*[12];
        cantidadesBombos[i] = 0;
    }

    int indiceUSA = -1;

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        if(sonIguales(equipos[i].getPais(), "EE. UU.") ||
            sonIguales(equipos[i].getPais(), "Estados Unidos") ||
            sonIguales(equipos[i].getPais(), "USA") ||
            sonIguales(equipos[i].getPais(), "United States")) {
            indiceUSA = i;
            break;
        }
    }

    if(indiceUSA != -1) {
        bombos[0][cantidadesBombos[0]] = &equipos[indiceUSA];
        cantidadesBombos[0]++;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        if(i == indiceUSA) continue;

        if(cantidadesBombos[0] < 12) {
            bombos[0][cantidadesBombos[0]] = &equipos[i];
            cantidadesBombos[0]++;
        } else if(cantidadesBombos[1] < 12) {
            bombos[1][cantidadesBombos[1]] = &equipos[i];
            cantidadesBombos[1]++;
        } else if(cantidadesBombos[2] < 12) {
            bombos[2][cantidadesBombos[2]] = &equipos[i];
            cantidadesBombos[2]++;
        } else if(cantidadesBombos[3] < 12) {
            bombos[3][cantidadesBombos[3]] = &equipos[i];
            cantidadesBombos[3]++;
        }
    }

    cout << "Bombos formados correctamente." << endl;

    activarEstado(BOMBOS_FORMADOS);
    desactivarEstadosDesde(GRUPOS_SORTEADOS | PARTIDOS_GRUPOS_GENERADOS | FASE_GRUPOS_SIMULADA | R16_GENERADO | FINALES_SIMULADAS);

    mostrarRecursos("Formacion de bombos");
}

void Torneo::mostrarBombos() const {


    if(!tieneEstado(BOMBOS_FORMADOS) || bombos[0] == 0 || bombos[1] == 0 ||
        bombos[2] == 0 || bombos[3] == 0) {
        cout << "Primero debe formar los bombos." << endl;
        return;
    }


    cout << "\n===== BOMBOS DEL SORTEO =====\n";

    int iteraciones = 0;

    for(int i = 0; i < 4; i++) {
        cout << "\n===== BOMBO " << i + 1 << " =====\n";

        for(int j = 0; j < cantidadesBombos[i]; j++) {
            iteraciones++;

            cout << " - "
                 << bombos[i][j]->getPais()
                 << " | Confederacion: "
                 << bombos[i][j]->getConfederacion()
                 << " | Ranking: "
                 << bombos[i][j]->getRankingFIFA()
                 << endl;
        }
    }

    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: Mostrar bombos" << endl;
    cout << "Iteraciones estimadas: " << iteraciones << endl;
    cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;
}

void Torneo::sortearGrupos() {
    medidor.reiniciar();

    if(!tieneEstado(BOMBOS_FORMADOS) || bombos[0] == 0 || bombos[1] == 0 || bombos[2] == 0 || bombos[3] == 0) {
        cout << "Primero debe formar los bombos." << endl;
        mostrarRecursos("Sorteo de grupos");
        return;
    }

    // Si se vuelve a sortear, todo lo que dependía de los grupos anteriores queda inválido.
    delete[] partidosGrupos;
    partidosGrupos = 0;
    cantidadPartidosGrupos = 0;

    delete[] clasificadosR16;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    bool listo = false;
    int intentos = 0;

    while(!listo && intentos < 10000) {
        medidor.sumarIteracion();

        intentos++;
        listo = true;

        delete[] grupos;
        grupos = 0;
        grupos = new Grupo[cantidadGrupos];

        for(int i = 0; i < cantidadGrupos; i++) {
            medidor.sumarIteracion();
            grupos[i] = Grupo(char('A' + i));
        }

        for(int b = 0; b < 4; b++) {
            for(int i = 0; i < cantidadesBombos[b]; i++) {
                medidor.sumarIteracion();

                int j = aleatorioEnRango(0, cantidadesBombos[b] - 1);

                Equipo* temp = bombos[b][i];
                bombos[b][i] = bombos[b][j];
                bombos[b][j] = temp;
            }
        }

        for(int b = 0; b < 4 && listo; b++) {
            for(int e = 0; e < 12 && listo; e++) {
                medidor.sumarIteracion();

                bool agregado = false;
                int inicio = aleatorioEnRango(0, 11);

                for(int k = 0; k < 12 && !agregado; k++) {
                    medidor.sumarIteracion();

                    int g = (inicio + k) % 12;

                    if(grupos[g].getCantidadEquipos() == b &&
                        grupos[g].puedeAgregarEquipo(bombos[b][e])) {

                        grupos[g].agregarEquipo(bombos[b][e]);
                        agregado = true;
                    }
                }

                if(!agregado) {
                    listo = false;
                }
            }
        }
    }

    if(listo) {
        cout << "Grupos sorteados correctamente." << endl;
    } else {
        cout << "No fue posible sortear grupos validos con restricciones." << endl;
        cout << "Se generaran grupos sin validar confederacion para continuar la simulacion." << endl;

        delete[] grupos;
        grupos = 0;
        grupos = new Grupo[cantidadGrupos];

        for(int i = 0; i < cantidadGrupos; i++) {
            medidor.sumarIteracion();
            grupos[i] = Grupo(char('A' + i));
        }

        for(int b = 0; b < 4; b++) {
            for(int e = 0; e < 12; e++) {
                medidor.sumarIteracion();
                grupos[e].agregarEquipo(bombos[b][e]);
            }
        }

        cout << "Grupos generados en modo simple." << endl;
    }

    activarEstado(GRUPOS_SORTEADOS);
    desactivarEstadosDesde(PARTIDOS_GRUPOS_GENERADOS | FASE_GRUPOS_SIMULADA | R16_GENERADO | FINALES_SIMULADAS);

    mostrarRecursos("Sorteo de grupos");
}

void Torneo::mostrarGrupos() const {
    if(grupos == 0) {
        cout << "No se han sorteado grupos." << endl;

        cout << "\n===== MEDICION DE RECURSOS =====\n";
        cout << "Funcionalidad: Mostrar grupos" << endl;
        cout << "Iteraciones estimadas: 0" << endl;
        cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;

        return;
    }

    cout << "\n===== GRUPOS DEL MUNDIAL =====\n";

    int iteraciones = 0;

    for(int i = 0; i < cantidadGrupos; i++) {
        iteraciones++;

        grupos[i].imprimir();
        cout << endl;
    }

    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: Mostrar grupos" << endl;
    cout << "Iteraciones estimadas: " << iteraciones << endl;
    cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;
}

int Torneo::getCantidadEquipos() const {
    return cantidadEquipos;
}

Equipo* Torneo::getEquipo(int i) const {
    if(i < 0 || i >= cantidadEquipos) return 0;
    return &equipos[i];
}

void Torneo::generarPartidosDeGrupos() {
    medidor.reiniciar();

    if(!tieneEstado(JUGADORES_CREADOS)) {
        cout << "Primero debe crear los jugadores artificiales." << endl;
        mostrarRecursos("Generacion de partidos de grupos");
        return;
    }

    if(!tieneEstado(GRUPOS_SORTEADOS) || grupos == 0) {
        cout << "Primero debe sortear los grupos." << endl;
        mostrarRecursos("Generacion de partidos de grupos");
        return;
    }

    delete[] partidosGrupos;
    partidosGrupos = 0;
    cantidadPartidosGrupos = 0;

    delete[] clasificadosR16;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    cantidadPartidosGrupos = 72;
    partidosGrupos = new Partido[cantidadPartidosGrupos];

    int indice = 0;
    Fecha fechaInicio(20, 6, 2026);

    for(int jornada = 0; jornada < 3; jornada++) {
        for(int g = 0; g < cantidadGrupos; g++) {
            medidor.sumarIteracion();

            if(grupos[g].getCantidadEquipos() < 4) {
                cout << "El grupo " << grupos[g].getLetra()
                << " no tiene 4 equipos. No se pueden generar partidos." << endl;

                delete[] partidosGrupos;
                partidosGrupos = 0;
                cantidadPartidosGrupos = 0;

                mostrarRecursos("Generacion de partidos de grupos");
                return;
            }

            Equipo* e0 = grupos[g].getEquipo(0);
            Equipo* e1 = grupos[g].getEquipo(1);
            Equipo* e2 = grupos[g].getEquipo(2);
            Equipo* e3 = grupos[g].getEquipo(3);

            if(e0 == 0 || e1 == 0 || e2 == 0 || e3 == 0) {
                cout << "Hay equipos nulos en el grupo " << grupos[g].getLetra() << endl;

                delete[] partidosGrupos;
                partidosGrupos = 0;
                cantidadPartidosGrupos = 0;

                mostrarRecursos("Generacion de partidos de grupos");
                return;
            }

            Equipo* equipoA[2];
            Equipo* equipoB[2];

            if(jornada == 0) {
                equipoA[0] = e0; equipoB[0] = e1;
                equipoA[1] = e2; equipoB[1] = e3;
            } else if(jornada == 1) {
                equipoA[0] = e0; equipoB[0] = e2;
                equipoA[1] = e1; equipoB[1] = e3;
            } else {
                equipoA[0] = e0; equipoB[0] = e3;
                equipoA[1] = e1; equipoB[1] = e2;
            }

            for(int p = 0; p < 2; p++) {
                medidor.sumarIteracion();

                Fecha f = fechaInicio;
                f.sumarDias(indice / 4);

                partidosGrupos[indice].setEquipo1(equipoA[p]);
                partidosGrupos[indice].setEquipo2(equipoB[p]);
                partidosGrupos[indice].setFecha(f);
                partidosGrupos[indice].setHora("00:00");
                partidosGrupos[indice].setSede("nombreSede");
                partidosGrupos[indice].setEtapa("Grupos");
                partidosGrupos[indice].configurarArbitrosFijos();

                indice++;
            }
        }
    }

    cout << "Partidos de fase de grupos generados correctamente." << endl;

    activarEstado(PARTIDOS_GRUPOS_GENERADOS);
    desactivarEstadosDesde(FASE_GRUPOS_SIMULADA | R16_GENERADO | FINALES_SIMULADAS);

    mostrarRecursos("Generacion de partidos de grupos");
}

void Torneo::simularFaseDeGrupos() {
    medidor.reiniciar();

    if(!tieneEstado(JUGADORES_CREADOS)) {
        cout << "Primero debe crear los jugadores artificiales." << endl;
        mostrarRecursos("Simulacion fase de grupos");
        return;
    }

    if(!tieneEstado(PARTIDOS_GRUPOS_GENERADOS) || partidosGrupos == 0) {
        generarPartidosDeGrupos();
        medidor.reiniciar();
    }

    if(partidosGrupos == 0) {
        cout << "No se pudieron generar los partidos." << endl;
        mostrarRecursos("Simulacion fase de grupos");
        return;
    }

    // Si se vuelve a simular la fase de grupos, se eliminan datos posteriores
    // para evitar clasificados, finales o estadisticas viejas.
    delete[] clasificadosR16;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        for(int j = 0; j < grupos[i].getCantidadEquipos(); j++) {
            medidor.sumarIteracion();

            Equipo* e = grupos[i].getEquipo(j);
            if(e != 0) {
                e->reiniciarDatosGrupo();
            }
        }
    }

    cout << "\n===== SIMULACION FASE DE GRUPOS =====\n";

    for(int i = 0; i < cantidadPartidosGrupos; i++) {
        medidor.sumarIteracion();

        partidosGrupos[i].simular(true);
        cout << "\nPartido " << i + 1 << endl;
        partidosGrupos[i].imprimir();
        partidosGrupos[i].imprimirGoleadores();
        partidosGrupos[i].imprimirResumenDisciplinario();
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();
        grupos[i].ordenarTabla();
    }

    cout << "\nFase de grupos simulada correctamente." << endl;

    activarEstado(FASE_GRUPOS_SIMULADA);
    desactivarEstadosDesde(R16_GENERADO | FINALES_SIMULADAS);

    mostrarRecursos("Simulacion fase de grupos");
}

void Torneo::mostrarTablaDeGrupos() {
    medidor.reiniciar();

    if(grupos == 0) {
        cout << "No hay grupos sorteados." << endl;
        mostrarRecursos("Mostrar tabla de grupos");
        return;
    }

    cout << "\n===== TABLAS DE GRUPOS =====\n";

    for(int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();

        grupos[i].ordenarTabla();

        cout << "\nGrupo " << grupos[i].getLetra() << endl;

        for(int j = 0; j < grupos[i].getCantidadEquipos(); j++) {
            medidor.sumarIteracion();

            Equipo* e = grupos[i].getEquipo(j);

            cout << j + 1 << ". "
                 << e->getPais()
                 << " | Pts: " << e->getPuntosGrupo()
                 << " | GF: " << e->getGolesFavorGrupo()
                 << " | GC: " << e->getGolesContraGrupo()
                 << " | DG: " << e->getDiferenciaGolesGrupo()
                 << endl;
        }
    }

    mostrarRecursos("Mostrar tabla de grupos");
}

void Torneo::clasificarADieciseisavos() {
    medidor.reiniciar();

    if(!tieneEstado(FASE_GRUPOS_SIMULADA) || grupos == 0) {
        cout << "Primero debe sortear y simular la fase de grupos." << endl;
        mostrarRecursos("Clasificacion a dieciseisavos");
        return;
    }

    delete[] clasificadosR16;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    Equipo* primeros[12];
    Equipo* segundos[12];
    Equipo* terceros[12];

    char gruposPrimeros[12];
    char gruposSegundos[12];
    char gruposTerceros[12];

    int cantidadPrimeros = 0;
    int cantidadSegundos = 0;
    int cantidadTerceros = 0;

    for(int i = 0; i < 12; i++) {
        primeros[i] = 0;
        segundos[i] = 0;
        terceros[i] = 0;

        gruposPrimeros[i] = '?';
        gruposSegundos[i] = '?';
        gruposTerceros[i] = '?';
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();

        grupos[i].ordenarTabla();

        Equipo* primero = grupos[i].getEquipo(0);
        Equipo* segundo = grupos[i].getEquipo(1);
        Equipo* tercero = grupos[i].getEquipo(2);

        if(primero != 0 && cantidadPrimeros < 12) {
            primeros[cantidadPrimeros] = primero;
            gruposPrimeros[cantidadPrimeros] = grupos[i].getLetra();
            cantidadPrimeros++;
        }

        if(segundo != 0 && cantidadSegundos < 12) {
            segundos[cantidadSegundos] = segundo;
            gruposSegundos[cantidadSegundos] = grupos[i].getLetra();
            cantidadSegundos++;
        }

        if(tercero != 0 && cantidadTerceros < 12) {
            terceros[cantidadTerceros] = tercero;
            gruposTerceros[cantidadTerceros] = grupos[i].getLetra();
            cantidadTerceros++;
        }
    }

    // Ordenar terceros de mejor a peor.
    for(int i = 0; i < cantidadTerceros - 1; i++) {
        for(int j = 0; j < cantidadTerceros - 1 - i; j++) {
            medidor.sumarIteracion();

            if(!esMejorEnTabla(terceros[j], terceros[j + 1])) {
                intercambiarEquipoYGrupo(terceros, gruposTerceros, j, j + 1);
            }
        }
    }

    // Copiar segundos para ordenarlos de peor a mejor.
    Equipo* segundosOrdenados[12];
    char gruposSegundosOrdenados[12];

    for(int i = 0; i < 12; i++) {
        segundosOrdenados[i] = segundos[i];
        gruposSegundosOrdenados[i] = gruposSegundos[i];
    }

    for(int i = 0; i < cantidadSegundos - 1; i++) {
        for(int j = 0; j < cantidadSegundos - 1 - i; j++) {
            medidor.sumarIteracion();

            if(!esPeorEnTabla(segundosOrdenados[j], segundosOrdenados[j + 1])) {
                intercambiarEquipoYGrupo(segundosOrdenados, gruposSegundosOrdenados, j, j + 1);
            }
        }
    }

    clasificadosR16 = new Equipo*[32];
    for(int i = 0; i < 32; i++) {
        clasificadosR16[i] = 0;
    }

    cantidadClasificadosR16 = 0;

    // 1) Ocho cabezas de grupo contra los ocho mejores terceros.
    for(int i = 0; i < 8; i++) {
        medidor.sumarIteracion();

        if(gruposPrimeros[i] == gruposTerceros[i]) {
            for(int k = i + 1; k < 8; k++) {
                if(gruposPrimeros[i] != gruposTerceros[k]) {
                    intercambiarEquipoYGrupo(terceros, gruposTerceros, i, k);
                    break;
                }
            }
        }

        clasificadosR16[cantidadClasificadosR16++] = primeros[i];
        clasificadosR16[cantidadClasificadosR16++] = terceros[i];
    }

    // 2) Los 4 cabezas de grupo restantes contra los 4 peores segundos.
    for(int i = 0; i < 4; i++) {
        medidor.sumarIteracion();

        int indicePrimero = 8 + i;
        int indiceSegundo = i;

        if(gruposPrimeros[indicePrimero] == gruposSegundosOrdenados[indiceSegundo]) {
            for(int k = i + 1; k < 4; k++) {
                if(gruposPrimeros[indicePrimero] != gruposSegundosOrdenados[k]) {
                    intercambiarEquipoYGrupo(segundosOrdenados, gruposSegundosOrdenados, indiceSegundo, k);
                    break;
                }
            }
        }

        clasificadosR16[cantidadClasificadosR16++] = primeros[indicePrimero];
        clasificadosR16[cantidadClasificadosR16++] = segundosOrdenados[indiceSegundo];
    }

    // 3) Los 8 segundos restantes se enfrentan entre sí.
    for(int i = 4; i < 12; i += 2) {
        medidor.sumarIteracion();

        if(gruposSegundosOrdenados[i] == gruposSegundosOrdenados[i + 1]) {
            for(int k = i + 2; k < 12; k++) {
                if(gruposSegundosOrdenados[i] != gruposSegundosOrdenados[k]) {
                    intercambiarEquipoYGrupo(segundosOrdenados, gruposSegundosOrdenados, i + 1, k);
                    break;
                }
            }
        }

        clasificadosR16[cantidadClasificadosR16++] = segundosOrdenados[i];
        clasificadosR16[cantidadClasificadosR16++] = segundosOrdenados[i + 1];
    }

    long long memoriaTemporal =
        (12 * sizeof(Equipo*) * 5) +
        (12 * sizeof(char) * 5);

    cout << "Clasificacion a dieciseisavos generada correctamente." << endl;
    cout << "Cantidad de clasificados: " << cantidadClasificadosR16 << endl;
    cout << "Memoria temporal de arreglos auxiliares: " << memoriaTemporal << " bytes" << endl;

    cout << "\n===== PARTIDOS CONFIGURADOS R16 =====\n";

    for(int i = 0; i < 16 && i * 2 + 1 < cantidadClasificadosR16; i++) {
        cout << i + 1 << ". "
             << clasificadosR16[i * 2]->getPais()
             << " vs "
             << clasificadosR16[i * 2 + 1]->getPais()
             << endl;
    }

    activarEstado(R16_GENERADO);
    desactivarEstadosDesde(FINALES_SIMULADAS);

    mostrarRecursos("Clasificacion a dieciseisavos");
}

void Torneo::mostrarClasificadosR16() const {
    if(clasificadosR16 == 0 || cantidadClasificadosR16 == 0) {
        cout << "No se han generado los clasificados a dieciseisavos." << endl;
        return;
    }

    cout << "\n===== CLASIFICADOS A DIECISEISAVOS =====\n";

    for(int i = 0; i < cantidadClasificadosR16; i++) {
        cout << i + 1 << ". "
             << clasificadosR16[i]->getPais()
             << " | Confederacion: "
             << clasificadosR16[i]->getConfederacion()
             << " | Pts: "
             << clasificadosR16[i]->getPuntosGrupo()
             << " | GF: "
             << clasificadosR16[i]->getGolesFavorGrupo()
             << " | GC: "
             << clasificadosR16[i]->getGolesContraGrupo()
             << " | DG: "
             << clasificadosR16[i]->getDiferenciaGolesGrupo()
             << endl;
    }

    cout << "\n===== PARTIDOS CONFIGURADOS R16 =====\n";

    for(int i = 0; i < 16 && i * 2 + 1 < cantidadClasificadosR16; i++) {
        cout << i + 1 << ". "
             << clasificadosR16[i * 2]->getPais()
             << " vs "
             << clasificadosR16[i * 2 + 1]->getPais()
             << endl;
    }

    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: Mostrar clasificados a dieciseisavos" << endl;
    cout << "Iteraciones estimadas: " << cantidadClasificadosR16 << endl;
    cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;
}
void Torneo::simularFasesFinales() {
    medidor.reiniciar();

    if(!tieneEstado(R16_GENERADO) || clasificadosR16 == 0 || cantidadClasificadosR16 < 32) {
        cout << "Primero debe clasificar los equipos a dieciseisavos." << endl;
        mostrarRecursos("Simulacion fases finales");
        return;
    }

    Fecha fechaFinales(10, 7, 2026);

    delete[] partidosR16;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;

    delete[] partidosR8;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;

    delete[] partidosQF;
    partidosQF = 0;
    cantidadPartidosQF = 4;

    delete[] partidosSF;
    partidosSF = 0;
    cantidadPartidosSF = 2;

    delete[] partidosFinales;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    if(top4 != 0) {
        for(int i = 0; i < 4; i++) {
            top4[i] = 0;
        }
    }

    partidosR16 = new Partido[cantidadPartidosR16];
    partidosR8 = new Partido[cantidadPartidosR8];
    partidosQF = new Partido[cantidadPartidosQF];
    partidosSF = new Partido[cantidadPartidosSF];
    partidosFinales = new Partido[cantidadPartidosFinales];

    Equipo** ganadoresR16 = new Equipo*[16];
    Equipo** ganadoresR8 = new Equipo*[8];
    Equipo** ganadoresQF = new Equipo*[4];
    Equipo** ganadoresSF = new Equipo*[2];
    Equipo** perdedoresSF = new Equipo*[2];

    for(int i = 0; i < 16; i++) ganadoresR16[i] = 0;
    for(int i = 0; i < 8; i++) ganadoresR8[i] = 0;
    for(int i = 0; i < 4; i++) ganadoresQF[i] = 0;
    for(int i = 0; i < 2; i++) {
        ganadoresSF[i] = 0;
        perdedoresSF[i] = 0;
    }

    cout << "\n===== DIECISEISAVOS DE FINAL =====\n";

    for(int i = 0; i < 16; i++) {
        medidor.sumarIteracion();

        Equipo* e1 = clasificadosR16[i * 2];
        Equipo* e2 = clasificadosR16[i * 2 + 1];

        if(e1 == 0 || e2 == 0) {
            cout << "Error: hay equipos nulos en dieciseisavos." << endl;

            delete[] ganadoresR16;
            delete[] ganadoresR8;
            delete[] ganadoresQF;
            delete[] ganadoresSF;
            delete[] perdedoresSF;

            mostrarRecursos("Simulacion fases finales");
            return;
        }

        partidosR16[i].setEquipo1(e1);
        partidosR16[i].setEquipo2(e2);
        partidosR16[i].setFecha(fechaFinales);
        partidosR16[i].setHora("00:00");
        partidosR16[i].setSede("nombreSede");
        partidosR16[i].setEtapa("Dieciseisavos");
        partidosR16[i].configurarArbitrosFijos();

        partidosR16[i].simular(false);
        partidosR16[i].imprimir();
        partidosR16[i].imprimirGoleadores();
        partidosR16[i].imprimirResumenDisciplinario();
        cout << endl;

        if(partidosR16[i].getGoles1() > partidosR16[i].getGoles2()) {
            ganadoresR16[i] = partidosR16[i].getEquipo1();
        } else {
            ganadoresR16[i] = partidosR16[i].getEquipo2();
        }
    }

    cout << "\n===== OCTAVOS DE FINAL =====\n";

    for(int i = 0; i < 8; i++) {
        medidor.sumarIteracion();

        Equipo* e1 = ganadoresR16[i * 2];
        Equipo* e2 = ganadoresR16[i * 2 + 1];

        if(e1 == 0 || e2 == 0) {
            cout << "Error: hay equipos nulos en octavos." << endl;

            delete[] ganadoresR16;
            delete[] ganadoresR8;
            delete[] ganadoresQF;
            delete[] ganadoresSF;
            delete[] perdedoresSF;

            mostrarRecursos("Simulacion fases finales");
            return;
        }

        partidosR8[i].setEquipo1(e1);
        partidosR8[i].setEquipo2(e2);
        partidosR8[i].setFecha(fechaFinales);
        partidosR8[i].setHora("00:00");
        partidosR8[i].setSede("nombreSede");
        partidosR8[i].setEtapa("Octavos");
        partidosR8[i].configurarArbitrosFijos();

        partidosR8[i].simular(false);
        partidosR8[i].imprimir();
        partidosR8[i].imprimirGoleadores();
        partidosR8[i].imprimirResumenDisciplinario();
        cout << endl;

        if(partidosR8[i].getGoles1() > partidosR8[i].getGoles2()) {
            ganadoresR8[i] = partidosR8[i].getEquipo1();
        } else {
            ganadoresR8[i] = partidosR8[i].getEquipo2();
        }
    }

    cout << "\n===== CUARTOS DE FINAL =====\n";

    for(int i = 0; i < 4; i++) {
        medidor.sumarIteracion();

        Equipo* e1 = ganadoresR8[i * 2];
        Equipo* e2 = ganadoresR8[i * 2 + 1];

        if(e1 == 0 || e2 == 0) {
            cout << "Error: hay equipos nulos en cuartos." << endl;

            delete[] ganadoresR16;
            delete[] ganadoresR8;
            delete[] ganadoresQF;
            delete[] ganadoresSF;
            delete[] perdedoresSF;

            mostrarRecursos("Simulacion fases finales");
            return;
        }

        partidosQF[i].setEquipo1(e1);
        partidosQF[i].setEquipo2(e2);
        partidosQF[i].setFecha(fechaFinales);
        partidosQF[i].setHora("00:00");
        partidosQF[i].setSede("nombreSede");
        partidosQF[i].setEtapa("Cuartos");
        partidosQF[i].configurarArbitrosFijos();

        partidosQF[i].simular(false);
        partidosQF[i].imprimir();
        partidosQF[i].imprimirGoleadores();
        partidosQF[i].imprimirResumenDisciplinario();
        cout << endl;

        if(partidosQF[i].getGoles1() > partidosQF[i].getGoles2()) {
            ganadoresQF[i] = partidosQF[i].getEquipo1();
        } else {
            ganadoresQF[i] = partidosQF[i].getEquipo2();
        }
    }

    cout << "\n===== SEMIFINALES =====\n";

    for(int i = 0; i < 2; i++) {
        medidor.sumarIteracion();

        Equipo* e1 = ganadoresQF[i * 2];
        Equipo* e2 = ganadoresQF[i * 2 + 1];

        if(e1 == 0 || e2 == 0) {
            cout << "Error: hay equipos nulos en semifinales." << endl;

            delete[] ganadoresR16;
            delete[] ganadoresR8;
            delete[] ganadoresQF;
            delete[] ganadoresSF;
            delete[] perdedoresSF;

            mostrarRecursos("Simulacion fases finales");
            return;
        }

        partidosSF[i].setEquipo1(e1);
        partidosSF[i].setEquipo2(e2);
        partidosSF[i].setFecha(fechaFinales);
        partidosSF[i].setHora("00:00");
        partidosSF[i].setSede("nombreSede");
        partidosSF[i].setEtapa("Semifinal");
        partidosSF[i].configurarArbitrosFijos();

        partidosSF[i].simular(false);
        partidosSF[i].imprimir();
        partidosSF[i].imprimirGoleadores();
        partidosSF[i].imprimirResumenDisciplinario();
        cout << endl;

        if(partidosSF[i].getGoles1() > partidosSF[i].getGoles2()) {
            ganadoresSF[i] = partidosSF[i].getEquipo1();
            perdedoresSF[i] = partidosSF[i].getEquipo2();
        } else {
            ganadoresSF[i] = partidosSF[i].getEquipo2();
            perdedoresSF[i] = partidosSF[i].getEquipo1();
        }
    }

    cout << "\n===== PARTIDO POR EL TERCER PUESTO =====\n";

    medidor.sumarIteracion();

    if(perdedoresSF[0] == 0 || perdedoresSF[1] == 0) {
        cout << "Error: no hay perdedores validos para tercer puesto." << endl;

        delete[] ganadoresR16;
        delete[] ganadoresR8;
        delete[] ganadoresQF;
        delete[] ganadoresSF;
        delete[] perdedoresSF;

        mostrarRecursos("Simulacion fases finales");
        return;
    }

    partidosFinales[0].setEquipo1(perdedoresSF[0]);
    partidosFinales[0].setEquipo2(perdedoresSF[1]);
    partidosFinales[0].setFecha(fechaFinales);
    partidosFinales[0].setHora("00:00");
    partidosFinales[0].setSede("nombreSede");
    partidosFinales[0].setEtapa("Tercer puesto");
    partidosFinales[0].configurarArbitrosFijos();

    partidosFinales[0].simular(false);
    partidosFinales[0].imprimir();
    partidosFinales[0].imprimirGoleadores();
    partidosFinales[0].imprimirResumenDisciplinario();

    Equipo* tercerLugar = 0;
    Equipo* cuartoLugar = 0;

    if(partidosFinales[0].getGoles1() > partidosFinales[0].getGoles2()) {
        tercerLugar = partidosFinales[0].getEquipo1();
        cuartoLugar = partidosFinales[0].getEquipo2();
    } else {
        tercerLugar = partidosFinales[0].getEquipo2();
        cuartoLugar = partidosFinales[0].getEquipo1();
    }

    cout << "\n===== FINAL =====\n";

    medidor.sumarIteracion();

    if(ganadoresSF[0] == 0 || ganadoresSF[1] == 0) {
        cout << "Error: no hay ganadores validos para la final." << endl;

        delete[] ganadoresR16;
        delete[] ganadoresR8;
        delete[] ganadoresQF;
        delete[] ganadoresSF;
        delete[] perdedoresSF;

        mostrarRecursos("Simulacion fases finales");
        return;
    }

    partidosFinales[1].setEquipo1(ganadoresSF[0]);
    partidosFinales[1].setEquipo2(ganadoresSF[1]);
    partidosFinales[1].setFecha(fechaFinales);
    partidosFinales[1].setHora("00:00");
    partidosFinales[1].setSede("nombreSede");
    partidosFinales[1].setEtapa("Final");
    partidosFinales[1].configurarArbitrosFijos();

    partidosFinales[1].simular(false);
    partidosFinales[1].imprimir();
    partidosFinales[1].imprimirGoleadores();
    partidosFinales[1].imprimirResumenDisciplinario();

    Equipo* campeon = 0;
    Equipo* subcampeon = 0;

    if(partidosFinales[1].getGoles1() > partidosFinales[1].getGoles2()) {
        campeon = partidosFinales[1].getEquipo1();
        subcampeon = partidosFinales[1].getEquipo2();
    } else {
        campeon = partidosFinales[1].getEquipo2();
        subcampeon = partidosFinales[1].getEquipo1();
    }

    top4[0] = campeon;
    top4[1] = subcampeon;
    top4[2] = tercerLugar;
    top4[3] = cuartoLugar;

    long long memoriaTemporal = (16 + 8 + 4 + 2 + 2) * sizeof(Equipo*);

    cout << "\n===== PRESENCIA POR CONFEDERACION =====\n";
    imprimirConfederacionMayor("R16 / dieciseisavos", clasificadosR16, cantidadClasificadosR16);
    imprimirConfederacionMayor("R8 / octavos", ganadoresR16, 16);
    imprimirConfederacionMayor("R4 / semifinalistas", ganadoresQF, 4);

    cout << "\nFases finales simuladas correctamente." << endl;
    cout << "Campeon del mundo: " << top4[0]->getPais() << endl;
    cout << "Memoria temporal de arreglos de ganadores: "
         << memoriaTemporal
         << " bytes" << endl;

    activarEstado(FINALES_SIMULADAS);

    mostrarRecursos("Simulacion fases finales");

    delete[] ganadoresR16;
    ganadoresR16 = 0;

    delete[] ganadoresR8;
    ganadoresR8 = 0;

    delete[] ganadoresQF;
    ganadoresQF = 0;

    delete[] ganadoresSF;
    ganadoresSF = 0;

    delete[] perdedoresSF;
    perdedoresSF = 0;
}

void Torneo::mostrarEstadisticasFinales() {
    medidor.reiniciar();

    if(!tieneEstado(FINALES_SIMULADAS) || top4 == 0 || top4[0] == 0) {
        cout << "Primero debe simular las fases finales." << endl;
        mostrarRecursos("Estadisticas finales");
        return;
    }

    cout << "\n===== ESTADISTICAS FINALES DEL TORNEO =====\n";

    cout << "\nTop 4 del mundial:\n";

    if(top4[0] != 0) cout << "1. Campeon: " << top4[0]->getPais() << endl;
    if(top4[1] != 0) cout << "2. Subcampeon: " << top4[1]->getPais() << endl;
    if(top4[2] != 0) cout << "3. Tercer puesto: " << top4[2]->getPais() << endl;
    if(top4[3] != 0) cout << "4. Cuarto puesto: " << top4[3]->getPais() << endl;

    cout << "\nMaximo goleador del equipo campeon:\n";

    Jugador* plantillaCampeon = top4[0]->getPlantilla();
    int cantidadJugadoresCampeon = top4[0]->getCantidadJugadores();

    Jugador* maximoCampeon = 0;

    if(plantillaCampeon != 0) {
        for(int i = 0; i < cantidadJugadoresCampeon; i++) {
            medidor.sumarIteracion();

            if(maximoCampeon == 0 || plantillaCampeon[i].getGoles() > maximoCampeon->getGoles()) {
                maximoCampeon = &plantillaCampeon[i];
            }
        }
    }

    if(maximoCampeon != 0) {
        cout << "#" << maximoCampeon->getNumeroCamiseta()
        << " " << maximoCampeon->getNombre()
        << " " << maximoCampeon->getApellido()
        << " | Goles: " << maximoCampeon->getGoles()
        << endl;
    } else {
        cout << "No hay jugadores registrados para el campeon." << endl;
    }

    Jugador* topJugadores[3] = {0, 0, 0};
    Equipo* equiposTopJugadores[3] = {0, 0, 0};

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        Jugador* plantilla = equipos[i].getPlantilla();
        int cant = equipos[i].getCantidadJugadores();

        if(plantilla == 0) {
            continue;
        }

        for(int j = 0; j < cant; j++) {
            medidor.sumarIteracion();

            Jugador* actual = &plantilla[j];

            for(int k = 0; k < 3; k++) {
                medidor.sumarIteracion();

                if(topJugadores[k] == 0 || actual->getGoles() > topJugadores[k]->getGoles()) {
                    for(int m = 2; m > k; m--) {
                        medidor.sumarIteracion();

                        topJugadores[m] = topJugadores[m - 1];
                        equiposTopJugadores[m] = equiposTopJugadores[m - 1];
                    }

                    topJugadores[k] = actual;
                    equiposTopJugadores[k] = &equipos[i];
                    break;
                }
            }
        }
    }

    cout << "\nTres mayores goleadores de toda la copa:\n";

    for(int i = 0; i < 3; i++) {
        medidor.sumarIteracion();

        if(topJugadores[i] != 0 && equiposTopJugadores[i] != 0) {
            cout << i + 1 << ". "
                 << equiposTopJugadores[i]->getPais()
                 << " | #"
                 << topJugadores[i]->getNumeroCamiseta()
                 << " "
                 << topJugadores[i]->getNombre()
                 << " "
                 << topJugadores[i]->getApellido()
                 << " | Goles: "
                 << topJugadores[i]->getGoles()
                 << endl;
        }
    }

    Equipo* equipoMasGolesHistoricos = 0;

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        if(equipoMasGolesHistoricos == 0 ||
            equipos[i].getGolesFavorHistoricos() > equipoMasGolesHistoricos->getGolesFavorHistoricos()) {
            equipoMasGolesHistoricos = &equipos[i];
        }
    }

    if(equipoMasGolesHistoricos != 0) {
        cout << "\nEquipo con mas goles historicos actualizados:\n";
        cout << equipoMasGolesHistoricos->getPais()
             << " | GF historicos: "
             << equipoMasGolesHistoricos->getGolesFavorHistoricos()
             << endl;
    }

    mostrarRecursos("Estadisticas finales");
}


void Torneo::guardarHistoricos(const char* nombreArchivo) {
    medidor.reiniciar();

    if(!tieneEstado(FINALES_SIMULADAS)) {
        cout << "Primero debe simular todo el torneo antes de guardar historicos." << endl;
        mostrarRecursos("Guardar historicos actualizados");
        return;
    }

    ofstream archivo(nombreArchivo);

    if(!archivo.is_open()) {
        cout << "No se pudo crear el archivo de historicos: " << nombreArchivo << endl;
        mostrarRecursos("Guardar historicos actualizados");
        return;
    }

    archivo << "Pais;Numero;Nombre;Apellido;Partidos;Goles;Minutos;Asistencias;Amarillas;Rojas;Faltas\n";

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        Jugador* plantilla = equipos[i].getPlantilla();
        int cantidad = equipos[i].getCantidadJugadores();

        if(plantilla == 0) {
            continue;
        }

        for(int j = 0; j < cantidad; j++) {
            medidor.sumarIteracion();

            archivo << equipos[i].getPais() << ";"
                    << plantilla[j].getNumeroCamiseta() << ";"
                    << plantilla[j].getNombre() << ";"
                    << plantilla[j].getApellido() << ";"
                    << plantilla[j].getPartidosJugados() << ";"
                    << plantilla[j].getGoles() << ";"
                    << plantilla[j].getMinutosJugados() << ";"
                    << plantilla[j].getAsistencias() << ";"
                    << plantilla[j].getTarjetasAmarillas() << ";"
                    << plantilla[j].getTarjetasRojas() << ";"
                    << plantilla[j].getFaltas()
                    << "\n";
        }
    }

    archivo.close();

    cout << "Historicos actualizados guardados correctamente en: "
         << nombreArchivo
         << endl;

    mostrarRecursos("Guardar historicos actualizados");
}

long long Torneo::calcularMemoriaTotal() const {
    long long total = sizeof(Torneo);

    total += medidor.memoriaEquipos(equipos, cantidadEquipos);
    total += medidor.memoriaGrupos(grupos, cantidadGrupos);

    for(int i = 0; i < 4; i++) {
        total += medidor.memoriaPunterosEquipo(bombos[i], cantidadesBombos[i]);
    }

    total += medidor.memoriaPartidos(partidosGrupos, cantidadPartidosGrupos);

    total += medidor.memoriaPunterosEquipo(clasificadosR16, cantidadClasificadosR16);

    total += medidor.memoriaPartidos(partidosR16, cantidadPartidosR16);
    total += medidor.memoriaPartidos(partidosR8, cantidadPartidosR8);
    total += medidor.memoriaPartidos(partidosQF, cantidadPartidosQF);
    total += medidor.memoriaPartidos(partidosSF, cantidadPartidosSF);
    total += medidor.memoriaPartidos(partidosFinales, cantidadPartidosFinales);

    total += medidor.memoriaPunterosEquipo(top4, 4);

    return total;
}

void Torneo::mostrarRecursos(const char* funcionalidad) {
    long long memoria = calcularMemoriaTotal();
    medidor.imprimirResumen(funcionalidad, memoria);
}