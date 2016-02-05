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

#include <context_history.h>
#include <context_history_types_internal.h>

#include "../shared.h"
#include "history.h"

using namespace ctx::test;

static context_history_h history_h = NULL;
static context_history_list_h list_h = NULL;
static context_history_record_h record_h = NULL;
static context_history_filter_h filter_h = NULL;

static void __release_resource()
{
	context_history_list_destroy(list_h);
	list_h = NULL;

	context_history_record_destroy(record_h);
	record_h = NULL;

	context_history_filter_destroy(filter_h);
	filter_h = NULL;
}

int test_history(int *argc, char ***argv)
{
	err = context_history_create(&history_h);
	if (err != ERR_NONE) {
		g_print(RED("Memory allocation failed\n"));
		return 0;
	}

	ctx::test::newline_after_tcname = true;

	while (true) {
		g_print("\nSelect the target item...\n");
		g_print("   1. Recently Used App           2. Frequently Used App\n");
		g_print("   3. Rarely Used App             4. Peak Time for App\n");
		g_print("   5. Peak Time for Music         6. Peak Time for Video\n");
		g_print("   7. Common Setting for App      8. Common Setting for Music\n");
		g_print("   9. Common Setting for Video   10. Frequently Communicated Address\n");

		while (!get_input_int("(to exit, ctrl-c)? ")) {;}

		switch (int_val) {
		case 1:
			run_testcase("/history/recently_used_app", _history_app_recently);
			break;
		case 2:
			run_testcase("/history/frequently_used_app", _history_app_frequently);
			break;
		case 3:
			run_testcase("/history/rarely_used_app", _history_app_rarely);
			break;
		case 4:
			run_testcase("/history/peak_time_for_app", _history_app_peak_time);
			break;
		case 5:
			run_testcase("/history/peak_time_for_music", _history_music_peak_time);
			break;
		case 6:
			run_testcase("/history/peak_time_for_video", _history_video_peak_time);
			break;
		case 7:
			run_testcase("/history/common_setting_for_app", _history_app_setting);
			break;
		case 8:
			run_testcase("/history/common_setting_for_music", _history_music_setting);
			break;
		case 9:
			run_testcase("/history/common_setting_for_video", _history_video_setting);
			break;
		case 10:
			run_testcase("/history/frequently_communicated_address", _history_comm_frequently);
			break;
		default:
			g_print("Invalid number\n");
			break;
		}

		__release_resource();
	};

	context_history_destroy(history_h);
	return 0;
}

static void __filter_set_int(context_history_filter_e type, const char *key)
{
	if (filter_h == NULL)
		context_history_filter_create(&filter_h);

	std::string msg = " ";
	msg = msg + key + "? ";

	if (get_input_int(msg.c_str()))
		context_history_filter_set_int(filter_h, type, int_val);
}

static void __filter_set_str(context_history_filter_e type, const char *key)
{
	if (filter_h == NULL)
		context_history_filter_create(&filter_h);

	std::string msg = " ";
	msg = msg + key + "? ";

	if (get_input_str(msg.c_str()))
		context_history_filter_set_string(filter_h, type, str_val.c_str());
}

static bool __get_list(context_history_data_e type)
{
	err = context_history_get_list(history_h, type, filter_h, &list_h);

	if (err == ERR_NOT_SUPPORTED) {
		g_print(YELLOW("Not Supported") "\n");
		return false;
	}

	if (err == ERR_NO_DATA) {
		g_print(YELLOW("No Data") "\n");
		return false;
	}

	ASSERT_CMPINT(err, ==, ERR_NONE);
	return true;
}

static bool __iterate_list(const char *int_attr[], const char *str_attr[])
{
	int cnt = 0;
	err = context_history_list_get_count(list_h, &cnt);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	for (int i = 0; i < cnt; ++i) {
		err = context_history_list_get_current(list_h, &record_h);
		ASSERT_CMPINT(err, ==, ERR_NONE);
		g_print("[%d] ", i);

		/* Integer attributes */
		for (int j = 0; int_attr[j] != NULL; ++j) {
			err = context_history_record_get_int(record_h, int_attr[j], &int_val);
			ASSERT_CMPINT(err, ==, ERR_NONE);
			g_print("%s(%d) ", int_attr[j], int_val);
		}

		/* String attributes */
		for (int j = 0; str_attr[j] != NULL; ++j) {
			char *tmp = NULL;
			err = context_history_record_get_string(record_h, str_attr[j], &tmp);
			ASSERT_CMPINT(err, ==, ERR_NONE);
			g_print("%s(%s) ", str_attr[j], tmp);
			g_free(tmp);
		}

		if (i < cnt - 1) {
			err = context_history_list_move_next(list_h);
			ASSERT_CMPINT(err, ==, ERR_NONE);
		}

		context_history_record_destroy(record_h);
		record_h = NULL;
		g_print("\n");
	}

	return true;
}

