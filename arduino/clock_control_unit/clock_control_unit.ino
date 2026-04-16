// -------- CLOCK PINS ----------
#define ASTABLE 13        

// -------- CONTROL SIGNALS -----
#define CO 3   // PC -> Bus
#define RO 4   // RAM -> Bus
#define IO 5   // IR -> Bus
#define AO 6   // A -> Bus
#define EO 7   // ALU -> Bus

#define MI 8   // Bus -> MAR
#define RI 9   // Bus -> RAM
#define II 10  // Bus -> IR
#define AI 11  // Bus -> A
#define BI 12  // Bus -> B
#define OI A0  // Bus -> Output

#define CE A1  // PC increment
#define SU A2 // ALU subtraction
#define HLT A3 // Halt clock

// ------ OPCODE ------

#define OP0 A7
#define OP1 A6
#define OP2 A5
#define OP3 A4   

//CONTROL UNIT VARIABLES
int step = 0;                  
static int isHalted = 0;  

//CLOCK VARIABLES
int astableDelay = 500;


void setup() {

  //CLOCK
  pinMode(ASTABLE, OUTPUT);    

  //CONTROL UNIT
  pinMode(CO, OUTPUT);
  pinMode(RO, OUTPUT);
  pinMode(IO, OUTPUT);
  pinMode(AO, OUTPUT);
  pinMode(EO, OUTPUT);

  pinMode(MI, OUTPUT);
  pinMode(RI, OUTPUT);
  pinMode(II, OUTPUT);
  pinMode(AI, OUTPUT);
  pinMode(BI, OUTPUT);
  pinMode(OI, OUTPUT);

  pinMode(CE, OUTPUT);
  pinMode(SU, OUTPUT);
  pinMode(HLT, OUTPUT);

  pinMode(OP0, INPUT);
  pinMode(OP1, INPUT);
  pinMode(OP2, INPUT);
  pinMode(OP3, INPUT);
}


void resetAll() {
  digitalWrite(CO, LOW);
  digitalWrite(RO, LOW);
  digitalWrite(IO, LOW);
  digitalWrite(AO, LOW);
  digitalWrite(EO, LOW);

  digitalWrite(MI, LOW);
  digitalWrite(RI, LOW);
  digitalWrite(II, LOW);
  digitalWrite(AI, LOW);
  digitalWrite(BI, LOW);
  digitalWrite(OI, LOW);

  digitalWrite(CE, LOW);
  digitalWrite(SU, LOW);
  digitalWrite(HLT, LOW);
}

//A6 and A7 cannot be directly taken as digital inputs
int getOpcodeBit(int pin) {
  if (pin == A6 || pin == A7) {
    return (analogRead(pin) > 512) ? 1 : 0;
  }
  return digitalRead(pin);
}

void loop() {
  // -------------------------
  // ASTABLE CLOCK (continuous)
  // -------------------------
  static int clockState = LOW;

  if (!isHalted)
  {  
    clockState = !clockState;
    digitalWrite(ASTABLE, clockState);
  }

  // -------------------------
  // CONTROL UNIT
  // -------------------------
  // change control signals every low pulse

  if (clockState == LOW && !isHalted) {

    resetAll();
    
    // Timing Steps:
    // T0 → MAR ← PC
    // T1 → IR ← RAM, PC++
    // T2+ → Instruction execution

    // ---- FETCH ----
    if (step == 0) {        // T0
      digitalWrite(CO, HIGH);
      digitalWrite(MI, HIGH);
    }

    if (step == 1) {        // T1
      digitalWrite(RO, HIGH);
      digitalWrite(II, HIGH);
      digitalWrite(CE, HIGH);
    }

    // read opcode from IR
    int opcode =
        (getOpcodeBit(OP3) << 3) |
        (getOpcodeBit(OP2) << 2) |
        (getOpcodeBit(OP1) << 1) |
        (getOpcodeBit(OP0));

    // ---- NOP ----
    if (opcode == 0b0000) {
      step = -1;
    }

    // ---- LDA ----
    if (opcode == 0b0001) {

      if (step == 2) {
        digitalWrite(IO, HIGH);
        digitalWrite(MI, HIGH);
      }

      if (step == 3) {
        digitalWrite(RO, HIGH);
        digitalWrite(AI, HIGH);
        step = -1;
      }
    }

    // ---- ADD ----
    if (opcode == 0b0010) {

      if (step == 2) {
        digitalWrite(IO, HIGH);
        digitalWrite(MI, HIGH);
      }

      if (step == 3) {
        digitalWrite(RO, HIGH);
        digitalWrite(BI, HIGH);
      }

      if (step == 4) {
        digitalWrite(EO, HIGH);
        digitalWrite(AI, HIGH);
        step = -1;
      }
    }

    // ---- SUB ----
    if (opcode == 0b0011) {

      if (step == 2) {
        digitalWrite(IO, HIGH);
        digitalWrite(MI, HIGH);
      }

      if (step == 3) {
        digitalWrite(RO, HIGH);
        digitalWrite(BI, HIGH);
      }

      if (step == 4) {
        digitalWrite(EO, HIGH);
        digitalWrite(AI, HIGH);
        digitalWrite(SU, HIGH);
        step = -1;
      }
    }

    // ---- STA ----
    if (opcode == 0b0100) {

      if (step == 2) {
        digitalWrite(IO, HIGH);
        digitalWrite(MI, HIGH);
      }

      if (step == 3) {
        digitalWrite(AO, HIGH);
        digitalWrite(RI, HIGH);
        step = -1;
      }
    }

    // ---- OUT ----
    if (opcode == 0b1110) {

      if (step == 2) {
        digitalWrite(AO, HIGH);
        digitalWrite(OI, HIGH);
        step = -1;
      }
    }

    // ---- HLT ----
    if (opcode == 0b1111) {
      digitalWrite(HLT, HIGH);
      isHalted = 1;
    }

    step = (step + 1) % 5;
  }
  delay(astableDelay); //clock cycle changes after CU                
}