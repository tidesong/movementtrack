#include <pebble.h>

Window *sMainWindow;
TextLayer *sTextLayerXYZ;
TextLayer *sTextLayerPrevInfo;
TextLayer *sTextLayerCurrInfo;

//Data handler is triggered with data from the accelerometer at the rate set earlier
static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d\n1 %d,%d,%d\n2 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z, 
    data[1].x, data[1].y, data[1].z, 
    data[2].x, data[2].y, data[2].z
  );

  //Show the data
  text_layer_set_text(sTextLayerXYZ, s_buffer);
  
  //Do some calculation and store in history
  
  //Based on history, determine the state the watch is in : rest, front crawl, backstroke, butterfly, turn(???)
}

static void mainWindowLoad(Window *window) {
  Layer *windowLayer = window_get_root_layer(window);
  
  sTextLayerXYZ = text_layer_create(GRect(0, 0, 144, 60));
  sTextLayerPrevInfo = text_layer_create(GRect(0, 60, 144, 20));
  sTextLayerCurrInfo = text_layer_create(GRect(0, 80, 144, 20));  
  
  text_layer_set_font(sTextLayerXYZ, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(sTextLayerXYZ, "No data yet.");
  text_layer_set_overflow_mode(sTextLayerXYZ, GTextOverflowModeWordWrap);
  
  text_layer_set_font(sTextLayerPrevInfo, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(sTextLayerPrevInfo, "No data yet.");
  text_layer_set_overflow_mode(sTextLayerPrevInfo, GTextOverflowModeWordWrap);
  
  text_layer_set_font(sTextLayerCurrInfo, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(sTextLayerCurrInfo, "No data yet.");
  text_layer_set_overflow_mode(sTextLayerCurrInfo, GTextOverflowModeWordWrap);
  
  layer_add_child(windowLayer, text_layer_get_layer(sTextLayerXYZ));
}

static void mainWindowUnload(Window *window) {
  text_layer_destroy(sTextLayerXYZ);
  text_layer_destroy(sTextLayerPrevInfo);
  text_layer_destroy(sTextLayerCurrInfo);  
}

void handle_init(void) {
  sMainWindow = window_create();
 
    // Set handlers to manage the elements inside the Window
  window_set_window_handlers(sMainWindow, (WindowHandlers) {
    .load = mainWindowLoad,
    .unload = mainWindowUnload
  });
  
  //subscribe to accelerometer
  uint32_t num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);
  //set sampling rate of accelerometer
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  
  window_stack_push(sMainWindow, true);
}

void handle_deinit(void) {

  window_destroy(sMainWindow);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
