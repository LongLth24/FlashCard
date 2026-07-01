#include "Flashcard.h"
#include <iostream>
#include <sstream>

// --- BST Implementation ---
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

void clearBST(BSTNode*& root) {
    if (root == nullptr) return;
    clearBST(root->left);
    clearBST(root->right);
    delete root;
    root = nullptr;
}

// --- Queue Implementation ---
bool Queue::isEmpty() { return front == nullptr; }
void Queue::enqueue(Flashcard card) {
    QueueNode* temp = new QueueNode{card, nullptr};
    if (rear == nullptr) { front = rear = temp; return; }
    rear->next = temp; rear = temp;
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
void Queue::clear() { while (!isEmpty()) dequeue(); }

// --- Stack Implementation ---
bool Stack::isEmpty() { return topNode == nullptr; }
void Stack::push(Flashcard card) { topNode = new StackNode{card, topNode}; }
Flashcard Stack::pop() {
    if (isEmpty()) return Flashcard();
    StackNode* temp = topNode;
    Flashcard card = temp->data;
    topNode = topNode->next;
    delete temp;
    return card;
}
void Stack::clear() { while (!isEmpty()) pop(); }

// --- Priority Queue Implementation ---
bool PriorityQueue::isEmpty() { return head == nullptr; }
void PriorityQueue::push(Flashcard card) {
    PQNode* temp = new PQNode{card, nullptr};
    if (head == nullptr || card.difficulty > head->data.difficulty) {
        temp->next = head; head = temp;
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
void PriorityQueue::clear() { while (!isEmpty()) pop(); }

// --- I/O & Helpers ---
void saveInOrder(BSTNode* root, std::ofstream& file) {
    if (root == nullptr) return;
    saveInOrder(root->left, file);
    file << root->data.word << "|" << root->data.definition << "|" << root->data.example << "|" << root->data.difficulty << "\n";
    saveInOrder(root->right, file);
}

void loadFromFile(BSTNode*& root, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Flashcard card; std::string diffStr;
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
    if (root->data.difficulty > mostForgotten.difficulty) mostForgotten = root->data;
    collectStats(root->right, total, wrongCount, mostForgotten);
}

// --- 5 Automated Test Cases ---
void runAutomatedTestCases(BSTNode*& dictionary) {
    std::cout << "\n========== CHẠY KIỂM THỬ TỰ ĐỘNG (5 TEST CASES) ==========\n";
    clearBST(dictionary);

    // Test Case 1: Thêm phần tử vào BST
    Flashcard c1{"apple", "qua tao", "An apple", 0};
    Flashcard c2{"banana", "qua chuoi", "Eat banana", 2};
    insertBST(dictionary, c1);
    insertBST(dictionary, c2);
    std::cout << "[Test 1] Thêm flashcard vào BST: ";
    if (searchBST(dictionary, "apple") && searchBST(dictionary, "banana")) std::cout << "PASSED\n";
    else std::cout << "FAILED\n";

    // Test Case 2: Tìm kiếm và cập nhật dữ liệu trùng trên BST
    Flashcard c3{"apple", "tao khuyet", "New Apple", 1};
    insertBST(dictionary, c3);
    BSTNode* res = searchBST(dictionary, "apple");
    std::cout << "[Test 2] Tìm kiếm & cập nhật phần tử BST: ";
    if (res && res->data.definition == "tao khuyet") std::cout << "PASSED\n";
    else std::cout << "FAILED\n";

    // Test Case 3: Xóa phần tử khỏi BST
    bool success = false;
    dictionary = deleteBST(dictionary, "banana", success);
    std::cout << "[Test 3] Xóa nút khỏi cây BST: ";
    if (success && searchBST(dictionary, "banana") == nullptr) std::cout << "PASSED\n";
    else std::cout << "FAILED\n";

    // Test Case 4: Kiểm tra cơ chế hàng đợi ưu tiên (Priority Queue)
    PriorityQueue testPQ;
    testPQ.push(Flashcard{"easy", "de", "", 0});
    testPQ.push(Flashcard{"hard", "kho", "", 5});
    testPQ.push(Flashcard{"medium", "trung binh", "", 2});
    Flashcard top = testPQ.pop();
    std::cout << "[Test 4] Sắp xếp ưu tiên (Spaced Repetition PQ): ";
    if (top.word == "hard" && top.difficulty == 5) std::cout << "PASSED\n";
    else std::cout << "FAILED\n";
    testPQ.clear();

    // Test Case 5: Cơ chế Undo dựa trên Stack
    Stack testStack;
    testStack.push(Flashcard{"one", "1", "", 0});
    testStack.push(Flashcard{"two", "2", "", 0});
    Flashcard last = testStack.pop();
    std::cout << "[Test 5] Cơ chế hoàn tác lệnh (Stack Undo): ";
    if (last.word == "two") std::cout << "PASSED\n";
    else std::cout << "FAILED\n";
    testStack.clear();
    
    clearBST(dictionary); // Reset trạng thái sạch cho người dùng
    std::cout << "=========================================================\n\n";
}
