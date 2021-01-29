/* Arduino Tutorial: Learn how to use an LCD 16x2 screen
   More info: http://www.ardumotive.com/how-to-use-an-lcd-dislpay-en.html  */

//Include LCD library
#include <LiquidCrystal.h>

// pins
#define LED_GREEN 8
#define LED_RED   9
#define PIN_RS 12
#define PIN_E  11
#define PIN_D4  5
#define PIN_D5  4
#define PIN_D6  3
#define PIN_D7  2
// LCD size
#define LCD_W 16
#define LCD_H  2

#define WIDTH 16
#define HEIGHT 8

// game related
#define EMPTY 0
#define ALIVE 1
#define DEAD  2
#define BORN  4

LiquidCrystal lcd(PIN_RS, PIN_E, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

char game[WIDTH * HEIGHT];
byte custom_top[8]    = { 31, 0,  0, 0,  0, 0,  0, 0 };
byte custom_topmid[8] = {  0, 0, 31, 0,  0, 0,  0, 0 };
byte custom_botmid[8] = {  0, 0,  0, 0, 31, 0,  0, 0 };
// byte custom_bot[8] = {  0, 0,  0, 0,  0, 0, 31, 0 }; this is the _
byte custom_2top[8]   = { 31, 0, 31, 0,  0, 0,  0, 0 };
// byte custom_2mid[8]= {  0, 0, 31, 0, 31, 0,  0, 0 }; this is the =
byte custom_2bot[8]   = {  0, 0,  0, 0, 31, 0, 31, 0 };
byte custom_3top[8]   = { 31, 0, 31, 0, 31, 0,  0, 0 };
byte custom_3bot[8]   = {  0, 0, 31, 0, 31, 0, 31, 0 };
byte custom_all[8]    = { 31, 0, 31, 0, 31, 0, 31, 0 };

void setup() {
  lcd.createChar(0, custom_top);
  lcd.createChar(1, custom_topmid);
  lcd.createChar(2, custom_botmid);
  lcd.createChar(3, custom_2top);
  lcd.createChar(4, custom_2bot);
  lcd.createChar(5, custom_3top);
  lcd.createChar(6, custom_3bot);
  lcd.createChar(7, custom_all);
  lcd.begin(LCD_W, LCD_H);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  randomSeed(analogRead(0));
  for (short i = 0; i < WIDTH * HEIGHT; ++i) {
    game[i] = random(0, 2);  // pick between 0 and 1
  }
}

char count_neighbors(short col, short row) {
  char neigh = 0;
  if (col > 0          && (game[col - 1 + row   * WIDTH] & ALIVE)) neigh++;
  if (col < WIDTH - 1  && (game[col + 1 + row   * WIDTH] & ALIVE)) neigh++;
  if (row > 0          && (game[col + (row - 1) * WIDTH] & ALIVE)) neigh++;
  if (row < HEIGHT - 1 && (game[col + (row + 1) * WIDTH] & ALIVE)) neigh++;
  if (col > 0         && row > 0          && (game[col - 1 + (row - 1) * WIDTH] & ALIVE)) neigh++;
  if (col < WIDTH - 1 && row > 0          && (game[col + 1 + (row - 1) * WIDTH] & ALIVE)) neigh++;
  if (col > 0         && row < HEIGHT - 1 && (game[col - 1 + (row - 1) * WIDTH] & ALIVE)) neigh++;
  if (col < WIDTH - 1 && row < HEIGHT - 1 && (game[col + 1 + (row + 1) * WIDTH] & ALIVE)) neigh++;
  
  return neigh;
}

void next_game(char* newborn, char* death) {
  for (short i = 0; i < WIDTH * HEIGHT; ++i) {
    const short col = i % WIDTH;
    const short row = i / WIDTH;
    const char neigh = count_neighbors(col, row);

    if (game[i] & ALIVE) {
      if (neigh < 2) {
        game[i] |= DEAD;
        (*death)++;
      } else if (neigh == 2 || neigh == 3) {
        game[i] |= BORN;
      } else if (neigh > 3) {
        game[i] |= DEAD;
        (*death)++;
      }
    } else if (game[i] == EMPTY && neigh == 3) {
      game[i] |= BORN;
      (*newborn)++;
    }
  }

  // swap
  for (short i = 0; i < WIDTH * HEIGHT; ++i) {
    if (game[i] & DEAD)
      game[i] = EMPTY;
    else if (game[i] & BORN)
      game[i] = ALIVE;
  }
}

void display_game() {
  byte custom[8] = {
    0b00000, 0b00000,
    0b00000, 0b00000,
    0b00000, 0b00000,
    0b00000, 0b00000
  };

  const short ratio_w = WIDTH / LCD_W;
  const short ratio_h = HEIGHT / LCD_H;

  // process char by char on the LCD
  for (short i = 0; i < LCD_W * LCD_H; ++i) {
    const short col = i % LCD_W;
    const short row = i / LCD_W;
    lcd.setCursor(col, row);

    const short x = col * ratio_w;
    const short y = row * ratio_h;

    char state = ((game[x + y       * WIDTH] & ALIVE) << 3) +
                 ((game[x + (y + 1) * WIDTH] & ALIVE) << 2) +
                 ((game[x + (y + 2) * WIDTH] & ALIVE) << 1) +
                  (game[x + (y + 3) * WIDTH] & ALIVE);

    switch (state) {
      // everyone is dead
      case 0: lcd.write(' '); break;
      // 1 living at the bottom
      case 1: lcd.write('_'); break;
      case 2: lcd.write(byte(2)); break;
      case 3: lcd.write(byte(4)); break;
      case 4: lcd.write(byte(1)); break;
      case 6: lcd.write('='); break;
      case 7: lcd.write(byte(6)); break;
      case 8: lcd.write(byte(0)); break;
      case 12: lcd.write(byte(3)); break;
      case 14: lcd.write(byte(5)); break;
      case 15: lcd.write(byte(7)); break;
    }
  }
}

void loop() {
  char death = 0;
  char newborn = 0;

  next_game(&newborn, &death);
  display_game();

  int led_pin = (death == newborn) ? 0 : ((death > newborn) ? LED_RED : LED_GREEN);
  if (led_pin != 0) {
    digitalWrite(led_pin, HIGH);
    delay(250);
    digitalWrite(led_pin, LOW);
    delay(250);
  }
}
