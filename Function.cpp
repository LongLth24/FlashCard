#include "Flashcard.h"
#include <iostream>
#include <sstream>

// ==========================================
// CÀI ĐẶT CÂY TÌM KIẾM NHỊ PHÂN (BST)
// ==========================================
void insertBST(BSTNode*& root, Flashcard card) {
    if (root == nullptr) {
        root = new BSTNode{card, nullptr, nullptr};
        return;
    }
    if (card.word < root->data.word) {
        insertBST(root->left, card);
    } else if (card.word > root->data.word) {
        insertBST(root->right, card);
    } else {
        root->data = card; 
    }
}

BSTNode* searchBST(BSTNode* root, std::string word) {
    if (root == nullptr || root->data.word == word) return root;
    if (word < root->data.word) return searchBST(root->left, word);
    return searchBST(root->right, word);
}

BSTNode* findMin(BSTNode* root) {
    while (root && root->left != nullptr) root = root->left;
    return root;
}

BSTNode* deleteBST(BSTNode* root, std::string word, bool& success) {
    if (root == nullptr) return root;
    if (word < root->data.word) {
        root->left = deleteBST(root->left, word, success);
    } else if (word > root->data.word) {
        root->right = deleteBST(root->right, word, success);
    } else {
        success = true;
        if (root->left == nullptr) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        BSTNode* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data.word, success);
    }
    return root;
}

// ==========================================
// CÀI ĐẶT HÀNG ĐỢI (QUEUE)
// ==========================================
bool Queue::isEmpty() { return front == nullptr; }

void Queue::enqueue(Flashcard card) {
    QueueNode* temp = new QueueNode{card, nullptr};
    if (rear == nullptr) {
        front = rear = temp;
        return;
    }
    rear->next = temp;
    rear = temp;
}

Flashcard Queue::dequeue() {
    if (isEmpty()) return Flashcard();
    QueueNode* temp = front;
    Flashcard card = temp->data;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    return card;
}

void Queue::clear() {
    while (!isEmpty()) dequeue();
}

// ==========================================
// CÀI ĐẶT NGĂN XẾP (STACK)
// ==========================================
bool Stack::isEmpty() { return topNode == nullptr; }

void Stack::push(Flashcard card) {
    StackNode* temp = new StackNode{card, topNode};
    topNode = temp;
}

Flashcard Stack::pop() {
    if (isEmpty()) return Flashcard();
    StackNode* temp = topNode;
    Flashcard card = temp->data;
    topNode = topNode->next;
    delete temp;
    return card;
}

void Stack::clear() {
    while (!isEmpty()) pop();
}

// ==========================================
// CÀI ĐẶT HÀNG ĐỢI ƯU TIÊN (PRIORITY QUEUE)
// ==========================================
bool PriorityQueue::isEmpty() { return head == nullptr; }

void PriorityQueue::push(Flashcard card) {
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

Flashcard PriorityQueue::pop() {
    if (isEmpty()) return Flashcard();
    PQNode* temp = head;
    Flashcard card = temp->data;
    head = head->next;
    delete temp;
    return card;
}

// ==========================================
// CÀI ĐẶT CÁC HÀM XỬ LÝ HỆ THỐNG
// ==========================================
void saveInOrder(BSTNode* root, std::ofstream& file) {
    if (root == nullptr) return;
    saveInOrder(root->left, file);
    file << root->data.word << "|" << root->data.definition << "|" 
         << root->data.example << "|" << root->data.difficulty << "\n";
    saveInOrder(root->right, file);
}

void loadFromFile(BSTNode*& root, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Flashcard card;
        std::string diffStr;
        std::getline(ss, card.word, '|');
        std::getline(ss, card.definition, '|');
        std::getline(ss, card.example, '|');
        std::getline(ss, diffStr, '|');
        if (!diffStr.empty()) card.difficulty = std::stoi(diffStr);
        insertBST(root, card);
    }
    file.close();
}

void populatePQFromBST(BSTNode* root, PriorityQueue& pq) {
    if (root == nullptr) return;
    populatePQFromBST(root->left, pq);
    pq.push(root->data);
    populatePQFromBST(root->right, pq);
}

void collectStats(BSTNode* root, int& total, int& wrongCount, Flashcard& mostForgotten) {
    if (root == nullptr) return;
    collectStats(root->left, total, wrongCount, mostForgotten);
    total++;
    if (root->data.difficulty > 0) wrongCount++;
    if (root->data.difficulty > mostForgotten.difficulty) {
        mostForgotten = root->data;
    }
    collectStats(root->right, total, wrongCount, mostForgotten);
}
