#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <utility>   
#include <cstdio>    
using namespace std;

struct Alumno  { string nombre; string matricula; float calif[3]; int grupo; };  
struct Maestro { string nombre; string materia; };
struct Grupo   { string nombre; string carrera; };

const string ARCHIVO_DATOS   = "datos.txt";
const string ARCHIVO_REPORTE = "reporte.txt";

const int MAX_GRUPOS   = 50;
const int MAX_DOCENTES = 50;
const int MAX_ALUMNOS  = 100;

struct EntradaCerrada {};

float calcularPromedio(const float calif[3]) {
    float suma = 0;
    for (int i = 0; i < 3; i++) {
        suma += calif[i];
    }
    return suma / 3;
}

string recortar(const string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fin - ini + 1);
}

string leerEntrada(const string& mensaje) {
    string texto;
    cout << mensaje;
    if (!getline(cin, texto)) {
        throw EntradaCerrada();
    }
    return texto;
}

int leerEntero(string mensaje, int min, int max) {
    while (true) {
        string texto = recortar(leerEntrada(mensaje));
        size_t pos = 0;
        int valor = 0;

        try {
            valor = stoi(texto, &pos);
        } catch (...) {
            cout << "Entrada invalida. Escribe un numero entero." << endl;
            continue;
        }

        if (pos != texto.size()) {   
            cout << "Entrada invalida. Escribe un numero entero." << endl;
            continue;
        }

        if (valor < min || valor > max) {
            cout << "El valor debe estar entre " << min << " y " << max << ". Intente de nuevo." << endl;
            continue;
        }

        return valor;
    }
}

float leerFloat(string mensaje, float min, float max) {
    while (true) {
        string texto = recortar(leerEntrada(mensaje));
        size_t pos = 0;
        float valor = 0;

        try {
            valor = stof(texto, &pos);
        } catch (...) {
            cout << "Entrada invalida. Escribe un numero." << endl;
            continue;
        }

        if (pos != texto.size()) {   
            cout << "Entrada invalida. Escribe un numero." << endl;
            continue;
        }

        if (!(valor >= min && valor <= max)) {
            cout << "El valor debe estar entre " << min << " y " << max << ". Intente de nuevo." << endl;
            continue;
        }

        return valor;
    }
}

string leerLinea(string mensaje) {
    while (true) {
        string texto = recortar(leerEntrada(mensaje));
        if (!texto.empty()) {
            return texto;
        }
        cout << "No puede estar vacio. Intente de nuevo." << endl;
    }
}

bool existeMatricula(const vector<Alumno>& alumnos, const string& matricula) {
    for (int i = 0; i < (int)alumnos.size(); i++) {
        if (alumnos[i].matricula == matricula) {
            return true;
        }
    }
    return false;
}

int elegirGrupo(const vector<Grupo>& grupos) {
    cout << "Grupos disponibles:" << endl;
    for (int i = 0; i < (int)grupos.size(); i++) {
        cout << (i + 1) << ". " << grupos[i].nombre << " (" << grupos[i].carrera << ")" << endl;
    }
    return leerEntero("Elija el numero del grupo: ", 1, (int)grupos.size()) - 1;
}

void registrarAlumnos(vector<Alumno>& alumnos, const vector<Grupo>& grupos) {
    if (grupos.empty()) {
        cout << "Primero debera registrar los grupos en la opcion 2." << endl;
        return;
    }

    int cantidad = leerEntero("Ingrese la cantidad de alumnos a registrar: ", 1, 100);

    for (int i = 0; i < cantidad; i++) {
        Alumno nuevo;

        nuevo.nombre = leerLinea("Ingrese el nombre del alumno " + to_string(i + 1) + ": ");

        while (true) {
            nuevo.matricula = leerLinea("Ingrese la matricula del alumno: ");
            if (!existeMatricula(alumnos, nuevo.matricula)) {
                break;
            }
            cout << "Esa matricula ya esta registrada. Intente de nuevo." << endl;
        }

        for (int j = 0; j < 3; j++) {
            nuevo.calif[j] = leerFloat("Ingrese la calificacion " + to_string(j + 1) + " (0-10): ", 0, 10);
        }

        nuevo.grupo = elegirGrupo(grupos);

        alumnos.push_back(nuevo);
        cout << "Alumno registrado: " << nuevo.nombre << endl;
    }

    cout << "Proceso de registro del alumnado finalizado." << endl;
}

