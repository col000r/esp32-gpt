#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "config.h"

const char *openAIEndpoint = "https://api.openai.com/v1/chat/completions";

// Initialize the display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset= */ U8X8_PIN_NONE, /* clock= */ 4, /* data= */ 5);

const int DISPLAY_WIDTH = 128; // Width of the display in pixels

void displayText(const char* text) {
    u8g2.begin();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);

        int y = 10;  // initial Y position
        int lineHeight = u8g2.getMaxCharHeight();
        String line = "";
        for (int i = 0; text[i] != '\0'; ) {
            String word = "";
            while (text[i] != ' ' && text[i] != '\0') {
                word += text[i];
                i++;
            }
            i++; // skip the space or move to the next character after '\0'

            if (u8g2.getStrWidth((line + word).c_str()) > DISPLAY_WIDTH) {
                u8g2.drawUTF8(0, y, line.c_str());
                y += lineHeight;
                line = word + " ";
            } else {
                line += word + " ";
            }
        }
        u8g2.drawUTF8(0, y, line.c_str());
    } while (u8g2.nextPage());
}


void sendRequest(String systemMessage, String userMessage) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(openAIEndpoint);
        http.addHeader("Authorization", "Bearer " + String(OPENAI_API_KEY));
        http.addHeader("Content-Type", "application/json");

        // Create JSON object for the request
        StaticJsonDocument<1024> jsonRequest;
        jsonRequest["model"] = "gpt-4o";
        
        JsonArray messages = jsonRequest.createNestedArray("messages");
        JsonObject systemMsg = messages.createNestedObject();
        systemMsg["role"] = "system";
        systemMsg["content"] = systemMessage;
        
        JsonObject userMsg = messages.createNestedObject();
        userMsg["role"] = "user";
        userMsg["content"] = userMessage;

        // Serialize JSON object to String
        String requestBody;
        serializeJson(jsonRequest, requestBody);

        // Send HTTP POST request
        int httpResponseCode = http.POST(requestBody);
        if (httpResponseCode > 0) {
            String response = http.getString();
            
            // Deserialize the response
            StaticJsonDocument<1024> jsonResponse;
            deserializeJson(jsonResponse, response);
            
            if (jsonResponse.containsKey("choices") && jsonResponse["choices"].size() > 0) {
                String gptResponse = jsonResponse["choices"][0]["message"]["content"].as<String>();
                Serial.println("Response: " + gptResponse);

                // Display the response on the screen
                displayText(gptResponse.c_str());
            } else {
                Serial.println("No choices received.");
            }
        } else {
            Serial.println("Error on sending POST: " + String(httpResponseCode));
        }

        // Free resources
        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi..");
    displayText("Connecting to WiFi...");  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    displayText("Connected!");  
    
    // Example usage
    String systemMessage = "You are a helpful assistant.";
    String userMessage = "Tell me a joke about modular synthesizers.";
    sendRequest(systemMessage, userMessage);
}

void loop() {
}
