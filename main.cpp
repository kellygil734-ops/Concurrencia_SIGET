#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>

using namespace std;

// Configuración del escenario
const int CAPACIDAD_BUFFER = 5;
queue<int> buffer_trafico;
mutex mtx;

int main() {
    cout << "--- INICIANDO SIMULACION DE CONCURRENCIA SIGET ---" << endl;

    // Se requieren al menos 3 hilos concurrentes
    thread s1(sensor_trafico, 1);
    thread s2(sensor_trafico, 2);
    thread analista(modulo_analisis);

    s1.join();
    s2.join();
    analista.join();

    cout << "--- SIMULACION FINALIZADA CON EXITO ---" << endl;
    return 0;
}
