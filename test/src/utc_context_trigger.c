/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include <glib.h>
#include <dlog.h>
#include <context_trigger.h>
#include <context_trigger_internal.h>
#include "utc_context_trigger.h"
#include <stdlib.h>

#define RED(X)		"\033[1;31m"X"\033[0m"
#define GREEN(X)	"\033[1;32m"X"\033[0m"
#define YELLOW(X)	"\033[1;33m"X"\033[0m"
#define BLUE(X)		"\033[1;34m"X"\033[0m"

static GMainLoop *mainloop = NULL;
static int rid;

app_control_h app;

void initialize(void)
{
	g_print("Initialize");

	int ret = app_control_create(&app);
	g_assert_cmpint(ret, ==, APP_CONTROL_ERROR_NONE);

	ret = app_control_set_operation(app, APP_CONTROL_OPERATION_DEFAULT);
	g_assert_cmpint(ret, ==, APP_CONTROL_ERROR_NONE);

	ret = app_control_set_app_id (app, "org.tizen.setting");
	g_assert_cmpint(ret, ==, APP_CONTROL_ERROR_NONE);
}

void release(void)
{
	g_print("Release");

	int ret = app_control_destroy(app);
	g_assert_cmpint(ret, ==, APP_CONTROL_ERROR_NONE);
}

static void triggered_cb(int rule_id, context_trigger_error_e error, void* user_data)
{
	g_print(GREEN("#ContextTrigger# Rule%d triggered. error = %d\n"), rule_id, error);

	g_main_loop_quit(mainloop);

	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(rid, ==, rule_id);
	rid = 0;
}

void utc_context_trigger_create_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;

	error = context_trigger_create(&handle);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_destroy(handle);
}

void utc_context_trigger_create_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_create(NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_destroy_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle;

	context_trigger_create(&handle);
	error = context_trigger_destroy(handle);

	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
}

void utc_context_trigger_destroy_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_set_callback_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;

	error = context_trigger_create(&handle);

	error = context_trigger_set_callback(handle, triggered_cb);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_destroy(handle);
}

void utc_context_trigger_set_callback_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;

	error = context_trigger_create(&handle);

	error = context_trigger_set_callback(NULL, triggered_cb);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_set_callback(handle, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_destroy(handle);
}

void utc_context_trigger_add_rule_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_h rule2 = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h event2 = NULL;
	context_trigger_rule_entry_h condition = NULL;
	context_trigger_rule_entry_h condition2 = NULL;
	context_trigger_rule_entry_h condition3 = NULL;
	int rule_id;
	int rule_id2;

	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_DAY_OF_WEEK, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition3);
	context_trigger_rule_entry_add_key(condition3, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "DayOfWeek");
	context_trigger_rule_entry_add_comparison_string(condition3, "DayOfWeek", "==", "Mon");

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_add_entry(rule, condition);
	context_trigger_rule_add_entry(rule, condition3);
	context_trigger_rule_set_action_app_control(rule, app);

	error = context_trigger_add_rule(rule, &rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(rule_id, >, 0);

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule2);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, &event2);
	context_trigger_rule_entry_add_key(event2, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event2, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, &condition2);
	context_trigger_rule_entry_add_key(condition2, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition2, "Level", "==", "Normal");
	context_trigger_rule_add_entry(rule2, event2);
	context_trigger_rule_add_entry(rule2, condition3);
	context_trigger_rule_add_entry(rule2, condition2);
	context_trigger_rule_set_action_app_control(rule2, app);

	error = context_trigger_add_rule(rule2, &rule_id2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(rule_id, ==, rule_id2);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_entry_destroy(event2);
	context_trigger_rule_entry_destroy(condition2);
	context_trigger_rule_entry_destroy(condition3);
	context_trigger_rule_destroy(rule);
	context_trigger_rule_destroy(rule2);
}

