#Ứng dụng Ôn tập Từ vựng Spaced Repetition

Ứng dụng CLI quản lý và học từ vựng tiếng Anh áp dụng nguyên lý lặp lại ngắt quãng dựa trên độ khó của thẻ nhớ nhằm tối ưu hóa khả năng ghi nhớ dài hạn.

## 🛠️ Cấu trúc dữ liệu tự thiết lập (Custom DSA)
Ứng dụng hoàn toàn đáp ứng yêu cầu của đồ án, không dùng cấu trúc mảng động, thư viện `std::vector` hay các container phân bổ sẵn, tất cả liên kết đều dùng danh sách động:
- **Binary Search Tree (BST):** Quản lý lưu trữ gốc bộ từ điển tĩnh, tối ưu tìm kiếm, thêm, xóa các nút từ vựng theo cấu trúc cây nhị phân cân bằng cục bộ chữ cái ABC với Big-O là $O(\log n)$.
- **Priority Queue (Hàng đợi ưu tiên liên kết đơn):** Đóng vai trò là bộ lõi thuật toán Spaced Repetition, tự động kéo và sắp xếp các từ vựng có số lần trả lời sai nhiều nhất lên hàng đầu.
- **Queue (Hàng đợi):** Phân bổ, trích xuất cấu trúc danh sách tuần tự cho các thẻ từ trong một phiên học cụ thể.
- **Stack (Ngăn xếp lệnh):** Hỗ trợ lưu trữ trạng thái snapshot lịch sử thẻ vừa trả lời, phục vụ tính năng Hoàn tác (`Undo`) nếu người dùng thao tác nhầm kết quả chấm điểm.

## Hướng dẫn biên dịch và Chạy chương trình
Yêu cầu hệ thống cài đặt trình biên dịch GCC hỗ trợ cờ tiêu chuẩn `-std=c++17`.

# Di chuyển vào thư mục src chứa mã nguồn
cd sr

#chạy chương trình
g++ -std=c++17 main.cpp functions.cpp -o flashcard_app

# Chạy ứng dụng trên môi trường CLI
./flashcard_app
