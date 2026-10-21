# Kết quả kiểm tra bản SmartRoom thực tế

Ngày kiểm tra: 13/10/2026.

## Biên dịch PlatformIO

| Môi trường | Kết quả | RAM | Flash |
|---|---|---|---|
| real — USB–UART | Thành công | 62.292 / 327.680 byte | 1.553.868 / 3.145.728 byte |
| usb_truc_tiep — USB CDC | Thành công | 62.196 / 327.680 byte | 1.539.012 / 3.145.728 byte |

Cả hai dùng Arduino-ESP32 2.0.17 và các phiên bản thư viện đã khóa trong platformio.ini.
Đây là kết quả build bằng PlatformIO; hướng dẫn Arduino IDE dùng cùng sketch và thư viện.

## Kiểm tra trên máy tính

- 30 kiểm tra logic đạt: đọc ADC, đổi lux, phát hiện lỗi cảm biến, ngưỡng tối/sáng, chế độ đèn và lệnh Serial. Kiểm tra gọi các hàm lấy từ firmware với phần cứng được thay bằng bộ giả lập kiểm thử.
- 12 kiểm tra API đạt: JSON, trạng thái và kiểm tra lệnh đổi chế độ. Dùng các hàm C++ của firmware với lớp truyền tải phục vụ kiểm thử.
- 13 kiểm tra trình duyệt đạt: lấy số đo, trạng thái mất kết nối, lỗi cảm biến, lệnh điều khiển, khôi phục, lịch sử 150 mẫu và bố cục ở chiều rộng 320/390/768/1440 px. Máy chủ HTTP phục vụ kiểm thử chạy trên máy tính, không phải ESP32.
- Word 32 trang đã được xuất và kiểm tra bố cục. Số dòng giải thích được đối chiếu với sketch đi kèm.

Project giao cho người học không chứa mã mô phỏng hoặc dữ liệu mẫu. Các bộ kiểm thử nội bộ không nằm trong gói này.

## Giới hạn và thử trên mạch

Chưa nạp và đo trực tiếp trên ESP32 cùng cảm biến thật. Build thành công không xác nhận dây nối, nguồn điện, Wi-Fi/BLE thực tế hoặc tín hiệu ngắt ngoài.

Sau khi lắp theo tài liệu: thử ON/OFF để kiểm tra LED; kiểm tra số đo và OLED; đợi PIR ổn định 60 giây rồi thử AUTO; cuối cùng thử Serial, web và BLE.

Serial qua USB CDC là giao tiếp USB, không tự chứng minh yêu cầu UART vật lý. Xem phần UART trong README và Word nếu giảng viên yêu cầu UART trên chân TX/RX. Bản này không triển khai SPI; việc thay SPI bằng UART cần phù hợp đề bài của giảng viên.
