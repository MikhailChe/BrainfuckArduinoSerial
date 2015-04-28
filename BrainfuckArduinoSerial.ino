uint16_t memPointer = 0;
char bfmem[512];

uint16_t codePointer = 0;
char bfcode[512];

bool debug = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  prompt();
  String s = readCommand();
  executeCommand(s);
}

void prompt() {
  Serial.print("brainfuck>");
}

String readCommand() {
  String input;
  char buf = 0;
  while (true) {
    if (Serial.available()) {
      buf = Serial.read();
      if (buf == '\r') {
        Serial.print(buf);
        break;
      } else if (buf == 127) {
        if (input.length() > 0) {
          Serial.print(buf);
          input = input.substring(0, input.length() - 1);
        }
      } else {
        input += buf;
        Serial.print(buf);

      }
    }
  }
  Serial.flush();
  input.trim();
  input.toLowerCase();
  Serial.println();
  return input;
}
void executeCommand(String str) {
  if (str.length() > 0) {
    if (str.equals("load")) {
      loadProgram();
    } else if (str.equals("exec")) {
      execProgram();
    } else if (str.equals("reset")) {
      resetmem();
      resetcode();
      memPointer = 0;
      codePointer = 0;
    } else if (str.equals("resetmem")) {
      resetmem();
    } else if (str.equals("resetcode")) {
      resetcode();
    } else if (str.equals("resetptr")) {
      memPointer = 0;
      codePointer = 0;
    } else if (str.equals("step")) {
      brainfuckStep();
    } else  if (str.equals("debug")) {
      debug = !debug;
      if (debug) {
        Serial.println("Debug mode is on");
      } else {
        Serial.println("Debug mode is off");
      }
    } else {
      Serial.println("Unknown command: \"" + str + "\". Try load or exec.");
    }
  }
}

void resetmem() {
  for (int i = 0; i < 512; i++) {
    bfmem[i] = 0;
  }
}

void resetcode() {
  for (int i = 0; i < 512; i++) {
    bfcode[i] = 0;
  }
}
void loadProgram() {
  Serial.println("Waiting for program code. Ctrl+c to finish");
  char buf = 0;
  codePointer = 0;
  while (codePointer < 512) {
    while (!Serial.available());
    buf = Serial.read();
    if (buf == 3) {
      Serial.print(buf);
      break;
    } else if (buf == '\r') {
      Serial.print(buf);
      Serial.println();
    } else if (buf == 127) {
      codePointer--;
    } else if ((buf == '<') || (buf == '>') || (buf == '+') || (buf == '-') || (buf == '[') || (buf == ']')  || (buf == '.') || (buf == ',')) {
      bfcode[codePointer] = buf;
      codePointer++;
      Serial.print(buf);
    }
  }
  Serial.println();
  Serial.println("Here is your program:");
  codePointer = 0;
  for (codePointer = 0; bfcode[codePointer] && codePointer < 512; codePointer++) {
    Serial.print(String(bfcode[codePointer]));
  }
  Serial.println();
}

void execProgram() {
  codePointer = 0;
  memPointer = 0;
  Serial.println();
  Serial.flush();
  brainfuckRun();
  Serial.println();
  Serial.flush();
}



void brainfuckDebufInfo() {
  Serial.print("\r\nIP: " + String(codePointer) + "\tDP:" + String(memPointer) + "\r\n" + String(bfcode[codePointer]) + "\t" + String((int)(bfmem[memPointer])));
  if (bfmem[memPointer] >= 0x20  && bfmem[memPointer] < 0x7F) {
    Serial.println("\t" + String(bfmem[memPointer]));
  } else {
    Serial.println();
  }
}

void brainfuckStep() {
  char bfcmd = bfcode[codePointer];
  codePointer++;
  switch (bfcmd) {
    case '>':
      memPointer++;
      break;
    case '<':
      memPointer--;
      break;
    case '+':
      bfmem[memPointer]++;
      break;
    case '-':
      bfmem[memPointer]--;
      break;
    case '.':
      Serial.print(bfmem[memPointer]);
      break;
    case ',':
      {
        while (!Serial.available());
        bfmem[memPointer] = Serial.read();
        break;
      }
    case '[':
      if (bfmem[memPointer] == 0) {
        int whilecp = codePointer;
        int level = 0;
        while (bfcode[whilecp]) {
          if (bfcode[whilecp] == ']') {
            if (level == 0) {
              break;
            } else {
              level--;
            }
          } else if (bfcode[whilecp] == '[') {
            level++;
          }
          whilecp++;
        }
        if (level == 0) {
          codePointer = whilecp;
        }
      }
      break;
    case ']':
      if (bfmem[memPointer] != 0) {
        int whilecp = codePointer - 2;
        int level = 0;
        while (whilecp) {
          if (bfcode[whilecp] == '[') {
            if (level == 0) {
              break;
            } else {
              level--;
            }
          } else if (bfcode[whilecp] == ']') {
            level++;
          }
          whilecp--;
        }
        if (level == 0) {
          codePointer = whilecp;
        }
      }
      break;
  }
  if (debug) {
    brainfuckDebufInfo();
  }
}
void brainfuckRun() {
  while (bfcode[codePointer]) {
  if (Serial.read() == 3)
    return;
    brainfuckStep();
  }
}
