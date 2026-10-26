# SmartRoom ESP32-C3 — Hệ Thống Giám Sát Môi Trường & Chiếu Sáng Thông Minh

Dự án mô hình phòng thông minh hoàn chỉnh chạy trên vi điều khiển **ESP32-C3**, kết hợp cảm biến DHT22, quang trở LDR, biến trở vi chỉnh ngưỡng, cảm biến chuyển động PIR, màn hình OLED SSD1306 và 3 đèn LED chỉ báo (Xanh, Đỏ, Trắng).

---

## 1. Sơ đồ Đấu Nối Phần Cứng Chi Tiết

| Linh kiện | Chân Linh Kiện | Chân ESP32-C3 | Màu dây gợi ý | Chức năng & Giao tiếp |
| :--- | :---: | :---: | :---: | :--- |
| **Biến trở xoay (POT)** | `VCC` / `GND` / `SIG` | `3V3.1` / `GND.1` / **`GPIO 3`** | Đỏ / Đen / Cam | Cài đặt ngưỡng độ nhạy ánh sáng (**ADC1_CH3**) |
| **Quang trở LDR** | `VCC` / `GND` / `AO` | `3V3.1` / `GND.1` / **`GPIO 1`** | Đỏ / Đen / Vàng | Đo cường độ sáng thực tế (**ADC1_CH1**) |
| **Cảm biến DHT22** | `VCC` / `GND` / `SDA` | `3V3.1` / `GND.1` / **`GPIO 6`** | Đỏ / Đen / Lam | Đo nhiệt độ & độ ẩm phòng (1-Wire Digital) |
| **Cảm biến PIR** | `VCC` / `GND` / `OUT` | `3V3.1` / `GND.1` / **`GPIO 0`** | Đỏ / Đen / Tím | Phát hiện người chuyển động (**External Interrupt**) |
| **Màn hình OLED** | `VCC` / `GND`<br>`DATA` / `CLK` | `3V3.1` / `GND.2`<br>**`GPIO 4`** / **`GPIO 5`** | Đỏ / Đen<br>Xanh lá / Vàng | Hiển thị thông số thời gian thực (**I2C `0x3C`**) |
| **LED Xanh (Safe)** | Anode (A) / Cathode (C)| **`GPIO 7`** (qua 220Ω) / `GND.2` | Xanh lá / Đen | **BẬT khi Môi trường AN TOÀN** (Nhiệt & Ẩm đạt chuẩn) |
| **LED Đỏ (Alert)** | Anode (A) / Cathode (C)| **`GPIO 10`** (qua 220Ω) / `GND.2` | Đỏ / Đen | **BẬT khi VƯỢT NGƯỠNG AN TOÀN** (Nhiệt/Ẩm bất thường) |
| **LED Trắng (Lamp)**| Anode (A) / Cathode (C)| **`GPIO 2`** (qua 220Ω) / `GND.2` | Trắng / Đen | **Bóng đèn phòng:** Tự động bật khi tối + có người |

---

## 2. Logic Hoạt Động Của Hệ Thống

### A. Khối Chiếu Sáng Phòng Thông Minh (Đèn LED Trắng)
- **Xác định phòng tối/sáng:** So sánh mức sáng đo được từ LDR ($0 \sim 100\%$) với ngưỡng cài đặt từ Biến trở ($0 \sim 100\%$).
- **Chế độ TỰ ĐỘNG (`AUTO`):**
  1. Khi phòng **TỐI** VÀ **CÓ NGƯỜI CHUYỂN ĐỘNG** $\rightarrow$ **Đèn Trắng BẬT ngay lập tức**.
  2. **Cơ chế chống tắt đèn khi người ngồi yên (Off-Delay Timer 30s):** Khi người ngồi yên (PIR trả về 0), hệ thống kích hoạt bộ đếm lùi 30 giây. Đèn **vẫn tiếp tục sáng**. Chỉ khi sau 30 giây liên tục không có bất kỳ cử động nào $\rightarrow$ Đèn mới tự động tắt.
  3. **Tự động ngắt khi đủ sáng:** Nếu phòng đủ sáng tự nhiên (mở rèm, trời hửng nắng) $\rightarrow$ Đèn tự động tắt dù trong phòng vẫn có người.
- **Chế độ THỦ CÔNG:** Nhập `1` qua Serial để BẬT, `0` để TẮT, `A` để về TỰ ĐỘNG.

### B. Khối Giám Sát Môi Trường An Toàn (LED Xanh & LED Đỏ)
- **Tiêu chuẩn an toàn:** Nhiệt độ $18^\circ\text{C} \le T \le 30^\circ\text{C}$ VÀ Độ ẩm $40\% \le H \le 75\%$.
- **Quy tắc đèn chỉ báo (Đối nghịch nhau):**
  - **Môi trường an toàn:** $\rightarrow$ **LED XANH SÁNG**, **LED ĐỎ TẮT**.
  - **Vượt ngưỡng / Bất thường / Lỗi cảm biến:** $\rightarrow$ **LED ĐỎ SÁNG CẢNH BÁO**, **LED XANH TẮT**.

---

## 3. Giao Diện OLED & Serial Monitor

### OLED SSD1306 (Tối giản 4 dòng):
```text
+-----------------------+
|  TEMP : 26.5 C        |
|  HUM  : 58.0 %        |
|  LDR  : DARK (50%)    |
|  STATE: SAFE | L:ON   |
+-----------------------+
```

### Serial Monitor:
```text
[ROOM | AUTO] Temp: 26.5 C | Hum: 58.0 % | Light: 42% (Set: 50% - DARK  ) | PIR: YES | State: SAFE [GREEN] | Lamp: ON
```

---

## 4. Hướng Dẫn Test Trực Quan Trên Wokwi

1. Mở file [diagram.json](file:///d:/Project/ESP32/Smart_Room/diagram.json).
2. Nhấn nút **Play ▶️** (hoặc nút **Restart 🔄**).
3. **Các tình huống kiểm thử:**
   - **Test LED Xanh & Đỏ (An toàn vs Vượt ngưỡng):**
     - Mặc định ($T = 26.5^\circ\text{C}, H = 58\%$) $\rightarrow$ Thấy **LED Xanh sáng, LED Đỏ tắt**.
     - Nhấp vào DHT22, kéo nhiệt độ lên $> 30^\circ\text{C}$ (ví dụ $35^\circ\text{C}$) $\rightarrow$ Thấy **LED Đỏ bật sáng cảnh báo, LED Xanh tắt ngay lập tức**.
   - **Test Đèn phòng (LED Trắng):**
     - Nhấp vào cảm biến chuyển động PIR, bấm nút **Simulate motion** để kích hoạt người di chuyển.
     - Nếu mức sáng LDR < ngưỡng Biến trở (phòng tối) $\rightarrow$ **LED Trắng bật sáng ngay lập tức**.
     - Thử thả PIR ra (người ngồi yên) $\rightarrow$ Đèn trắng **vẫn tiếp tục sáng duy trì trong 30 giây**, sau đó mới tự động tắt!
     - Xoay biến trở POT hoặc kéo thanh trượt LDR để kiểm tra việc đóng/ngắt ngưỡng sáng.
