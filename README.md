# SmartRoom ESP32-C3 — Kiến trúc Module PlatformIO

Dự án mô hình phòng thông minh SmartRoom sử dụng vi điều khiển **ESP32-C3** kết hợp cảm biến DHT11, BH1750, PIR HC-SR501, biến trở ADC, màn hình OLED SSD1306/SSD1315 và 2 đèn LED chỉ báo. Hỗ trợ giao diện Web nội bộ qua WiFi SoftAP và đồng bộ dữ liệu qua BLE.

---

## 1. Cấu trúc thư mục dự án

```text
Smart_Room/
├── .vscode/               # Cấu hình VS Code
├── docs/                  # Tài liệu, hình ảnh đấu nối phần cứng
├── platformio.ini          # Cấu hình PlatformIO (board, thư viện, partition)
├── wokwi.toml             # Cấu hình nạp firmware cho mô phỏng Wokwi
├── diagram.json           # Sơ đồ kết nối phần cứng và linh kiện ảo Wokwi
│
├── include/               # Header dùng chung toàn hệ thống
│   ├── config.h           # Định nghĩa chân GPIO, hằng số ngưỡng, cấu hình WiFi & BLE
│   └── app_state.h        # Struct RoomState lưu trữ dữ liệu cảm biến & cờ trạng thái
│
└── src/                   # Mã nguồn C++ phân tách theo từng module chức năng
    ├── main.cpp           # Điều phối luồng setup() và loop()
    │
    ├── sensors/           # Module cảm biến (DHT11, BH1750, PIR, ADC biến trở, ISR Timer)
    │   ├── sensor_manager.h
    │   └── sensor_manager.cpp
    │
    ├── controller/        # Module logic nghiệp vụ (AUTO/MANUAL, bật/tắt đèn, cảnh báo, UART)
    │   ├── room_controller.h
    │   └── room_controller.cpp
    │
    ├── display/           # Module hiển thị OLED SSD1306/SSD1315 (U8g2)
    │   ├── oled_display.h
    │   └── oled_display.cpp
    │
    └── network/           # Module kết nối (WiFi SoftAP, WebServer HTTP & BLE GATT Server)
        ├── network_manager.h
        ├── network_manager.cpp
        └── web_page.h     # Giao diện web lưu trong bộ nhớ Flash (PROGMEM)
```

---

## 2. Bảng chân nối phần cứng (ESP32-C3)

| Linh kiện | Chân ESP32-C3 | Chức năng |
| :--- | :--- | :--- |
| **Biến trở** | `GPIO 3` (ADC1_CH3) | Chỉnh ngưỡng ánh sáng (20 .. 500 lux) |
| **DHT11** | `GPIO 6` | Đo nhiệt độ & độ ẩm phòng |
| **PIR HC-SR501** | `GPIO 0` | Phát hiện chuyển động người (Ngắt ngoài) |
| **LED Xanh** | `GPIO 7` | Đèn chiếu sáng phòng |
| **LED Đỏ** | `GPIO 10` | Đèn cảnh báo (quá nhiệt, quá ẩm, lỗi cảm biến) |
| **I2C Bus** | `SDA: GPIO 4`, `SCL: GPIO 5` | Dùng chung cho BH1750 (0x23) và OLED (0x3C) |

---

## 3. Hướng dẫn chạy trên Wokwi (Mô phỏng không cần phần cứng)

1. Cài đặt tiện ích mở rộng **Wokwi Simulator** trên VS Code.
2. Biên dịch firmware PlatformIO:
   - Nhấn biểu tượng `✓` ở thanh đáy VS Code (hoặc gõ `pio run -e real`).
3. Mở file [diagram.json](file:///d:/Project/ESP32/Smart_Room/diagram.json).
4. Nhấn phím `F1` → Chọn **Wokwi: Start Simulator** (hoặc bấm nút Play màu xanh trên góc preview).
5. Tương tác với mô phỏng:
   - Kéo biến trở để đổi ngưỡng ánh sáng.
   - Nhấp vào cảm biến DHT11 để chỉnh nhiệt độ / độ ẩm.
   - Nhấp vào cảm biến PIR để kích hoạt chuyển động (`Simulate motion`).
   - Nhấp vào BH1750 để đổi độ sáng môi trường.
   - Quan sát trạng thái trên màn hình OLED và hai đèn LED.

---

## 4. Biên dịch và nạp vào ESP32-C3 thật

### Dùng VS Code:
- Chọn môi trường `real` (nếu dùng board có chip nạp USB-UART rời) hoặc `usb_truc_tiep` (C3 SuperMini).
- Bấm nút **Upload** (`→`) và **Monitor** (`⚡`).

### Dùng dòng lệnh PlatformIO:
```powershell
# Biên dịch
pio run -e real

# Nạp code
pio run -e real -t upload

# Mở Serial Monitor
pio device monitor -b 115200
```

- Lệnh UART điều khiển qua Serial Monitor:
  - `A`: Chuyển sang chế độ Tự động (AUTO).
  - `1`: Bật đèn cưỡng bức (MANUAL ON).
  - `0`: Tắt đèn cưỡng bức (MANUAL OFF).
