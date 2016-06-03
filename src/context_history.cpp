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

#include <map>
#include <Json.h>
#include <context_history.h>
#include <context_history_types_internal.h>
#include <DBusClient.h>

#define TYPE_INT 0
#define TYPE_STRING 1
#define FILTER_KEY_LIMIT 10

#define GROUP_COMMON 0
#define GROUP1 1
#define GROUP2 2
#define GROUP3 3
#define GROUP4 4
#define GROUP5 5

// handles
typedef struct _context_history_handle_s {
	/* At this point, this handle has no purpose.
	   But, it will be used to support other functionalities later,
	   e.g., async read or session management. */
	int tmp;
} _cx_history_handle;

typedef struct _context_history_filter_handle_s {
	ctx::Json jfilter;
} _cx_history_filter_handle;

typedef struct _context_history_list_handle_s {
	ctx::Json jlist;
	int current;
	_context_history_list_handle_s() {
		current = 0;
	}
} _cx_history_list_handle;

typedef struct _context_history_record_handle_s {
	ctx::Json jrecord;
} _cx_history_record_handle;

static std::string convert_filter_to_string(context_history_filter_e filter_type);
static std::string convert_data_to_string(context_history_data_e data_type);
static bool check_record_key_data_type(int type, std::string key);
static bool check_filter_data_int(context_history_filter_e filter_type, int val);
static bool check_filter_data_string(context_history_filter_e filter_type, const char* val);
static bool check_invalid_filter(context_history_data_e data_type, context_history_filter_h filter);

