#include <iostream>
#include "Flashcard.h"

int main() {
    BSTNode* dictionary = nullptr;
    const std::string filename = "data_flashcard.txt";
    
    // Nạp dữ liệu khi khởi động
    loadFromFile(dictionary, filename);

    int choice;
    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "    HỆ THỐNG HỌC TỪ VỰNG SPACED REPETITION\n";
        std::cout << "=========================================\n";
        std::cout << "1. Thêm / Sửa đổi Flashcard\n";
        std::cout << "2. Xóa Flashcard khỏi từ điển\n";
        std::cout << "3. Tìm kiếm từ vựng (BST)\n";
        std::cout << "4. Bắt đầu phiên học mới (Queue + Stack + PQ)\n";
        std::cout << "5. Thống kê học tập\n";
        std::cout << "6. Lưu tiến độ ra File\n";
        std::cout << "0. Thoát chương trình\n";
        std::cout << "Lựa chọn của bạn: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 0) {
            std::cout << "Cảm ơn bạn đã sử dụng phần mềm!\n";
            break;
        }

        switch (choice) {
            case 1: {
                Flashcard card;
                std::cout << "Nhập từ tiếng Anh: "; std::getline(std::cin, card.word);
                std::cout << "Nhập nghĩa tiếng Việt: "; std::getline(std::cin, card.definition);
                std::cout << "Nhập ví dụ minh họa: "; std::getline(std::cin, card.example);
                
                BSTNode* exist = searchBST(dictionary, card.word);
                if (exist != nullptr) {
                    card.difficulty = exist->data.difficulty;
                    std::cout << "Đã cập nhật thông tin cho từ '" << card.word << "'.\n";
                } else {
                    card.difficulty = 0;
                    std::cout << "Đã thêm từ mới thành công.\n";
                }
                insertBST(dictionary, card);
                break;
            }
            case 2: {
                std::string word;
                std::cout << "Nhập từ cần xóa: "; std::getline(std::cin, word);
                bool success = false;
                dictionary = deleteBST(dictionary, word, success);
                if (success) std::cout << "Đã xóa từ '" << word << "' thành công.\n";
                else std::cout << "Không tìm thấy từ này trong từ điển.\n";
                break;
            }
            case 3: {
                std::string word;
                std::cout << "Nhập từ cần tìm kiếm: "; std::getline(std::cin, word);
                BSTNode* result = searchBST(dictionary, word);
                if (result != nullptr) {
                    std::cout << "\n[KẾT QUẢ TÌM KIẾM]\n";
                    std::cout << "-> Từ: " << result->data.word << "\n";
                    std::cout << "-> Nghĩa: " << result->data.definition << "\n";
                    std::cout << "-> Ví dụ: " << result->data.example << "\n";
                    std::cout << "-> Số lần trả lời sai: " << result->data.difficulty << "\n";
                } else {
                    std::cout << "Không tìm thấy từ '" << word << "' trong hệ thống.\n";
                }
                break;
            }
            case 4: {
                PriorityQueue pq;
                populatePQFromBST(dictionary, pq);

                if (pq.isEmpty()) {
                    std::cout << "Từ điển trống. Vui lòng thêm từ vựng trước khi học.\n";
                    break;
                }

                Queue sessionQueue;
                Stack undoStack;
                
                int count = 0;
                while (!pq.isEmpty() && count < 5) {
                    sessionQueue.enqueue(pq.pop());
                    count++;
                }

                std::cout << "\n--- BẮT ĐẦU PHIÊN HỌC CHUYÊN SÂU (" << count << " TỪ) ---\n";
                std::cout << "(Nhập 'u' tại phần chấm điểm để quay lại thẻ trước đó)\n";

                while (!sessionQueue.isEmpty()) {
                    Flashcard currentCard = sessionQueue.dequeue();
                    
                    std::cout << "\nTừ cần dịch: " << currentCard.word << "\n";
                    std::cout << "Nhấn Enter để xem đáp án...";
                    std::string dummy; std::getline(std::cin, dummy);

                    std::cout << "-> Nghĩa đúng: " << currentCard.definition << "\n";
                    std::cout << "-> Ví dụ: " << currentCard.example << "\n";
                    
                    std::cout << "Bạn trả lời đúng hay sai? (1: Đúng, 0: Sai, U: Hoàn tác/Undo): ";
                    std::string answer; std::getline(std::cin, answer);

                    if (answer == "u" || answer == "U") {
                        if (undoStack.isEmpty()) {
                            std::cout << "[Lỗi] Không có thẻ nào trước đó để hoàn tác!\n";
                            Queue tempQueue;
                            tempQueue.enqueue(currentCard);
                            while (!sessionQueue.isEmpty()) tempQueue.enqueue(sessionQueue.dequeue());
                            sessionQueue = tempQueue;
                        } else {
                            Flashcard previousCard = undoStack.pop();
                            
                            BSTNode* nodeInBST = searchBST(dictionary, previousCard.word);
                            if (nodeInBST) nodeInBST->data.difficulty = previousCard.difficulty;

                            Queue tempQueue;
                            tempQueue.enqueue(previousCard);
                            tempQueue.enqueue(currentCard);
                            while (!sessionQueue.isEmpty()) tempQueue.enqueue(sessionQueue.dequeue());
                            sessionQueue = tempQueue;
                            std::cout << "[Hệ thống] Đã thực hiện Undo thành công!\n";
                        }
                    } else {
                        undoStack.push(currentCard);

                        BSTNode* nodeInBST = searchBST(dictionary, currentCard.word);
                        if (nodeInBST != nullptr) {
                            if (answer == "0") {
                                nodeInBST->data.difficulty += 1;
                                std::cout << "-> Đã ghi nhận sai.\n";
                            } else {
                                if (nodeInBST->data.difficulty > 0) nodeInBST->data.difficulty -= 1;
                                std::cout << "-> Đã ghi nhận đúng.\n";
                            }
                        }
                    }
                }
                std::cout << "\nHoàn thành phiên học!\n";
                break;
            }
            case 5: {
                int totalWords = 0;
                int weakWordsCount = 0;
                Flashcard mostForgotten;
                mostForgotten.difficulty = -1;

                collectStats(dictionary, totalWords, weakWordsCount, mostForgotten);

                std::cout << "\n================ THỐNG KÊ HỌC TẬP ================\n";
                std::cout << " Tổng số từ có trong từ điển: " << totalWords << "\n";
                std::cout << " Số từ hay quên (sai >= 1 lần): " << weakWordsCount << "\n";
                if (mostForgotten.difficulty > 0) {
                    std::cout << " Từ hay quên nhất: '" << mostForgotten.word 
                              << "' (Sai " << mostForgotten.difficulty << " lần)\n";
                } else {
                    std::cout << " Từ hay quên nhất: Chưa có dữ liệu.\n";
                }
                std::cout << " Tỷ lệ học thuộc: " 
                          << (totalWords > 0 ? ((totalWords - weakWordsCount) * 100 / totalWords) : 0) << "%\n";
                std::cout << "==================================================\n";
                break;
            }
            case 6: {
                std::ofstream file(filename);
                if (file.is_open()) {
                    saveInOrder(dictionary, file);
                    file.close();
                    std::cout << "Đã lưu tiến độ vào '" << filename << "'.\n";
                } else {
                    std::cout << "Lỗi mở file để ghi!\n";
                }
                break;
            }
            default:
                std::cout << "Lựa chọn không hợp lệ!\n";
                break;
        }
    }
    return 0;
}
