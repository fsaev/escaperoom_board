#include <Keypad.h>
#include "input.h"
#include "defines.h"

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte row_pins[ROWS] = {KEYPAD_ROW1, KEYPAD_ROW2, KEYPAD_ROW3, KEYPAD_ROW4}; //connect to the row pinouts of the kpd
byte col_pins[COLS] = {KEYPAD_COL1, KEYPAD_COL2, KEYPAD_COL3}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

String debug_msg;

uint8_t input_enabled = 0;

char input[8];
uint8_t input_idx = 0;

char accepted_input[8];
uint8_t accepted_input_len = 0;

void init_input(void){

}

void reset_input(void){
    input_idx = 0;
    accepted_input_len = 0;
}

void enable_input(void){
    input_enabled = 1;
    reset_input();
}

void disable_input(void){
    input_enabled = 0;
    reset_input();
}

uint8_t get_accepted_input_len(void){
    return accepted_input_len;
}

void tick_input(void){
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i = 0; i < LIST_MAX; i++){ // Scan the whole key list.

            if (kpd.key[i].stateChanged){   // Only find keys that have changed state.
                switch(kpd.key[i].kstate){  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                        //debug_msg = " PRESSED.";
                        if(kpd.key[i].kchar == '*'){
                            if(input_idx){
                                Serial.println(" Wipe!");
                                //memset(input, 0, sizeof(char) * 8);
                                input_idx = 0;
                            }
                        }else if(kpd.key[i].kchar == '#'){
                            memcpy(accepted_input, input, sizeof(char) * input_idx);
                            accepted_input_len = input_idx;

                            //memset(input, 0, sizeof(char) * 8);
                            input_idx = 0;

                            Serial.print("\n\rAccepted input: ");
                            for(uint8_t i = 0; i < accepted_input_len; i++){
                                Serial.print(accepted_input[i]);
                            }
                            Serial.println("");
                        }else if(input_idx < 8){
                            input[input_idx++] = kpd.key[i].kchar;
                            Serial.print(kpd.key[i].kchar);
                        }
                        break;
                }
                // Serial.print("Key ");
                // Serial.print(kpd.key[i].kchar);
                // Serial.println(debug_msg);
            }
        }
    }
}