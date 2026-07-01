#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>

// Cấu trúc đối tượng Flashcard
struct Flashcard {
    std::string word;
    std::string definition;
    std::string example;
    int difficulty = 0; // Số lần trả lời sai
};

// --- Cây tìm kiếm nhị phân (BST) ---
struct BSTNode {
    Flashcard data;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

// --- Hàng đợi (Queue) dùng cho Phiên học ---
struct QueueNode {
    Flashcard data;
    QueueNode* next = nullptr;
};

struct Queue {
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;

    bool isEmpty() { return front == nullptr; }

    void enqueue(Flashcard card) {
        QueueNode* temp = new QueueNode{card, nullptr};
        if (rear == nullptr) {
            front = rear = temp;
            return;
        }
        rear->next = temp;
        rear = temp;
    }

    Flashcard dequeue() {
        if (isEmpty()) return Flashcard();
        QueueNode* temp = front;
        Flashcard card = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return card;
    }

    void clear() {
        while (!isEmpty()) dequeue();
    }
};

// --- Ngăn xếp (Stack) dùng cho Undo ---
struct StackNode {
    Flashcard data;
    StackNode* next = nullptr;
};

struct Stack {
    StackNode* topNode = nullptr;

    bool isEmpty() { return topNode == nullptr; }

    void push(Flashcard card) {
        StackNode* temp = new StackNode{card, topNode};
        topNode = temp;
    }

    Flashcard pop() {
        if (isEmpty()) return Flashcard();
        StackNode* temp = topNode;
        Flashcard card = temp->data;
        topNode = topNode->next;
        delete temp;
        return card;
    }

    void clear() {
        while (!isEmpty()) pop();
    }
};

// --- Hàng đợi ưu tiên (Priority Queue) dùng cho Spaced Repetition ---
struct PQNode {
    Flashcard data;
    PQNode* next = nullptr;
};

struct PriorityQueue {
    PQNode* head = nullptr;

    bool isEmpty() { return head == nullptr; }

    void push(Flashcard card) {
        PQNode* temp = new PQNode{card, nullptr};
        if (head == nullptr || card.difficulty > head->data.difficulty) {
            temp->next = head;
            head = temp;
        } else {
            PQNode* current = head;
            while (current->next != nullptr && current->next->data.difficulty >= card.difficulty) {
                current = current->next;
            }
            temp->next = current->next;
            current->next = temp;
        }
    }

    Flashcard pop() {
        if (isEmpty()) return Flashcard();
        PQNode* temp = head;
        Flashcard card = temp->data;
        head = head->next;
        delete temp;
        return card;
    }
};

#endif