void utc_context_trigger_add_rule_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;
	int rule_id;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");
	context_trigger_rule_add_entry(rule, condition);

	error = context_trigger_add_rule(rule, &rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_add_entry(rule, event);

	error = context_trigger_add_rule(rule, &rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_set_action_app_control(rule, app);

	error = context_trigger_add_rule(NULL, &rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_add_rule(rule, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_remove_rule_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);

	error = context_trigger_remove_rule(rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_remove_rule_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);
	context_trigger_enable_rule_with_handle(handle, rule_id, NULL);

	error = context_trigger_remove_rule(-1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_remove_rule(rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_ENABLED);

	error = context_trigger_remove_rule(rule_id + 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST);

	context_trigger_disable_rule_with_handle(handle, rule_id);
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_enable_rule_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "High");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Full");

	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_add_entry(rule, condition);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);
	rid = rule_id;

	error = context_trigger_enable_rule_with_handle(handle, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	g_print(YELLOW("\n#ContextTrigger# When any type of headphone is connected, if battery is normal, high, or full...\n"));
	mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	g_main_loop_unref(mainloop);

	context_trigger_disable_rule_with_handle(handle, rule_id);
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_enable_rule_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);

	error = context_trigger_enable_rule_with_handle(NULL, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_enable_rule_with_handle(handle, -1, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_enable_rule_with_handle(handle, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_CALLBACK_NOT_REGISTERED);

	context_trigger_set_callback(handle, triggered_cb);

	error = context_trigger_enable_rule_with_handle(handle, rule_id+1, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST);

	context_trigger_enable_rule_with_handle(handle, rule_id, NULL);

	error = context_trigger_enable_rule_with_handle(handle, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_ENABLED);

	context_trigger_disable_rule_with_handle(handle, rule_id);
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_disable_rule_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);
	context_trigger_enable_rule_with_handle(handle, rule_id, NULL);

	error = context_trigger_disable_rule_with_handle(handle, rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_disable_rule_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);
	context_trigger_enable_rule_with_handle(handle, rule_id, NULL);

	error = context_trigger_disable_rule_with_handle(NULL, rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_disable_rule_with_handle(handle, -1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_disable_rule_with_handle(handle, rule_id + 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST);

	context_trigger_disable_rule_with_handle(handle, rule_id);

	error = context_trigger_disable_rule_with_handle(handle, rule_id);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_NOT_ENABLED);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_get_own_rule_ids_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule1 = NULL;
	context_trigger_rule_h rule2 = NULL;
	context_trigger_rule_entry_h event1 = NULL;
	context_trigger_rule_entry_h event2 = NULL;
	int rule_id1;
	int rule_id2;
	int* enabled_arr;
	int enabled_count;
	int* disabled_arr;
	int disabled_count;

	error = context_trigger_get_own_rule_ids(&enabled_arr, &enabled_count, &disabled_arr, &disabled_count);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(enabled_count, ==, 0);
	g_assert_cmpint(disabled_count, ==, 0);

	if (enabled_arr) {
		free(enabled_arr);
		enabled_arr = NULL;
	}

	if (disabled_arr) {
		free(disabled_arr);
		disabled_arr = NULL;
	}

	context_trigger_create(&handle);
	context_trigger_set_callback(handle, triggered_cb);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule1);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event1);
	context_trigger_rule_add_entry(rule1, event1);
	context_trigger_rule_set_action_app_control(rule1, app);
	context_trigger_add_rule(rule1, &rule_id1);
	context_trigger_enable_rule_with_handle(handle, rule_id1, NULL);

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule2);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event2);
	context_trigger_rule_add_entry(rule2, event2);
	context_trigger_rule_set_action_app_control(rule2, app);
	context_trigger_add_rule(rule2, &rule_id2);

	error = context_trigger_get_own_rule_ids(&enabled_arr, &enabled_count, &disabled_arr, &disabled_count);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpint(enabled_count, ==, 1);
	g_assert_cmpint(disabled_count, ==, 1);
	g_assert_cmpint(rule_id1, ==, enabled_arr[0]);
	g_assert_cmpint(rule_id2, ==, disabled_arr[0]);

	if (enabled_arr) {
		free(enabled_arr);
		enabled_arr = NULL;
	}

	if (disabled_arr) {
		free(disabled_arr);
		disabled_arr = NULL;
	}

	context_trigger_disable_rule_with_handle(handle, rule_id1);
	context_trigger_remove_rule(rule_id1);
	context_trigger_remove_rule(rule_id2);
	context_trigger_rule_entry_destroy(event1);
	context_trigger_rule_entry_destroy(event2);
	context_trigger_rule_destroy(rule1);
	context_trigger_rule_destroy(rule2);
	context_trigger_destroy(handle);
}

