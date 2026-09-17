#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>//do not need this yet, this is for WIFI capabilities

//Select camera model in board_config.h
#include "board_config.h"//note that board_config.h already includes camera_pins.h

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//Begin serial communication to see messages in Serial Monitor

  // Create a camera configuration struct and fill it using pin definitions from camera_pins.h
  camera_config_t config = {};

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;

  //config.frame_size = FRAMESIZE_QVGA;//USE LOWER RES FOR TESTING TO REDUCE MEMORY USE
  config.frame_size = FRAMESIZE_QVGA;//LOWER RES FOR DEBUGGING
  config.pixel_format = PIXFORMAT_JPEG;// for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  /*
  Above block of code basically just says:
  camera clock = 20 MHz
  resolution = QVGA
  image format = JPEG
  JPEG quality = 12
  use 1 frame buffer
  */

  //Check PSRAM
  Serial.print("PSRAM found: ");
  Serial.println(psramFound() ? "YES" : "NO");
  Serial.print("PSRAM size: ");
  Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");
  Serial.println(ESP.getFreePsram());

  //Use PSRAM if available
  if (psramFound()) {
    config.fb_location = CAMERA_FB_IN_PSRAM;
  } else {
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  //Start cam driver and check to see if it works
  Serial.println("About to initialize camera");

  esp_err_t err = esp_camera_init(&config);

  Serial.printf("esp_camera_init returned: %d / 0x%08X\n", err, (uint32_t)err);

  if (err != ESP_OK) {//ESP_OK is already defined by the ESP32 library
    Serial.println("Camera init failed");
    return;
  }

  Serial.println("Camera initialized");

  //capturing camera frame
  camera_fb_t *fb = esp_camera_fb_get();

  //camera_fb_t is the type for the camera frame
  //*fb =fb is a pointer to that frame
  //esp_camera_fb_get() = asks the camera driver for one captured frame

  //check to see if it worked
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.print("Width: ");
  Serial.println(fb->width);

  Serial.print("Height: ");
  Serial.println(fb->height);

  Serial.print("Size in bytes: ");
  Serial.println(fb->len);

  //now check format
  Serial.print("Format: ");
  Serial.println(fb->format);

  esp_camera_fb_return(fb);
}

void loop() {
  // put your main code here, to run repeatedly:
}



// #include <Arduino.h>
// #include "esp_camera.h"
// #include <WiFi.h>//do not need this yet, this is for WIFI capabilities

// //Select camera model in board_config.h
// #include "board_config.h"//note that board_config.h already includes camera_pins.h

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);//Begin serial communication to see messages in Serial Monitor

//   // Create a camera configuration struct and fill it using pin definitions from camera_pins.h
//     camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sccb_sda = SIOD_GPIO_NUM;
//   config.pin_sccb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//  //config.frame_size = FRAMESIZE_QVGA;//USE LOWER RES FOR TESTING TO REDUCE MEMORY USE
//   config.frame_size = FRAMESIZE_UXGA; //THIS IS ACTUAL FRAME SIZE BUT FOR TESTING USE LOWER RES
//   config.pixel_format = PIXFORMAT_JPEG;  // for streaming
//   //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
//   config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
//   config.fb_location = CAMERA_FB_IN_PSRAM;
//   config.jpeg_quality = 12;
//   config.fb_count = 1;

//   /*
//   Above block of code basically just says:
// camera clock = 20 MHz
// resolution = UXGA
// image format = JPEG
// store image buffers in PSRAM
// JPEG quality = 12
// use 1 frame buffer
//   */

// //Start cam driver and check to see if it works
// esp_err_t err = esp_camera_init(&config);
// if (err != ESP_OK) {//ESP_OK is already defined by the ESP32 library
//   Serial.println("Camera init failed");
//   return;
// }

// Serial.println("Camera initialized");

// //capturing camera frame
// camera_fb_t *fb = esp_camera_fb_get();
// //camera_fb_t is the type for the camera frame
// //*fb =fb is a pointer to that frame
// //esp_camera_fb_get() = asks the camera driver for one captured frame

// //check to see if it worked
// if (!fb) {
//   Serial.println("Camera capture failed");
//   return;
// }
// Serial.print("Width: ");
// Serial.println(fb->width);

// Serial.print("Height: ");
// Serial.println(fb->height);

// Serial.print("Size in bytes: ");
// Serial.println(fb->len);

// esp_camera_fb_return(fb);


// //now check format
// Serial.print("Format: ");
// Serial.println(fb->format);
// }

// void loop() {
//   // put your main code here, to run repeatedly:

// }

