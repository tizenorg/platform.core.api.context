/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CONTEXT_TEST_TRIGGER_CONDITION_H__
#define __CONTEXT_TEST_TRIGGER_CONDITION_H__

int test_trigger_condition(int *argc, char ***argv);

bool _trigger_cond_battery();
bool _trigger_cond_charger();
bool _trigger_cond_gps();
bool _trigger_cond_headphone();
bool _trigger_cond_usb();
bool _trigger_cond_wifi();
bool _trigger_cond_psmode();
bool _trigger_cond_call();

bool _trigger_cond_app_use_freq1();
bool _trigger_cond_app_use_freq2();
bool _trigger_cond_app_use_freq3();
bool _trigger_cond_app_use_freq4();

bool _trigger_cond_comm_freq1();
bool _trigger_cond_comm_freq2();
bool _trigger_cond_comm_freq3();
bool _trigger_cond_comm_freq4();

bool _trigger_cond_music_freq1();
bool _trigger_cond_music_freq2();
bool _trigger_cond_music_freq3();
bool _trigger_cond_music_freq4();

bool _trigger_cond_video_freq1();
bool _trigger_cond_video_freq2();
bool _trigger_cond_video_freq3();
bool _trigger_cond_video_freq4();

#endif
