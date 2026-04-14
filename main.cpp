#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

// Variables globales para sincronización
mutex m;
condition_variable prod, cons;
queue<int> buffer_trafico;
const int CAPACIDAD = 5; 

void sensor_trafico(int id) {
    for(int i = 0; i < 5; i++) {
        unique_lock<mutex> lck(m);
        
        // Espera si el buffer está lleno (Control de concurrencia)
        prod.wait(lck, [] { return buffer_trafico.size() < CAPACIDAD; });

        int flujo = rand() % 100; 
        buffer_trafico.push(flujo);
        
        cout << "[SENSOR " << id << "] Detectado flujo: " << flujo << " veh/min. Enviado al SIGET." << endl;

        cons.notify_one(); 
        lck.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(800));
    }
}

void modulo_analisis() {
    for(int i = 0; i < 10; i++) {
        unique_lock<mutex> lck(m);

        // Espera si no hay datos para procesar
        cons.wait(lck, [] { return !buffer_trafico.empty(); });

        int dato = buffer_trafico.front();
        buffer_trafico.pop();
        
        cout << "   [ANALISIS] Procesando datos del SIGET... Flujo actual: " << dato << ". Ajustando semaforos." << endl;

        prod.notify_one(); 
        lck.unlock();

        this_thread::sleep_for(chrono::milliseconds(1200));
    }
}

int main() {
    cout << "--- INICIANDO SIMULACION DE CONCURRENCIA SIGET ---" << endl;
    cout << "Escenario: Productor-Consumidor (Sensores y Analista)" << endl;
    cout << "----------------------------------------------------" << endl;

    // Ejecución concurrente de 3 hilos
    thread s1(sensor_trafico, 1);
    thread s2(sensor_trafico, 2);
    thread analista(modulo_analisis);

    s1.join();
    s2.join();
    analista.join();

    cout << "----------------------------------------------------" << endl;
    cout << "--- SIMULACION FINALIZADA CON EXITO ---" << endl;

    return 0;
}
