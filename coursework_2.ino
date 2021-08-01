#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define CHOOSER 4
#define WHITE 0x7
#define YELLOW 0x3
#define VIOLET 0x5
#define RED 0x1
#define GREEN 0x2

unsigned long startTime;
unsigned long currentTime;
const unsigned long period = 250;

//The following define the symbols that appear in the sequences
char leftArrow[8] = {3, 6, 12, 31, 31, 12, 6, 3};
char rightArrow[8] = {24, 12, 6, 31, 31, 6, 12, 24};
char upArrow[8] = {4, 14, 31, 31, 21, 4, 4, 4};
char downArrow[8] = {4, 4, 4, 21, 31, 31, 14, 4};

//The symbol that appears on the left hand side of the menu options
char menuSelector[8] = {0, 4, 14, 31, 31, 14, 4, 0};

//Array of the symbols to choose from when creating a sequence using random()
char symbols[] = {LEFT, RIGHT, UP, DOWN};

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); 
  lcd.createChar(LEFT, leftArrow);
  lcd.createChar(RIGHT, rightArrow);
  lcd.createChar(UP, upArrow);
  lcd.createChar(DOWN, downArrow);  
  lcd.createChar(CHOOSER, menuSelector);
  startTime = millis();
}

int menu() {
  //This function creates the main menu. It has two options - "Practice" and "Story mode". 
  //Player navigates using up and down buttons. Returns 1 if the chosen option is "Practice" 
  //and 2 if the chosen option is "Story mode".
  lcd.setCursor(4,0);
  lcd.print("Practice");
  lcd.setCursor(3,1);
  lcd.print("Story mode");
  int play_mode;
  while (true) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      currentTime = millis();
      if (currentTime - startTime >= period) {
        if (buttons & BUTTON_DOWN) {
          //Press down button to select "Story mode"
          lcd.setCursor(2,0);
          lcd.print("  Practice");
          lcd.setCursor(1,1);
          lcd.print("\x04 Story mode");
          play_mode = 2;
        } else if (buttons & BUTTON_UP) {
          //Press up button to select "Practice"
          lcd.setCursor(1,1);
          lcd.print("  Story mode");
          lcd.setCursor(2,0);
          lcd.print("\x04 Practice");
          play_mode = 1;
        } else if (buttons & BUTTON_SELECT) {
          //On select button pressed, the player has made a choice, so the game proceeds to the next step.
          delay(150);
          break;
        }
        startTime = currentTime;
      }   
    }    
  }
  return play_mode; //Returns either 1 or 2 depending on chosen option
}

int difficulty_level() {
  //This function creates a menu to choose the difficulty level for story mode. 
  //Player navigates using left and right buttons. Returns 0 if chosen difficulty is "Beginner",
  //1 if it's "Advanced" and 2 if it's "Expert".
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Difficulty level");
  String levels[3] = {"BEGINNER", "ADVANCED", "EXPERT"};
  int lvl = 0;
  int difficulty_level = lvl;
  lcd.setCursor(4,1);
  lcd.print(levels[lvl]);
  while (true) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      currentTime = millis();
      if (currentTime - startTime >= period) {
        if (buttons & BUTTON_RIGHT) {
          if (lvl < 2) {
            lvl++;
            difficulty_level = lvl;
            if (lvl == 2) {
              lcd.setCursor(4,1);
              lcd.print("        ");
              lcd.setCursor(5,1);
              lcd.print(levels[lvl]);
            } else {
              lcd.setCursor(4,1);
              lcd.print(levels[lvl]);
            }          
            lcd.setCursor(0,0);
            lcd.print("Difficulty level");
          }     
        } if (buttons & BUTTON_LEFT) {
          if (lvl > 0) {
            lvl--;
            difficulty_level = lvl;
            if (lvl == 2) {
              lcd.setCursor(4,1);
              lcd.print("        ");
              lcd.setCursor(5,1);
              lcd.print(levels[lvl]);
            } else {
              lcd.setCursor(4,1);
              lcd.print(levels[lvl]);
            } 
            lcd.setCursor(0,0);
            lcd.print("Difficulty level");
          }
        } else if (buttons & BUTTON_SELECT) {
          delay(150);
          break;
        }
        startTime = currentTime;
      }
    }
  }
  return difficulty_level; //Returns 0 (beginner), 1 (advanced) or 2 (expert)
}

