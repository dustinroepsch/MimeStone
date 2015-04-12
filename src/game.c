#include <pebble.h>
#define HURTZ 10 
#define MAXTURNS 100
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define FORWARD 4
#define BACKWARD 5
#define EMPTY -1
//Global variables
bool countdown = true;
int tick = 0;
int tolerance = 20;
int pattern[MAXTURNS];
int patternSize = 0;
char test[15] = "               ";
int testInt = 0;
static Window *s_main_window;
//static TextLayer *s_output_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_happy;
static GBitmap *s_neutral;
static GBitmap *s_sad;
//Modifier funcions


//Gamestate Function

static void tap_handler(AccelAxisType axis, int32_t direction) {

  int currentDirection = EMPTY;
  switch (axis) {
  case ACCEL_AXIS_X:
    if (direction > 0) {
      currentDirection = FORWARD;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "FORWARD");
      
    } else {
      currentDirection = BACKWARD;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "BACKWARD");

    }
    break;
  case ACCEL_AXIS_Y:
    if (direction > 0) {
      currentDirection = RIGHT;
    
      APP_LOG(APP_LOG_LEVEL_DEBUG, "RIGHT");

    } else {
      currentDirection = LEFT;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "LEFT");

    }
    break;
  case ACCEL_AXIS_Z:
    if (direction > 0) {
      currentDirection = LEFT;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "LEFT");

    } else {
      currentDirection = RIGHT;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "RIGHT");

    }
    break;
  }
  
  
  if (tick < patternSize){
    if (currentDirection == pattern[tick]){
    // text_layer_set_text(s_output_layer, ":)");
      bitmap_layer_set_bitmap(s_image_layer, s_happy);
      vibes_long_pulse();

    }else{
     // text_layer_set_text(s_output_layer, ":(");
      bitmap_layer_set_bitmap(s_image_layer, s_sad);
      static  uint32_t const segments[] = { 100, 100, 100,100,100,100,100,100,100,100,900 };
  VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};
 
      vibes_enqueue_custom_pattern(pat);

     tick = 0;
      patternSize = 0;
      return;
    }
  }
  if (tick == patternSize){
    if (patternSize + 1 == MAXTURNS){
      //you cheat
    }else{
      pattern[tick] = currentDirection;
      patternSize++;
  //    text_layer_set_text(s_output_layer, "added");
      bitmap_layer_set_bitmap(s_image_layer,s_neutral);
      tick = 0;
      vibes_long_pulse();

      return;
    }
  }
  tick++;
 // static char s_buffer[5];
  //test[testInt] = '0' + currentDirection;
  //testInt++;
  
 //snprintf(s_buffer, sizeof(s_buffer), "T:%d", currentDirection);
 // text_layer_set_text(s_output_layer,test);

 // if (tick == patternSize + 1){
   // tick = 0;
 // }
}
//Window functions

static void main_window_load(Window *window){
    Layer *window_layer = window_get_root_layer(window);
     GRect bounds = layer_get_frame(window_layer);

//  s_image_layer = layer_create(bounds);
     s_image_layer = bitmap_layer_create(bounds);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer));
  s_happy = gbitmap_create_with_resource(RESOURCE_ID_HAPPY);
  s_neutral = gbitmap_create_with_resource(RESOURCE_ID_NEUTRAL);
  s_sad = gbitmap_create_with_resource(RESOURCE_ID_SAD);
  
   /** 
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
    text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text(s_output_layer, "move");
    text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
    layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
    */
      bitmap_layer_set_bitmap(s_image_layer,s_happy);
}


static void main_window_unload(Window *window) {
  gbitmap_destroy(s_happy);
  gbitmap_destroy(s_neutral);
  gbitmap_destroy(s_sad);

  bitmap_layer_destroy(s_image_layer);
}

//Init & deinit

static void init(){
  s_main_window = window_create();

  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });  
  window_stack_push(s_main_window, true);
  accel_tap_service_subscribe(tap_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  accel_service_set_samples_per_update(5);
  int i = 0;
  for (i = 0; i < MAXTURNS; i++){
    pattern[i] = EMPTY;
  }
  
}
static void deinit(){
  window_destroy(s_main_window);
  accel_tap_service_unsubscribe();
  
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}