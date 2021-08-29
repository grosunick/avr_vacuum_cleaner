#include "ng_avr/boards/attiny13.h"
#include "ng_avr/ng_pwm.h"
#include "ng_avr/ng_button.h"

#pragma pack(push,1)
struct store {
    uint8_t isOn: 1;
    uint8_t isHalfPower: 1;

    store() {
        isOn = 0;
        isHalfPower = 1;
    }

    void btnOnClickHandler() {
        if (isOn == 0) {
            isOn = 1;
        } else if (isOn && isHalfPower == 1) {
            isHalfPower = 0;
        } else {
            isOn = 0;
            isHalfPower = 1;
        }
    }

    void btnOffClickHandler() {
        isOn = 0;
        isHalfPower = 1;
    }

    uint8_t isHalfPowerMode() {
        return isOn && isHalfPower;
    }

    uint8_t isFullPowerMode() {
        return isOn && !isHalfPower;
    }

    uint8_t isOff() {
        return !isOn;
    }
} state;
#pragma pack(pop)

int main() {
    DDRB |= _BV(PORTB0);
    PORTB &= ~_BV(PORTB0);


    // initialization
    ng::Button btnOn(4);
    // ng::Button btnOff(5);

    for (;;) {


        btnOn.tick();
        // btnOff.tick();

        if (btnOn.isClicked()) {
            state.btnOnClickHandler();
        }

        // if (btnOff.isClicked()) {
        //     state.btnOffClickHandler();
        // }

        if (state.isHalfPowerMode()) {
            ng::fastPWM(PORTB0, 128);
        }

        if (state.isFullPowerMode()) {
             ng::fastPWM(PORTB0, 255);
        }

        if (state.isOff()) {
            ng::fastPWM(PORTB0, 0);
        }
    }
}