// life-cycle
SO_EXPORT int context_history_create(context_history_h* handle)
{
	ASSERT_NOT_NULL(handle);

	*handle = new(std::nothrow) _cx_history_handle();
	ASSERT_ALLOC(*handle);

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_destroy(context_history_h handle)
{
	ASSERT_NOT_NULL(handle);
	delete handle;
	return CONTEXT_HISTORY_ERROR_NONE;
}

// Read filter manipulation
SO_EXPORT int context_history_filter_create(context_history_filter_h* filter)
{
	ASSERT_NOT_NULL(filter);

	*filter = new(std::nothrow) _cx_history_filter_handle();
	ASSERT_ALLOC(*filter);

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_filter_destroy(context_history_filter_h filter)
{
	ASSERT_NOT_NULL(filter);
	delete filter;

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_filter_set_int(context_history_filter_h filter, context_history_filter_e filter_type, int val)
{
	ASSERT_NOT_NULL(filter);

	std::string filter_str = convert_filter_to_string(filter_type);
	if (filter_str.empty()) {
		return CONTEXT_HISTORY_ERROR_INVALID_PARAMETER;
	}

	// Check filter and its data type
	IF_FAIL_RETURN_TAG(check_filter_data_int(filter_type, val), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Filter type mismatched");

	filter->jfilter.set(NULL, filter_str.c_str(), val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_filter_set_string(context_history_filter_h filter, context_history_filter_e filter_type, const char* val)
{
	ASSERT_NOT_NULL(filter);
	ASSERT_NOT_NULL(val);

	std::string filter_str = convert_filter_to_string(filter_type);
	if (filter_str.empty()) {
		return CONTEXT_HISTORY_ERROR_INVALID_PARAMETER;
	}

	// Check filter and its data type
	IF_FAIL_RETURN_TAG(check_filter_data_string(filter_type, val), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Filter type mismatched");

	filter->jfilter.set(NULL, filter_str.c_str(), val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_get_list(context_history_h handle, context_history_data_e data_type, context_history_filter_h filter, context_history_list_h* list)
{
	ASSERT_NOT_NULL(handle);
	ASSERT_NOT_NULL(list);
	*list = NULL;

	/*TODO: Boundary check for filter values has to be done*/

	std::string data_type_str = convert_data_to_string(data_type);
	if (data_type_str.empty()) {
		return CONTEXT_HISTORY_ERROR_INVALID_PARAMETER;
	}

	// Check data type & filter
	if (filter)
		IF_FAIL_RETURN_TAG(check_invalid_filter(data_type, filter), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Invalid filter key");

	int req_id;
	ctx::Json tmp_list;
	ctx::DBusClient dbusClient;

	int err = dbusClient.readSync(data_type_str, filter ? filter->jfilter : NULL, &req_id, &tmp_list);
	IF_FAIL_RETURN_TAG(err == ERR_NONE, err, _E, "Getting list failed");

	_J("Read response", tmp_list);

	IF_FAIL_RETURN_TAG(tmp_list.getSize(NULL, CONTEXT_HISTORY_DATA) > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _D, "No data");

	*list = new(std::nothrow) _cx_history_list_handle();
	ASSERT_ALLOC(*list);
	(*list)->jlist = tmp_list;
	(*list)->current = 0;

	return CONTEXT_HISTORY_ERROR_NONE;
}

// Data object manipulation
SO_EXPORT int context_history_list_get_count(context_history_list_h list, int* count)
{
	ASSERT_NOT_NULL(list);
	ASSERT_NOT_NULL(count);
	*count = 0;

	int result = list->jlist.getSize(NULL, CONTEXT_HISTORY_DATA);
	IF_FAIL_RETURN(result > 0, CONTEXT_HISTORY_ERROR_OPERATION_FAILED);

	*count = result;

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_list_get_current(context_history_list_h list, context_history_record_h* record)
{
	ASSERT_NOT_NULL(list);
	ASSERT_NOT_NULL(record);
	*record = NULL;

	ctx::Json tmp_record;
	int error = list->jlist.getAt(NULL, CONTEXT_HISTORY_DATA, list->current, &tmp_record);
	IF_FAIL_RETURN_TAG(error, CONTEXT_HISTORY_ERROR_OPERATION_FAILED, _E, "Record load failed");

	*record = new(std::nothrow) _cx_history_record_handle();
	ASSERT_ALLOC(*record);
	(*record)->jrecord = tmp_record;

	return CONTEXT_HISTORY_ERROR_NONE;
}


SO_EXPORT int context_history_list_move_first(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	list->current = 0;

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_list_move_next(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	IF_FAIL_RETURN_TAG(list->current+1 < list->jlist.getSize(NULL, CONTEXT_HISTORY_DATA), CONTEXT_HISTORY_ERROR_NO_DATA, _D, "End of list");

	list->current++;

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_list_destroy(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	delete list;

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_record_get_int(context_history_record_h record, const char* key, int* val)
{
	ASSERT_NOT_NULL(record && val && key);

	// Check key and data type
	IF_FAIL_RETURN_TAG(check_record_key_data_type(TYPE_INT, key), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Data type mismatched");

	std::list<std::string> key_list;
	record->jrecord.getKeys(&key_list);
	IF_FAIL_RETURN_TAG(key_list.size() > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _E, "No data");

	// Check invalid record key
	IF_FAIL_RETURN_TAG(record->jrecord.get(NULL, key, val), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Invalid record key");

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_record_get_string(context_history_record_h record, const char* key, char** val)
{
	ASSERT_NOT_NULL(record && val && key);

	// Check key and data type
	IF_FAIL_RETURN_TAG(check_record_key_data_type(TYPE_STRING, key), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Data type mismatched");

	std::list<std::string> key_list;
	record->jrecord.getKeys(&key_list);
	IF_FAIL_RETURN_TAG(key_list.size() > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _E, "No data");

	// Check Invalid record key
	std::string str;
	IF_FAIL_RETURN_TAG(record->jrecord.get(NULL, key, &str), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Value load failed");

	*val = g_strdup(str.c_str());
	ASSERT_ALLOC(*val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

SO_EXPORT int context_history_record_destroy(context_history_record_h record)
{
	ASSERT_NOT_NULL(record);
	delete record;

	return CONTEXT_HISTORY_ERROR_NONE;
}

std::string convert_filter_to_string(context_history_filter_e filter_type)
{
	std::string str;
	switch (filter_type) {
	case CONTEXT_HISTORY_FILTER_TIME_SPAN:
		str = FILTER_TIME_SPAN;
		break;
	case CONTEXT_HISTORY_FILTER_RESULT_SIZE:
		str = FILTER_RESULT_SIZE;
		break;
	case CONTEXT_HISTORY_FILTER_APP_ID:
		str = FILTER_APP_ID;
		break;
	case CONTEXT_HISTORY_FILTER_DAY_OF_WEEK:
		str = FILTER_DAY_OF_WEEK;
		break;
	case CONTEXT_HISTORY_FILTER_START_TIME:
		str = FILTER_START_TIME;
		break;
	case CONTEXT_HISTORY_FILTER_END_TIME:
		str = FILTER_END_TIME;
		break;
	case CONTEXT_HISTORY_FILTER_WIFI_BSSID:
		str = FILTER_WIFI_BSSID;
		break;
	case CONTEXT_HISTORY_FILTER_AUDIO_JACK:
		str = FILTER_AUDIO_JACK;
		break;
	case CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE:
		str = FILTER_COMMUNICATION_TYPE;
		break;
	default:
		break;
	}
	return str;
}

std::string convert_data_to_string(context_history_data_e data_type)
{
	std::string str;
	switch (data_type) {
	case CONTEXT_HISTORY_RECENTLY_USED_APP:
		str = SUBJ_RECENTLY_USED_APP;
		break;
	case CONTEXT_HISTORY_FREQUENTLY_USED_APP:
		str = SUBJ_FREQUENTLY_USED_APP;
		break;
	case CONTEXT_HISTORY_RARELY_USED_APP:
		str = SUBJ_RARELY_USED_APP;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_APP:
		str = SUBJ_PEAK_TIME_FOR_APP;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC:
		str = SUBJ_PEAK_TIME_FOR_MUSIC;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO:
		str = SUBJ_PEAK_TIME_FOR_VIDEO;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_APP:
		str = SUBJ_COMMON_SETTING_FOR_APP;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC:
		str = SUBJ_COMMON_SETTING_FOR_MUSIC;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO:
		str = SUBJ_COMMON_SETTING_FOR_VIDEO;
		break;
	case CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS:
		str = SUBJ_FREQUENTLY_COMMUNICATED_ADDRESS;
		break;
	default:
		break;
	}
	return str;
}

bool check_record_key_data_type(int type, std::string key)
{
	if ((key.compare(CONTEXT_HISTORY_APP_ID) == 0) ||
			key.compare(CONTEXT_HISTORY_ADDRESS) == 0) {
		return (type == TYPE_STRING);
	}

	if ((key.compare(CONTEXT_HISTORY_TOTAL_COUNT) == 0) ||
			key.compare(CONTEXT_HISTORY_TOTAL_DURATION) == 0 ||
			key.compare(CONTEXT_HISTORY_LAST_TIME) == 0 ||
			key.compare(CONTEXT_HISTORY_HOUR_OF_DAY) == 0 ||
			key.compare(CONTEXT_HISTORY_AUDIO_JACK) == 0 ||
			key.compare(CONTEXT_HISTORY_SYSTEM_VOLUME) == 0 ||
			key.compare(CONTEXT_HISTORY_MEDIA_VOLUME) == 0) {
		return (type == TYPE_INT);
	}

	return false;
}

bool check_filter_data_int(context_history_filter_e filter_type, int val)
{
	switch (filter_type) {
	case CONTEXT_HISTORY_FILTER_TIME_SPAN:
	case CONTEXT_HISTORY_FILTER_RESULT_SIZE:
	case CONTEXT_HISTORY_FILTER_START_TIME:
	case CONTEXT_HISTORY_FILTER_END_TIME:
		if (val > 0 )
			return true;
		break;
	case CONTEXT_HISTORY_FILTER_DAY_OF_WEEK:
		if (val == CONTEXT_HISTORY_FILTER_DAY_OF_WEEK_WEEKDAYS ||
			val == CONTEXT_HISTORY_FILTER_DAY_OF_WEEK_WEEKENDS ||
			val == CONTEXT_HISTORY_FILTER_DAY_OF_WEEK_ALL)
			return true;
		break;
	case CONTEXT_HISTORY_FILTER_AUDIO_JACK:
		if (val == CONTEXT_HISTORY_FILTER_AUDIO_JACK_NOT_CONNECTED ||
			val == CONTEXT_HISTORY_FILTER_AUDIO_JACK_CONNECTED)
			return true;
		break;
	case CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE:
		if (val == CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE_CALL ||
			val == CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE_MESSAGE ||
			val == CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE_ALL)
			return true;
		break;
	default:
		return false;
	}
	return false;
}

bool check_filter_data_string(context_history_filter_e filter_type, const char* val)
{
	IF_FAIL_RETURN(val, false);

	switch (filter_type) {
	case CONTEXT_HISTORY_FILTER_APP_ID:
	case CONTEXT_HISTORY_FILTER_WIFI_BSSID:
		return (g_strstr_len(val, -1, ";") == NULL);

	default:
		return false;
	}
}

bool check_invalid_filter(context_history_data_e data_type, context_history_filter_h filter)
{
	/* This should be aligned with context_history_filter_e */
	static const char *filter_key[FILTER_KEY_LIMIT] = {
		NULL,
		FILTER_TIME_SPAN,
		FILTER_RESULT_SIZE,
		FILTER_APP_ID,
		FILTER_DAY_OF_WEEK,
		FILTER_START_TIME,
		FILTER_END_TIME,
		FILTER_WIFI_BSSID,
		FILTER_AUDIO_JACK,
		FILTER_COMMUNICATION_TYPE,
	};

	bool allowed[FILTER_KEY_LIMIT] = {false};

	allowed[CONTEXT_HISTORY_FILTER_TIME_SPAN] = true;
	allowed[CONTEXT_HISTORY_FILTER_START_TIME] = true;
	allowed[CONTEXT_HISTORY_FILTER_END_TIME] = true;
	allowed[CONTEXT_HISTORY_FILTER_RESULT_SIZE] = true;

	switch (data_type) {
	case CONTEXT_HISTORY_RECENTLY_USED_APP:
	case CONTEXT_HISTORY_FREQUENTLY_USED_APP:
		allowed[CONTEXT_HISTORY_FILTER_WIFI_BSSID] = true;
		allowed[CONTEXT_HISTORY_FILTER_AUDIO_JACK] = true;
		break;

	case CONTEXT_HISTORY_RARELY_USED_APP:
		break;

	case CONTEXT_HISTORY_PEAK_TIME_FOR_APP:
		allowed[CONTEXT_HISTORY_FILTER_APP_ID] = true;
		allowed[CONTEXT_HISTORY_FILTER_DAY_OF_WEEK] = true;
		break;

	case CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC:
	case CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO:
		allowed[CONTEXT_HISTORY_FILTER_DAY_OF_WEEK] = true;
		break;

	case CONTEXT_HISTORY_COMMON_SETTING_FOR_APP:
		allowed[CONTEXT_HISTORY_FILTER_RESULT_SIZE] = false;
		allowed[CONTEXT_HISTORY_FILTER_APP_ID] = true;
		break;

	case CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC:
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO:
		allowed[CONTEXT_HISTORY_FILTER_RESULT_SIZE] = false;
		break;

	case CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS:
		allowed[CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE] = true;
		break;

	default:
		return false;
	}

	bool found = true;
	std::list<std::string> keys;
	filter->jfilter.getKeys(&keys);

	for (std::list<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
		std::string key = (*it);
		found = false;
		for (int i = 1; i < FILTER_KEY_LIMIT; ++i) {
			if (allowed[i] && key == filter_key[i]) {
				found = true;
				break;
			}
		}
		if (found == true)
			continue;

		return false;
	}

	return true;
}