int set_sequence_length() {
  //This function creates the menu to set the length (number of symbols to display)
  //for the sequence in practice mode. Minimum value is 4 and maximum value is 10. 
  //Player navigates using left and right buttons.
  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("Set length");
  lcd.setCursor(7,0);
  int s_length = 4;
  lcd.print(s_length);
  while (true) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      currentTime = millis();
      if (currentTime - startTime >= period) {
        if (buttons & BUTTON_RIGHT) {
          if (s_length < 10) {
            s_length++;
            if (s_length != 10) {
              lcd.setCursor(8,0);
              lcd.print(" ");
            }
            lcd.setCursor(7,0);
            lcd.print(s_length);
            lcd.setCursor(3,1);
            lcd.print("Set length");   
          }                     
        } else if (buttons & BUTTON_LEFT) {
            if (s_length > 4) {
              s_length--;
              if (s_length != 10) {
                lcd.setCursor(8,0);
                lcd.print(" ");
              }
              lcd.setCursor(7,0);
              lcd.print(s_length);
              lcd.setCursor(3,1);
              lcd.print("Set length");
            }                  
        } else if (buttons & BUTTON_SELECT) {
          delay(150);          
          break;
        }
        startTime = currentTime;
      }     
    }    
  }  
  return s_length; //Returns the number which the player chose for the sequence length
}

int set_sequence_size() {
  //This function creates the menu to set the size (number of symbols to choose from) 
  //for the sequence in practice mode. Minimum value is 2 (two symbols - left and right) 
  //and maximum value is 4 (all four symbols). Player navigates using left and right buttons.
  lcd.clear();
  lcd.setCursor(4,1);
  lcd.print("Set size");
  lcd.setCursor(7,0);
  int s_size = 2;
  lcd.print(s_size);
  while (true) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      currentTime = millis();
      if (currentTime - startTime >= period) {
        if (buttons & BUTTON_RIGHT) {
          if (s_size < 4) {
            s_size++;
            lcd.setCursor(7,0);
            lcd.print(s_size);
            lcd.setCursor(4,1);
            lcd.print("Set size");   
          }                     
        } else if (buttons & BUTTON_LEFT) {
            if (s_size > 2) {
              s_size--;
              lcd.setCursor(7,0);
              lcd.print(s_size);
              lcd.setCursor(4,1);
              lcd.print("Set size");              
            }                  
        } else if (buttons & BUTTON_SELECT) {
          delay(150);
          break;
        }
        startTime = currentTime;
      }          
    }   
  }
  return s_size; //Returns 2, 3 or 4
}

char* create_sequence(char sequence[], int sequence_length, int sequence_size, int timer) {
  //This function creates the sequence of symbols which the player needs to remember and then repeat.
  //Length, size and time between each symbol are set depending on level and difficulty. 
  //Takes an empty array, randomizes each symbol from the predefined array of characters containing all 4 symbols,
  //stores them in the array and then returns the array.
  for (int i = 0; i <= sequence_length - 1; i++) {
    lcd.setCursor(7,0);
    randomSeed(analogRead(0));
    char symbol = symbols[random(0,sequence_size)];
    sequence[i] = symbol;
    lcd.write(symbol);
    delay(timer);
    lcd.clear();
    delay(timer);
  }
  return sequence;
}

char* create_user_sequence(char user_sequence[], int sequence_length) {
  //This function creates the sequence of symbols which the player enters using the left (corresponds to left
  //arrow symbol), right (corresponds to right arrow symbol), up (corresponds to up arrow symbol) 
  //and down (corresponds to down arrow symbol) buttons. Length depends on level and difficulty. 
  //Takes an empty array, waits for user input as many times as the sequence is long, stores the corresponding
  //symbols in the array and returns it. 
  int i = 0;
  while (i <= sequence_length - 1) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      lcd.setCursor(7,0);
      if (buttons & BUTTON_LEFT) {
        user_sequence[i] = LEFT;
        lcd.write(LEFT);
        delay(500);
        lcd.clear();
        i++;  
      } else if (buttons & BUTTON_RIGHT) {
        user_sequence[i] = RIGHT;
        lcd.write(RIGHT);
        delay(500);
        lcd.clear();
        i++;
      } else if (buttons & BUTTON_UP) {
        user_sequence[i] = UP;
        lcd.write(UP);
        delay(500);
        lcd.clear();
        i++;
      } else if (buttons & BUTTON_DOWN) {
        user_sequence[i] = DOWN;
        lcd.write(DOWN);
        delay(500);
        lcd.clear();
        i++;
      }
    }
  }
  return user_sequence;
}

