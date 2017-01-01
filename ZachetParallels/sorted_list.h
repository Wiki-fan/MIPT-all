#pragma once

#include <mutex>

#pragma once
#include "../stdafx.h"

class sorted_list {
    struct Node {
        int data;
        std::mutex mutex;
        Node* next = nullptr;

        Node(int data) :data(data) {}
    };

    Node* head;
public:

    ~sorted_list() {
        Node* cur = head;
        while (cur != nullptr) {
            Node* toDelete = cur;
            cur = cur->next;
            delete toDelete;
        }
    }

    // По убыванию
    void insert( int x ) {
        if (head == nullptr) {
            head = new Node(x);
        }

        Node* cur = head;
        while(true) {
            std::unique_lock<std::mutex> lock(cur->mutex);
            if  (cur->next != 0) {
                if (cur->data <= x) {


            }
        }
        }


    }
    bool erase( int x ) {

    }
    bool has( int x ) const;
};
