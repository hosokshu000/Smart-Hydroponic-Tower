#define TdsSensorPin 26  // ADC0 (GP26)
#define VREF 3.3         // Reference voltage (3.3V for RP Pico)
#define SCOUNT 30        // Number of samples

int analogBuffer[SCOUNT];   // Store ADC readings
int analogBufferTemp[SCOUNT];

int bufferIndex = 0;
float averageVoltage = 0;
float tdsValue = 0;
float temperature = 25;  // Default temperature (modify as needed)

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);  // Set ADC resolution to 12 bits
}

void loop() {
    // Read samples
    for (int i = 0; i < SCOUNT; i++) {
        analogBuffer[i] = analogRead(TdsSensorPin);
        delay(10);
    }

    // Sort samples and take the median for stability
    for (int i = 0; i < SCOUNT - 1; i++) {
        for (int j = i + 1; j < SCOUNT; j++) {
            if (analogBuffer[i] > analogBuffer[j]) {
                int temp = analogBuffer[i];
                analogBuffer[i] = analogBuffer[j];
                analogBuffer[j] = temp;
            }
        }
    }

    // Calculate the median
    int medianIndex = SCOUNT / 2;
    int medianValue = analogBuffer[medianIndex];

    // Convert ADC value to voltage
    averageVoltage = VREF * (medianValue / 4095.0);  // 12-bit ADC (0-4095)

    // Convert voltage to TDS using the CQRobot formula
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage -

                255.86 * compensationVoltage * compensationVoltage +

                857.39 * compensationVoltage) * 0.5;  // 0.5 is the K-value

    Serial.print("TDS Value: ");
    Serial.print(tdsValue);
    Serial.println(" ppm");
}
