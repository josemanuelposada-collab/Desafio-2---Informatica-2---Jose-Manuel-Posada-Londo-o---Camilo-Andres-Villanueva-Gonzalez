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
    medidor.reiniciar();

    if(equipos != 0) {
        delete[] equipos;
        equipos = 0;
    }

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

    mostrarRecursos("Carga de equipos desde CSV");
}

void Torneo::crearJugadoresDeTodosLosEquipos() {
    medidor.reiniciar();

    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        mostrarRecursos("Creacion de jugadores artificiales");
        return;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        equipos[i].crearPlantillaArtificial();
        medidor.sumarIteracion();
    }

    cout << "Jugadores artificiales creados correctamente." << endl;

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

    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        mostrarRecursos("Formacion de bombos");
        return;
    }

    ordenarEquiposPorRanking();

    for(int i = 0; i < 4; i++) {
        medidor.sumarIteracion();

        delete[] bombos[i];
        bombos[i] = new Equipo*[12];
        cantidadesBombos[i] = 0;
    }

    int indiceUSA = -1;

    for(int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();

        if(sonIguales(equipos[i].getPais(), "EE. UU.") ||
            sonIguales(equipos[i].getPais(), "Estados Unidos") ||
            sonIguales(equipos[i].getPais(), "USA")) {
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

    mostrarRecursos("Formacion de bombos");
}

void Torneo::mostrarBombos() const {
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

    if(bombos[0] == 0 || bombos[1] == 0 || bombos[2] == 0 || bombos[3] == 0) {
        cout << "Primero debe formar los bombos." << endl;
        mostrarRecursos("Sorteo de grupos");
        return;
    }

    bool listo = false;
    int intentos = 0;

    while(!listo && intentos < 10000) {
        medidor.sumarIteracion();

        intentos++;
        listo = true;

        delete[] grupos;
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

    mostrarRecursos("Sorteo de grupos");
}

void Torneo::mostrarGrupos() const {
    if(grupos == 0) {
        cout << "No se han sorteado grupos." << endl;
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

    if(grupos == 0) {
        cout << "Primero debe sortear los grupos." << endl;
        mostrarRecursos("Generacion de partidos de grupos");
        return;
    }

    delete[] partidosGrupos;

    cantidadPartidosGrupos = 72;
    partidosGrupos = new Partido[cantidadPartidosGrupos];

    int indice = 0;
    Fecha fechaInicio(20, 6, 2026);

    for(int g = 0; g < cantidadGrupos; g++) {
        medidor.sumarIteracion();

        if(grupos[g].getCantidadEquipos() < 4) {
            cout << "El grupo " << grupos[g].getLetra()
            << " no tiene 4 equipos. No se pueden generar partidos." << endl;
            mostrarRecursos("Generacion de partidos de grupos");
            return;
        }

        Equipo* e0 = grupos[g].getEquipo(0);
        Equipo* e1 = grupos[g].getEquipo(1);
        Equipo* e2 = grupos[g].getEquipo(2);
        Equipo* e3 = grupos[g].getEquipo(3);

        if(e0 == 0 || e1 == 0 || e2 == 0 || e3 == 0) {
            cout << "Hay equipos nulos en el grupo " << grupos[g].getLetra() << endl;
            mostrarRecursos("Generacion de partidos de grupos");
            return;
        }

        Equipo* lista1[6] = {e0, e0, e0, e1, e1, e2};
        Equipo* lista2[6] = {e1, e2, e3, e2, e3, e3};

        for(int p = 0; p < 6; p++) {
            medidor.sumarIteracion();

            Fecha f = fechaInicio;
            f.sumarDias(indice / 4);

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

    mostrarRecursos("Generacion de partidos de grupos");
}

void Torneo::simularFaseDeGrupos() {
    medidor.reiniciar();

    if(partidosGrupos == 0) {
        generarPartidosDeGrupos();
        medidor.reiniciar();
    }

    if(partidosGrupos == 0) {
        cout << "No se pudieron generar los partidos." << endl;
        mostrarRecursos("Simulacion fase de grupos");
        return;
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
    }

    for(int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();
        grupos[i].ordenarTabla();
    }

    cout << "\nFase de grupos simulada correctamente." << endl;

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

    if(grupos == 0) {
        cout << "Primero debe sortear y simular la fase de grupos." << endl;
        mostrarRecursos("Clasificacion a dieciseisavos");
        return;
    }

    delete[] clasificadosR16;
    clasificadosR16 = new Equipo*[32];
    cantidadClasificadosR16 = 0;

    Equipo** terceros = new Equipo*[12];
    int cantidadTerceros = 0;

    for(int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();

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

    for(int i = 0; i < cantidadTerceros - 1; i++) {
        for(int j = 0; j < cantidadTerceros - 1 - i; j++) {
            medidor.sumarIteracion();

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

    for(int i = 0; i < 8 && i < cantidadTerceros; i++) {
        medidor.sumarIteracion();

        clasificadosR16[cantidadClasificadosR16] = terceros[i];
        cantidadClasificadosR16++;
    }

    long long memoriaTemporal = 12 * sizeof(Equipo*);

    delete[] terceros;

    cout << "Clasificacion a dieciseisavos generada correctamente." << endl;
    cout << "Cantidad de clasificados: " << cantidadClasificadosR16 << endl;
    cout << "Memoria temporal de terceros: " << memoriaTemporal << " bytes" << endl;

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

    cout << "\n===== MEDICION DE RECURSOS =====\n";
    cout << "Funcionalidad: Mostrar clasificados a dieciseisavos" << endl;
    cout << "Iteraciones estimadas: " << cantidadClasificadosR16 << endl;
    cout << "Memoria estimada: " << calcularMemoriaTotal() << " bytes" << endl;
}

void Torneo::simularFasesFinales() {
    medidor.reiniciar();

    if(clasificadosR16 == 0 || cantidadClasificadosR16 < 32) {
        cout << "Primero debe clasificar los equipos a dieciseisavos." << endl;
        mostrarRecursos("Simulacion fases finales");
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
        medidor.sumarIteracion();

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
        medidor.sumarIteracion();

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
        medidor.sumarIteracion();

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
        medidor.sumarIteracion();

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

    medidor.sumarIteracion();

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

    medidor.sumarIteracion();

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

    long long memoriaTemporal = (16 + 8 + 4 + 2 + 2) * sizeof(Equipo*);

    cout << "\nFases finales simuladas correctamente." << endl;
    cout << "Campeon del mundo: " << top4[0]->getPais() << endl;
    cout << "Memoria temporal de arreglos de ganadores: "
         << memoriaTemporal
         << " bytes" << endl;

    mostrarRecursos("Simulacion fases finales");

    delete[] ganadoresR16;
    delete[] ganadoresR8;
    delete[] ganadoresQF;
    delete[] ganadoresSF;
    delete[] perdedoresSF;
}

void Torneo::mostrarEstadisticasFinales() {
    medidor.reiniciar();

    if(top4 == 0 || top4[0] == 0) {
        cout << "Primero debe simular las fases finales." << endl;
        mostrarRecursos("Estadisticas finales");
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
        medidor.sumarIteracion();

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
        medidor.sumarIteracion();

        Jugador* plantilla = equipos[i].getPlantilla();
        int cant = equipos[i].getCantidadJugadores();

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