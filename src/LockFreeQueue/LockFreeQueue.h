//Michael and Scott lock free queue
#ifndef __LockFreeQueue_h__
#define __LockFreeQueue_h__

#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <limits>

template <typename T>
class LockFreeQueue {
public:
    LockFreeQueue() {
        Node* dummy = new Node;
        head.store(dummy);
        tail.store(dummy);
        size.store(0);
        stopped.store(false);
    }

    ~LockFreeQueue() {
        while (Node* old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    bool push(T value, int64_t id = -1) {
        // if (stopped.load()) {
        //     return false;
        // }
        Node* new_node = new Node(value, id);
        Node* old_tail = nullptr;
        while (true) {
            old_tail = tail.load();
            if(id>=0){
                if(id != old_tail->id+1){
                    if((id != 0) || (old_tail->id != maxID)){
                        delete new_node;
                        return false;
                    }
                }
            }
            Node* next = old_tail->next.load();
            if (old_tail == tail.load()) {
                if (next == nullptr) {
                    if (old_tail->next.compare_exchange_weak(next, new_node)) {
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(old_tail, next);
                }
            }
        }
        
        tail.compare_exchange_weak(old_tail, new_node);
        size.fetch_add(1, std::memory_order_relaxed); 
        // cout<<"file "<<id<<" is pushed as id :"<<tail.load()->id<<endl;
        return true;
    }

    bool pop(T& value) {
        Node* old_head = nullptr;
        while (true) {
            old_head = head.load();
            Node* old_tail = tail.load();
            Node* next = old_head->next.load();
            if (old_head == head.load()) {
                if (old_head == old_tail) {
                    if (next == nullptr) {
                        return false;
                    }
                    tail.compare_exchange_weak(old_tail, next);
                } else {
                    if (head.compare_exchange_weak(old_head, next)) {
                        value = std::move(next->value);
                        size.fetch_sub(1, std::memory_order_relaxed);
                        break;
                    }
                }
            }
        }
        delete old_head;
        return true;
    }

    size_t getSize() const {
        return size.load(std::memory_order_relaxed);
    }
    void stop() {
        stopped.store(true);
    }
    bool isStopped() const {
        return stopped.load();
    }

private:
    struct Node {
        std::atomic<Node*> next;
        T value;
        int64_t id;

        Node() : next(nullptr), id(-1) {}
        Node(T val, int64_t id) : next(nullptr), value(std::move(val)), id(id) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    std::atomic<size_t> size; 
    int64_t maxID = std::numeric_limits<int64_t>::max();
    std::atomic<bool> stopped; 
};
#endif