void registrarGrupos(vector<Grupo>& grupos) {
    int cantidad = leerEntero("Ingrese la cantidad de grupos a registrar: ", 1, 50);

    for (int i = 0; i < cantidad; i++) {
        Grupo nuevo;
        nuevo.nombre = leerLinea("Ingrese el nombre del grupo " + to_string(i + 1) + ": ");
        nuevo.carrera = leerLinea("Ingrese la carrera del grupo: ");
        grupos.push_back(nuevo);
    }

    cout << "Grupos registrados exitosamente." << endl;
}

void registrarDocentes(vector<Maestro>& maestros) {
    int cantidad = leerEntero("Ingrese la cantidad de docentes a registrar: ", 1, 50);

    for (int i = 0; i < cantidad; i++) {
        Maestro nuevo;
        nuevo.nombre = leerLinea("Ingrese el nombre del docente " + to_string(i + 1) + ": ");
        nuevo.materia = leerLinea("Ingrese la materia del docente: ");
        maestros.push_back(nuevo);
    }

    cout << "Docentes registrados exitosamente." << endl;
}

void escribirReporte(ostream& out, const vector<Alumno>& alumnos,
                     const vector<Grupo>& grupos, const vector<Maestro>& maestros) {
    out << fixed << setprecision(2);

    vector<Alumno> ordenados = alumnos;
    for (int i = 0; i < (int)ordenados.size() - 1; i++) {
        for (int j = 0; j < (int)ordenados.size() - 1 - i; j++) {
            if (calcularPromedio(ordenados[j].calif) < calcularPromedio(ordenados[j + 1].calif)) {
                swap(ordenados[j], ordenados[j + 1]);
            }
        }
    }

    out << "\n===== REPORTE DE ALUMNOS (ordenado por promedio) =====" << endl;
    if (ordenados.empty()) {
        out << "No hay alumnos registrados." << endl;
    }
    for (int i = 0; i < (int)ordenados.size(); i++) {
        out << (i + 1) << ". " << ordenados[i].nombre
            << " | Matricula: " << ordenados[i].matricula
            << " | Grupo: " << grupos[ordenados[i].grupo].nombre
            << " | Promedio: " << calcularPromedio(ordenados[i].calif) << endl;
    }

    out << "\n--- Mejor alumno por grupo ---" << endl;
    for (int i = 0; i < (int)grupos.size(); i++) {
        bool hayAlumnos = false;
        float mejorPromedio = 0;
        string mejorAlumno = "";

        for (int j = 0; j < (int)alumnos.size(); j++) {
            if (alumnos[j].grupo == i) {
                float promedio = calcularPromedio(alumnos[j].calif);
                if (!hayAlumnos || promedio > mejorPromedio) {
                    mejorPromedio = promedio;
                    mejorAlumno = alumnos[j].nombre;
                    hayAlumnos = true;
                }
            }
        }

        if (hayAlumnos) {
            out << "Grupo: " << grupos[i].nombre
                << " | Mejor alumno: " << mejorAlumno
                << " | Promedio: " << mejorPromedio << endl;
        } else {
            out << "El grupo " << grupos[i].nombre << " no tiene alumnos registrados." << endl;
        }
    }

    out << "\n--- Alumnos por carrera ---" << endl;
    for (int i = 0; i < (int)grupos.size(); i++) {
        bool repetida = false;
        for (int j = 0; j < i; j++) {
            if (grupos[j].carrera == grupos[i].carrera) {
                repetida = true;
                break;
            }
        }
        if (repetida) continue;

        int cantidadAlumnos = 0;
        for (int j = 0; j < (int)grupos.size(); j++) {
            if (grupos[j].carrera == grupos[i].carrera) {
                for (int k = 0; k < (int)alumnos.size(); k++) {
                    if (alumnos[k].grupo == j) {
                        cantidadAlumnos++;
                    }
                }
            }
        }

        out << "Carrera: " << grupos[i].carrera << " | Alumnos: " << cantidadAlumnos << endl;
    }

    out << "\n--- Docentes registrados ---" << endl;
    if (maestros.empty()) {
        out << "No hay docentes registrados." << endl;
    }
    for (int i = 0; i < (int)maestros.size(); i++) {
        out << maestros[i].nombre << " - " << maestros[i].materia << endl;
    }
}

