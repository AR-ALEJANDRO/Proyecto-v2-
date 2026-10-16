#include <string>
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct Alumno   {string nombre; string matricula; float calif[3]; string grupo; };
struct Maestro  {string nombre; string materia; };
struct Grupo    {string nombre; string carrera; };

float calcularPromedio(const float calif[3]) {
    float suma = 0;
    for (int i = 0; i < 3; i++) {
        suma += calif[i];
    }
    return suma / 3;
}

int main() {
    int opcion;
    vector<Alumno> alumnos;
    vector<Grupo> grupos;
    vector<Maestro> maestros;

    do {
        cout << "\n-Sistema de Alumnos-" << endl;
        cout << "1. Registra los Alumnos" << endl;
        cout << "2. Registra los Grupos y Carreras" << endl;
        cout << "3. Registra a los Docentes" << endl;
        cout << "4. Muestra de Reportes" << endl;
        cout << "5. Salir" << endl;

        cout << "Elige una opcion: ";
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
        }

        if (opcion < 1 || opcion > 5) {
            cout << "Opcion invalida. Intenta de nuevo." << endl;
        } else {
            switch (opcion) {
                case 1: {
                    int cantidad;

                    if (grupos.empty()) {
                        cout << "Primero debera registrar los grupos en la opcion 2." << endl;
                        break;
                    }

                    cout << "Ingrese la cantidad de alumnos a registrar: ";
                    cin >> cantidad;

                    if (cantidad <= 0) {
                        cout << "Cantidad invalida. Debe ser mayor a 0." << endl;
                        break;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    for (int i = 0; i < cantidad; i++) {
                        Alumno nuevo;

                        cout << "Ingrese el nombre del alumno " << (i + 1) << ": ";
                        getline(cin, nuevo.nombre);

                        cout << "Ingrese la matricula del alumno: ";
                        cin >> nuevo.matricula;

                        for (int j = 0; j < 3; j++) {
                            cout << "Ingrese la calificacion " << j + 1 << ": ";
                            cin >> nuevo.calif[j];
                        }

                        cout << "Grupos disponibles:" << endl;
                        for (int k = 0; k < grupos.size(); k++) {
                            cout << grupos[k].nombre << endl;
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        bool grupoValido = false;
                        cout << "Ingrese el grupo del alumno: ";
                        getline(cin, nuevo.grupo);
                        for (int k = 0; k < grupos.size(); k++) {
                            if (grupos[k].nombre == nuevo.grupo) {
                                grupoValido = true;
                                break;
                            }
                        }

                        if (grupoValido) {
                            cout << "Alumno registrado: " << nuevo.nombre << endl;
                            alumnos.push_back(nuevo);
                        } else {
                            cout << "Error: el grupo no existe. Alumno no registrado." << endl;
                        }
                    }

                    cout << "Proceso de registro del alumnado finalizado." << endl;
                    break;
                }

                case 2: {
                    int cantidadGrupos;
                    cout << "Ingrese la cantidad de grupos a registrar: ";
                    cin >> cantidadGrupos;

                    if (cantidadGrupos <= 0) {
                        cout << "Cantidad invalida. Debe ser mayor a 0." << endl;
                        break;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    for (int i = 0; i < cantidadGrupos; i++) {
                        Grupo nuevoGrupo;

                        cout << "Ingrese el nombre del grupo " << (i + 1) << ": ";
                        getline(cin, nuevoGrupo.nombre);

                        cout << "Ingrese la carrera del grupo: ";
                        getline(cin, nuevoGrupo.carrera);

                        grupos.push_back(nuevoGrupo);
                    }

                    cout << "Grupos registrados exitosamente." << endl;
                    break;
                }

                case 3: {
                    int cantidadDocentes;

                    cout << "Ingrese la cantidad de docentes a registrar: ";
                    cin >> cantidadDocentes;

                    if (cantidadDocentes <= 0) {
                        cout << "Cantidad invalida. Debe ser mayor a 0." << endl;
                        break;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    for (int i = 0; i < cantidadDocentes; i++) {
                        Maestro nuevoDocente;

                        cout << "Ingrese el nombre del docente " << (i + 1) << ": ";
                        getline(cin, nuevoDocente.nombre);

                        cout << "Ingrese la materia del docente: ";
                        getline(cin, nuevoDocente.materia);

                        maestros.push_back(nuevoDocente);
                    }

                    cout << "Docentes registrados exitosamente." << endl;
                    break;
                }

                case 4: {
                    // Promedio por alumno
                    cout << "\n-.Reporte de Promedios.-" << endl;
                    if (alumnos.empty()) {
                        cout << "No hay alumnos registrados." << endl;
                    }
                    for (int i = 0; i < alumnos.size(); i++) {
                        float promedio = calcularPromedio(alumnos[i].calif);
                        cout << "Alumno: " << alumnos[i].nombre
                             << ", Promedio: " << promedio << endl;
                    }

                    // Mejor alumno por grupo
                    cout << "\n--- Mejor alumno por grupo ---" << endl;
                    for (int i = 0; i < grupos.size(); i++) {
                        bool hayAlumnos = false;
                        float mejorPromedio = 0;
                        string mejorAlumno = "";

                        for (int j = 0; j < alumnos.size(); j++) {
                            if (alumnos[j].grupo == grupos[i].nombre) {
                                float promedio = calcularPromedio(alumnos[j].calif);
                                if (!hayAlumnos || promedio > mejorPromedio) {
                                    mejorPromedio = promedio;
                                    mejorAlumno = alumnos[j].nombre;
                                    hayAlumnos = true;
                                }
                            }
                        }

                        if (hayAlumnos) {
                            cout << "Grupo: " << grupos[i].nombre
                                 << " | Mejor alumno: " << mejorAlumno
                                 << " | Promedio: " << mejorPromedio << endl;
                        } else {
                            cout << "El grupo " << grupos[i].nombre
                                 << " no tiene alumnos registrados." << endl;
                        }
                    }

                    // Alumnos por carrera (cada carrera una sola vez)
                    cout << "\n--- Alumnos por carrera ---" << endl;
                    for (int i = 0; i < grupos.size(); i++) {
                        bool repetida = false;
                        for (int j = 0; j < i; j++) {
                            if (grupos[j].carrera == grupos[i].carrera) {
                                repetida = true;
                                break;
                            }
                        }
                        if (repetida) continue;

                        int cantidadAlumnos = 0;
                        for (int j = 0; j < grupos.size(); j++) {
                            if (grupos[j].carrera == grupos[i].carrera) {
                                for (int k = 0; k < alumnos.size(); k++) {
                                    if (alumnos[k].grupo == grupos[j].nombre) {
                                        cantidadAlumnos++;
                                    }
                                }
                            }
                        }

                        cout << "Carrera: " << grupos[i].carrera
                             << " | Alumnos: " << cantidadAlumnos << endl;
                    }

                    // Docentes
                    cout << "\n--- Docentes registrados ---" << endl;
                    if (maestros.empty()) {
                        cout << "No hay docentes registrados." << endl;
                    }
                    for (int i = 0; i < maestros.size(); i++) {
                        cout << maestros[i].nombre << " - " << maestros[i].materia << endl;
                    }
                    break;
                }

                case 5:
                    cout << "Saliendo del programa..." << endl;
                    break;
            }
        }

    } while (opcion != 5);

    return 0;
}