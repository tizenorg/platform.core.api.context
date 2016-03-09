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

#define CONTEXT_HISTORY_DATA		"QueryResult"

#define SUBJ_RECENTLY_USED_APP		"stats/app/recently"
#define SUBJ_FREQUENTLY_USED_APP	"stats/app/often"
#define SUBJ_RARELY_USED_APP		"stats/app/rarely"
#define SUBJ_PEAK_TIME_FOR_APP		"stats/app/peak_time"
#define SUBJ_PEAK_TIME_FOR_MUSIC	"stats/music/peak_time"
#define SUBJ_PEAK_TIME_FOR_VIDEO	"stats/video/peak_time"
#define SUBJ_COMMON_SETTING_FOR_APP		"stats/app/setting"
#define SUBJ_COMMON_SETTING_FOR_MUSIC	"stats/music/setting"
#define SUBJ_COMMON_SETTING_FOR_VIDEO	"stats/video/setting"
#define SUBJ_FREQUENTLY_COMMUNICATED_ADDRESS	"stats/contact/often"

#define FILTER_TIME_SPAN	"TimeSpan"
#define FILTER_RESULT_SIZE	"ResultSize"
#define FILTER_APP_ID		"AppId"
#define FILTER_DAY_OF_WEEK	"DayOfWeek"
#define FILTER_START_TIME	"StartTime"
#define FILTER_END_TIME		"EndTime"
#define FILTER_AUDIO_JACK	"AudioJack"
#define FILTER_WIFI_BSSID	"BSSID"
#define FILTER_COMMUNICATION_TYPE	"CommunicationType"

#endif /* __TIZEN_CONTEXT_HISTORY_TYPES_INTERNAL_H__ */
