#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
#include <fstream>

// Cấu trúc đối tượng Flashcard
struct Flashcard {
    std::string word;
    std::string definition;
    std::string example;
    int difficulty = 0; 
};

// --- Cây tìm kiếm nhị phân (BST) ---
struct BSTNode {
    Flashcard data;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

void insertBST(BSTNode*& root, Flashcard card);
BSTNode* searchBST(BSTNode* root, std::string word);
BSTNode* findMin(BSTNode* root);
BSTNode* deleteBST(BSTNode* root, std::string word, bool& success);

// --- Hàng đợi (Queue) dùng cho Phiên học ---
struct QueueNode {
    Flashcard data;
    QueueNode* next = nullptr;
};

struct Queue {
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;

    bool isEmpty();
    void enqueue(Flashcard card);
    Flashcard dequeue();
    void clear();
};

// --- Ngăn xếp (Stack) dùng cho Undo ---
struct StackNode {
    Flashcard data;
    StackNode* next = nullptr;
};

struct Stack {
    StackNode* topNode = nullptr;

    bool isEmpty();
    void push(Flashcard card);
    Flashcard pop();
    void clear();
};

// --- Hàng đợi ưu tiên (Priority Queue) ---
struct PQNode {
    Flashcard data;
    PQNode* next = nullptr;
};

struct PriorityQueue {
    PQNode* head = nullptr;

    bool isEmpty();
    void push(Flashcard card);
    Flashcard pop();
};

// --- Các hàm chức năng bổ trợ Hệ thống ---
void saveInOrder(BSTNode* root, std::ofstream& file);
void loadFromFile(BSTNode*& root, const std::string& filename);
void populatePQFromBST(BSTNode* root, PriorityQueue& pq);
void collectStats(BSTNode* root, int& total, int& wrongCount, Flashcard& mostForgotten);

#endif
