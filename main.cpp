#include <iostream>
#include "torneo.h"

using namespace std;

int main() {
    Torneo torneo;
    char opcion;

    do {
        cout << "\n=========================================\n";
        cout << "        UdeAWorldCup - Menu principal\n";
        cout << "=========================================\n";
        cout << "1. Cargar equipos desde CSV\n";
        cout << "2. Crear jugadores artificiales\n";
        cout << "3. Mostrar equipos cargados\n";
        cout << "4. Formar bombos\n";
        cout << "5. Mostrar bombos\n";
        cout << "6. Sortear grupos\n";
        cout << "7. Mostrar grupos\n";
        cout << "8. Generar partidos de grupos\n";
        cout << "9. Simular fase de grupos\n";
        cout << "A. Mostrar tabla de grupos\n";
        cout << "B. Clasificar a dieciseisavos\n";
        cout << "C. Mostrar clasificados a dieciseisavos\n";
        cout << "D. Simular fases finales\n";
        cout << "E. Mostrar estadisticas finales\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        if(!(cin >> opcion)) {
            cout << "\nNo hay entrada disponible. Activa 'Run in terminal' en Qt Creator.\n";
            return 1;
        }

        switch(opcion) {
        case '1':
            torneo.cargarEquiposDesdeCSV("selecciones_clasificadas_mundial.csv");
            break;
        case '2':
            torneo.crearJugadoresDeTodosLosEquipos();
            break;
        case '3':
            torneo.mostrarEquipos();
            break;
        case '4':
            torneo.formarBombos();
            break;
        case '5':
            torneo.mostrarBombos();
            break;
        case '6':
            torneo.sortearGrupos();
            break;
        case '7':
            torneo.mostrarGrupos();
            break;
        case '8':
            torneo.generarPartidosDeGrupos();
            break;
        case '9':
            torneo.simularFaseDeGrupos();
            break;
        case 'A':
        case 'a':
            torneo.mostrarTablaDeGrupos();
            break;
        case 'B':
        case 'b':
            torneo.clasificarADieciseisavos();
            break;
        case 'D':
        case 'd':
            torneo.simularFasesFinales();
            break;
        case 'E':
        case 'e':
            torneo.mostrarEstadisticasFinales();
            break;

        case 'C':
        case 'c':
            torneo.mostrarClasificadosR16();
            break;
        case '0':
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "\nOpcion invalida.\n";
            break;
        }

    } while(opcion != '0');

    return 0;
}