void play_practice_mode(int sequence_length, int sequence_size) {
  //Practice mode. Takes sequence length and size set by the player (set_sequence_length()
  //and set_sequence_size() functions). Creates and displays a sequence depending on these values,
  //waits for the player to repeat the sequence and then compares the two arrays - if they are the same,
  //displays "Good job" and returns to main menu. Otherwise displays "Try again" and returns to main menu.
  int timer = 1000;
  lcd.clear();
  delay(700);
  char sequence[sequence_length];
  char user_sequence[sequence_length];
  create_sequence(sequence, sequence_length, sequence_size, timer);
  create_user_sequence(user_sequence, sequence_length);
  int index = 0;
  bool areSame;
  while (index <= sequence_length - 1) {
   if (sequence[index] == user_sequence[index]) {
     index++;
     areSame = true;
   } else {
     areSame = false;
     break;
   }
  }
  if (areSame) {
   lcd.setCursor(4,0);
   delay(700);
   lcd.setBacklight(GREEN);
   lcd.print("Good job");
   delay(1000);
   lcd.setBacklight(WHITE);
   lcd.clear();
  } else {
   lcd.setCursor(3,0);
   delay(700);
   lcd.print("Try again");
   delay(1000);
   lcd.clear();
  }
}

void play_story_mode(int difficulty, int sequence_length, int sequence_size, int timer) {
  //Story mode. Takes the difficulty chosen by the player (beginner, advanced or expert), initial sequence
  //length, size and timer depending on the difficulty. Starting from level 1, displays a sequence, 
  //waits for player's turn to repeat the sequence and if the player is correct, proceeds to next level (number of 
  //levels is 10, level 10 being the most challenging). If the player enters an incorrect sequence, displays "Try again"
  //and returns to main menu.
  int level = 1; //Game starts at level 1 - easiest
  
  while (level <= 10) { //Game is 10 levels long
    lcd.clear();
    delay(700);

    //When the player reaches levels 5 and 8 accordingly, the display 
    //turns yellow and purple to indicate increasing difficulty
    if (level == 5) { 
      lcd.setBacklight(YELLOW);      
    }
    if (level == 8) { 
      lcd.setBacklight(VIOLET);
    }

    //Display level at the beginning of every round
    lcd.setCursor(4,0);
    lcd.print("Level");
    lcd.setCursor(10,0);
    lcd.print(level);
    delay(1200);
    lcd.clear();
    delay(700);

    //Creates and displays a sequence, after which waits for player to repeat it
    char sequence[sequence_length];
    char user_sequence[sequence_length];
    create_sequence(sequence, sequence_length, sequence_size, timer);
    create_user_sequence(user_sequence, sequence_length);

    //Checks if the game's sequence and the one that the player repeated are the same
    //and displays messages accordingly
    int index = 0;
    bool areSame;
    while (index <= sequence_length - 1) {
      if (sequence[index] == user_sequence[index]) {
        index++;
        areSame = true;
      } else {
        areSame = false;
        break;
      }
    }

    //If they are the same, the game proceeds to next level, becoming more challenging 
    //at specific levels by increasing sequence length, size and decreasing the timer between symbols
    //depending on difficulty 
    if (areSame) {
      lcd.setCursor(4,0);
      delay(700);
      lcd.setBacklight(GREEN);
      lcd.print("Good job");
      delay(1000);
      if (level >= 5 && level <8) {
        lcd.setBacklight(YELLOW);
      } else if (level >= 8) {
        lcd.setBacklight(VIOLET);
      } else {
        lcd.setBacklight(WHITE);
      }
      level++;

      //Specifies how and when the game becomes more challenging at beginner difficulty
      if (difficulty == 0) { 
        if ((level >= 3 && level <= 4) || (level >= 7 && level <= 8)) {
          //Initial length at beginner level: 4. 
          //Increase by one at levels 3, 4, 7 and 8; maximum length reached is 8
          sequence_length++; 
        }     
        if (level == 4 || level == 7) {
          //Initial size of sequence at beginner level is 2.
          //Increase  by one at levels 4 and 7 until maximum size (4) is reached.
          sequence_size++; 
        }
        if (level == 3 || level == 6 || level == 10) {
          //Initial timer: 1000. Decrease by 100 at levels 3, 6 and 10.
          timer = timer - 100;
        }      
      } else if (difficulty == 1) { 
        //Specifies how and when the game becomes more challenging at advanced difficulty      
          if ((level >= 3 && level <= 4) || (level >= 7 && level <= 9)) {
            //Initial length at advanced level: 5. Increase by one at levels
            //3, 4, 7, 8 and 9, making the maximum length reached 10.
            sequence_length++; 
          }
          if (level == 5) {
            //Initial size: 3. Increase size by one at level 5.
            sequence_size++;
          }
          if (level == 4 || level == 7) {
            //Initial timer: 850. Decrease by 150 at levels 4 and 7.
            timer = timer - 150;
          }
      } else if (difficulty == 2) { 
        //Specifies how and when the game becomes more challenging at expert difficulty
          if ((level >= 3 && level <= 5) || (level >= 8 && level <= 10)) {
            //Initial length at expert difficulty: 6. Increase by one at levels
            //3, 4, 5, 8, 9 and 10, making the maximum length reached 12.
            sequence_length++;
          }
          if (level == 5 || level == 8) {
            //Initial timer: 700. Decrease by 200 at levels 5 and 8.
            timer = timer - 200;
          }
      }
      if (level == 11) {
        //If the player successfully completed level 10 at any difficulty, display a winning screen.
        //When the player presses the select button, return to main menu.
        delay(700);
        lcd.setBacklight(GREEN);
        lcd.setCursor(0,0);
        lcd.print("CONGRATULATIONS!");
        lcd.setCursor(4,1);
        lcd.print("YOU WON!");
        while (true) {
          uint8_t buttons = lcd.readButtons();
          if (buttons) {
            if (buttons & BUTTON_SELECT) {
              delay(150);
              break;
            }
          }
        }
      }
    } else {
      //If the sequence the player entered and the one dislayed by the game are not the same, 
      //display a losing screen and return to the main menu.
      lcd.setCursor(3,0);
      delay(700);
      lcd.print("Try again");
      delay(1000);
      break;
    }
  }
}

