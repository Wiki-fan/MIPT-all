#pragma once

#include <atomic>
#include <memory>

template<typename T>
class QueueWithoutSmr {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node(const std::shared_ptr<T>& data) :
                data(data), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    QueueWithoutSmr() :
            head(new Node(nullptr)), tail(head.load()) {}
    QueueWithoutSmr(const QueueWithoutSmr& other) = delete;
    QueueWithoutSmr& operator=(const QueueWithoutSmr& other)= delete;

    //void push(T new_value) {
    void enqueue(T new_value) {
        //  Заводим новую вершину с данными.
        std::shared_ptr<T> newData(std::make_shared<T>(new_value));
        Node* newNode = new Node(newData);

        // Старый хвост.
        Node* currTail;

        while (true) {
            // Подгружаем себе данные о хвосте.
            currTail = tail.load();
            Node* currTailNext = tail.load()->next;
            // Если получили, что хвост действительно последний
            if (!currTailNext) {
                // Долбимся CAS в tail->next и пытаемся записать newNode.
                if (tail.load()->next.compare_exchange_strong(currTailNext, newNode)) { // 1
                    break;
                }
            } else {
                // Понимаем, что какой-то другой поток поменял next у хвоста, но не поменял tail на новый хвост
                // (сделал 1, но не 2)
                // Помогаем ему.
                tail.compare_exchange_strong(currTail, currTailNext);
            }
        }
        // Меняем хвост на новый.
        tail.compare_exchange_strong(currTail, newNode); // 2
    }

    //std::shared_ptr<T> pop() {
    bool dequeue(T& newValue) {
        while (true) {
            // Подгружаем голову и хвост.
            Node* currHead = head.load();
            Node* currTail = tail.load();
            Node* currHeadNext = currHead->next.load();
            if (currHead == currTail) {
                if (!currHeadNext) {
                    // Это происходит, если в очереди лишь фиктивная вершина. Просто возвращаем 0.
                    //return nullptr;
                    return false;
                } else {
                    // Какой-то поток ещё не успел переставить в процессе pop() голову на новую.
                    // Помогаем ему.
                    tail.compare_exchange_strong(currHead, currHeadNext);
                }
            } else {
                // Долбимся CAS и пытаемся поменять голову на следующую.
                if (head.compare_exchange_strong(currHead, currHeadNext)) {
                    std::shared_ptr<T> item = currHeadNext->data;
                    // Можем вернуть.
                    // Сюда надо впихнуть SMR
                    //return item;
                    newValue = *item;
                    return true;
                }
            }
        }
    }

};
