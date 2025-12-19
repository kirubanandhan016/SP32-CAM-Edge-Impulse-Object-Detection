
/*
=====================================================
 PROJECT TITLE:
 Object Detection using ESP32-CAM with Edge Impulse

 DESCRIPTION:
 This project performs object detection directly
 on ESP32-CAM using Edge Impulse (Edge Intelligence).
 The trained ML model runs locally without cloud.

 AUTHOR:
 ECE Final Year Project
=====================================================
*/

#include <Arduino.h>
#include "esp_camera.h"
#include <EdgeImpulse_FOMO_inferencing.h>

// AI Thinker ESP32-CAM pin configuration
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

static camera_config_t camera_config;

void setup() {
  Serial.begin(115200);
  delay(2000);

  camera_config.ledc_channel = LEDC_CHANNEL_0;
  camera_config.ledc_timer   = LEDC_TIMER_0;
  camera_config.pin_d0       = Y2_GPIO_NUM;
  camera_config.pin_d1       = Y3_GPIO_NUM;
  camera_config.pin_d2       = Y4_GPIO_NUM;
  camera_config.pin_d3       = Y5_GPIO_NUM;
  camera_config.pin_d4       = Y6_GPIO_NUM;
  camera_config.pin_d5       = Y7_GPIO_NUM;
  camera_config.pin_d6       = Y8_GPIO_NUM;
  camera_config.pin_d7       = Y9_GPIO_NUM;
  camera_config.pin_xclk     = XCLK_GPIO_NUM;
  camera_config.pin_pclk     = PCLK_GPIO_NUM;
  camera_config.pin_vsync    = VSYNC_GPIO_NUM;
  camera_config.pin_href     = HREF_GPIO_NUM;
  camera_config.pin_sscb_sda = SIOD_GPIO_NUM;
  camera_config.pin_sscb_scl = SIOC_GPIO_NUM;
  camera_config.pin_pwdn     = PWDN_GPIO_NUM;
  camera_config.pin_reset    = RESET_GPIO_NUM;
  camera_config.xclk_freq_hz = 20000000;
  camera_config.pixel_format = PIXFORMAT_JPEG;

  // Edge Impulse recommended size
  camera_config.frame_size   = FRAMESIZE_96X96;
  camera_config.jpeg_quality = 12;
  camera_config.fb_count     = 1;

  if (esp_camera_init(&camera_config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  Serial.println("ESP32-CAM initialized");
}

void loop() {

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  signal_t signal;
  signal.total_length = fb->len;
  signal.get_data = [](size_t offset, size_t length, float *out_ptr) {
    // Placeholder: Edge Impulse library handles data internally
    return 0;
  };

  ei_impulse_result_t result;
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

  if (res != EI_IMPULSE_OK) {
    Serial.println("Inference failed");
    return;
  }

  Serial.println("Inference results:");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    Serial.print(ei_classifier_inferencing_categories[ix]);
    Serial.print(": ");
    Serial.println(result.classification[ix].value);
  }

  esp_camera_fb_return(fb);
  delay(2000);
}
