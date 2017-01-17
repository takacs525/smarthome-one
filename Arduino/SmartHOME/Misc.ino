
void Wait10s() {
  delay(10000);
}

void Wait1m() {
  for (int i=0; i <= 5; i++){
    Wait10s();
  }
}

void Wait(int minute) {
  for (int i=1; i <= minute; i++){
    Wait1m();
  }
}

