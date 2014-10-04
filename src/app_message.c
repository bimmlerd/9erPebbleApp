#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_connection_layer;

// Key values for AppMessage Dictionary
enum {
  DEPARTURE_KEY = 0,
  MESSAGE_KEY = 1
};

static void main_window_load(Window *window) {
  // Create Connection TextLayer
  s_connection_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_connection_layer, GColorClear);
  text_layer_set_text_color(s_connection_layer, GColorBlack);
text_layer_set_font(s_connection_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_connection_layer, GTextAlignmentCenter);
  text_layer_set_text(s_connection_layer, "--");
  
  // Add as a child to the root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_connection_layer));
}
static void main_window_unload(Window *window) {
  text_layer_destroy(s_connection_layer);
}


// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
  static char connection_buffer[8];
  
  Tuple *tuple = dict_read_first(received);
  if(tuple) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Received Message: %s", tuple->value->cstring);
    snprintf(connection_buffer, sizeof(connection_buffer), "%s", tuple->value->cstring);
    //APP_LOG(APP_LOG_LEVEL_INFO, "In Connection Buffer: %s", connection_buffer);
    text_layer_set_text(s_connection_layer, connection_buffer);
  }}



// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {  
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}
  
void init() {
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  // Register AppMessage handlers
  app_message_register_inbox_received(in_received_handler); 
  app_message_register_inbox_dropped(in_dropped_handler); 
  app_message_register_outbox_failed(out_failed_handler);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void deinit() {
  app_message_deregister_callbacks();
  window_destroy(s_main_window);
}

int main( void ) {
  init();
  app_event_loop();
  deinit();
}