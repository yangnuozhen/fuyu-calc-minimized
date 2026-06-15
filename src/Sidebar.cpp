#include "Sidebar.h"

int curtime = 0;
std::string batterylevel;

void drawSidebar() {
    if (HIDE_SIDEBAR == false) {
        u8g2.drawRFrame(213, 0, 43, 64, 2);
        if (millis() - curtime > 500) {
            if (SHOW_BATTERY_PERCENTAGE){
                batterylevel = getBatteryPercentageStr();
            }else{
                batterylevel = numToStr(getBatteryVoltage()).substr(0, 3) + "V";
            }
            
            curtime = millis();
        }
        u8g2.drawStr(215, 10, ("Bat:" + batterylevel).c_str());
    }
}
