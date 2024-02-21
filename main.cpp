#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Функция для вычисления факториала числа
unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Функция, которую будет выполнять каждый поток
void calculateFactorials(int start, int end, int num, std::vector<unsigned long long>& results) {
    for (int i = start; i <= end; ++i) {
        results[i] = factorial(i);
    }
    std::cout << "Thread " << num << " finished." << std::endl;
}

int main() {
    int numThreads;
    while (true) {
        std::cout << "Enter the number of threads: ";
        // Проверка на корректный ввод, обработка ошибок
        if (!(std::cin >> numThreads) || numThreads <= 0) {
            std::cerr << "Invalid input for number of threads. Please enter a positive integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    int number;
    while (true) {
        std::cout << "Enter the number to calculate factorial: ";
        // Проверка на корректный ввод, обработка ошибок
        if (!(std::cin >> number) || number < 0) {
            std::cerr << "Invalid input for number to calculate factorial. Please enter a non-negative integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

 
    std::vector<std::thread> threads(numThreads);
    std::vector<unsigned long long> results(number + 1, 0);

    int chunkSize = number / numThreads;

    auto start = std::chrono::high_resolution_clock::now();

    // Создание и запуск потоков
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize + 1;
        int end = (i == numThreads - 1) ? number : (i + 1) * chunkSize;
        threads[i] = std::thread(calculateFactorials, start, end, i+1, std::ref(results));
    }

    // Ожидание завершения всех потоков и проверка на успешное присоединение
    for (int i = 0; i < numThreads; ++i) {
        if (threads[i].joinable()) {
            threads[i].join();
        } else {
            std::cerr << "Thread " << i+1 << " is not joinable." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Вывод результатов
    std::cout << "Factorial of " << number << " is: " << results[number] << std::endl;
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}

