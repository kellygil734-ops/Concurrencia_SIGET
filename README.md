# Simulación de Concurrencia - SIGET

Este proyecto desarrolla una solución técnica para la gestión de procesos concurrentes en el **Sistema Inteligente de Gestión de Tráfico (SIGET)**, aplicando modelos de sincronización de sistemas operativos.

## Problema Seleccionado
**Productor-Consumidor (Adaptado):**
Se simula el flujo de información donde:
* **Productores:** Sensores de tráfico distribuidos que envían datos de flujo vehicular a un búfer compartido.
* **Consumidor:** Un módulo de análisis que procesa los datos para la toma de decisiones (ajuste de semáforos).

## Tecnologías y Mecanismos de Concurrencia
La solución está desarrollada en **C++** utilizando las siguientes herramientas para garantizar la fiabilidad:

* **Multithreading (`std::thread`):** Ejecución simultánea de al menos tres hilos (2 sensores y 1 analista).
* **Exclusión Mutua (`std::mutex`):** Garantiza que solo un hilo acceda al búfer de datos a la vez, evitando asi la corrupción de la información.
* **Sincronización (`std::condition_variable`):** Gestiona la espera activa. El analista duerme si no hay datos, y los sensores esperan si el búfer está lleno (para prevención de desbordamiento).
* **Prevención de Bloqueos:** Implementación de lógica de notificación para evitar *deadlocks* (interbloqueos).

## Instrucciones de Ejecución
Para compilar y ejecutar la simulación en un entorno Linux/GDB:

1. **Compilación:**
   ```bash
   g++ -pthread main.cpp -o siget_concurrencia
