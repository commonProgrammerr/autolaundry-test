#include <WiFi.h>
#include <HTTPClient.h>

#define wifi_ssid "TRON ST CORP"
#define wifi_password "@tst2023#root00!!"
#define log_url "http://192.168.9.69:3000/api/log"

#define buzzer 14 // 14
#define buzzerCh 0

#define led 3    // 45
#define jumper 3 // 45

#define flushButton 4  // 15 // 12
#define flushOutput 13 // 9 // 21

// entradas de contato em tensão
#define inputPinWetContact1 19 // 7
#define inputPinWetContact2 20 // 15
#define inputPinWetContact3 40 // 16
#define inputPinWetContact4 41 // 19

#define buttonPin1 5  // 4 // 11
#define buttonPin2 6  // 5 // 10
#define buttonPin3 7  // 6 // 3
#define buttonPin4 15 // 7 // 20

#define pump1 12 // 13 // 1
#define pump2 11 // 12 // 2
#define pump3 10 // 11 // 38
#define pump4 9  // 10 // 37

typedef struct
{
  byte in;
  byte out;
} Out2In_t;

// check expected routine
bool input_check(byte port, byte expect) { return digitalRead(port) == expect; }

void test_InOut(Out2In_t *tests)
{
  int pass_count = 0;
  // const int total_tests = static_cast<int>(tests.size);
  const int total_tests = 4;
  String test_log = "";

  Serial.println("Starting tests routine: ");
  Serial.println(total_tests);

  for (byte i = 0; i < total_tests; i++)
  {
    // In-Out1:OK In-Out2:OK
    pinMode(tests[i].in, INPUT_PULLUP);
    pinMode(tests[i].out, OUTPUT);
    digitalWrite(tests[i].out, HIGH);
    delay(100);
    bool test_result = input_check(tests[i].in, HIGH);
    digitalWrite(tests[i].out, LOW);

    if (test_result)
      pass_count++;

    Serial.print("In-Out");
    Serial.print(i + 1);
    Serial.println(test_result ? ":OK " : ":FAIL ");
    test_log = ((test_log + "In-Out") + (i + 1)) + (test_result ? ":OK " : ":FAIL ");
  }

  HTTPClient http;
  http.begin(log_url);
  http.addHeader("test_number", String(total_tests));
  http.addHeader("pass_number", String(pass_count));

  String mac = WiFi.macAddress();
  mac.replace(":", "");
  http.addHeader("mac", mac);
  int httpResponseCode = http.POST(test_log);
  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    Serial.print("HTTP Response ");
    Serial.println(payload);
    Serial.print("HTTP "); //+"code"
  }
  else
  {
    Serial.println();
    Serial.print("HTTP Error "); //+"code"
  }
  Serial.print("code: ");
  Serial.println(httpResponseCode);
  http.end();
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(wifi_ssid, wifi_password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Out2In_t tests[] =
      {
          {.in = inputPinWetContact1, .out = pump1},
          {.in = inputPinWetContact2, .out = pump2},
          {.in = inputPinWetContact3, .out = pump3},
          {.in = inputPinWetContact4, .out = pump4},
      };

  test_InOut(tests);
}

void loop()
{
  Serial.println("FIM");
  while (1)
    ;
}