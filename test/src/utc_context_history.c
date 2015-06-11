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
#include <context_history.h>
#include "utc_context_history.h"
#include <stdlib.h>

#define RED(X)		"\033[1;31m"X"\033[0m"
#define GREEN(X)	"\033[1;32m"X"\033[0m"
#define YELLOW(X)	"\033[1;33m"X"\033[0m"
#define BLUE(X)		"\033[1;34m"X"\033[0m"

void utc_context_history_create_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;

	error = context_history_create(&handle);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_destroy(handle);
}

void utc_context_history_create_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_create(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}


void utc_context_history_destroy_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle;

	context_history_create(&handle);
	error = context_history_destroy(handle);

	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);
}

void utc_context_history_destroy_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_filter_create_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_filter_h filter = NULL;

	error = context_history_filter_create(&filter);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_filter_destroy(filter);
}

void utc_context_history_filter_create_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_filter_create(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_filter_destroy_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_filter_h filter;

	context_history_filter_create(&filter);
	error = context_history_filter_destroy(filter);

	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);
}

void utc_context_history_filter_destroy_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_filter_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_filter_set_int_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_filter_h filter = NULL;

	context_history_filter_create(&filter);

	error = context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 10);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_filter_destroy(filter);
}

void utc_context_history_filter_set_int_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_filter_set_int(NULL, CONTEXT_HISTORY_FILTER_TIME_SPAN, 10);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_filter_set_string_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_filter_h filter = NULL;

	context_history_filter_create(&filter);

	error = context_history_filter_set_string(filter, CONTEXT_HISTORY_FILTER_APP_ID, "org.tizen.sample");
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_filter_destroy(filter);
}

void utc_context_history_filter_set_string_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_filter_set_string(NULL, CONTEXT_HISTORY_FILTER_APP_ID, "org.tizen.sample");
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_get_list_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	error = context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_get_list_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	error = context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, NULL, &list);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	context_history_list_destroy(list);
	context_history_destroy(handle);
}

void utc_context_history_list_get_count_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	int count;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_get_count(list, &count);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_get_count_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_get_count(list, NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_get_current_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;
	context_history_record_h record = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_get_current(list, &record);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_get_current_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_get_current(list, NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_move_first_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_move_first(list);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_move_first_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_list_move_first(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_list_move_next_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error = context_history_list_move_next(list);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_move_next_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_list_move_next(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_list_destroy_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	error =  context_history_list_destroy(list);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_list_destroy_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_list_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_record_create_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_record_h record = NULL;

	error =  context_history_record_create(&record);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
}

void utc_context_history_record_create_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error =  context_history_record_create(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_record_get_int_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	int value;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;
	context_history_record_h record = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	context_history_list_get_current(list, &record);

	error = context_history_record_get_int(record, "UsedCount", &value);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_record_get_int_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;
	context_history_record_h record = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	context_history_list_get_current(list, &record);

	error = context_history_record_get_int(record, "UsedCount", NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	context_history_record_destroy(record);
	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_record_get_string_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	char *value = NULL;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;
	context_history_record_h record = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	context_history_list_get_current(list, &record);

	error = context_history_record_get_string(record, "AppId", &value);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	g_free(value);

	context_history_record_destroy(record);
	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_record_get_string_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_h handle = NULL;
	context_history_filter_h filter = NULL;
	context_history_list_h list = NULL;
	context_history_record_h record = NULL;

	context_history_create(&handle);

	context_history_filter_create(&filter);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
	context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 10);

	context_history_get_list(handle, CONTEXT_HISTORY_FREQUENTLY_USED_APP, filter, &list);

	context_history_list_get_current(list, &record);

	error = context_history_record_get_string(record, "AppId", NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	context_history_record_destroy(record);
	context_history_list_destroy(list);
	context_history_filter_destroy(filter);
	context_history_destroy(handle);
}

void utc_context_history_record_set_int_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_record_h record = NULL;

	context_history_record_create(&record);

	error =  context_history_record_set_int(record, "LastPosition", 200);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
}

void utc_context_history_record_set_int_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error =  context_history_record_set_int(NULL, "LastPosition", 200);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_record_set_string_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_record_h record = NULL;

	context_history_record_create(&record);

	error =  context_history_record_set_string(record, "AppId", "org.tizen.sample");
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
}

void utc_context_history_record_set_string_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error =  context_history_record_set_string(NULL, "AppId", "org.tizen.sample");
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_record_destroy_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_record_h record = NULL;

	context_history_record_create(&record);

	error = context_history_record_destroy(record);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);
}

void utc_context_history_record_destroy_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_record_destroy(NULL);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}

void utc_context_history_record_insert_p(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;
	context_history_record_h record = NULL;

	context_history_record_create(&record);

	context_history_record_set_string(record, "AppId", "org.tizen.sample");
	context_history_record_set_int(NULL, "LastPosition", 200);

	error = context_history_record_insert(record, CONTEXT_HISTORY_START_MUSIC);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_NONE);

	context_history_record_destroy(record);
}

void utc_context_history_record_insert_n(void)
{
	int error = CONTEXT_HISTORY_ERROR_NONE;

	error = context_history_record_insert(NULL, CONTEXT_HISTORY_START_MUSIC);
	g_assert_cmpint(error, ==, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);
}


