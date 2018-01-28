#pragma once

#include <atomic>
#include <memory>
#include <iostream>
#include "hazard_pointer.h"

#define CONTEST

#ifdef CONTEST
#define QueueWithHazardPointers lock_free_queue
#endif

template<typename T>
class QueueWithHazardPointers {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node(const std::shared_ptr<T>& data) :
                data(data), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

    static void deleteNodes(Node* nodes) {
        while (nodes) {
            Node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

public:
    QueueWithHazardPointers() :
            head(new Node(nullptr)), tail(head.load()) {}
    QueueWithHazardPointers(const QueueWithHazardPointers& other) = delete;
    QueueWithHazardPointers& operator=(const QueueWithHazardPointers& other)= delete;

    ~QueueWithHazardPointers() {
        //std::cout << "destructor" <<std::endl;
        deleteNodes(head.load());
    }

#ifdef CONTEST
    void enqueue(T new_value) {
#else
    void push(T new_value) {
#endif
        //std::cout <<"push" <<std::endl;
        //  Заводим новую вершину с данными.
        std::shared_ptr<T> newData(std::make_shared<T>(new_value));
        Node* newNode = new Node(newData);

        // Старый хвост.
        Node* currTail;
        auto hp = getHazardPointersForCurrentThread();

        while (true) {
            // Подгружаем себе данные о хвосте.
            currTail = tail.load();
            (*hp)[0].store(currTail);
            // Если данные изменились в процессе установки указателя опасности, повторяем.
            if (currTail != tail.load()) {
                continue;
            }

            Node* currTailNext = tail.load()->next;
            // Если получили, что хвост действительно последний
            if (!currTailNext) {
                // Долбимся CAS в tail->next и пытаемся записать newNode.
                if (tail.load()->next.compare_exchange_strong(currTailNext, newNode)) { // 1
                    break;
                }
            } else {
                // Понимаем, что какой-то другой поток поменял next у хвоста, но не поменял tail на новый хвост
                // (сделал 1, но не 2). Помогаем ему.
                tail.compare_exchange_strong(currTail, currTailNext);
            }
        }
        // Меняем хвост на новый.
        tail.compare_exchange_strong(currTail, newNode); // 2
        (*hp)[0].store(nullptr);
    }

#ifdef CONTEST
    bool dequeue(T& newValue) {
#else
    std::shared_ptr<T> pop() {
#endif
        auto hp = getHazardPointersForCurrentThread();
        while (true) {
            // Подгружаем голову и хвост.
            Node* currHead = head.load();
            (*hp)[0].store(currHead);
            if (currHead != head.load()) {
                continue;
            }

            Node* currTail = tail.load();
            Node* currHeadNext = currHead->next.load();
            (*hp)[1].store(currHeadNext);
            if (currHeadNext != currHead->next.load()) {
                continue;
            }

            if (currHead == currTail) {
                if (currHeadNext == nullptr) {
                    // Это происходит, если в очереди лишь фиктивная вершина. Просто возвращаем 0.
#ifdef CONTEST
                    (*hp)[0].store(nullptr);
                    return false;
#else
                    return nullptr;
#endif
                } else {
                    // Какой-то поток ещё не успел переставить в процессе pop() голову на новую.
                    // Помогаем ему.
                    tail.compare_exchange_strong(currHead, currHeadNext);
                }
            } else {
                // Долбимся CAS и пытаемся поменять голову на следующую.
                if (head.compare_exchange_strong(currHead, currHeadNext)) {
                    // head указывает же на фиктивную вершину
                    std::shared_ptr<T> item(nullptr);
                    item.swap(currHeadNext->data);
                    // Можем вернуть.
                    (*hp)[0] = nullptr;
                    (*hp)[1] = nullptr;
                    if (outstandingHazardPointersFor(currHead)) {
                        reclaimLater(currHead);
                    } else {
                        //std::cout <<"in-place delete\n";
                        delete currHead;
                    }
                    deleteNodesWithNoHazards();
#ifdef CONTEST
                    newValue = *item;
                    return true;
#else
                    return item;
#endif
                }
            }
        }
    }

};

#ifdef CONTEST
#undef QueueWithHazardPointers
#endif
