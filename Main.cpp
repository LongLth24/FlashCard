#include <iostream>
#include "Flashcard.h"

int main() {
    BSTNode* dictionary = nullptr;
    const std::string filename = "data_flashcard.txt";
    loadFromFile(dictionary, filename);

    int choice;
    while (true) {
        std::cout << "=========================================\n";
        std::cout << "    HỆ THỐNG HỌC TỪ VỰNG SPACED REPETITION\n";
        std::cout << "=========================================\n";
        std::cout << "1. Thêm / Sửa đổi Flashcard\n";
        std::cout << "2. Xóa Flashcard khỏi từ điển\n";
        std::cout << "3. Tìm kiếm từ vựng (BST)\n";
        std::cout << "4. Bắt đầu phiên học mới (Queue + Stack + PQ)\n";
        std::cout << "5. Thống kê học tập\n";
        std::cout << "6. Lưu tiến độ ra File\n";
        std::cout << "7. Chạy 5 Test Cases kiểm thử tự động\n";
        std::cout << "0. Thoát chương trình\n";
        std::cout << "Lựa chọn của bạn: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        std::cin.ignore();

        if (choice == 0) { std::cout << "Tạm biệt!\n"; break; }

        switch (choice) {
            case 1: {
                Flashcard card;
                std::cout << "Nhập từ: "; std::getline(std::cin, card.word);
                std::cout << "Nhập nghĩa: "; std::getline(std::cin, card.definition);
                std::cout << "Nhập ví dụ: "; std::getline(std::cin, card.example);
                BSTNode* exist = searchBST(dictionary, card.word);
                if (exist) { card.difficulty = exist->data.difficulty; }
                insertBST(dictionary, card);
                std::cout << "Thao tác thành công.\n";
                break;
            }
            case 2: {
                std::string word; std::cout << "Nhập từ xóa: "; std::getline(std::cin, word);
                bool success = false; dictionary = deleteBST(dictionary, word, success);
                std::cout << (success ? "Đã xóa thành công.\n" : "Không tìm thấy từ.\n");
                break;
            }
            case 3: {
                std::string word; std::cout << "Nhập từ tìm: "; std::getline(std::cin, word);
                BSTNode* res = searchBST(dictionary, word);
                if (res) {
                    std::cout << "-> Nghĩa: " << res->data.definition << " | Lỗi: " << res->data.difficulty << "\n";
                } else std::cout << "Không tồn tại.\n";
                break;
            }
            case 4: {
                PriorityQueue pq; populatePQFromBST(dictionary, pq);
                if (pq.isEmpty()) { std::cout << "Từ điển trống!\n"; break; }
                Queue sessionQueue; Stack undoStack; int count = 0;
                while (!pq.isEmpty() && count < 5) { sessionQueue.enqueue(pq.pop()); count++; }

                std::cout << "\n--- PHIÊN HỌC CHUYÊN SÂU (" << count << " TỪ) ---\n(Nhập 'u' để Undo)\n";
                while (!sessionQueue.isEmpty()) {
                    Flashcard current = sessionQueue.dequeue();
                    std::cout << "\nTừ: " << current.word << "\nNhấn Enter xem đáp án...";
                    std::string d; std::getline(std::cin, d);
                    std::cout << "-> Nghĩa đúng: " << current.definition << "\n";
                    std::cout << "Đúng (1) | Sai (0) | Hoàn tác (U): ";
                    std::string ans; std::getline(std::cin, ans);

                    if (ans == "u" || ans == "U") {
                        if (undoStack.isEmpty()) {
                            std::cout << "[Lỗi] Không thể hoàn tác!\n";
                            Queue temp; temp.enqueue(current);
                            while (!sessionQueue.isEmpty()) temp.enqueue(sessionQueue.dequeue());
                            sessionQueue = temp;
                        } else {
                            Flashcard prev = undoStack.pop();
                            BSTNode* node = searchBST(dictionary, prev.word);
                            if (node) node->data.difficulty = prev.difficulty;
                            Queue temp; temp.enqueue(prev); temp.enqueue(current);
                            while (!sessionQueue.isEmpty()) temp.enqueue(sessionQueue.dequeue());
                            sessionQueue = temp;
                            std::cout << "[Undo] Đã quay lại từ trước đó.\n";
                        }
                    } else {
                        undoStack.push(current);
                        BSTNode* node = searchBST(dictionary, current.word);
                        if (node) {
                            if (ans == "0") node->data.difficulty += 1;
                            else if (node->data.difficulty > 0) node->data.difficulty -= 1;
                        }
                    }
                }
                break;
            }
            case 5: {
                int total = 0, wrong = 0; Flashcard most{-1};
                collectStats(dictionary, total, wrong, most);
                std::cout << "\nTổng số từ: " << total << "\nSố từ cần chú ý: " << wrong << "\n";
                break;
            }
            case 6: {
                std::ofstream file(filename);
                if (file.is_open()) { saveInOrder(dictionary, file); file.close(); std::cout << "Đã lưu.\n"; }
                break;
            }
            case 7: {
                runAutomatedTestCases(dictionary);
                loadFromFile(dictionary, filename); // Khôi phục lại dữ liệu gốc từ file sau khi test
                break;
            }
            default: break;
        }
    }
    clearBST(dictionary);
    return 0;
}
