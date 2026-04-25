#include "torneo.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>

using namespace std;

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

    inicializarAleatorio();
}

Torneo::~Torneo() {
    delete[] equipos;
    delete[] grupos;
    delete[] partidosGrupos;
    delete[] clasificadosR16;

    delete[] partidosR16;
    delete[] partidosR8;
    delete[] partidosQF;
    delete[] partidosSF;
    delete[] partidosFinales;
    delete[] top4;

    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
    }
}
void Torneo::cargarEquiposDesdeCSV(const char* nombreArchivo) {
    if(equipos != 0) {
        delete[] equipos;
        equipos = 0;
    }

    ifstream archivo(nombreArchivo);

    if(!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    cantidadEquipos = 48;
    equipos = new Equipo[cantidadEquipos];

    char linea[512];

    // Saltar encabezado
    archivo.getline(linea, 512);

    int i = 0;

    while(archivo.getline(linea, 512) && i < cantidadEquipos) {
        char pais[100] = "";
        char confederacion[50] = "";
        char director[50] = "DT";

        char rankingStr[20] = "0";
        char gfStr[20] = "0";
        char gcStr[20] = "0";
        char pgStr[20] = "0";
        char peStr[20] = "0";
        char ppStr[20] = "0";

        int campo = 0;
        int pos = 0;
        int b = 0;
        char buffer[100];

        while(true) {
            char c = linea[pos];

            if(c == ',' || c == '\0') {
                buffer[b] = '\0';

                if(campo == 0) copiarCadenaEnBuffer(pais, buffer);
                else if(campo == 1) copiarCadenaEnBuffer(confederacion, buffer);
                else if(campo == 2) copiarCadenaEnBuffer(rankingStr, buffer);
                else if(campo == 3) copiarCadenaEnBuffer(gfStr, buffer);
                else if(campo == 4) copiarCadenaEnBuffer(gcStr, buffer);
                else if(campo == 5) copiarCadenaEnBuffer(pgStr, buffer);
                else if(campo == 6) copiarCadenaEnBuffer(peStr, buffer);
                else if(campo == 7) copiarCadenaEnBuffer(ppStr, buffer);

                campo++;
                b = 0;

                if(c == '\0') break;
            } else {
                buffer[b] = c;
                b++;
            }

            pos++;
        }

        equipos[i] = Equipo(
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

        i++;
    }

    archivo.close();

    cout << "Equipos cargados correctamente." << endl;
}

void Torneo::crearJugadoresDeTodosLosEquipos() {
    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        equipos[i].crearPlantillaArtificial();
    }

    cout << "Jugadores artificiales creados correctamente." << endl;
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
}

void Torneo::ordenarEquiposPorRanking() {
    if(equipos == 0) return;

    for(int i = 0; i < cantidadEquipos - 1; i++) {
        for(int j = 0; j < cantidadEquipos - 1 - i; j++) {
            if(equipos[j + 1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

void Torneo::formarBombos() {
    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }

    ordenarEquiposPorRanking();

    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
        bombos[i] = new Equipo*[12];
        cantidadesBombos[i] = 0;
    }

    int indiceUSA = -1;

    for(int i = 0; i < cantidadEquipos; i++) {
        if(sonIguales(equipos[i].getPais(), "EE. UU.") ||
            sonIguales(equipos[i].getPais(), "Estados Unidos") ||
            sonIguales(equipos[i].getPais(), "USA")) {
            indiceUSA = i;
            break;
        }
    }

    // El anfitrión va al bombo 1
    if(indiceUSA != -1) {
        bombos[0][cantidadesBombos[0]] = &equipos[indiceUSA];
        cantidadesBombos[0]++;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
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
}

void Torneo::mostrarBombos() const {
    for(int i = 0; i < 4; i++) {
        cout << "\n===== BOMBO " << i + 1 << " =====\n";

        for(int j = 0; j < cantidadesBombos[i]; j++) {
            cout << " - "
                 << bombos[i][j]->getPais()
                 << " | Confederacion: "
                 << bombos[i][j]->getConfederacion()
                 << " | Ranking: "
                 << bombos[i][j]->getRankingFIFA()
                 << endl;
        }
    }
}

void Torneo::sortearGrupos() {
    if(bombos[0] == 0 || bombos[1] == 0 || bombos[2] == 0 || bombos[3] == 0) {
        cout << "Primero debe formar los bombos." << endl;
        return;
    }

    bool listo = false;
    int intentos = 0;

    while(!listo && intentos < 500) {
        intentos++;
        listo = true;

        delete[] grupos;
        grupos = new Grupo[cantidadGrupos];

        for(int i = 0; i < cantidadGrupos; i++) {
            grupos[i] = Grupo(char('A' + i));
        }

        // Mezclar cada bombo
        for(int b = 0; b < 4; b++) {
            for(int i = 0; i < cantidadesBombos[b]; i++) {
                int j = aleatorioEnRango(0, cantidadesBombos[b] - 1);

                Equipo* temp = bombos[b][i];
                bombos[b][i] = bombos[b][j];
                bombos[b][j] = temp;
            }
        }

        // Asignar un equipo de cada bombo a cada grupo
        for(int b = 0; b < 4 && listo; b++) {
            for(int e = 0; e < 12 && listo; e++) {
                bool agregado = false;

                for(int g = 0; g < 12 && !agregado; g++) {
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
        cout << "No fue posible sortear grupos validos." << endl;
    }
}

void Torneo::mostrarGrupos() const {
    if(grupos == 0) {
        cout << "No se han sorteado grupos." << endl;
        return;
    }

    cout << "\n===== GRUPOS DEL MUNDIAL =====\n";

    for(int i = 0; i < cantidadGrupos; i++) {
        grupos[i].imprimir();
        cout << endl;
    }
}

int Torneo::getCantidadEquipos() const {
    return cantidadEquipos;
}

Equipo* Torneo::getEquipo(int i) const {
    if(i < 0 || i >= cantidadEquipos) return 0;
    return &equipos[i];
}

void Torneo::generarPartidosDeGrupos() {
    if(grupos == 0) {
        cout << "Primero debe sortear los grupos." << endl;
        return;
    }

    delete[] partidosGrupos;

    cantidadPartidosGrupos = 72; // 12 grupos * 6 partidos
    partidosGrupos = new Partido[cantidadPartidosGrupos];

    int indice = 0;
    Fecha fechaInicio(20, 6, 2026);

    for(int g = 0; g < cantidadGrupos; g++) {
        Equipo* e0 = grupos[g].getEquipo(0);
        Equipo* e1 = grupos[g].getEquipo(1);
        Equipo* e2 = grupos[g].getEquipo(2);
        Equipo* e3 = grupos[g].getEquipo(3);

        Equipo* lista1[6] = {e0, e0, e0, e1, e1, e2};
        Equipo* lista2[6] = {e1, e2, e3, e2, e3, e3};

        for(int p = 0; p < 6; p++) {
            Fecha f = fechaInicio;
            f.sumarDias(indice / 4); // máximo 4 partidos por día

            partidosGrupos[indice].setEquipo1(lista1[p]);
            partidosGrupos[indice].setEquipo2(lista2[p]);
            partidosGrupos[indice].setFecha(f);
            partidosGrupos[indice].setHora("00:00");
            partidosGrupos[indice].setSede("nombreSede");
            partidosGrupos[indice].setEtapa("Grupos");
            partidosGrupos[indice].configurarArbitrosFijos();

            indice++;
        }
    }

    cout << "Partidos de fase de grupos generados correctamente." << endl;
}

void Torneo::simularFaseDeGrupos() {
    if(partidosGrupos == 0) {
        generarPartidosDeGrupos();
    }

    if(partidosGrupos == 0) {
        cout << "No se pudieron generar los partidos." << endl;
        return;
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        for(int j = 0; j < grupos[i].getCantidadEquipos(); j++) {
            Equipo* e = grupos[i].getEquipo(j);
            if(e != 0) {
                e->reiniciarDatosGrupo();
            }
        }
    }

    cout << "\n===== SIMULACION FASE DE GRUPOS =====\n";

    for(int i = 0; i < cantidadPartidosGrupos; i++) {
        partidosGrupos[i].simular(true);
        cout << "\nPartido " << i + 1 << endl;
        partidosGrupos[i].imprimir();
        partidosGrupos[i].imprimirGoleadores();
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        grupos[i].ordenarTabla();
    }

    cout << "\nFase de grupos simulada correctamente." << endl;
}


void Torneo::mostrarTablaDeGrupos() {
    if(grupos == 0) {
        cout << "No hay grupos sorteados." << endl;
        return;
    }

    cout << "\n===== TABLAS DE GRUPOS =====\n";

    for(int i = 0; i < cantidadGrupos; i++) {
        grupos[i].ordenarTabla();

        cout << "\nGrupo " << grupos[i].getLetra() << endl;

        for(int j = 0; j < grupos[i].getCantidadEquipos(); j++) {
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
}

void Torneo::clasificarADieciseisavos() {
    if(grupos == 0) {
        cout << "Primero debe sortear y simular la fase de grupos." << endl;
        return;
    }

    delete[] clasificadosR16;
    clasificadosR16 = new Equipo*[32];
    cantidadClasificadosR16 = 0;

    Equipo** terceros = new Equipo*[12];
    int cantidadTerceros = 0;

    // Ordenar cada grupo y tomar primeros, segundos y terceros
    for(int i = 0; i < cantidadGrupos; i++) {
        grupos[i].ordenarTabla();

        Equipo* primero = grupos[i].getEquipo(0);
        Equipo* segundo = grupos[i].getEquipo(1);
        Equipo* tercero = grupos[i].getEquipo(2);

        if(primero != 0) {
            clasificadosR16[cantidadClasificadosR16] = primero;
            cantidadClasificadosR16++;
        }

        if(segundo != 0) {
            clasificadosR16[cantidadClasificadosR16] = segundo;
            cantidadClasificadosR16++;
        }

        if(tercero != 0) {
            terceros[cantidadTerceros] = tercero;
            cantidadTerceros++;
        }
    }

    // Ordenar terceros por puntos, diferencia de goles y goles a favor
    for(int i = 0; i < cantidadTerceros - 1; i++) {
        for(int j = 0; j < cantidadTerceros - 1 - i; j++) {
            bool cambiar = false;

            if(terceros[j]->getPuntosGrupo() < terceros[j + 1]->getPuntosGrupo()) {
                cambiar = true;
            } else if(terceros[j]->getPuntosGrupo() == terceros[j + 1]->getPuntosGrupo()) {
                if(terceros[j]->getDiferenciaGolesGrupo() < terceros[j + 1]->getDiferenciaGolesGrupo()) {
                    cambiar = true;
                } else if(terceros[j]->getDiferenciaGolesGrupo() == terceros[j + 1]->getDiferenciaGolesGrupo()) {
                    if(terceros[j]->getGolesFavorGrupo() < terceros[j + 1]->getGolesFavorGrupo()) {
                        cambiar = true;
                    }
                }
            }

            if(cambiar) {
                Equipo* temp = terceros[j];
                terceros[j] = terceros[j + 1];
                terceros[j + 1] = temp;
            }
        }
    }

    // Pasan los 8 mejores terceros
    for(int i = 0; i < 8 && i < cantidadTerceros; i++) {
        clasificadosR16[cantidadClasificadosR16] = terceros[i];
        cantidadClasificadosR16++;
    }

    delete[] terceros;

    cout << "Clasificacion a dieciseisavos generada correctamente." << endl;
    cout << "Cantidad de clasificados: " << cantidadClasificadosR16 << endl;
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
}

void Torneo::simularFasesFinales() {
    if(clasificadosR16 == 0 || cantidadClasificadosR16 < 32) {
        cout << "Primero debe clasificar los equipos a dieciseisavos." << endl;
        return;
    }

    Fecha fechaFinales(10, 7, 2026);

    delete[] partidosR16;
    delete[] partidosR8;
    delete[] partidosQF;
    delete[] partidosSF;
    delete[] partidosFinales;

    partidosR16 = new Partido[16];
    partidosR8 = new Partido[8];
    partidosQF = new Partido[4];
    partidosSF = new Partido[2];
    partidosFinales = new Partido[2];

    Equipo** ganadoresR16 = new Equipo*[16];
    Equipo** ganadoresR8 = new Equipo*[8];
    Equipo** ganadoresQF = new Equipo*[4];
    Equipo** ganadoresSF = new Equipo*[2];
    Equipo** perdedoresSF = new Equipo*[2];

    cout << "\n===== DIECISEISAVOS DE FINAL =====\n";

    for(int i = 0; i < 16; i++) {
        Equipo* e1 = clasificadosR16[i * 2];
        Equipo* e2 = clasificadosR16[i * 2 + 1];

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
        cout << endl;

        if(partidosR16[i].getGoles1() > partidosR16[i].getGoles2()) {
            ganadoresR16[i] = partidosR16[i].getEquipo1();
        } else {
            ganadoresR16[i] = partidosR16[i].getEquipo2();
        }
    }

    cout << "\n===== OCTAVOS DE FINAL =====\n";

    for(int i = 0; i < 8; i++) {
        Equipo* e1 = ganadoresR16[i * 2];
        Equipo* e2 = ganadoresR16[i * 2 + 1];

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
        cout << endl;

        if(partidosR8[i].getGoles1() > partidosR8[i].getGoles2()) {
            ganadoresR8[i] = partidosR8[i].getEquipo1();
        } else {
            ganadoresR8[i] = partidosR8[i].getEquipo2();
        }
    }

    cout << "\n===== CUARTOS DE FINAL =====\n";

    for(int i = 0; i < 4; i++) {
        Equipo* e1 = ganadoresR8[i * 2];
        Equipo* e2 = ganadoresR8[i * 2 + 1];

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
        cout << endl;

        if(partidosQF[i].getGoles1() > partidosQF[i].getGoles2()) {
            ganadoresQF[i] = partidosQF[i].getEquipo1();
        } else {
            ganadoresQF[i] = partidosQF[i].getEquipo2();
        }
    }

    cout << "\n===== SEMIFINALES =====\n";

    for(int i = 0; i < 2; i++) {
        Equipo* e1 = ganadoresQF[i * 2];
        Equipo* e2 = ganadoresQF[i * 2 + 1];

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

    cout << "\nFases finales simuladas correctamente." << endl;
    cout << "Campeon del mundo: " << top4[0]->getPais() << endl;

    delete[] ganadoresR16;
    delete[] ganadoresR8;
    delete[] ganadoresQF;
    delete[] ganadoresSF;
    delete[] perdedoresSF;
}

void Torneo::mostrarEstadisticasFinales() {
    if(top4 == 0 || top4[0] == 0) {
        cout << "Primero debe simular las fases finales." << endl;
        return;
    }

    cout << "\n===== ESTADISTICAS FINALES DEL TORNEO =====\n";

    cout << "\nTop 4 del mundial:\n";
    cout << "1. Campeon: " << top4[0]->getPais() << endl;
    cout << "2. Subcampeon: " << top4[1]->getPais() << endl;
    cout << "3. Tercer puesto: " << top4[2]->getPais() << endl;
    cout << "4. Cuarto puesto: " << top4[3]->getPais() << endl;

    cout << "\nMaximo goleador del equipo campeon:\n";

    Jugador* plantillaCampeon = top4[0]->getPlantilla();
    int cantidadJugadoresCampeon = top4[0]->getCantidadJugadores();

    Jugador* maximoCampeon = 0;

    for(int i = 0; i < cantidadJugadoresCampeon; i++) {
        if(maximoCampeon == 0 || plantillaCampeon[i].getGoles() > maximoCampeon->getGoles()) {
            maximoCampeon = &plantillaCampeon[i];
        }
    }

    if(maximoCampeon != 0) {
        cout << "#" << maximoCampeon->getNumeroCamiseta()
        << " " << maximoCampeon->getNombre()
        << " " << maximoCampeon->getApellido()
        << " | Goles: " << maximoCampeon->getGoles()
        << endl;
    }

    Jugador* topJugadores[3] = {0, 0, 0};
    Equipo* equiposTopJugadores[3] = {0, 0, 0};

    for(int i = 0; i < cantidadEquipos; i++) {
        Jugador* plantilla = equipos[i].getPlantilla();
        int cant = equipos[i].getCantidadJugadores();

        for(int j = 0; j < cant; j++) {
            Jugador* actual = &plantilla[j];

            for(int k = 0; k < 3; k++) {
                if(topJugadores[k] == 0 || actual->getGoles() > topJugadores[k]->getGoles()) {
                    for(int m = 2; m > k; m--) {
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
        if(topJugadores[i] != 0) {
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
}