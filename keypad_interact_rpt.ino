#include <Keyboard.h>
#include <KeyboardLayout.h>
//included to turn off onboard neopixel
#include <Adafruit_DotStar.h>

// Include EEPROM-like API for FlashStorage
#include <FlashAsEEPROM.h>

#define DLY 5 
#define NUMPIXELS 1 // Number of LEDs in strip

// neopixel control pins
#define DATAPIN    7
#define CLOCKPIN   8

//zero indicates no key was pressed
#define NO_KEY 0

//hold time (until repeat starts)
#define HOLD 800
//repeat interval
#define REPEAT 20

//program mode states
#define SELKEY 0 //prompt to select a key to program
#define SELTYP 1 //prompt to select a character type
#define SELLTR 2 //select a letter
#define SELSYM 3 //select a symbol
#define PROG 4 //save/done/do another
#define EXIT 5 //exit program mode

//current state for program screens
char state=SELKEY;

//indices (from altCodes array) of the characters mapped to the keys 
uint8_t keyIndices[6] = {52,51,32,141,114,87}; //en dash, em dash, degree, superscript 2, pi, multiplication


//big list of alt key codes for special characters (comments are indices)
String altCodes[170][2] =
{
  {"160","a with Acute"}, //0
  {"0193","A with Acute"}, //1
  {"131","a with Circumflex"}, //2
  {"0194","A with Circumflex"}, //3
  {"132","a with Diaeresis"}, //4
  {"142","A with Diaeresis"}, //5
  {"133","a with Grave"}, //6
  {"0192","A with Grave"}, //7
  {"134","a with Ring"}, //8
  {"143","A with Ring"}, //9
  {"0227","a with Tilde"}, //10
  {"0195","A with Tilde"}, //11
  {"0180","Acute"}, //12
  {"145","ae"}, //13
  {"146","AE"}, //14
  {"247","Almost Equal To"}, //15
  {"224","alpha"}, //16
  {"14","Beamed Eighth Notes"}, //17
  {"225","Beta"}, //18
  {"254","Black Square"}, //19
  {"245","Bottom Half Integral"}, //20
  {"0166","Broken Bar"}, //21
  {"7","Bullet"}, //22
  {"135","c with Cedilla"}, //23
  {"128","C with Cedilla"}, //24
  {"0184","Cedilla"}, //25
  {"155","Cent"}, //26
  {"0136","Circumflex"}, //27
  {"5","Club"}, //28
  {"0169","Copyright"}, //29
  {"0164","Currency"}, //30
  {"0134","Dagger"}, //31
  {"248","Degree"}, //32
  {"235","Delta"}, //33
  {"0168","Diaeresis"}, //34
  {"4","Diamond"}, //35
  {"246","Division"}, //36
  {"0135","Double Dagger"}, //37
  {"19","Double Exclamation"}, //38
  {"0132","Double Low Quote"}, //39
  {"25","Down Arrow"}, //40
  {"130","e with Acute"}, //41
  {"144","E with Acute"}, //42
  {"136","e with Circumflex"}, //43
  {"0202","E with Circumflex"}, //44
  {"137","e with Diaeresis"}, //45
  {"0203","E with Diaeresis"}, //46
  {"138","e with Grave"}, //47
  {"0200","E with Grave"}, //48
  {"13","Eighth Note"}, //49
  {"0133","Ellipsis"}, //50
  {"0151","Em Dash"}, //51
  {"0150","En Dash"}, //52
  {"238","Epsilon"}, //53
  {"0223","eszett"}, //54
  {"0240","eth"}, //55
  {"0208","Eth"}, //56
  {"0128","Euro"}, //57
  {"12","Female"}, //58
  {"159","florin"}, //59
  {"226","Gamma"}, //60
  {"242","Greater-Than or Equal To"}, //61
  {"3","Heart"}, //62
  {"161","i with Acute"}, //63
  {"0205","I with Acute"}, //64
  {"140","i with Circumflex"}, //65
  {"0206","I with Circumflex"}, //66
  {"139","i with Diaeresis"}, //67
  {"0207","I with Diaeresis"}, //68
  {"141","i with Grave"}, //69
  {"0204","I with Grave"}, //70
  {"240","Identical To"}, //71
  {"236","Infinity"}, //72
  {"239","Intersection"}, //73
  {"173","Inverted Exclamation"}, //74
  {"168","Inverted Question"}, //75
  {"27","Left Arrow"}, //76
  {"174","Left Double Angle Quote"}, //77
  {"0147","Left Double Quote"}, //78
  {"0145","Left Single Quote"}, //79
  {"29","Left-Right Arrow"}, //80
  {"243","Less-Than or Equal To"}, //81
  {"0175","Macron"}, //82
  {"11","Male"}, //83
  {"250","Middle Dot"}, //84
  {"0181","mu"}, //85
  {"230","Mu"}, //86
  {"0215","Multiplication"}, //87
  {"164","n with Tilde"}, //88
  {"165","N with Tilde"}, //89
  {"0160","No-Break Space"}, //90
  {"170","Not"}, //91
  {"162","o with Acute"}, //92
  {"0211","O with Acute"}, //93
  {"147","o with Circumflex"}, //94
  {"0212","O with Circumflex"}, //95
  {"148","o with Diaeresis"}, //96
  {"153","O with Diaeresis"}, //97
  {"149","o with Grave"}, //98
  {"0210","O with Grave"}, //99
  {"0248","o with Stroke"}, //100
  {"0216","O with Stroke"}, //101
  {"0245","o with Tilde"}, //102
  {"0213","O with Tilde"}, //103
  {"0156","oe"}, //104
  {"0140","OE"}, //105
  {"234","Omega"}, //106
  {"171","One Half"}, //107
  {"172","One Quarter"}, //108
  {"20","Paragraph"}, //109
  {"0137","Per Mille"}, //110
  {"158","Peseta"}, //111
  {"237","phi"}, //112
  {"232","Phi"}, //113
  {"227","pi"}, //114
  {"241","Plus minus"}, //115
  {"31","Point Down"}, //116
  {"17","Point Left "}, //117
  {"16","Point Right"}, //118
  {"30","Point Up"}, //119
  {"156","Pound"}, //120
  {"0174","Registered"}, //121
  {"169","Reversed Not"}, //122
  {"28","Right Angle"}, //123
  {"26","Right Arrow"}, //124
  {"175","Right Double Angle Quote"}, //125
  {"0148","Right Double Quote"}, //126
  {"0146","Right Single Quote"}, //127
  {"0154","s with Caron"}, //128
  {"0138","S with Caron"}, //129
  {"21","Section"}, //130
  {"229","sigma"}, //131
  {"228","Sigma"}, //132
  {"0139","Single Left Angle Quote"}, //133
  {"0130","Single Low Quote"}, //134
  {"0155","Single Right Angle Quote"}, //135
  {"0152","Small Tilde"}, //136
  {"0173","Soft Hyphen"}, //137
  {"6","Spade"}, //138
  {"251","Square Root"}, //139
  {"0185","Superscript 1"}, //140
  {"253","Superscript 2"}, //141
  {"0179","Superscript 3"}, //142
  {"0170","Superscript a"}, //143
  {"252","Superscript n"}, //144
  {"231","Tau"}, //145
  {"233","Theta"}, //146
  {"0254","thorn"}, //147
  {"0222","Thorn"}, //148
  {"0190","Three Quarters"}, //149
  {"244","Top Half Integral"}, //150
  {"0153","Trade Mark"}, //151
  {"163","u with Acute"}, //152
  {"0218","U with Acute"}, //153
  {"150","u with Circumflex"}, //154
  {"0219","U with Circumflex"}, //155
  {"0252","u with Diaeresis"}, //156
  {"0220","U with Diaeresis"}, //157
  {"151","u with Grave"}, //158
  {"0217","U with Grave"}, //159
  {"24","Up Arrow"}, //160
  {"23","Up Down Arrow with Base"}, //161
  {"18","Up-Down Arrow"}, //162
  {"0253","y with Acute"}, //163
  {"0221","Y with Acute"}, //164
  {"152","y with Diaeresis"}, //165
  {"0159","Y with Diaeresis"}, //166
  {"157","Yen"}, //167
  {"0158","z with Caron"}, //168
  {"0142","Z with Caron"}, //169
};

