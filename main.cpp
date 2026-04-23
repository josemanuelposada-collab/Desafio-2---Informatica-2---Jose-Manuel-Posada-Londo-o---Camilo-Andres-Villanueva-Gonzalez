#include <iostream>
#include "jugador.h"
#include "equipo.h"
#include "grupo.h"
#include "fecha.h"
#include "registro_jugador_partido.h"
#include "utilidades.h"

using namespace std;

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

void probarJugador() {
    cout << "\n===== PRUEBA CLASE JUGADOR =====\n";

    Jugador j1("Lionel", "Messi", 10);

    cout << "\nJugador recien creado:\n";
    j1.imprimir();

    j1.sumarGol();
    j1.sumarGol();
    j1.sumarAsistencia();
    j1.sumarAmarilla();
    j1.sumarFalta();
    j1.sumarPartido();
    j1.sumarMinutos(90);

    cout << "\nJugador luego de actualizar estadisticas:\n";
    j1.imprimir();

    Jugador j2(j1); // constructor de copia
    cout << "\nCopia del jugador:\n";
    j2.imprimir();
}

void probarEquipo() {
    cout << "\n===== PRUEBA CLASE EQUIPO =====\n";

    Equipo equipo("Argentina", "CONMEBOL", "Scaloni", 1, 52, 18, 30, 10, 5, 0, 0, 0);

    cout << "\nResumen del equipo:\n";
    equipo.imprimirResumen();

    equipo.crearPlantillaArtificial();

    cout << "\nCantidad de jugadores en plantilla: "
         << equipo.getCantidadJugadores() << endl;

    cout << "\nPrimeros 5 jugadores de la plantilla:\n";
    Jugador* plantilla = equipo.getPlantilla();
    for(int i = 0; i < 5 && i < equipo.getCantidadJugadores(); i++) {
        plantilla[i].imprimir();
    }

    int* once = equipo.obtenerOnceAleatorio();
    if(once != 0) {
        cout << "\nOnce aleatorio (indices de la plantilla):\n";
        for(int i = 0; i < 11; i++) {
            cout << once[i] << " ";
        }
        cout << endl;
        delete[] once;
    }
}

void imprimirTablaGrupo(const Grupo& grupo) {
    cout << "\nTabla del grupo " << grupo.getLetra() << ":\n";
    for(int i = 0; i < grupo.getCantidadEquipos(); i++) {
        Equipo* e = grupo.getEquipo(i);
        if(e != 0) {
            cout << i + 1 << ". "
                 << e->getPais()
                 << " | Pts: " << e->getPuntosGrupo()
                 << " | GF: " << e->getGolesFavorGrupo()
                 << " | GC: " << e->getGolesContraGrupo()
                 << " | DG: " << e->getDiferenciaGolesGrupo()
                 << endl;
        }
    }
}

void probarGrupo() {
    cout << "\n===== PRUEBA CLASE GRUPO =====\n";

    Equipo argentina("Argentina", "CONMEBOL", "Scaloni", 1, 52, 18, 30, 10, 5, 0, 0, 0);
    Equipo brasil("Brasil", "CONMEBOL", "Dorival", 5, 48, 20, 28, 8, 7, 0, 0, 0);
    Equipo espana("Espana", "UEFA", "De la Fuente", 3, 44, 16, 25, 9, 6, 0, 0, 0);
    Equipo alemania("Alemania", "UEFA", "Nagelsmann", 8, 41, 19, 22, 11, 8, 0, 0, 0);
    Equipo francia("Francia", "UEFA", "Deschamps", 2, 50, 17, 27, 9, 5, 0, 0, 0);

    Grupo grupoA('A');

    cout << "\nAgregando equipos al grupo:\n";
    cout << "Argentina: " << (grupoA.agregarEquipo(&argentina) ? "agregado" : "no agregado") << endl;
    cout << "Brasil: "    << (grupoA.agregarEquipo(&brasil) ? "agregado" : "no agregado") << endl;
    cout << "Espana: "    << (grupoA.agregarEquipo(&espana) ? "agregado" : "no agregado") << endl;
    cout << "Alemania: "  << (grupoA.agregarEquipo(&alemania) ? "agregado" : "no agregado") << endl;
    cout << "Francia: "   << (grupoA.agregarEquipo(&francia) ? "agregado" : "no agregado") << endl;

    cout << "\nGrupo conformado:\n";
    grupoA.imprimir();

    // Simulacion manual de resultados para evidenciar tabla y ordenamiento
    argentina.reiniciarDatosGrupo();
    espana.reiniciarDatosGrupo();
    alemania.reiniciarDatosGrupo();

    argentina.registrarResultadoGrupo(2, 0); // victoria
    argentina.registrarResultadoGrupo(1, 1); // empate
    argentina.registrarResultadoGrupo(3, 2); // victoria

    espana.registrarResultadoGrupo(1, 0);    // victoria
    espana.registrarResultadoGrupo(0, 0);    // empate
    espana.registrarResultadoGrupo(0, 2);    // derrota

    alemania.registrarResultadoGrupo(0, 1);  // derrota
    alemania.registrarResultadoGrupo(2, 2);  // empate
    alemania.registrarResultadoGrupo(1, 0);  // victoria

    grupoA.ordenarTabla();

    imprimirTablaGrupo(grupoA);
}

void probarFecha() {
    cout << "\n===== PRUEBA CLASE FECHA =====\n";

    Fecha inicio(20, 6, 2026);
    Fecha otra(10, 7, 2026);

    cout << "\nFecha inicial: ";
    inicio.imprimir();

    cout << "\nOtra fecha: ";
    otra.imprimir();

    cout << "\n\nSumando 15 dias a la fecha inicial...\n";
    inicio.sumarDias(15);

    cout << "Nueva fecha: ";
    inicio.imprimir();

    cout << "\n\nDiferencia de dias entre ambas fechas: "
         << inicio.diferenciaDias(otra) << endl;
}

void probarRegistroJugadorPartido() {
    cout << "\n===== PRUEBA CLASE REGISTRO_JUGADOR_PARTIDO =====\n";

    Jugador jugador("Julian", "Alvarez", 9);

    cout << "\nJugador antes del partido:\n";
    jugador.imprimir();

    RegistroJugadorPartido registro(&jugador);
    registro.sumarGol();
    registro.sumarGol();
    registro.sumarAmarilla();
    registro.sumarFalta();
    registro.setMinutosJugados(90);

    cout << "\nRegistro del partido:\n";
    registro.imprimir();

    registro.aplicarAHistorico();

    cout << "\nJugador luego de aplicar el registro al historico:\n";
    jugador.imprimir();
}

void mostrarMenu() {
    cout << "\n=========================================\n";
    cout << "        UdeAWorldCup - Menu base\n";
    cout << "=========================================\n";
    cout << "1. Probar clase Jugador\n";
    cout << "2. Probar clase Equipo\n";
    cout << "3. Probar clase Grupo\n";
    cout << "4. Probar clase Fecha\n";
    cout << "5. Probar clase RegistroJugadorPartido\n";
    cout << "0. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    inicializarAleatorio();

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        switch(opcion) {
        case 1:
            probarJugador();
            pausar();
            break;
        case 2:
            probarEquipo();
            pausar();
            break;
        case 3:
            probarGrupo();
            pausar();
            break;
        case 4:
            probarFecha();
            pausar();
            break;
        case 5:
            probarRegistroJugadorPartido();
            pausar();
            break;
        case 0:
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "\nOpcion invalida.\n";
            pausar();
            break;
        }

    } while(opcion != 0);

    return 0;
}