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

#ifndef __CONTEXT_TEST_HISTORY_H__
#define __CONTEXT_TEST_HISTORY_H__

int test_history(int *argc, char ***argv);

bool _history_app_recently();
bool _history_app_frequently();
bool _history_app_rarely();
bool _history_app_peak_time();
bool _history_app_setting();

bool _history_music_peak_time();
bool _history_music_setting();
bool _history_video_peak_time();
bool _history_video_setting();

bool _history_comm_frequently();

#endif