//indices (from altCodes array) of letter characters
uint8_t letterIndices[85] = {0,1,2,3,4,5,6,7,8,9,10,11,13,14,16,18,23,24,33,41,42,43,44,45,46,47,48,54,55,56,60,63,64,65,66,67,68,69,70,85,86,88,89,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,112,113,114,128,129,131,132,143,144,145,146,147,148,152,153,154,155,156,157,158,159,163,164,165,166,168,169};
//indices (from altCodes array) for symbol character
uint8_t symbolIndices[87] = {12,15,17,20,22,25,26,27,28,29,30,31,32,34,35,36,37,38,39,40,49,50,51,52,53,57,58,59,61,62,71,72,73,74,75,76,77,78,79,80,81,82,83,84,87,88,89,90,91,107,108,109,110,111,115,116,117,118,119,120,121,122,123,124,125,126,127,130,133,134,135,136,137,138,139,140,141,142,143,144,149,150,151,160,161,162,167};

//setup for keypad
const byte ROWS = 3; //three rows
const byte COLS = 2; //two columns
char keys[ROWS][COLS] = //pseudo keyCodes for looking up alt sequences
{
  {1,2},
  {3,4},
  {5,6}
};

//time of last key hold; used for repeat
unsigned long lastHold=millis();
//time of last key repeat; used for repeat
unsigned long lastRepeat=millis();
//last key pressed
char lastKey=NO_KEY;
//key to update in program mode
char keyToProgram=NO_KEY;

