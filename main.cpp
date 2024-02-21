#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Функция для вычисления факториала числа
unsigned long long calculateFactorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Функция, которую будет выполнять каждый поток
void processNumbers(int start, int end, int threadNum, std::vector<unsigned long long>& results) {
    for (int i = start; i <= end; ++i) {
        results[i] = calculateFactorial(i);
    }
    std::cout << "Thread " << threadNum << " completed its task." << std::endl;
}

int main() {
    int numThreads;
    while (true) {
        std::cout << "Enter the number of threads to use: ";
        // Проверка на корректный ввод, обработка ошибок
        if (!(std::cin >> numThreads) || numThreads <= 0) {
            std::cerr << "Invalid input for the number of threads. Please enter a positive integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    int numberToCalculate;
    while (true) {
        std::cout << "Enter the number to calculate factorial for: ";
        // Проверка на корректный ввод, обработка ошибок
        if (!(std::cin >> numberToCalculate) || numberToCalculate < 0) {
            std::cerr << "Invalid input for the number to calculate factorial. Please enter a non-negative integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

 
    std::vector<std::thread> taskThreads(numThreads);
    std::vector<unsigned long long> resultsArray(numberToCalculate + 1, 0);

    int chunkSize = numberToCalculate / numThreads;

    auto startTime = std::chrono::high_resolution_clock::now();

    // Создание и запуск потоков
    for (int i = 0; i < numThreads; ++i) {
        int begin = i * chunkSize + 1;
        int finish = (i == numThreads - 1) ? numberToCalculate : (i + 1) * chunkSize;
        taskThreads[i] = std::thread(processNumbers, begin, finish, i+1, std::ref(resultsArray));
    }

    // Ожидание завершения всех потоков и проверка на успешное присоединение
    for (int i = 0; i < numThreads; ++i) {
        if (taskThreads[i].joinable()) {
            taskThreads[i].join();
        } else {
            std::cerr << "Thread " << i+1 << " cannot be joined." << std::endl;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    // Вывод результатов
    std::cout << "Factorial of " << numberToCalculate << " is: " << resultsArray[numberToCalculate] << std::endl;
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}
