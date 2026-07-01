# 📚 Ứng dụng Ôn tập Từ vựng Spaced Repetition (C++17)

## Tên ứng dụng
Ứng dụng quản lý và học từ vựng tiếng Anh áp dụng thuật toán lặp lại ngắt quãng (Spaced Repetition) chạy trên giao diện dòng lệnh (CLI). Chương trình giúp người dùng tối ưu hóa việc ghi nhớ từ mới bằng cách tự động phân loại và ưu tiên ôn tập các từ vựng hay quên dựa trên lịch sử tương tác.

---

## Cấu trúc dữ liệu sử dụng
Để đáp ứng yêu cầu khắt khen của môn học, ứng dụng tự hiện thực 100% bằng con trỏ và danh sách động liên kết, tuyệt đối không sử dụng các container có sẵn như `std::vector` hay `std::list`.
- **Binary Search Tree (BST):** - *Dùng để:* Lưu trữ, quản lý bộ từ điển gốc (thêm, xóa, sửa đổi thông tin) và tìm kiếm từ vựng.
  - *Vì:* BST cho phép tìm kiếm, chèn và xóa một từ vựng với độ phức tạp thời gian trung bình tối ưu là $O(\log n)$, đồng thời hỗ trợ duyệt cây theo thứ tự In-order để xuất/lưu từ điển ra file theo đúng thứ tự bảng chữ cái ABC một cách dễ dàng.
- **Priority Queue (Hàng đợi ưu tiên):** - *Dùng để:* Phân loại và xếp hạng các từ vựng theo thuật toán Spaced Repetition trước khi bắt đầu phiên học.
  - *Vì:* Bản chất liên kết động của hàng đợi ưu tiên giúp tự động đẩy các từ có trọng số độ khó (`difficulty` - số lần trả lời sai) cao nhất lên đầu hàng đợi để người học bắt buộc phải ôn tập lại các từ này sớm hơn.
- **Queue (Hàng đợi):** - *Dùng để:* Lưu trữ và điều phối danh sách các thẻ từ vựng được phân bổ riêng cho phiên học hiện tại (Session Queue).
  - *Vì:* Tuân thủ đúng nguyên lý Vào trước - Ra trước (FIFO), giúp người dùng học và kiểm tra tuần tự từng từ một từ đầu đến cuối phiên học mà không bị đảo lộn.
- **Stack (Ngăn xếp):** - *Dùng để:* Lưu vết trạng thái của thẻ từ vừa học nhằm phục vụ tính năng Hoàn tác (`Undo`).
  - *Vì:* Tuân thủ nguyên lý Vào sau - Ra trước (LIFO), cho phép trích xuất ngay lập tức trạng thái của thẻ từ vựng gần nhất mà người dùng vừa thực hiện chấm điểm để quay lại lượt trước đó nếu lỡ bấm nhầm.

---

## Compile và chạy
Yêu cầu trình biên dịch GCC hỗ trợ chuẩn tiêu chuẩn C++17. Mở Terminal tại thư mục dự án và chạy lệnh sau:


g++ -std=c++17 src/main.cpp src/functions.cpp -o app && ./app

##Chức năng

1. Thêm / Sửa đổi Flashcard: Thêm từ mới vào từ điển BST. Nếu từ khóa đã tồn tại, hệ thống tự động cập nhật lại nghĩa và ví dụ mà không làm mất tiến độ học cũ (giữ nguyên độ khó).

2. Xóa Flashcard: Loại bỏ hoàn toàn một từ vựng ra khỏi cấu trúc cây tìm kiếm nhị phân BST và giải phóng bộ nhớ.

3. Tìm kiếm từ vựng: Truy vấn nhanh một từ vựng bất kỳ trên cây BST để xem chi tiết ngữ nghĩa, ví dụ minh họa và số lần trả lời sai.

4. Phiên học thông minh (Spaced Repetition): Trích xuất tối đa 5 từ cần ưu tiên ôn tập nhất. Trong phiên học, người dùng có thể bấm U để kích hoạt tính năng Undo (quay lại thẻ vừa trả lời) nhờ cấu trúc dữ liệu Stack.

5. Thống kê học tập: Xem báo cáo tổng số từ, số từ hay quên nhất, số từ thuộc nhóm nguy hiểm và tỷ lệ phần trăm ghi nhớ thành công.

6. Lưu/Đọc tiến độ từ File: Tự động nạp dữ liệu cũ khi mở app và đồng bộ ghi lại toàn bộ cây dữ liệu ra file text tĩnh khi người dùng yêu cầu để không bị mất tiến độ.

7. Chạy thử nghiệm tự động: Chức năng chạy kiểm thử tự động 5 test case cốt lõi để đánh giá độ ổn định của chương trình.

**Test cases**
1. Chương trình tích hợp sẵn hệ thống kiểm thử tự động độc lập qua Menu số 7 với 5 kịch bản chính:

	Test Case 1 (Kiểm thử chèn dữ liệu): Thêm thành công các flashcard mẫu vào cây BST và kiểm tra các liên kết con trỏ nhánh left, right.

	Test Case 2 (Kiểm thử cập nhật & Tìm kiếm): Chèn một từ trùng khóa để kiểm tra cơ chế ghi đè dữ liệu mới, sau đó dùng hàm tìm kiếm để xác thực tính chính xác của dữ liệu.

	Test Case 3 (Kiểm thử xóa dữ liệu): Tiến hành loại bỏ một từ vựng ra khỏi BST (bao gồm cả trường hợp nút cần xóa có 2 nút con) và kiểm tra lại để đảm bảo hàm tìm kiếm không còn thấy từ đó.

	Test Case 4 (Kiểm thử cấu trúc Spaced Repetition): Nạp vào hàng đợi ưu tiên các từ có độ khó khác nhau, sau đó pop phần tử đầu tiên để kiểm tra xem từ có độ khó lớn nhất (sai nhiều nhất) có được đưa lên đầu hay không.

	Test Case 5 (Kiểm thử tính năng Undo): Nạp tuần tự các flashcard vào Stack, thực hiện lệnh pop để kiểm tra tính đúng đắn của cơ chế LIFO (phải lấy được phần tử vừa được đưa vào cuối cùng).

2. Cấu trúc file
```bash
src/
  main.cpp       — Menu giao diện CLI và điều khiển luồng chạy chính của ứng dụng
  structures.h   — Khai báo các struct (Flashcard, Node) và nguyên mẫu hàm của 4 cấu trúc dữ liệu
  functions.cpp  — Cài đặt chi tiết thuật toán của BST, Stack, Queue, Priority Queue và I/O file
