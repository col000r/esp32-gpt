What is it?
-----------
A super simple example for how to talk to the OpenAI API and the GPT-4o model from an ESP32. (Other models work as well)

Instructions
------------
* Go to https://platform.openai.com/api-keys and **create an OpenAI API key** (Register an account if you don't have one)
* Add your **API key** and **WiFi details** to `src/config.example.h` and **rename** it to `src/config.h`
* Connect an **ESP32** microcontroller and configure `platformio.ini` accordingly (I was using a ttgo-lora32-v1)
* Open `src/main.cpp` and check out the sample usage at the end of `setup()`:
  ```c++
  // Example usage
  String systemMessage = "You are a helpful assistant.";
  String userMessage = "Tell me a joke about modular synthesizers.";
  sendRequest(systemMessage, userMessage);
  ```
* If you want to use a different **model** than **gpt-4o**, change this line: `jsonRequest["model"] = "gpt-4o";`
* **Build and run**, open the** Serial Monitor** and see the result of your request.

Full Tutorial
-------------
Read a **full tutorial** on my blog here: https://markus.hofer.rocks/gpt4o-microcontroller