//prompt for key selection
String selKeyMenu="Make sure numlock is on and press the key you want to program\n";
//character type selection prompt
String selTypeMenu="What type of character do you need?\n"
"[letter][symbol]\n"
"[      ][      ]\n"
"[back  ][exit  ]\n";
//character selection prompt
String selCharMenu="Select a character.\n"
"[prev  ][next]\n"
"[select][    ]\n"
"[back  ][exit]\n\n";
//character selection prompt
String progMenu="What's next?\n"
"[another][    ]\n"
"[       ][    ]\n"
"[save   ][done]\n"
"another=select another key to program\n"
"save=write to flash\n"
"done=keep changes but don't write to flash\n";

//keypad numeric keycodes
char kpLookup[10] = {KEY_KP_0,KEY_KP_1,KEY_KP_2,KEY_KP_3,KEY_KP_4,KEY_KP_5,KEY_KP_6,KEY_KP_7,KEY_KP_8,KEY_KP_9};

//pins for rows and columns; rearrange as needed to match your wiring
byte rowPins[ROWS] = {2, 1, 0}; //connect to the row pinouts of the keypad  (top, middle, bottom)
byte colPins[COLS] = {4, 3}; //connect to the column pinouts of the keypad (left, right)

//neopixel (used to turn off onboard neopixel)
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

//initialize all the things
void setup() 
{
  // Initialize all pixels to 'off'
  strip.begin();
  strip.setPixelColor(0,0,0,0);
  strip.show(); 

  if (!EEPROM.isValid()) //initialize flash after compile
  {
    writeToFlash();
  }
  else //read stored values on powerup or reset
  {
    for(uint8_t i=0;i<sizeof(keyIndices);i++)
    {
      keyIndices[i]=EEPROM.read(i);
    }
  }

  Keyboard.begin(); //init keyboard object
}

