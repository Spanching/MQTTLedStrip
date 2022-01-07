#define BTN_STATUS D6
#define BTN_AP D5

int btn_ledstatus = 0;
int btn_ap = 0;

int ledstatus_change=0;

void toggleEnabled();

void setup_buttons(){
  pinMode(BTN_STATUS, INPUT_PULLUP);
  pinMode(BTN_AP, INPUT_PULLUP);
}

// ledstatus button to dis-/enable the ledstatus led 
// AP button to reset WifiManager to input new credentials
void loop_buttons(){
  btn_ledstatus = !digitalRead(BTN_STATUS);
  btn_ap = !digitalRead(BTN_AP);
  if(btn_ledstatus){
    btn_ledstatus_pressed();
  }else{
    ledstatus_change = 0;
  }
  if(btn_ap){
    setup_wifi(true);
  }
}

void btn_ledstatus_pressed(){
  if(ledstatus_change==0){
    toggleEnabled();
    ledstatus_change = 1;
  }
}
