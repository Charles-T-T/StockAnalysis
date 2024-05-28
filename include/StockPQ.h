#pragma once

#include "Stock.h"
#include <vector>
#include <utility>

/* 基于最小堆实现一个优先队列 */
template<typename T, typename Compare>
class MyPQ {
private:
    std::vector<T> heap;
    Compare cmp;

    // 上浮操作
    void HeapUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (cmp(heap[parent], heap[idx])) {
                std::swap(heap[parent], heap[idx]);
                idx = parent;
            } else {
                break;
            }
        }
    }

    // 下沉操作
    void HeapDown(int idx) {
        int n = heap.size();
        while (2 * idx + 1 < n) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int maxChild = left;
            if (right < n && cmp(heap[left], heap[right])) {
                maxChild = right;
            }
            if (cmp(heap[idx], heap[maxChild])) {
                std::swap(heap[idx], heap[maxChild]);
                idx = maxChild;
            } else {
                break;
            }
        }
    }

public:
    MyPQ(const Compare& comp = Compare()) : cmp(comp) {}

    // 插入元素
    void push(const T& value) {
        heap.push_back(value);
        HeapUp(heap.size() - 1);
    }

    // 弹出元素
    void pop() {
        if (heap.empty()) {
            return;
        }
        heap[0] = heap.back();
        heap.pop_back();
        HeapDown(0);
    }

    // 获取堆顶元素
    const T& top() const {
        return heap.front();
    }

    // 判断堆是否为空
    bool empty() const {
        return heap.empty();
    }

    // 获取堆的大小
    std::size_t size() const {
        return heap.size();
    }
};
