#pragma once

#include <atomic>
#include <memory>
#include <iostream>

//#define CONTEST

template<typename T>
class QueueWithIdleDelete {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node(const std::shared_ptr<T>& data) :
                data(data), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    std::atomic<unsigned> threadsInPop;
    std::atomic<Node*> toBeDeleted;

public:
    QueueWithIdleDelete() :
            head(new Node(nullptr)), tail(head.load()), threadsInPop(0), toBeDeleted(nullptr) {}
    QueueWithIdleDelete(const QueueWithIdleDelete& other) = delete;
    QueueWithIdleDelete& operator=(const QueueWithIdleDelete& other)= delete;

    ~QueueWithIdleDelete() {
        deleteNodes(toBeDeleted.load());
        delete head.load();
    }

#ifdef CONTEST
    void enqueue(T new_value) {
#else
        void push(T new_value) {
#endif
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

#ifdef CONTEST
    bool dequeue(T& newValue) {
#else
        std::shared_ptr<T> pop() {
#endif
        while (true) {
            // Подгружаем голову и хвост.
            Node* currHead = head.load();
            Node* currTail = tail.load();
            Node* currHeadNext = currHead->next.load();
            if (currHead == currTail) {
                if (!currHeadNext) {
                    // Это происходит, если в очереди лишь фиктивная вершина. Просто возвращаем 0.
#ifdef CONTEST
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
                    // Сюда надо впихнуть SMR

                    tryReclaim(currHead);
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

    static void deleteNodes(Node* nodes) {
        while (nodes) {
            Node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void tryReclaim(Node* oldHead) {
        if (threadsInPop == 1) {
            // Подгружаем себе список, меняя его на nullptr.
            Node* nodesToDelete = toBeDeleted.exchange(nullptr);
            // Если мы при этом остались единственные в pop, то можем поудалять. Никто больше в эти вершины не влезет.
            if (!--threadsInPop) {
                deleteNodes(nodesToDelete);
            } else if (nodesToDelete) {
                // Другие потоки успели влезть в pop
                // Если захапали ненулевой указатель, то проматываемся до конца и подцепляем
                chainPendingNodes(nodesToDelete);
            }
            // Эту вершину всегда можем удалить, если единственные в pop(), даже если другие не можем.
            delete oldHead;
        } else {
            // Если мы - не единственный поток в pop(), удалять небезопасно, просто добавляем вершину в конец списка на удаление.
            chainPendingNode(oldHead);
            --threadsInPop;
        }
    }

    // Впихнуть в список на удаление вершины
    void chainPendingNodes(Node* nodes) {
        // Промотаться до конца списка
        Node* last = nodes;
        while (Node* const next = last->next) {
            last = next;
        }
        chainPendingNodes(nodes, last);
    }

    // Подцепить к списку
    void chainPendingNodes(Node* first, Node* last) {
        Node* newLastNext;
        // Подцепляем toBeDeleted к концу имеющегося вписка.
        // Долбимся CAS, пока не разместим начало нового списка в toBeDeleted.
        // Мы считаем, что в newLastNext хранится toBeDeleted - мы так и записывали - но другие потоки могли поменять это.
        do {
            newLastNext = toBeDeleted.load();
        } while (!toBeDeleted.compare_exchange_strong(newLastNext, first));
        last->next.store(newLastNext);

    }

    void chainPendingNode(Node* node) {
        chainPendingNodes(node, node);
    }

};
