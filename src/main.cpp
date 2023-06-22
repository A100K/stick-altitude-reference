
#include <Arduino.h>
#include <M5Unified.h>
#include <esp_log.h>

static auto &dsp = (M5.Display);

void setup(void)
{
  auto cfg = M5.config();

  cfg.external_imu = true;

  M5.begin(cfg);

  const char* name;
  switch (M5.Imu.getType())
  {
  case m5::imu_none:        name = "not found";   break;
  case m5::imu_sh200q:      name = "sh200q";      break;
  case m5::imu_mpu6050:     name = "mpu6050";     break;
  case m5::imu_mpu6886:     name = "mpu6886";     break;
  case m5::imu_mpu9250:     name = "mpu9250";     break;
  case m5::imu_bmi270:      name = "bmi270";      break;
  default:                  name = "unknown";     break;
  };
  M5_LOGI("imu:%s", name);


 int32_t w = dsp.width();
  int32_t h = dsp.height();
  if (w < h)
  {
    dsp.setRotation(dsp.getRotation() ^ 1);
    w = dsp.width();
    h = dsp.height();
  }
  int32_t graph_area_h = ((h - 8) / 18) * 18;
  int32_t text_area_h = h - graph_area_h;
  float fontsize = text_area_h / 8;
  dsp.setTextSize(fontsize);
  dsp.setTextColor(TFT_WHITE);
}

void loop(void)
{
  dsp.fillScreen(TFT_BLACK);
  auto imu_update = M5.Imu.update();
  if (imu_update)
  {

    auto data = M5.Imu.getImuData();

    float angle = ((atan2(data.accel.y, data.accel.z) * 180.0) / PI);
    dsp.setCursor(5, 5);
    dsp.printf("Angle: %3.1f", angle);
  }

  vTaskDelay(1000);
  
}