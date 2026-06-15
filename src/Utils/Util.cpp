#include "Util.h"

bool sdAvailable = false;
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET);
BleKeyboard bleKeyboard("SCI-CALC", "SHAP", 100);

bool sample = false;
byte ROW_MISO[ROWCNT] = {33, 32, 35, 34, 39};
byte COL_MOSI[COLCNT] = {13, 12, 14, 27, 26, 25};

bool HIDE_SIDEBAR = false;
bool SHOW_BATTERY_PERCENTAGE = false;

char keys[ROWCNT][COLCNT] = {
    {'i', '/', '*', '-', 'a', 'b'},
    {'7', '8', '9', '+', 'c', 'd'},
    {'4', '5', '6', '^', 'e', 'f'},
    {'1', '2', '3', 'e', 'g', 'h'},
    {'0', '.', 'f', 'd', 'i', 'j'}};

// Animation settings
AnimationType menuScroll = BOUNCE;
AnimationType cursorScroll = LINEAR;
AnimationType menuEntry = BOUNCE;
AnimationType sliderMove = BOUNCE;

std::string getKeyString(uint16_t x)
{
    switch (x)
    {
    case KEY_UP_ARROW:
        return "UP";
    case KEY_DOWN_ARROW:
        return "DOWN";
    case KEY_LEFT_ARROW:
        return "LEFT";
    case KEY_RIGHT_ARROW:
        return "RIGHT";
    case KEY_BACKSPACE:
        return "BKSP";
    case KEY_TAB:
        return "TAB";
    case KEY_RETURN:
        return "ENTER";
    case KEY_ESC:
        return "ESC";
    case KEY_INSERT:
        return "INS";
    case KEY_PRTSC:
        return "PRTSC";
    case KEY_DELETE:
        return "DEL";
    case KEY_PAGE_UP:
        return "PG UP";
    case KEY_PAGE_DOWN:
        return "PG DN";
    case KEY_HOME:
        return "HOME";
    case KEY_END:
        return "END";
    case KEY_CAPS_LOCK:
        return "CAPS LK";
    case KEY_F1:
        return "F1";
    case KEY_F2:
        return "F2";
    case KEY_F3:
        return "F3";
    case KEY_F4:
        return "F4";
    case KEY_F5:
        return "F5";
    case KEY_F6:
        return "F6";
    case KEY_F7:
        return "F7";
    case KEY_F8:
        return "F8";
    case KEY_F9:
        return "F9";
    case KEY_F10:
        return "F10";
    case KEY_F11:
        return "F11";
    case KEY_F12:
        return "F12";
    case KEY_F13:
        return "F13";
    case KEY_F14:
        return "F14";
    case KEY_F15:
        return "F15";
    case KEY_F16:
        return "F16";
    case KEY_F17:
        return "F17";
    case KEY_F18:
        return "F18";
    case KEY_F19:
        return "F19";
    case KEY_F20:
        return "F20";
    case KEY_F21:
        return "F21";
    case KEY_F22:
        return "F22";
    case KEY_F23:
        return "F23";
    case KEY_F24:
        return "F24";
    default:
        std::string res(1, char(x));
        return res;
    }
}

std::string numToStr(long double x)
{
    float tmp = float(x);
    std::ostringstream ss;
    ss.precision(6);
    ss << tmp;

    return ss.str();
}
const int ADC_PIN = 36;
float filtered_voltage = -1.0; // 存储滤波后的电压历史值
const float ALPHA = 0.15;      // 滤波系数 (0.0~1.0)，越小越平滑但响应变慢。0.15非常适合电池测压

// 瞬间返回的电压获取函数
float getBatteryVoltage() {
    // 1. 只读一次，无循环，无 delay (耗时约为几十微秒)
    float adc_v = analogReadMilliVolts(ADC_PIN) / 1000.0;
    float current_voltage = adc_v * 2.0; // 1:1 分压还原

    // 2. 一阶低通滤波计算
    if (filtered_voltage < 0.0) {
        filtered_voltage = current_voltage; // 第一次运行，初始化
    } else {
        // 核心公式：本次滤波值 = ALPHA * 本次采样值 + (1 - ALPHA) * 上次滤波值
        filtered_voltage = (ALPHA * current_voltage) + ((1.0 - ALPHA) * filtered_voltage);
    }

    return filtered_voltage; 
}

uint8_t getBatteryLevel()
{
    float voltage = getBatteryVoltage();
    float BATTERY_FULL_VOLTAGE = 4.3;
    float BATTERY_EMPTY_VOLTAGE = 2.8;

    if (voltage > BATTERY_FULL_VOLTAGE)
    {
        voltage = BATTERY_FULL_VOLTAGE;
    }
    else if (voltage < BATTERY_EMPTY_VOLTAGE)
    {
        voltage = BATTERY_EMPTY_VOLTAGE;
    }

    float percentage = (voltage - BATTERY_EMPTY_VOLTAGE) / (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE) * 100;
    return (uint8_t)percentage;
}

std::string getBatteryPercentageStr()
{
    float voltage = getBatteryVoltage();
    float BATTERY_MAX_VOLTAGE = 4.3;
    float BATTERY_FULL_VOLTAGE = 3.7;
    float BATTERY_EMPTY_VOLTAGE = 2.8;

    if (voltage >= BATTERY_FULL_VOLTAGE)
    {
        return "CHG";
    }

    if (voltage > BATTERY_FULL_VOLTAGE)
    {
        voltage = BATTERY_FULL_VOLTAGE;
    }
    else if (voltage < BATTERY_EMPTY_VOLTAGE)
    {
        voltage = BATTERY_EMPTY_VOLTAGE;
    }

    float percentage = (voltage - BATTERY_EMPTY_VOLTAGE) / (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE) * 100;
    return (std::to_string(percentage) + "%");
}
