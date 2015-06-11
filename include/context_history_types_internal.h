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

#ifndef __TIZEN_CONTEXT_HISTORY_TYPES_INTERNAL_H__
#define __TIZEN_CONTEXT_HISTORY_TYPES_INTERNAL_H__

#include <json.h>

#define CONTEXT_HISTORY_DATA "QueryResult"

#define CONTEXT_HISTORY_SUBJECT_RECENTLY_USED_APP "app/history/recently_used"
#define CONTEXT_HISTORY_SUBJECT_FREQUENTLY_USED_APP "app/history/frequently_used"
#define CONTEXT_HISTORY_SUBJECT_RARELY_USED_APP "app/history/rarely_used"
#define CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_APP "app/history/peak_time"
#define CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_MUSIC "music/history/peak_time"
#define CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_VIDEO "video/history/peak_time"
#define CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_APP "app/history/common_setting"
#define CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_MUSIC "music/history/common_setting"
#define CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_VIDEO "video/history/common_setting"
#define CONTEXT_HISTORY_SUBJECT_FREQUENTLY_COMMUNICATED_ADDRESS "social/history/frequently_communicated"

#define CONTEXT_HISTORY_EVENT_START_MUSIC "music/event/start"
#define CONTEXT_HISTORY_EVENT_STOP_MUSIC "music/event/stop"
#define CONTEXT_HISTORY_EVENT_START_VIDEO "video/event/start"
#define CONTEXT_HISTORY_EVENT_STOP_VIDEO "video/event/stop"

#define CONTEXT_HISTORY_FILTER_KEY_TIME_SPAN "TimeSpan"
#define CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE "ResultSize"
#define CONTEXT_HISTORY_FILTER_KEY_APP_ID "AppId"
#define CONTEXT_HISTORY_FILTER_KEY_DAY_OF_WEEK "DayOfWeek"
#define CONTEXT_HISTORY_FILTER_KEY_START_TIME "StartTime"
#define CONTEXT_HISTORY_FILTER_KEY_END_TIME "EndTime"
#define CONTEXT_HISTORY_FILTER_KEY_AUDIO_JACK "AudioJack"
#define CONTEXT_HISTORY_FILTER_KEY_WIFI_BSSID "BSSID"
#define CONTEXT_HISTORY_FILTER_KEY_COMMUNICATION_TYPE "CommunicationType"

#endif // __TIZEN_CONTEXT_HISTORY_TYPES_INTERNAL_H__
