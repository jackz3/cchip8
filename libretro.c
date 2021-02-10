#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <stdio.h>
#if defined(_WIN32) && !defined(_XBOX)
#include <windows.h>
#endif
#include "libretro.h"
#include "chip8.h"
#include<SDL2/SDL.h>

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define VIDEO_PIXELS VIDEO_WIDTH * VIDEO_HEIGHT
#define VIDEO_REFRESH_RATE 60.0f

static uint8_t *frame_buf;
static struct retro_log_callback logging;
static retro_log_printf_t log_cb;
static bool use_audio_cb;
static float last_aspect;
static float last_sample_rate;
char retro_base_directory[4096];
char retro_game_path[4096];
const uint8_t cycles = OPS_PS / 60;
uint16_t g_frame[VIDEO_WIDTH * VIDEO_HEIGHT];

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}


static retro_environment_t environ_cb;

void retro_init(void)
{
   frame_buf = (uint8_t*)malloc(VIDEO_PIXELS * sizeof(uint32_t));
   const char *dir = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
   {
      snprintf(retro_base_directory, sizeof(retro_base_directory), "%s", dir);
   }
   
}

void retro_deinit(void)
{
   free(frame_buf);
   frame_buf = NULL;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "chip8";
   info->library_version  = "0.1";
   info->need_fullpath    = true;
   info->valid_extensions = "ch8";
}

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   float aspect                = 0.0f;
   float sampling_rate         = 30000.0f;


   info->geometry.base_width   = VIDEO_WIDTH;
   info->geometry.base_height  = VIDEO_HEIGHT;
   info->geometry.max_width    = VIDEO_WIDTH;
   info->geometry.max_height   = VIDEO_HEIGHT;
   info->geometry.aspect_ratio = (float)VIDEO_WIDTH / VIDEO_HEIGHT;

   last_aspect                 = aspect;
   last_sample_rate            = sampling_rate;
   info->timing.fps            = VIDEO_REFRESH_RATE;
   // info->timing.sample_rate    = VIDEO_REFRESH_RATE * AUDIO_SEGMENT_LENGTH;
   
   int pixel_format = RETRO_PIXEL_FORMAT_RGB565;
   environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

static struct retro_rumble_interface rumble;

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;

   static const struct retro_controller_description controllers[] = {
      { "Nintendo DS", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0) },
   };

   static const struct retro_controller_info ports[] = {
      { controllers, 1 },
      { NULL, 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

static unsigned x_coord;
static unsigned y_coord;
static unsigned phase;
static int mouse_rel_x;
static int mouse_rel_y;

void retro_reset(void)
{
   x_coord = 0;
   y_coord = 0;
}

static void check_variables(void)
{

}

static void audio_callback(void)
{
   for (unsigned i = 0; i < 30000 / 60; i++, phase++)
   {
      int16_t val = 0x800 * sinf(2.0f * M_PI * phase * 300.0f / 30000.0f);
      audio_cb(val, val);
   }

   phase %= 100;
}

static void audio_set_state(bool enable)
{
   (void)enable;
}

void pull_input() {
   input_poll_cb();
   // memset(keypad, 0, 16);
   printf("status: %d, %d\n", keypad[5], keypad[7]);
   if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_1)) {
      keypad[1] = 1;
      printf("key 1 pressed");
   } else {
      keypad[1] = 0;
   }
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) {
      printf("key 5 pressed !!!!!!");
      keypad[5] = 1;
   } else {
      keypad[5] = 0;
   }
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) {
      printf("key 7 pressed !!!!!!");
      keypad[7] = 1;
   } else {
      keypad[7] = 0;
   }
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) {
      keypad[8] = 1;
   } else {
      keypad[8] = 0;
   }
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) {
      keypad[9] = 1;
   } else {
      keypad[9] = 0;
   }
   printf("status end: %d, %d\n", keypad[5], keypad[7]);
}

Uint32 lastTime = 0, currentTime;
void retro_run(void)
{
   unsigned i;
   pull_input();

   if (halt) {
      printf("halt\n");
      return;
   }
   emulateCycle(cycles);
   memset(g_frame, 0, VIDEO_WIDTH * VIDEO_HEIGHT);
   for (int i =0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
      if (gfx[i]) {
         g_frame[i] = 0xff << 5;
      }
   }

   currentTime = SDL_GetTicks();
   int delta = 17 - (currentTime - lastTime);
   if (delta> 0) {
      SDL_Delay(delta);
   }

   video_cb(g_frame, 64, 32, 64 * 2);

   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
   
   lastTime = currentTime + delta;
}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_0, "0"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_1, "1"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_2, "2"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_3, "3"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_4, "4"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_5, "5"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_6, "6"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_7, "7"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_8, "8"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_9, "9"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_a, "a"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_b, "b"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_c, "c"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_d, "d"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_e, "e"},
      { 0, RETRO_DEVICE_KEYBOARD, 0, RETROK_f, "f"},
      { 0 },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
      return false;
   }

   snprintf(retro_game_path, sizeof(retro_game_path), "%s", info->path);
   initialize();
   loadGame(info->path);
   struct retro_audio_callback audio_cb = { audio_callback, audio_set_state };
   // use_audio_cb = environ_cb(RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK, &audio_cb);

   // check_variables();

   (void)info;
   return true;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   return false;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

