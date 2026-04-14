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
condition_variable cv_producir, cv_consumir;

// Función del Sensor (Productor)
void sensor_trafico(int id) {
    for (int i = 0; i < 5; ++i) { // 5 lecturas por sensor
        unique_lock<mutex> lock(mtx);
        
        // Esperar si el búfer está lleno (Control de concurrencia)
        cv_producir.wait(lock, [] { return buffer_trafico.size() < CAPACIDAD_BUFFER; });

        int dato = rand() % 100; // Simulación de flujo vehicular
        buffer_trafico.push(dato);
        cout << "[SENSOR " << id << "] Detectado flujo: " << dato << " veh/min. Enviado al SIGET." << endl;

        cv_consumir.notify_one(); // Notificar al módulo de análisis
        lock.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(800)); // Simular tiempo entre lecturas
    }
}

// Función del Módulo de Análisis (Consumidor)
void modulo_analisis() {
    for (int i = 0; i < 10; ++i) { // Procesa el total de lecturas
        unique_lock<mutex> lock(mtx);

        // Esperar si no hay datos (Evitar lectura en vacío)
        cv_consumir.wait(lock, [] { return !buffer_trafico.empty(); });

        int dato = buffer_trafico.front();
        buffer_trafico.pop();
        cout << "   [ANALISIS] Procesando datos del SIGET... Flujo actual: " << dato << ". Ajustando semáforos." << endl;

        cv_producir.notify_one(); // Notificar espacio disponible en búfer
        lock.unlock();

        this_thread::sleep_for(chrono::milliseconds(1200)); // Simular tiempo de procesamiento
    }
}

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