void mostrarReportes(const vector<Alumno>& alumnos, const vector<Grupo>& grupos,
                     const vector<Maestro>& maestros) {
    escribirReporte(cout, alumnos, grupos, maestros);
}

void exportarReporte(const vector<Alumno>& alumnos, const vector<Grupo>& grupos,
                     const vector<Maestro>& maestros) {
    ofstream archivo(ARCHIVO_REPORTE);
    if (!archivo) {
        cout << "\nNo se pudo crear " << ARCHIVO_REPORTE << "." << endl;
        return;
    }
    escribirReporte(archivo, alumnos, grupos, maestros);
    archivo.close();
    if (!archivo) {
        cout << "\nError al escribir " << ARCHIVO_REPORTE << ". El reporte puede estar incompleto." << endl;
        return;
    }
    cout << "\nReporte exportado a " << ARCHIVO_REPORTE << endl;
}

bool guardarDatos(const vector<Alumno>& alumnos, const vector<Grupo>& grupos,
                  const vector<Maestro>& maestros) {
    const string temporal = ARCHIVO_DATOS + ".tmp";
    ofstream archivo(temporal);
    if (!archivo) {
        cout << "No se pudo crear " << temporal << ". " << ARCHIVO_DATOS << " no se modifico." << endl;
        return false;
    }

    archivo << grupos.size() << "\n";
    for (int i = 0; i < (int)grupos.size(); i++) {
        archivo << grupos[i].nombre << "\n" << grupos[i].carrera << "\n";
    }

    archivo << maestros.size() << "\n";
    for (int i = 0; i < (int)maestros.size(); i++) {
        archivo << maestros[i].nombre << "\n" << maestros[i].materia << "\n";
    }

    archivo << alumnos.size() << "\n";
    for (int i = 0; i < (int)alumnos.size(); i++) {
        archivo << alumnos[i].nombre << "\n"
                << alumnos[i].matricula << "\n"
                << alumnos[i].calif[0] << " " << alumnos[i].calif[1] << " " << alumnos[i].calif[2] << "\n"
                << alumnos[i].grupo << "\n";
    }

    archivo.close();
    if (!archivo) {
        remove(temporal.c_str());
        cout << "Error al escribir " << temporal << ". " << ARCHIVO_DATOS << " no se modifico." << endl;
        return false;
    }

    
    if (rename(temporal.c_str(), ARCHIVO_DATOS.c_str()) != 0) {
        const string respaldo = ARCHIVO_DATOS + ".bak";
        remove(respaldo.c_str());
        bool reemplazado = false;
        if (rename(ARCHIVO_DATOS.c_str(), respaldo.c_str()) == 0) {
            if (rename(temporal.c_str(), ARCHIVO_DATOS.c_str()) == 0) {
                reemplazado = true;
                remove(respaldo.c_str());
            } else {
                rename(respaldo.c_str(), ARCHIVO_DATOS.c_str());   
            }
        }
        if (!reemplazado) {
            remove(temporal.c_str());
            cout << "No se pudo reemplazar " << ARCHIVO_DATOS << ". Se conservo el original." << endl;
            return false;
        }
    }

    return true;
}

bool cargarDatos(vector<Alumno>& alumnos, vector<Grupo>& grupos, vector<Maestro>& maestros) {
    ifstream archivo(ARCHIVO_DATOS);
    if (!archivo) {
        return true;
    }

    int n;

    if (!(archivo >> n) || n < 0 || n > MAX_GRUPOS) return false;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < n; i++) {
        Grupo g;
        getline(archivo, g.nombre);
        getline(archivo, g.carrera);
        if (!archivo) return false;
        g.nombre = recortar(g.nombre);
        g.carrera = recortar(g.carrera);
        if (g.nombre.empty() || g.carrera.empty()) return false;
        grupos.push_back(g);
    }

    if (!(archivo >> n) || n < 0 || n > MAX_DOCENTES) return false;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < n; i++) {
        Maestro m;
        getline(archivo, m.nombre);
        getline(archivo, m.materia);
        if (!archivo) return false;
        m.nombre = recortar(m.nombre);
        m.materia = recortar(m.materia);
        if (m.nombre.empty() || m.materia.empty()) return false;
        maestros.push_back(m);
    }

    if (!(archivo >> n) || n < 0 || n > MAX_ALUMNOS) return false;
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < n; i++) {
        Alumno a;
        getline(archivo, a.nombre);
        getline(archivo, a.matricula);
        archivo >> a.calif[0] >> a.calif[1] >> a.calif[2] >> a.grupo;
        archivo.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!archivo || a.grupo < 0 || a.grupo >= (int)grupos.size()) return false;

        a.nombre = recortar(a.nombre);
        a.matricula = recortar(a.matricula);
        if (a.nombre.empty() || a.matricula.empty()) return false;

        for (int k = 0; k < 3; k++) {
            if (!(a.calif[k] >= 0 && a.calif[k] <= 10)) return false;
        }

        if (existeMatricula(alumnos, a.matricula)) return false;

        alumnos.push_back(a);
    }

    return true;
}

