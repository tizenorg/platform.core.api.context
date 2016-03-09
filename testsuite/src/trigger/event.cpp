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

#include <time.h>
#include <request_handler.h>
#include <context_trigger.h>
#include <context_trigger_types_internal.h>

#include "../shared.h"
#include "event.h"

using namespace ctx::test;

static void __domain_cb(const char *item, int rid, int error, ctx::Json data)
{
	g_print("[%d] %s: Error=%#x, Data=%s\n", callback_count--, item, error, data.str().c_str());

	if (callback_count == 0) {
		json_val = data;
		stop_mainloop();
	}
}

int test_trigger_event(int *argc, char ***argv)
{
	g_test_init(argc, argv, NULL);

	ctx::request_handler::register_callback(CT_EVENT_BATTERY, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_CHARGER, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_GPS, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_HEADPHONE, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_POWER_SAVING_MODE, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_USB, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_WIFI, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_CALL, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_TIME, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_EMAIL, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_MESSAGE, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_ACTIVITY_STATIONARY, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_ACTIVITY_WALKING, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_ACTIVITY_RUNNING, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_ACTIVITY_IN_VEHICLE, __domain_cb);
	ctx::request_handler::register_callback(CT_EVENT_PLACE, __domain_cb);

	run_testcase("/trigger/event/battery", _trigger_event_battery);
	run_testcase("/trigger/event/charger", _trigger_event_charger);
	run_testcase("/trigger/event/gps", _trigger_event_gps);
	run_testcase("/trigger/event/headphone", _trigger_event_headphone);
	run_testcase("/trigger/event/usb", _trigger_event_usb);
	run_testcase("/trigger/event/wifi", _trigger_event_wifi);
	run_testcase("/trigger/event/psmode", _trigger_event_psmode);
	run_testcase("/trigger/event/call", _trigger_event_call);
	run_testcase("/trigger/event/time", _trigger_event_time);
	run_testcase("/trigger/event/email", _trigger_event_email);
	run_testcase("/trigger/event/message", _trigger_event_message);
	run_testcase("/trigger/event/activity", _trigger_event_activity);
	run_testcase("/trigger/event/place", _trigger_event_place);

	return g_test_run();
}

static bool __support(context_trigger_event_e ev)
{
	err = context_trigger_rule_event_is_supported(ev, &bool_val);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	if (!bool_val) {
		g_print(YELLOW("Not Supported") "\n");
		return false;
	}

	return true;
}

bool _trigger_event_battery()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_BATTERY)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_BATTERY, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Change the battery lavel (or charging status) %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_BATTERY, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_LEVEL);
	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CHARGING);

	return true;
}

bool _trigger_event_charger()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_CHARGER)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_CHARGER, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Change the charger connection state %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_CHARGER, req_id);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);

	return true;
}

bool _trigger_event_gps()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_GPS)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_GPS, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Change the GPS status %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_GPS, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);

	return true;
}

bool _trigger_event_headphone()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_HEADPHONE)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_HEADPHONE, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Connect/disconnect a headphone %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_HEADPHONE, req_id);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);
	if (int_val == CONTEXT_TRIGGER_FALSE) return true;

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_TYPE);

	return true;
}

bool _trigger_event_usb()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_USB)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_USB, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Connect/disconnect USB %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_USB, req_id);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_CONNECTED);

	return true;
}

bool _trigger_event_wifi()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_WIFI)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_WIFI, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Change the WiFi state %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_WIFI, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);

	if (str_val == CONTEXT_TRIGGER_CONNECTED)
		ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_BSSID);

	return true;
}

bool _trigger_event_psmode()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_POWER_SAVING_MODE)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_POWER_SAVING_MODE, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Turn on/off the power-saving mode %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_POWER_SAVING_MODE, req_id);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_IS_ENABLED);

	return true;
}

bool _trigger_event_call()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_CALL)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_CALL, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Make a phone call to this device to change call state %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_CALL, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_STATE);
	if (str_val == CONTEXT_TRIGGER_IDLE) return true;

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_MEDIUM);
	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_ADDRESS);

	return true;
}

