/*
  BinaryClock 8x8 LED matrix
  Reads current time from serial, displays on LED matrix in binary format
 */

// pins 1-8 (right side, from bottom) connected to digital pins 2-9
// pins 9-16 (left side, from bottom) connected to digital pins 10-13, 16-19 (A2-A5)
// the following arrays map the LED matrix row/col pins to the arduino pins when connected
// in above order

int rows[] = {16, 12, 9, 19, 2, 8, 3, 6};
int cols[] = {13, 4, 5, 17, 7, 18, 11, 10};

int img[][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
};

// the setup function runs once when you press reset or power the board
void setup() {
  for(int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }
  for(int i = 0; i < 8; i++) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], LOW);
  }
  
  Serial.begin(9600);
}

char buf[6] = {'\0'};
int bufPos = 0;
long clockSeconds = 0;

void updateRow(int row, int num) {
  img[row][0] = num & 0b00000001;
  img[row][1] = num & 0b00000010;
  img[row][2] = num & 0b00000100;
  img[row][3] = num & 0b00001000;
  img[row][4] = num & 0b00010000;
  img[row][5] = num & 0b00100000;
  img[row][6] = num & 0b01000000;
  img[row][7] = num & 0b10000000;
}

void updateClock() {
  //clockSeconds = millis() / 1000; // for testing
  
  // read time from serial, time is in seconds since 00:00:00 today
  if(Serial.available()) {
    buf[bufPos] = Serial.read();
    if(buf[bufPos] == '\0') {
      clockSeconds = atol(buf);
      bufPos = 0;
    } else {
      bufPos++;
    }
  }
  
  // split into seconds, mins, hours
  int seconds = clockSeconds % 60;
  int mins = (clockSeconds / 60) % 60;
  int hours = (clockSeconds / 60 / 60) % 24;
  
  // convert these to strings
  char seconds_s[3] = {'\0'};
  sprintf(seconds_s, "%02d", seconds);
  
  char mins_s[3] = {'\0'};
  sprintf(mins_s, "%02d", mins);
    
  char hours_s[3] = {'\0'};
  sprintf(hours_s, "%02d", hours);

  // update rows with binary number
  updateRow(7, seconds_s[1] - '0');
  updateRow(6, seconds_s[0] - '0');
  updateRow(5, 0);
  updateRow(4, mins_s[1] - '0');
  updateRow(3, mins_s[0] - '0');
  updateRow(2, 0);
  updateRow(1, hours_s[1] - '0');
  updateRow(0, hours_s[0] - '0');
}

// the loop function runs over and over again forever
void loop() {
  updateClock();
  
  for(int i = 0; i < 8; i++) {
    digitalWrite(rows[i], HIGH);
    
    // draw image
    for(int j = 0; j < 8; j++) {
      if(img[i][j])
        digitalWrite(cols[j], LOW);
    }
    
    // clear column leds
    for(int j = 0; j < 8; j++) {
      digitalWrite(cols[j], HIGH);
    }
    digitalWrite(rows[i], LOW);
  }
  
  delay(4); // "brightness adjustment" - the more delay the less brightnes (but more flicker!)
}
