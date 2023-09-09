// Define pins for encoder
#define ENCODER_A 2
#define ENCODER_B 3
#define ENCODER_SW 4

// Define variables for counting pulses and direction
volatile long count = 0; // Current count
volatile bool direction = true; // True for clockwise, false for counterclockwise

// Interrupt service routine for encoder A pin
void isr() {
  // Read state of both pins
  bool aState = digitalRead(ENCODER_A);
  bool bState = digitalRead(ENCODER_B);

  // Compare state of A and B pins to determine direction
  if (aState == bState) {
    // If A and B are equal, encoder is rotating clockwise
    direction = true;
    count++; // Increment count
  } else {
    // If A and B are different, encoder is rotating counterclockwise
    direction = false;
    count--; // Decrement count
  }
}

// Define filter coefficients
float b[] = {0.0055, -0.0018, -0.0148, -0.0239, -0.0206, 0.0055, 0.0428, 0.0776, 0.0928, 0.0776, 0.0428, 0.0055, -0.0206, -0.0239, -0.0148, -0.0018, 0.0055};

// Define filter order
int n = sizeof(b) / sizeof(float) - 1;

// Define buffer for past input samples
float x[n + 1] = {0};

// Define variable for filtered output
float y = 0;

void setup() {
  // Set pin modes for encoder
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  // Attach interrupt to encoder A pin
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), isr, CHANGE);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read encoder signal and store it in x[0]
  x[0] = analogRead(ENCODER_A);

  // Apply filter formula
  y = 0;
  for (int i = 0; i <= n; i++) {
    y += b[i] * x[i];
  }

  // Shift past input samples by one position
  for (int i = n; i > 0; i--) {
    x[i] = x[i - 1];
  }

  // Print current count and direction to serial monitor
  Serial.print("Count: ");
  Serial.print(count);
  Serial.print(" Direction: ");
  if (direction) {
    Serial.print("Clockwise");
  } else {
    Serial.print("Counterclockwise");
  }

   // Print filtered output to serial monitor
   Serial.print(" Filtered output: ");
   Serial.println(y);

   // Delay for readability
   delay(100);
}
