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

#include <request_handler.h>
#include <context_trigger.h>
#include <context_trigger_types_internal.h>
#include <context_history.h>

#include "../shared.h"
#include "condition.h"

using namespace ctx::test;

int test_trigger_condition(int *argc, char ***argv)
{
	run_testcase("/trigger/cond/battery", _trigger_cond_battery);
	run_testcase("/trigger/cond/charger", _trigger_cond_charger);
	run_testcase("/trigger/cond/gps", _trigger_cond_gps);
	run_testcase("/trigger/cond/headphone", _trigger_cond_headphone);
	run_testcase("/trigger/cond/usb", _trigger_cond_usb);
	run_testcase("/trigger/cond/wifi", _trigger_cond_wifi);
	run_testcase("/trigger/cond/psmode", _trigger_cond_psmode);
	run_testcase("/trigger/cond/call", _trigger_cond_call);

	run_testcase("/trigger/cond/app_freq1", _trigger_cond_app_use_freq1);
	run_testcase("/trigger/cond/app_freq2", _trigger_cond_app_use_freq2);
	run_testcase("/trigger/cond/app_freq3", _trigger_cond_app_use_freq3);
	run_testcase("/trigger/cond/app_freq4", _trigger_cond_app_use_freq4);

	run_testcase("/trigger/cond/comm_freq1", _trigger_cond_comm_freq1);
	run_testcase("/trigger/cond/comm_freq2", _trigger_cond_comm_freq2);
	run_testcase("/trigger/cond/comm_freq3", _trigger_cond_comm_freq3);
	run_testcase("/trigger/cond/comm_freq4", _trigger_cond_comm_freq4);

	run_testcase("/trigger/cond/music_freq1", _trigger_cond_music_freq1);
	run_testcase("/trigger/cond/music_freq2", _trigger_cond_music_freq2);
	run_testcase("/trigger/cond/music_freq3", _trigger_cond_music_freq3);
	run_testcase("/trigger/cond/music_freq4", _trigger_cond_music_freq4);

	run_testcase("/trigger/cond/video_freq1", _trigger_cond_video_freq1);
	run_testcase("/trigger/cond/video_freq2", _trigger_cond_video_freq2);
	run_testcase("/trigger/cond/video_freq3", _trigger_cond_video_freq3);
	run_testcase("/trigger/cond/video_freq4", _trigger_cond_video_freq4);

	return 0;
}

static bool __support(context_trigger_condition_e cond)
{
	err = context_trigger_rule_condition_is_supported(cond, &bool_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	if (!bool_val) {
		g_print(YELLOW("Not Supported") "\n");
		return false;
	}

	return true;
}

bool _trigger_cond_battery()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_BATTERY)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_BATTERY, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_LEVEL);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CHARGING);

	return true;
}

bool _trigger_cond_charger()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_CHARGER)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_CHARGER, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);

	return true;
}

bool _trigger_cond_gps()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_GPS)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_GPS, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);

	return true;
}

bool _trigger_cond_headphone()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_HEADPHONE)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_HEADPHONE, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);
	if (int_val == CONTEXT_TRIGGER_FALSE) return true;

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_TYPE);

	return true;
}

bool _trigger_cond_usb()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_USB)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_USB, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);

	return true;
}

bool _trigger_cond_wifi()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_WIFI)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_WIFI, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);

	if (str_val == CONTEXT_TRIGGER_CONNECTED)
		ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_BSSID);

	return true;
}

bool _trigger_cond_psmode()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_POWER_SAVING_MODE)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_POWER_SAVING_MODE, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_ENABLED);

	return true;
}

bool _trigger_cond_call()
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_CALL)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_CALL, NULL, &req_id, &json_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);
	if (str_val == CONTEXT_TRIGGER_IDLE) return true;

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_MEDIUM);
	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_ADDRESS);

	return true;
}

static std::string& __get_objective_app_id()
{
	static std::string app_id;

	if (!app_id.empty())
		return app_id;

	int list_size = 0;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);
	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 1);
	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);
	context_history_list_get_count(list, &list_size);

	if (list_size > 0) {
		char *buffer = NULL;
		context_history_record_h record = NULL;
		context_history_list_get_current(list, &record);
		context_history_record_get_string(record, CONTEXT_HISTORY_APP_ID, &buffer);
		if (buffer) {
			app_id = buffer;
			g_free(buffer);
		}
		context_history_record_destroy(record);
	}

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);

	if (app_id.empty())
		app_id = "org.tizen.gallery";

	return app_id;
}