void loop() {
  lcd.setBacklight(WHITE);
  lcd.clear();

  //Display main menu and wait for player's choice of game mode.
  int menu_choice = menu();

  //The player has chosen pracice mode.
  if (menu_choice == 1) {
    int sequence_length = set_sequence_length(); //Player sets the length they want to practice
    int sequence_size = set_sequence_size(); //Player sets size
    delay(500);
    lcd.clear();
    play_practice_mode(sequence_length, sequence_size); //Play practice mode, taking the values the player has set
  } else if (menu_choice == 2) { //The player has chosen story mode.
      int difficulty = difficulty_level(); //Choose difficulty level
      if (difficulty == 0) { //If player has chosen beginner difficulty
        int sequence_length = 4;
        int sequence_size = 2;
        int timer = 1000;
        play_story_mode(difficulty, sequence_length, sequence_size, timer);
      } else if (difficulty == 1) { //If player has chosen advanced difficulty
          int sequence_length = 5;
          int sequence_size = 3;
          int timer = 850;
          play_story_mode(difficulty, sequence_length, sequence_size, timer);
      } else if (difficulty == 2) { //If player has chosen expert difficulty
          int sequence_length = 6;
          int sequence_size = 4;
          int timer = 700;
          play_story_mode(difficulty, sequence_length, sequence_size, timer);
      }
  }
}