bool cargarTodo(vector<Alumno>& alumnos, vector<Grupo>& grupos, vector<Maestro>& maestros) {
    alumnos.clear();
    grupos.clear();
    maestros.clear();
    if (!cargarDatos(alumnos, grupos, maestros)) {
        alumnos.clear();
        grupos.clear();
        maestros.clear();
        cout << "El archivo de datos esta danado. Se inicia sin datos." << endl;
        return false;
    }
    return true;
}

int main() {
    vector<Alumno> alumnos;
    vector<Grupo> grupos;
    vector<Maestro> maestros;

    bool archivoDanado = !cargarTodo(alumnos, grupos, maestros);
    int opcion = 0;
    try {
    do {
        cout << "\n-Sistema de Alumnos-" << endl;
        cout << "1. Registra los Alumnos" << endl;
        cout << "2. Registra los Grupos y Carreras" << endl;
        cout << "3. Registra a los Docentes" << endl;
        cout << "4. Muestra de Reportes" << endl;
        cout << "5. Guardar datos" << endl;
        cout << "6. Cargar datos" << endl;
        cout << "7. Exportar reporte a archivo" << endl;
        cout << "8. Salir" << endl;

        opcion = leerEntero("Elige una opcion: ", 1, 8);

        switch (opcion) {
            case 1:
                registrarAlumnos(alumnos, grupos);
                break;
            case 2:
                registrarGrupos(grupos);
                break;
            case 3:
                registrarDocentes(maestros);
                break;
            case 4:
                mostrarReportes(alumnos, grupos, maestros);
                break;
            case 5:
                if (guardarDatos(alumnos, grupos, maestros)) {
                    archivoDanado = false;
                    cout << "Datos guardados en " << ARCHIVO_DATOS << endl;
                }
                break;
            case 6:
                if (leerEntero("Se perderan los cambios no guardados. Continuar? (1 = si, 2 = no): ", 1, 2) == 2) {
                    break;
                }
                archivoDanado = !cargarTodo(alumnos, grupos, maestros);
                cout << "Datos en memoria: " << alumnos.size() << " alumnos, "
                     << grupos.size() << " grupos, " << maestros.size() << " docentes." << endl;
                break;
            case 7:
                exportarReporte(alumnos, grupos, maestros);
                break;
            case 8:
                if (archivoDanado &&
                    leerEntero(ARCHIVO_DATOS + " esta danado y se sobrescribira. 1 = guardar de todos modos, 2 = salir sin guardar: ", 1, 2) == 2) {
                    cout << "Saliendo sin guardar. " << ARCHIVO_DATOS << " no se modifico." << endl;
                } else if (guardarDatos(alumnos, grupos, maestros)) {
                    cout << "Datos guardados. Saliendo del programa..." << endl;
                } else {
                    cout << "ERROR: los datos NO se guardaron. Saliendo del programa..." << endl;
                }
                break;
        }
    } while (opcion != 8);
    } catch (const EntradaCerrada&) {
        cout << "\nEntrada cerrada." << endl;
        if (archivoDanado) {
            cout << ARCHIVO_DATOS << " estaba danado: no se guardo para no sobrescribirlo." << endl;
        } else if (guardarDatos(alumnos, grupos, maestros)) {
            cout << "Datos guardados en " << ARCHIVO_DATOS << ". Saliendo del programa..." << endl;
        } else {
            cout << "ERROR: los datos NO se guardaron. Saliendo del programa..." << endl;
        }
        return 1;
    }

    return 0;
}
