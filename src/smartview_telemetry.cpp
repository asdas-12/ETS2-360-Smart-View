// ETS2 360 Smart View telemetry bridge (SDK 1.16 headers).
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <shlobj.h>
#include <cstdio>
#include <cstring>
#include <string>
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "common/scssdk_telemetry_truck_common_channels.h"

static scs_log_t g_log = nullptr;
static float g_speed = 0.0f, g_rpm = 0.0f;
static scs_s32_t g_gear = 0;
static bool g_paused = true;
static bool g_left_blinker = false, g_right_blinker = false;
static char g_path[MAX_PATH] = {};

static bool make_path() {
  char docs[MAX_PATH] = {};
  if (SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, docs) != S_OK) return false;
  std::string dir = std::string(docs) + "\\Euro Truck Simulator 2";
  CreateDirectoryA(dir.c_str(), nullptr);
  std::string file = dir + "\\360smartview_telemetry.json";
  if (file.size() >= MAX_PATH) return false;
  strcpy_s(g_path, file.c_str());
  return true;
}
static void write_data() {
  if (!g_path[0]) return;
  FILE* f = nullptr;
  if (fopen_s(&f, g_path, "wt") || !f) return;
  fprintf(f, "{\n  \"speed_kmh\": %.2f,\n  \"rpm\": %.1f,\n  \"gear\": %d,\n  \"paused\": %s,\n  \"left_blinker\": %s,\n  \"right_blinker\": %s\n}\n",
    double(g_speed) * 3.6, double(g_rpm), int(g_gear), g_paused ? "true" : "false",
    g_left_blinker ? "true" : "false", g_right_blinker ? "true" : "false");
  fclose(f);
}
static SCSAPI_VOID on_float(const scs_string_t, const scs_u32_t, const scs_value_t* v, const scs_context_t c) {
  if (v && v->type == SCS_VALUE_TYPE_float) *static_cast<float*>(c) = v->value_float.value;
}
static SCSAPI_VOID on_s32(const scs_string_t, const scs_u32_t, const scs_value_t* v, const scs_context_t c) {
  if (v && v->type == SCS_VALUE_TYPE_s32) *static_cast<scs_s32_t*>(c) = v->value_s32.value;
}
static SCSAPI_VOID on_bool(const scs_string_t, const scs_u32_t, const scs_value_t* v, const scs_context_t c) {
  if (v && v->type == SCS_VALUE_TYPE_bool) *static_cast<bool*>(c) = (v->value_bool.value != 0);
}
static SCSAPI_VOID on_frame(const scs_event_t, const void*, const scs_context_t) { if (!g_paused) write_data(); }
static SCSAPI_VOID on_pause(const scs_event_t e, const void*, const scs_context_t) {
  g_paused = (e == SCS_TELEMETRY_EVENT_paused);
  write_data();
}
SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params) {
  if (version != SCS_TELEMETRY_VERSION_1_01) return SCS_RESULT_unsupported;
  const auto* p = static_cast<const scs_telemetry_init_params_v101_t*>(params);
  g_log = p->common.log;
  if (strcmp(p->common.game_id, SCS_GAME_ID_EUT2) != 0 && g_log)
    g_log(SCS_LOG_TYPE_warning, "360 Smart View: intended for ETS2.");
  if (!make_path()) return SCS_RESULT_generic_error;
  g_speed = g_rpm = 0.0f; g_gear = 0; g_paused = true; g_left_blinker = g_right_blinker = false;
  if (p->register_for_event(SCS_TELEMETRY_EVENT_frame_end, on_frame, nullptr) != SCS_RESULT_ok ||
      p->register_for_event(SCS_TELEMETRY_EVENT_paused, on_pause, nullptr) != SCS_RESULT_ok ||
      p->register_for_event(SCS_TELEMETRY_EVENT_started, on_pause, nullptr) != SCS_RESULT_ok)
    return SCS_RESULT_generic_error;
  if (p->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_each_frame, on_float, &g_speed) != SCS_RESULT_ok ||
      p->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_each_frame, on_float, &g_rpm) != SCS_RESULT_ok ||
      p->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, SCS_U32_NIL, SCS_VALUE_TYPE_s32, SCS_TELEMETRY_CHANNEL_FLAG_each_frame, on_s32, &g_gear) != SCS_RESULT_ok ||
      p->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_lblinker, SCS_U32_NIL, SCS_VALUE_TYPE_bool, SCS_TELEMETRY_CHANNEL_FLAG_each_frame, on_bool, &g_left_blinker) != SCS_RESULT_ok ||
      p->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_rblinker, SCS_U32_NIL, SCS_VALUE_TYPE_bool, SCS_TELEMETRY_CHANNEL_FLAG_each_frame, on_bool, &g_right_blinker) != SCS_RESULT_ok)
    return SCS_RESULT_generic_error;
  if (g_log) g_log(SCS_LOG_TYPE_message, "360 Smart View telemetry bridge initialized.");
  write_data();
  return SCS_RESULT_ok;
}
SCSAPI_VOID scs_telemetry_shutdown(void) { g_paused = true; write_data(); g_log = nullptr; }
BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