//do forever
void loop() 
{
  char keyCode=NO_KEY;
  while(keyCode==NO_KEY) //wait for keypress
  {
    keyCode=getKeyCode();
  }
  switch(keyCode)
  {
    case 11: //if bottom 2 keys pressed, toggle numlock
      Keyboard.write(KEY_NUM_LOCK);
      lastKey=11;
      break;
    case 7: //if middle 2 keys pressed, switch to program mode
      programMode();
      lastKey=7;
      break;
    case 1: //regular keypress
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      typeAltCode(keyIndices[keyCode-1]); //look up the alt code index for the pressed key and send the corresponding alt sequence
      break;
    default:
      break;
  }
}

//key program mode
void programMode()
{
  boolean exit=false;
  state=SELKEY;
  while(!exit) //program mode loop
  {
    switch (state) 
    {
      case SELKEY: //select key character
        selKey();
        break;
      case SELTYP: //select character type
        selType();
        break;
      case SELLTR: //select a letter character
        selectCharacter(letterIndices,sizeof(letterIndices));
        break;
      case SELSYM: //select a symbol character
        selectCharacter(symbolIndices,sizeof(symbolIndices));
        break;
      case PROG: //save the character(s)
        prog();
        break;
      case EXIT: //exit programming mode
        exit=true;
        break;
      default:
        break;
    }
    if(state==EXIT)
    {
      cls();
      exit=true;
      lastKey=getKeyCode();
      lastHold=millis();
      delay(500);
    }
  }
}

//select key to program
void selKey()
{
  cls(); //clear screen
  Keyboard.print(selKeyMenu); //type selection prompt
  char keyCode=NO_KEY;
  while(keyCode==NO_KEY)
  {
    keyCode=getKeyCode(); 
  }
  if(keyCode==7) //exit on 2 middle keys pressed
  {
    cls();
    state=EXIT;
  }
  else //otherwise, select key to program
  {
    keyToProgram=keyCode-1;
    state=SELTYP;
  }
}

//select character type
void selType()
{
  cls();
  Keyboard.print(selTypeMenu);
  char keyCode=NO_KEY;
  while(keyCode==NO_KEY)
  {
    keyCode=getKeyCode();
  }
  switch(keyCode)
  {
    case 7:
      state=EXIT;  //exit on 2 middle keys pressed
      break;
    case 1:
      state=SELLTR;  //go to the letter selection prompt
      break;
    case 2:
      state=SELSYM;  //go to the symbol selection prompt
      break;
    case 5:
      state=SELKEY;  //go back to the key selection prompt
      break;
    case 6:
      state=EXIT;  //exit program mode
      break;
    default:
      break;
  }
}

//select a character from the list (letters or symbols)
void selectCharacter(uint8_t indices[],uint8_t len)
{
  uint8_t index=0;
  cls();
  Keyboard.print(selCharMenu);
  char keyCode=NO_KEY;
  boolean selecting=true;
  while(selecting)
  {
    writeChrDescription(indices[index]);
    while(keyCode==NO_KEY)
    {
      keyCode=getKeyCode();
    }
    switch(keyCode)
    {
      case 7: //exit on 2 middle keys pressed
        state=EXIT;
        break;
      case 1: //previous character
      index--;
        if(index==255) index=len-1;
        break;
      case 2: //next character
        index=(index+1)%len;
        break;
      case 3: //select this character
        keyIndices[keyToProgram]=indices[index];
        state=PROG;
        selecting=false;
        break;
      case 5: //back to select type
        state=SELTYP;
        selecting=false;
        break;
      case 6: //exit programming mode
        state=EXIT;
        selecting=false;
        break;
      default:
        break;
    }
    keyCode=NO_KEY;
  }
}

