#include <iostream>
#include <future>
#include <thread>

void task_productor(std::promise<std::string> prm, std::future<std::string> fut) {
    std::cout << "Procesando... \n";
    std::this_thread::sleep_for(std::chrono::seconds (1));
    prm.set_value("Como estas");
    auto respuesta = fut.get();
    std::cout << "La respuesta recibido es: " << respuesta << std::endl;
}

void task_consumidor(std::future<std::string> fut, std::promise<std::string> prm) {
    auto message = fut.get();
    std::cout << "El mensaje recibido es: " << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds (1));
    prm.set_value("Todo bien");
}

int main() {
    std::promise<std::string> prm;
    std::promise<std::string> prm2;
    std::future<std::string> fut = prm.get_future();
    std::future<std::string> fut2 = prm2.get_future();

    std::jthread hilo_productor(task_productor, std::move(prm), std::move(fut2));
    std::jthread hilo_consumidor(task_consumidor, std::move(fut), std::move(prm2));

    hilo_productor.join();
    hilo_consumidor.join();
    return 0;
}
