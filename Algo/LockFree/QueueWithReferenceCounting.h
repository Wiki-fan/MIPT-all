#include <atomic>
#include <memory>
#include <iostream>

// Не работает
template<typename T>
class QueueWithReferenceCounting {
private:
    struct Node;
    struct CountedNodePtr {
        int external_count = 0;
        Node* ptr = nullptr;
    };

    struct NodeCounter {
        unsigned counter:30; // Просто внутренний счётчик "поколений".
        unsigned referenced:2; // Сколько внешних ссылок есть на эту вершину.
    };

    // Вершина содержит не только данные и указатель на следующую, но и счётчик.
    struct Node {
        std::atomic<T*> data;
        std::atomic<NodeCounter> counter;
        std::atomic<CountedNodePtr> next;

        Node() {
            NodeCounter new_count;
            new_count.counter=0;
            new_count.referenced=2;
            counter.store(new_count);
            next.store({0, nullptr});
        }

        Node(const std::shared_ptr<T>& item) {
            counter.store({0, 2});
            next.store({0, nullptr});
            data.store(item.get());
        }

        // Уменьшить количество counter. Если можно, удалить.
        void release_ref() {
            NodeCounter old_counter = counter.load();
            NodeCounter new_counter;
            do {
                new_counter = old_counter;
                --new_counter.counter;
            } while (!counter.compare_exchange_strong(old_counter, new_counter));
            // Удаляем вершину, если на неё ничего не ссылается.
            if (!new_counter.counter && !new_counter.referenced) {
                delete this;
            }
        }
    };

    // Увеличить количество ссылок извне у счётчика.
    static void increaseReferenceCount(std::atomic<CountedNodePtr>& counter, CountedNodePtr& old_counter) {
        CountedNodePtr new_counter;
        do {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!counter.compare_exchange_strong(old_counter, new_counter));
        old_counter.external_count = new_counter.external_count;
    }

    // Убрать внешнюю ссылку
    static void decreaseReferenceCount(CountedNodePtr& old_node_ptr) {
        Node* const ptr = old_node_ptr.ptr;
        int const count_increase = old_node_ptr.external_count - 2;
        NodeCounter old_counter = ptr->counter.load();
        NodeCounter new_counter;
        do {
            new_counter = old_counter;
            --new_counter.referenced;
            new_counter.counter += count_increase;
        } while (!ptr->counter.compare_exchange_strong(old_counter, new_counter));

        // Удаляем вершину, если на неё ничего не ссылается.
        if (!new_counter.counter && !new_counter.referenced) {
            delete ptr;
        }
    }

    // Просто выставляем новый tail и уменьшаем нужные счётчики.
    void setNewTail(CountedNodePtr& old_tail, CountedNodePtr const& new_tail) {
        Node* const current_tail_ptr = old_tail.ptr;
        // Правда ли, что здесь достаточно одного strong?
        while (!tail.compare_exchange_weak(old_tail, new_tail) && old_tail.ptr == current_tail_ptr) {}

        if (old_tail.ptr == current_tail_ptr) {
            decreaseReferenceCount(old_tail);
        } else {
            // Если хвост кто-то поменял тогда, когда мы хотели его поменять
            current_tail_ptr->release_ref();
        }
    }

    std::atomic<CountedNodePtr> head;
    std::atomic<CountedNodePtr> tail;

public:
    QueueWithReferenceCounting() {
        /*head.store({0, new Node(nullptr)});
        tail.store(head.load());*/
        CountedNodePtr node ({0, new Node(nullptr)});
        head.store(node);
        tail.store(node);
    }

    QueueWithReferenceCounting(const QueueWithReferenceCounting& other) = delete;
    QueueWithReferenceCounting& operator=(const QueueWithReferenceCounting& other) = delete;

    ~QueueWithReferenceCounting() {}

#ifdef CONTEST
    void enqueue(T new_value) {
#else
    void push(T new_value) {
#endif
        std::cout <<"push" <<std::endl;
        std::unique_ptr<T> new_data(std::make_unique<T>(new_value));
        CountedNodePtr new_next;
        new_next.ptr = new Node;
        new_next.external_count = 1;
        CountedNodePtr old_tail = tail.load();
        while(true) {
            increaseReferenceCount(tail, old_tail);
            T* old_data = nullptr;
            if (old_tail.ptr->data.compare_exchange_strong(
                    old_data, new_data.get())) {
                CountedNodePtr old_next = {0};
                if (!old_tail.ptr->next.compare_exchange_strong(
                        old_next, new_next)) {
                    delete new_next.ptr;
                    new_next = old_next;
                }
                setNewTail(old_tail, new_next);
                new_data.release();
                break;
            } else {
                CountedNodePtr old_next = {0};
                if (old_tail.ptr->next.compare_exchange_strong(
                        old_next, new_next)) {
                    old_next = new_next;
                    new_next.ptr = new Node;
                }
                setNewTail(old_tail, old_next);
            }
        }
    }

#ifdef CONTEST
    bool dequeue(T& newValue) {
#else
    std::shared_ptr<T> pop() {
#endif
        std::cout <<"pop" <<std::endl;
        CountedNodePtr old_head = head.load();
        while (true) {
            // Увеличиваем количество внешних ссылок, чтобы кто-нибудь не удалил.
            increaseReferenceCount(head, old_head);
            Node* const ptr = old_head.ptr;
            if (ptr == tail.load().ptr) {
                // Голова и хвост ссылаются на одну и ту же вершину - фиктивную.
#ifdef CONTEST
                return false;
#else
                //return nullptr;
                return std::unique_ptr<T>();
#endif
            }
            CountedNodePtr next = ptr->next.load();
            if (head.compare_exchange_strong(old_head, next)) {
                T* const res = ptr->data.exchange(nullptr);
                // Пытаемся удалить.
                decreaseReferenceCount(old_head);
#ifdef CONTEST
                newValue = *item;
                return true;
#else
                return std::unique_ptr<T>(res);
#endif
            }
            ptr->release_ref();
        }
    }
};
