
#include "Animation/Animation.h"
#include "Sidebar.h"
#include "UIElements/UIElement.h"
#include "Utils/Keyboard.h"
#include "Utils/Util.h"

Keyboard kb;

namespace
{
class MinimalUi : public UIElement
{
public:
    MinimalUi() : UIElement(0, 0, WIDTH, HEIGHT) {}

    void activate() override
    {
        currentElement = this;
    }

    void update() override
    {
        draw();
    }

    void draw() override
    {
        const unsigned long seconds = millis() / 1000;
        const auto changedKey = kb.getChangedKey();
        const auto pressedKey = kb.getRisingEdgeKey();

        u8g2.setFont(u8g2_font_profont10_mf);
        u8g2.drawStr(4, 10, "SCI-CALC minimal");
        u8g2.drawStr(4, 22, sdAvailable ? "SD: mounted" : "SD: unavailable");
        u8g2.drawStr(4, 34, ("Battery: " + batterylevel()).c_str());
        u8g2.drawStr(4, 46, ("Uptime: " + std::to_string(seconds) + "s").c_str());
        u8g2.drawStr(4, 58, keyState(changedKey, pressedKey).c_str());
    }

private:
    static std::string batterylevel()
    {
        if (SHOW_BATTERY_PERCENTAGE)
        {
            return getBatteryPercentageStr();
        }
        return numToStr(getBatteryVoltage()).substr(0, 4) + "V";
    }

    static std::string keyState(const std::pair<int, int> &changedKey, const std::pair<int, int> &pressedKey)
    {
        if (changedKey.first >= 0)
        {
            return "Key: r" + std::to_string(changedKey.first) + " c" + std::to_string(changedKey.second);
        }
        if (pressedKey.first >= 0)
        {
            return "Press: r" + std::to_string(pressedKey.first) + " c" + std::to_string(pressedKey.second);
        }
        return "Key: idle";
    }
};
}

UIElement *currentElement = nullptr;
MinimalUi minimalUi;

void appInit()
{
    u8g2.begin();
    u8g2.setFontMode(0);
    Serial.begin(115200);
    u8g2.setFont(u8g2_font_profont10_mf);
    u8g2.setFontPosCenter();
    u8g2.setFontMode(1);
    u8g2.enableUTF8Print();

    Serial.begin(115200);
    if (SD.begin(4))
    {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE)
        {
            Serial.println("No SD_MMC card attached");
        }
        else
        {
            Serial.println("SD Card init success.");
            sdAvailable = true;
        }
    }

    pinMode(36, INPUT);
}

void setup()
{
    appInit();
    kb.init();
    bleKeyboard.begin();
    currentElement = &minimalUi;
    currentElement->init();
    u8g2.clearBuffer();
    currentElement->activate();
}

void loop()
{
    animateAll();
    bleKeyboard.setBatteryLevel(getBatteryLevel());
    u8g2.clearBuffer();
    kb.update();
    drawSidebar();
    currentElement->update();
    updateTmp();
    u8g2.sendBuffer();
}