//save selected characters
void prog()
{
  cls();
  Keyboard.print(progMenu);
  char keyCode=NO_KEY;
  boolean selecting=true;
  while(keyCode==NO_KEY)
  {
    keyCode=getKeyCode();
  }
  switch(keyCode)
  {
    case 7:
      state=EXIT; //exit on 2 middle keys pressed
      break;
    case 1:
      state=SELKEY;  //select another key to program
      break;
    case 5:  //write all keys to flash
      cls();
      Keyboard.print("Saving to flash\n");
      writeToFlash();
      Keyboard.print("Done\n");
      delay(2000);
      break;
    case 6:  //exit - keys are programmed in RAM but not written to flash
      state=EXIT;
      break;
    default:
      break;
  }
  keyCode=NO_KEY;
}

//relace current line with character followed by a description
void writeChrDescription(uint8_t index)
{
  clr();
  typeAltCode(index);
  Keyboard.print("  ");
  Keyboard.print(altCodes[index][1]);
}

//get the key code from the keypad (0-11)
char getKeyCode()
{
  char keyCode=0;
  delay(100);
  //set all column pins to input
  for(int c=0;c<COLS;c++)
  {
    pinMode(colPins[c],INPUT_PULLUP);
  }
  //set all rows to output high
  for(int r=0;r<ROWS;r++)
  {
    pinMode(rowPins[r],OUTPUT);
    digitalWrite(rowPins[r],HIGH);
  }
  //cycle through rows, dropping one at a time to LOW
  for(int r=0;r<ROWS;r++)
  {
    digitalWrite(rowPins[r],LOW);
    //cycle through columns, reading state
    for(int c=0;c<COLS;c++)
    {
      if(digitalRead(colPins[c])==LOW)
      {
        keyCode+=keys[r][c];
      }
    }
    digitalWrite(rowPins[r],HIGH);
  }
  //if key is held down, start repeat
  if(keyCode==lastKey)
  {
    if(millis()>=lastHold+HOLD)
    {
      if(millis()>=lastRepeat+REPEAT)
      {
        lastRepeat=millis();
        return keyCode;
      }
    }
    return NO_KEY;
  }
  lastHold=millis();
  lastKey=keyCode;
  return keyCode;
}



//press alt, type alt sequence, release keys
void typeAltCode(uint8_t index)
{
  String seq=altCodes[index][0];
  Keyboard.press(KEY_LEFT_ALT);  //hold alt
  delay(DLY);
  for(uint8_t i=0;seq[i]!=0;i++) //type alt code on num pad
  {
    Keyboard.write(kpLookup[seq[i]-48]);
  } 
  Keyboard.release(KEY_LEFT_ALT); //release alt
}

//clear screen
void cls()
{
  //end of page
  Keyboard.press(KEY_LEFT_CTRL); //hold ctrl
  delay(DLY);
  Keyboard.write(KEY_END); //type end
  Keyboard.release(KEY_LEFT_CTRL); //relese ctrl
  delay(DLY);
  //select to beginning of page
  Keyboard.press(KEY_LEFT_CTRL); //hold ctrl+shift
  Keyboard.press(KEY_LEFT_SHIFT);
  delay(DLY);
  Keyboard.write(KEY_HOME); //type home
  Keyboard.release(KEY_LEFT_SHIFT); //release ctrl+shift
  Keyboard.release(KEY_LEFT_CTRL); 
  delay(DLY);
  //delete selection
  Keyboard.write(KEY_DELETE);
  delay(DLY);
}

//clear line
void clr()
{
  Keyboard.write(KEY_END); //end of line
  delay(DLY);
  //select to beginning of line
  Keyboard.press(KEY_LEFT_SHIFT); 
  delay(DLY);
  Keyboard.write(KEY_HOME);
  Keyboard.release(KEY_LEFT_SHIFT);
  delay(DLY);
  //delete selection
  Keyboard.write(KEY_DELETE);
  delay(DLY);
}

//store codes in flash
void writeToFlash()
{
  for(uint8_t i=0;i<sizeof(keyIndices);i++)
  {
    EEPROM.write(i, keyIndices[i]);
  }
  EEPROM.commit();
}

