// Ŭnicode please
#pragma once

void sora_next_page();
void sora_prev_page();
bool sora_is_next_page_exist();
bool sora_is_prev_page_exist();
void sora_reset_gyro();
void sora_reset_accel();
void sora_update_gyro(float yaw, float roll, float pitch);
void sora_update_accel(float x, float y, float z);

void sora_unload_texture();
void sora_reload_texture();

void sora_set_lang_korean();
void sora_set_lang_english();
void sora_set_apk_file_path(const char *abs_path);

void sora_unload_shader();

void sora_use_accel();
void sora_use_gyro();