static bool __trigger_cond_app_use_freq_base(ctx::json option)
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_APP_USE_FREQUENCY, &option, &req_id, &json_val);
	if (err == ERR_NO_DATA) {
		g_print(RED("No Data "));
		return true;
	}
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_APP_ID);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_RANK);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_TOTAL_COUNT);

	return true;
}

bool _trigger_cond_app_use_freq1()
{
	/* Case 1: App ID only */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_APP_ID, __get_objective_app_id());

	return __trigger_cond_app_use_freq_base(option);
}

bool _trigger_cond_app_use_freq2()
{
	/* Case 2: App ID, Day of Week */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_APP_ID, __get_objective_app_id());
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);

	return __trigger_cond_app_use_freq_base(option);
}

bool _trigger_cond_app_use_freq3()
{
	/* Case 3: App ID, Time of Day */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_APP_ID, __get_objective_app_id());
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_app_use_freq_base(option);
}

bool _trigger_cond_app_use_freq4()
{
	/* Case 3: App ID, Time of Day, Day of Week */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_APP_ID, __get_objective_app_id());
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_app_use_freq_base(option);
}

static std::string& __get_objective_address()
{
	static std::string address;

	if (!address.empty())
		return address;

	int list_size = 0;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);
	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 1);
	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS, filter, &list);
	context_history_list_get_count(list, &list_size);

	if (list_size > 0) {
		char *buffer = NULL;
		context_history_record_h record = NULL;
		context_history_list_get_current(list, &record);
		context_history_record_get_string(record, CONTEXT_HISTORY_ADDRESS, &buffer);
		if (buffer) {
			address = buffer;
			g_free(buffer);
		}
		context_history_record_destroy(record);
	}

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);

	if (address.empty())
		address = "000-000-0000";

	return address;
}

static bool __trigger_cond_comm_freq_base(ctx::json option)
{
	if (!__support(CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY)) return false;

	err = ctx::request_handler::read_sync(CT_CONDITION_COMMUNICATION_FREQUENCY, &option, &req_id, &json_val);
	if (err == ERR_NO_DATA) {
		g_print(RED("No Data "));
		return true;
	}
	ASSERT_CMPINT(err, ==, ERR_NONE);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_ADDRESS);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_RANK);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_TOTAL_COUNT);

	return true;
}

bool _trigger_cond_comm_freq1()
{
	/* Case 1: Address only */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_ADDRESS, __get_objective_address());

	return __trigger_cond_comm_freq_base(option);
}

bool _trigger_cond_comm_freq2()
{
	/* Case 2: Address, Day of Week */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_ADDRESS, __get_objective_address());
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);

	return __trigger_cond_comm_freq_base(option);
}

bool _trigger_cond_comm_freq3()
{
	/* Case 3: Address, Time of Day */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_ADDRESS, __get_objective_address());
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_comm_freq_base(option);
}

bool _trigger_cond_comm_freq4()
{
	/* Case 4: Address, Day of Week, Time of Day */
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_ADDRESS, __get_objective_address());
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_comm_freq_base(option);
}

static bool __trigger_cond_media_freq_base(context_trigger_condition_e item, const char *item_str, ctx::json option)
{
	if (!__support(item)) return false;

	err = ctx::request_handler::read_sync(item_str, &option, &req_id, &json_val);
	if (err == ERR_NO_DATA) {
		g_print(YELLOW("No Data "));
		return true;
	}
	ASSERT_CMPINT(err, ==, ERR_NONE);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_TOTAL_COUNT);

	return true;
}

bool _trigger_cond_music_freq1()
{
	ctx::json option;

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,
			CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_music_freq2()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,
			CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_music_freq3()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,
			CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_music_freq4()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,
			CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_video_freq1()
{
	ctx::json option;

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY,
			CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_video_freq2()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY,
			CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_video_freq3()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY,
			CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY, option);
}

bool _trigger_cond_video_freq4()
{
	ctx::json option;
	option.set(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, CONTEXT_TRIGGER_WEEKDAY);
	option.set(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, "10-16");

	return __trigger_cond_media_freq_base(
			CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY,
			CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY, option);
}
