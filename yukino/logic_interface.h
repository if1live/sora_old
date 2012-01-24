// Ŭnicode please
#pragma once

void sora_next_page();
void sora_prev_page();
void sora_reset_gyro();
void sora_reset_accel();
void sora_update_gyro(float yaw, float roll, float pitch);
void sora_update_accel(float x, float y, float z);

void sora_unload_texture();
void sora_reload_texture();

void sora_set_lang_korean();
void sora_set_lang_english();