bool _trigger_event_time()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_TIME)) return false;

	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	tzset();
	localtime_r(&rawtime, &timeinfo);
	int dow = timeinfo.tm_wday;
	int tod = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;
	tod = (tod + 70) / 60;

	std::string dow_str;
	switch (dow) {
	case 0:
		dow_str = CONTEXT_TRIGGER_SUN;
		break;
	case 1:
		dow_str = CONTEXT_TRIGGER_MON;
		break;
	case 2:
		dow_str = CONTEXT_TRIGGER_TUE;
		break;
	case 3:
		dow_str = CONTEXT_TRIGGER_WED;
		break;
	case 4:
		dow_str = CONTEXT_TRIGGER_THU;
		break;
	case 5:
		dow_str = CONTEXT_TRIGGER_FRI;
		break;
	case 6:
		dow_str = CONTEXT_TRIGGER_SAT;
		break;
	default:
		dow_str = CONTEXT_TRIGGER_WEEKDAY;
		break;
	}
	ctx::Json option;
	option.append(NULL, CONTEXT_TRIGGER_DAY_OF_WEEK, dow_str);
	option.append(NULL, CONTEXT_TRIGGER_TIME_OF_DAY, tod);

	err = ctx::request_handler::subscribe(CT_EVENT_TIME, &option, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	callback_count = 1;
	g_print("\n> Wait until %02d:%02d %s.\n", tod / 60, tod % 60, dow_str.c_str());
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_CALL, req_id);

	ASSERT_CONTAIN_INT(json_val, CONTEXT_TRIGGER_TIME_OF_DAY);
	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_DAY_OF_WEEK);

	return true;
}

bool _trigger_event_email()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_EMAIL)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_EMAIL, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Send/receive emails %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_EMAIL, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_EVENT);

	return true;
}

bool _trigger_event_message()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_MESSAGE)) return false;

	err = ctx::request_handler::subscribe(CT_EVENT_MESSAGE, NULL, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Receive SMS/MMS %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_MESSAGE, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_EVENT);
	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_TYPE);

	return true;
}

bool _trigger_event_activity()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_ACTIVITY_STATIONARY)) return false;
	if (!__support(CONTEXT_TRIGGER_EVENT_ACTIVITY_WALKING)) return false;
	if (!__support(CONTEXT_TRIGGER_EVENT_ACTIVITY_RUNNING)) return false;
	if (!__support(CONTEXT_TRIGGER_EVENT_ACTIVITY_IN_VEHICLE)) return false;

	int req_ids[4];

	err = ctx::request_handler::subscribe(CT_EVENT_ACTIVITY_STATIONARY, NULL, &req_ids[0], NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	err = ctx::request_handler::subscribe(CT_EVENT_ACTIVITY_WALKING, NULL, &req_ids[1], NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	err = ctx::request_handler::subscribe(CT_EVENT_ACTIVITY_RUNNING, NULL, &req_ids[2], NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	err = ctx::request_handler::subscribe(CT_EVENT_ACTIVITY_IN_VEHICLE, NULL, &req_ids[3], NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Mimic any type of activity %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_ACTIVITY_STATIONARY, req_ids[0]);
	ctx::request_handler::unsubscribe(CT_EVENT_ACTIVITY_WALKING, req_ids[1]);
	ctx::request_handler::unsubscribe(CT_EVENT_ACTIVITY_RUNNING, req_ids[2]);
	ctx::request_handler::unsubscribe(CT_EVENT_ACTIVITY_IN_VEHICLE, req_ids[3]);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_EVENT);
	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_ACCURACY);

	return true;
}

bool _trigger_event_place()
{
	if (!__support(CONTEXT_TRIGGER_EVENT_PLACE)) return false;

	ctx::Json option;
	option.set(NULL, CONTEXT_TRIGGER_PLACE_ID, 2);	/* 2 => office */

	err = ctx::request_handler::subscribe(CT_EVENT_PLACE, &option, &req_id, NULL);
	ASSERT_CMPINT(err, ==, ERR_NONE);

	g_print("\n> Mimic fence in/out event to office %d times.\n", CALLBACK_COUNT);
	start_mainloop();

	ctx::request_handler::unsubscribe(CT_EVENT_PLACE, req_id);

	ASSERT_CONTAIN_STR(json_val, CONTEXT_TRIGGER_EVENT);

	return true;
}