bool _history_app_recently()
{
	const char *int_attr[] = {CONTEXT_HISTORY_TOTAL_COUNT, CONTEXT_HISTORY_TOTAL_DURATION, CONTEXT_HISTORY_LAST_TIME, NULL};
	const char *str_attr[] = {CONTEXT_HISTORY_APP_ID, NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_AUDIO_JACK, FILTER_AUDIO_JACK);
	__filter_set_str(CONTEXT_HISTORY_FILTER_WIFI_BSSID, FILTER_WIFI_BSSID);

	bool_val = __get_list(CONTEXT_HISTORY_RECENTLY_USED_APP);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_app_frequently()
{
	const char *int_attr[] = {CONTEXT_HISTORY_TOTAL_COUNT, CONTEXT_HISTORY_TOTAL_DURATION, CONTEXT_HISTORY_LAST_TIME, NULL};
	const char *str_attr[] = {CONTEXT_HISTORY_APP_ID, NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_AUDIO_JACK, FILTER_AUDIO_JACK);
	__filter_set_str(CONTEXT_HISTORY_FILTER_WIFI_BSSID, FILTER_WIFI_BSSID);

	bool_val = __get_list(CONTEXT_HISTORY_FREQUENTLY_USED_APP);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_app_rarely()
{
	const char *int_attr[] = {CONTEXT_HISTORY_TOTAL_COUNT, CONTEXT_HISTORY_TOTAL_DURATION, CONTEXT_HISTORY_LAST_TIME, NULL};
	const char *str_attr[] = {CONTEXT_HISTORY_APP_ID, NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);

	bool_val = __get_list(CONTEXT_HISTORY_RARELY_USED_APP);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_app_peak_time()
{
	const char *int_attr[] = {CONTEXT_HISTORY_HOUR_OF_DAY, CONTEXT_HISTORY_TOTAL_COUNT, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_DAY_OF_WEEK, FILTER_DAY_OF_WEEK);
	__filter_set_str(CONTEXT_HISTORY_FILTER_APP_ID, FILTER_APP_ID);

	bool_val = __get_list(CONTEXT_HISTORY_PEAK_TIME_FOR_APP);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_app_setting()
{
	const char *int_attr[] = {CONTEXT_HISTORY_AUDIO_JACK, CONTEXT_HISTORY_SYSTEM_VOLUME, CONTEXT_HISTORY_MEDIA_VOLUME, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_str(CONTEXT_HISTORY_FILTER_APP_ID, FILTER_APP_ID);

	bool_val = __get_list(CONTEXT_HISTORY_COMMON_SETTING_FOR_APP);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_music_peak_time()
{
	const char *int_attr[] = {CONTEXT_HISTORY_HOUR_OF_DAY, CONTEXT_HISTORY_TOTAL_COUNT, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_DAY_OF_WEEK, FILTER_DAY_OF_WEEK);

	bool_val = __get_list(CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_music_setting()
{
	const char *int_attr[] = {CONTEXT_HISTORY_AUDIO_JACK, CONTEXT_HISTORY_SYSTEM_VOLUME, CONTEXT_HISTORY_MEDIA_VOLUME, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);

	bool_val = __get_list(CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_video_peak_time()
{
	const char *int_attr[] = {CONTEXT_HISTORY_HOUR_OF_DAY, CONTEXT_HISTORY_TOTAL_COUNT, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_DAY_OF_WEEK, FILTER_DAY_OF_WEEK);

	bool_val = __get_list(CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_video_setting()
{
	const char *int_attr[] = {CONTEXT_HISTORY_AUDIO_JACK, CONTEXT_HISTORY_SYSTEM_VOLUME, CONTEXT_HISTORY_MEDIA_VOLUME, NULL};
	const char *str_attr[] = {NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);

	bool_val = __get_list(CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}

bool _history_comm_frequently()
{
	const char *int_attr[] = {CONTEXT_HISTORY_TOTAL_COUNT, CONTEXT_HISTORY_TOTAL_DURATION, CONTEXT_HISTORY_LAST_TIME, NULL};
	const char *str_attr[] = {CONTEXT_HISTORY_ADDRESS, NULL};

	__filter_set_int(CONTEXT_HISTORY_FILTER_RESULT_SIZE, FILTER_RESULT_SIZE);
	__filter_set_int(CONTEXT_HISTORY_FILTER_TIME_SPAN, FILTER_TIME_SPAN);
	__filter_set_int(CONTEXT_HISTORY_FILTER_START_TIME, FILTER_START_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_END_TIME, FILTER_END_TIME);
	__filter_set_int(CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE, FILTER_COMMUNICATION_TYPE);

	bool_val = __get_list(CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS);
	IF_FAIL_RETURN(bool_val, false);

	bool_val = __iterate_list(int_attr, str_attr);
	IF_FAIL_RETURN(bool_val, false);

	return true;
}