void utc_context_trigger_get_own_rule_ids_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	int* arr1;
	int count1;
	int* arr2;
	int count2;

	error = context_trigger_get_own_rule_ids(&arr1, NULL, NULL, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(NULL, &count1, NULL, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(NULL, NULL, &arr2, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(NULL, NULL, NULL, &count2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(&arr1, &count1, &arr2, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(&arr1, &count1, NULL, &count2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(NULL, &count1, &arr2, &count2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_own_rule_ids(&arr1, NULL, &arr2, &count2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_get_rule_by_id_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;
	context_trigger_rule_h rule_result = NULL;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);

	error = context_trigger_get_rule_by_id(rule_id, &rule_result);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_rule_destroy(rule_result);
}

void utc_context_trigger_get_rule_by_id_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;
	context_trigger_rule_h rule_result = NULL;

	error = context_trigger_get_rule_by_id(0, &rule_result);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);

	error = context_trigger_get_rule_by_id(rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_get_rule_by_id(rule_id + 1, &rule_result);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_create_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;

	error = context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_create_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;

	error = context_trigger_rule_create(-1, &rule);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_destroy_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);

	error = context_trigger_rule_destroy(rule);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
}

void utc_context_trigger_rule_destroy_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_rule_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_add_entry_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");

	error = context_trigger_rule_add_entry(rule, event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	error = context_trigger_rule_add_entry(rule, condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_add_entry_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h event2 = NULL;
	context_trigger_rule_entry_h condition = NULL;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event2);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_DAY_OF_WEEK, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);

	error = context_trigger_rule_add_entry(NULL, event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_add_entry(rule, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_add_entry(rule, event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	error = context_trigger_rule_add_entry(rule, event2);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	error = context_trigger_rule_add_entry(rule, condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	error = context_trigger_rule_add_entry(rule, condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(event2);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_set_action_app_control_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "High");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Full");

	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_add_entry(rule, condition);

	error = context_trigger_rule_set_action_app_control(rule, app);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);

	error = context_trigger_enable_rule_with_handle(handle, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	rid = rule_id;

	g_print(YELLOW("\n#ContextTrigger# When any type of headphone is connected, if battery is normal, high, or full.... setting app will be executed...\n"));
	mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	g_main_loop_unref(mainloop);

	context_trigger_disable_rule_with_handle(handle, rule_id);
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_rule_set_action_app_control_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_rule_set_action_app_control(NULL, app);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_h rule = NULL;
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	app_control_h invalid_app = NULL;

	error = context_trigger_rule_set_action_app_control(rule, invalid_app);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_set_action_notification_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_h handle = NULL;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;
	int rule_id;

	context_trigger_create(&handle);
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	context_trigger_rule_entry_add_comparison_int(event, "IsConnected", "==", CONTEXT_TRIGGER_TRUE);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "Level");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Normal");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "High");
	context_trigger_rule_entry_add_comparison_string(condition, "Level", "==", "Full");

	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_add_entry(rule, condition);

	error = context_trigger_rule_set_action_notification(rule, "Notification title", "Notification content", NULL, app);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_add_rule(rule, &rule_id);
	context_trigger_set_callback(handle, triggered_cb);

	error = context_trigger_enable_rule_with_handle(handle, rule_id, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	rid = rule_id;

	g_print(YELLOW("\n#ContextTrigger# When any type of headphone is connected, if battery is normal, high, or full.... notification will be posted...\n"));
	mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	g_main_loop_unref(mainloop);

	context_trigger_disable_rule_with_handle(handle, rule_id);
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_entry_destroy(condition);
	context_trigger_rule_destroy(rule);
	context_trigger_destroy(handle);
}

void utc_context_trigger_rule_set_action_notification_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_rule_set_action_notification(NULL, "Notification title", "Notification content", NULL, NULL);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_h rule = NULL;
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);

	error = context_trigger_rule_set_action_notification(rule, NULL, "Notification content", NULL, NULL);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_set_action_notification(rule, "Notification title", NULL, NULL, NULL);
	g_assert_cmpint(error, == , CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_set_description_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);

	error = context_trigger_rule_set_description(rule, "Rule description test");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_destroy(rule);
}

void utc_context_trigger_rule_set_description_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_rule_set_description(NULL, "Rule description test");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_get_description_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;
	context_trigger_rule_h rule_result = NULL;
	const char* description = "When Headphone state changed";
	char* des_result;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_set_description(rule, description);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_get_rule_by_id(rule_id, &rule_result);

	error = context_trigger_rule_get_description(rule_result, &des_result);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert_cmpstr(des_result, ==, description);

	if (des_result) {
		free(des_result);
		des_result = NULL;
	}
	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_rule_destroy(rule_result);
}

void utc_context_trigger_rule_get_description_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_entry_h event = NULL;
	int rule_id;
	context_trigger_rule_h rule_result = NULL;
	const char* description = "When Headphone state changed";
	char* des_result;

	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);
	context_trigger_rule_set_description(rule, description);
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_set_action_app_control(rule, app);
	context_trigger_add_rule(rule, &rule_id);
	context_trigger_get_rule_by_id(rule_id, &rule_result);

	error = context_trigger_rule_get_description(NULL, &des_result);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_get_description(rule_result, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_remove_rule(rule_id);
	context_trigger_rule_entry_destroy(event);
	context_trigger_rule_destroy(rule);
	context_trigger_rule_destroy(rule_result);
}

void utc_context_trigger_rule_event_create_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	error = context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_event_create_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	error = context_trigger_rule_event_create(-1, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, -1, &event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_event_is_supported_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	bool supported;

	error = context_trigger_rule_event_is_supported(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert(supported);

	error = context_trigger_rule_event_is_supported(CONTEXT_TRIGGER_EVENT_ON_TIME, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert(supported);
}

void utc_context_trigger_rule_event_is_supported_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	bool supported;

	error = context_trigger_rule_event_is_supported(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_event_is_supported(-1, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_condition_create_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h condition = NULL;

	error = context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(condition);
}

void utc_context_trigger_rule_condition_create_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h condition = NULL;

	error = context_trigger_rule_condition_create(-1, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_HEADPHONE, -1, &condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_condition_is_supported_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	bool supported;

	error = context_trigger_rule_condition_is_supported(CONTEXT_TRIGGER_CONDITION_SYSTEM_HEADPHONE, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert(supported);

	error = context_trigger_rule_condition_is_supported(CONTEXT_TRIGGER_CONDITION_DAY_OF_MONTH, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
	g_assert(supported);
}

void utc_context_trigger_rule_condition_is_supported_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	bool supported;

	error = context_trigger_rule_condition_is_supported(CONTEXT_TRIGGER_CONDITION_SYSTEM_HEADPHONE, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_condition_is_supported(-1, &supported);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_entry_destroy_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_entry_h condition = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);

	error = context_trigger_rule_entry_destroy(event);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	error = context_trigger_rule_entry_destroy(condition);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);
}

void utc_context_trigger_rule_entry_destroy_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;

	error = context_trigger_rule_entry_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);
}

void utc_context_trigger_rule_entry_add_key_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);

	error = context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_entry_add_key_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_HEADPHONE, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);

	error = context_trigger_rule_entry_add_key(NULL, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "IsConnected");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_key(event, -1, "IsConnected");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, NULL);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_entry_add_comparison_int_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_ON_TIME, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "TimeOfDay");

	error = context_trigger_rule_entry_add_comparison_int(event, "TimeOfDay", "==", 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_entry_add_comparison_int_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_ON_TIME, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);

	error = context_trigger_rule_entry_add_comparison_int(event, "TimeOfDay", "==", 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NO_DATA);

	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, "TimeOfDay");

	error = context_trigger_rule_entry_add_comparison_int(NULL, "TimeOfDay", "==", 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_int(event, NULL, "==", 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_int(event, "TimeOfDay", NULL, 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_int(event, "TimeOfDay", "and", 16);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_entry_destroy(event);
}

/*
void utc_context_trigger_rule_entry_add_comparison_double_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);

	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "res");

	error = context_trigger_rule_entry_add_comparison_double(event, "res", "==", 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_entry_add_comparison_double_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);

	error = context_trigger_rule_entry_add_comparison_double(event, "res", "==", 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NO_DATA);

	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "res");

	error = context_trigger_rule_entry_add_comparison_double(NULL, "res", "==", 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_double(event, NULL, "==", 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_double(event, "res", NULL, 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_double(event, "res", "and", 1);
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_entry_destroy(event);
}
*/
void utc_context_trigger_rule_entry_add_comparison_string_p(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "Level");

	error = context_trigger_rule_entry_add_comparison_string(event, "Level", "==", "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NONE);

	context_trigger_rule_entry_destroy(event);
}

void utc_context_trigger_rule_entry_add_comparison_string_n(void)
{
	int error = CONTEXT_TRIGGER_ERROR_NONE;
	context_trigger_rule_entry_h event = NULL;

	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_SYSTEM_BATTERY, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	error = context_trigger_rule_entry_add_comparison_string(event, "Level", "==", "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_NO_DATA);

	context_trigger_rule_entry_add_key(event, CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, "Level");

	error = context_trigger_rule_entry_add_comparison_string(NULL, "Level", "==", "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_string(event, NULL, "==", "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_string(event, "Level", NULL, "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	error = context_trigger_rule_entry_add_comparison_string(event, "Level", "and", "Normal");
	g_assert_cmpint(error, ==, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	context_trigger_rule_entry_destroy(event);
}
