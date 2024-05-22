void setup() {
  Serial.begin(9600); // Rozpoczynamy komunikację szeregową na prędkości 9600 bps
}

void loop() {
  if (Serial.available()) { // Jeżeli są dostępne dane do odczytu
    char receivedChar = Serial.read(); // Odczytaj odebrane dane

    if (receivedChar == 'H') { // Jeżeli otrzymaliśmy zapytanie o "H"
      Serial.println("Hello from Arduino!"); // Wysyłamy odpowiedź "Hello from Arduino!"
    }
  }
}