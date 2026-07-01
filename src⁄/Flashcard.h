#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
#include <fstream>

struct Flashcard {
    std::string word;
    std::string definition;
    std::string example;
    int difficulty = 0; 
};

// --- Binary Search Tree (BST) ---
struct BSTNode {
    Flashcard data;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

void insertBST(BSTNode*& root, Flashcard card);
BSTNode* searchBST(BSTNode* root, std::string word);
BSTNode* findMin(BSTNode* root);
BSTNode* deleteBST(BSTNode* root, std::string word, bool& success);
void clearBST(BSTNode*& root);

// --- Queue ---
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

// --- Stack ---
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

// --- Priority Queue (Linked List dựa trên thuộc tính difficulty) ---
struct PQNode {
    Flashcard data;
    PQNode* next = nullptr;
};

struct PriorityQueue {
    PQNode* head = nullptr;

    bool isEmpty();
    void push(Flashcard card);
    Flashcard pop();
    void clear();
};

// --- Hệ thống quản lý và Tiện ích ---
void saveInOrder(BSTNode* root, std::ofstream& file);
void loadFromFile(BSTNode*& root, const std::string& filename);
void populatePQFromBST(BSTNode* root, PriorityQueue& pq);
void collectStats(BSTNode* root, int& total, int& wrongCount, Flashcard& mostForgotten);
void runAutomatedTestCases(BSTNode*& dictionary);

#endif
