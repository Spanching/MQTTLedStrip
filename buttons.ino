#define BTN_STATUS D7
#define BTN_AP D8

int btn_status = 0;
int btn_ap = 0;

int status_change=0;

void setup_buttons(){
  pinMode(BTN_STATUS, INPUT_PULLUP);
  pinMode(BTN_AP, INPUT_PULLUP);
}

// status button to dis-/enable the status led 
// AP button to reset WifiManager to input new credentials
void loop_buttons(){
  btn_status = !digitalRead(BTN_STATUS);
  btn_ap = !digitalRead(BTN_AP);
  if(btn_status){
    btn_status_pressed();
  }else{
    status_change = 0;
  }
  if(btn_ap){
    setup_wifi(true);
  }
}

void btn_status_pressed(){
  if(status_change==0){
    toggleEnabled();
    status_change = 1;
  }
}
