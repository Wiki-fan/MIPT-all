#pragma once

#include <atomic>
#include <thread>

unsigned const MAX_HAZARD_POINTERS = 100;
unsigned const HAZARD_POINTERS_FOR_THREAD = 2;

struct HazardPointer {
    std::atomic<std::thread::id> id;
    std::array<std::atomic<void*>, HAZARD_POINTERS_FOR_THREAD> pointers;
};

// Глобальный массив из всех возможных указателей опасности
HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner {
public:
    HPOwner(const HPOwner&) = delete;
    HPOwner operator=(const HPOwner&) = delete;

    HPOwner() : hp(nullptr) {
        // Просто ищем ближайший неинициализированный и занимаем его.
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
            std::thread::id oldId;
            if (g_hazardPointers[i].id.compare_exchange_strong(
                    oldId, std::this_thread::get_id())) {
                hp = &g_hazardPointers[i];
                break;
            }
        }
        if (!hp) {
            throw std::runtime_error("No hazard pointers available");
        }
    }

    // void*, т.к. мы хотим хранить указатели опасности для любого типа в одном массиве.
    std::array<std::atomic<void*>, HAZARD_POINTERS_FOR_THREAD>* getPointers() {
        return &hp->pointers;
    }

    ~HPOwner() {
        for (auto&& ptr:hp->pointers) {
            ptr.store(nullptr);
        }
        hp->id.store(std::thread::id());
    }

private:
    HazardPointer* hp;
};

// public
// Позволяет получить указатель опасности текущего потока.
std::array<std::atomic<void*>, HAZARD_POINTERS_FOR_THREAD>* getHazardPointersForCurrentThread() {
    thread_local static HPOwner hazard;
    return hazard.getPointers();
}

// Проверяет, есть ли у кого указатель опасности, выставленный на эту вершину.
bool outstandingHazardPointersFor(void* p) {
    for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
        for (unsigned j = 0; j < HAZARD_POINTERS_FOR_THREAD; ++j) {
            if (g_hazardPointers[i].pointers.at(j) == p) {
                return true;
            }
        }
    }
    return false;
}

// Тупо удаляет
template<typename T>
void doDelete(void* p) {
    delete static_cast<T*>(p);
}

struct DataToReclaim {
    void* data;
    std::function<void(void*)> deleter;
    DataToReclaim* next;

    template<typename T>
    DataToReclaim(T* p)
            : data(p), deleter(&doDelete<T>), next(nullptr) {}

    ~DataToReclaim() {
        deleter(data);
    }

};

// Список вершин, которые теоретически можно удалить.
std::atomic<DataToReclaim*> nodesToReclaim;

// Вставка в начало
void addToReclaimList(DataToReclaim* node) {
    node->next = nodesToReclaim.load();
    while (!nodesToReclaim.compare_exchange_weak(node->next, node)) {}
}

// public
template<typename T>
void reclaimLater(T* data) {
    addToReclaimList(new DataToReclaim(data));
}

// public
void deleteNodesWithNoHazards() {
    DataToReclaim* current = nodesToReclaim.exchange(nullptr);
    while (current) {
        DataToReclaim* const next = current->next;
        if (!outstandingHazardPointersFor(current->data)) {
            //std::cout <<"Reclaim delete\n";
            delete current;
        } else {
            addToReclaimList(current);
        }
        current = next;
    }
}
