#pragma once
#include "semaphore.h"
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

// Реализация с условными переменными.
class RobotWithCondvars {
public:
    RobotWithCondvars() {}

    void start() {
        std::thread t0(&RobotWithCondvars::go, this, 0);
        std::thread t1(&RobotWithCondvars::go, this, 1);
        t0.join();
        t1.join();
    }

private:
    bool current = 0; // Какой ногой нужно пойти сейчас. Не нужно делать atomic, т.к. все обращения выполняются под мьютексом.
    std::mutex m;
    std::condition_variable cv[2];

    void go(int leg) {
        std::unique_lock<std::mutex> lock(m);
        for (int i = 0; i < 1000; ++i) { // Для примера взят такой цикл. С успехом заменяется на while(true), если надо по условию.
            // Ждём, пока не наша очередь ходить. Если начинаем выполняться, то включается действие мьютекса, и все операции выполняются под ним.
            cv[leg].wait(lock, [&] { return current == leg; });
            if (leg == 0) step("left"); else step("right");
            current = 1 - leg; // Меняем текущую ногу.
            cv[1 - leg].notify_one(); // Уведомляем другую, что её очередь. Заметим, что после этого мы в следующей итерации встанем в ожидание на cv[leg],
            // так как условие его будет ложно. И в этом состоянии будем пребывать, пока другой поток не вызовет notify.
        }

    }

    void step(std::string s)
    {
        std::cout << s << std::endl;
        std::cout.flush(); // Чтобы избежать рассинхронизации вывода.
    }
};

//Реализация с семафорами.
class RobotWithSemaphores {
public:
    // Это - начальные значения переменной counter семафора. Так удобнее.
    RobotWithSemaphores():sem1(1), sem2(0) {};

    void start() {
        std::thread t0(&RobotWithSemaphores::go, this, 0);
        std::thread t1(&RobotWithSemaphores::go, this, 1);
        t0.join();
        t1.join();
    }

private:
    Semaphore sem1, sem2;

    void go(int leg) {
        if(leg == 0) {
            for (int i = 0; i < 1000; ++i) {
                sem1.wait(); // 1 Заходим сюда, так как sem1 по умолчанию 1.
                step("left");
                sem2.post(); // 2 Теперь другой поток сможет зайти.
                sem1.wait(); // 3 Начинаем ждать.
                sem1.post(); // 7 Так как после ожидания в пункте 3 значение семафора уменьшилось, вернём его на место, то есть в 1.
            }
            sem2.post(); // Разрешаем второму исполниться последний раз, ведь после того, как первый закончит исполняться,
            // семафор второго будет заблокирован.
        } else if (leg == 1) {
            for (int i = 0; i < 1000; ++i) {
                sem2.wait(); // 4 Ждём здесь, пока первый поток не увеличит sem2. Он во время этого либо уже будет ждать
                // на sem1, либо начнёт это делать вскоре.
                step("right");
                sem1.post(); // 5 Увеличиваем sem1, теперь он 1. Первый поток теперь может исполняться.
                sem2.wait(); // 6 Становимся ждать.
                sem2.post(); // 8 Вернём на место уменьшившееся после выполнения 6 значение семафора, теперь он 0.
                // Сохранился инвариант, бывший в начале задачи.
            }
        }
    }

    void step(std::string s)
    {
        std::cout << s << std::endl;
        std::cout.flush(); // Чтобы избежать рассинхронизации вывода.
    }
};
