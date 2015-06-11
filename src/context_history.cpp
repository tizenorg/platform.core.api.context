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

#include <map>
#include <json.h>
#include <request_handler.h>
#include <context_history.h>
#include <context_history_types_internal.h>

#define TYPE_INT 0
#define TYPE_STRING 1

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
	ctx::json jfilter;
} _cx_history_filter_handle;

typedef struct _context_history_list_handle_s {
	ctx::json jlist;
	int current;
	_context_history_list_handle_s()
	{
		current = 0;
	}
} _cx_history_list_handle;

typedef struct _context_history_record_handle_s {
	ctx::json jrecord;
} _cx_history_record_handle;

static std::string convert_filter_to_string(context_history_filter_e filter_type);
static std::string convert_data_to_string(context_history_data_e data_type);
static std::string convert_event_to_string(context_history_event_e event_type);
static bool check_record_key_data_type(int type, std::string key);
static bool check_filter_data_int(context_history_filter_e filter_type, int val);
static bool check_filter_data_string(context_history_filter_e filter_type, const char* val);
static bool check_invalid_filter(context_history_data_e data_type, context_history_filter_h filter);

// life-cycle
EXTAPI int context_history_create(context_history_h* handle)
{
	ASSERT_NOT_NULL(handle);

	*handle = new(std::nothrow) _cx_history_handle();
	ASSERT_ALLOC(*handle);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_destroy(context_history_h handle)
{
	ASSERT_NOT_NULL(handle);
	delete handle;
	return CONTEXT_HISTORY_ERROR_NONE;
}

// Read filter manipulation
EXTAPI int context_history_filter_create(context_history_filter_h* filter)
{
	ASSERT_NOT_NULL(filter);

	*filter = new(std::nothrow) _cx_history_filter_handle();
	ASSERT_ALLOC(*filter);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_filter_destroy(context_history_filter_h filter)
{
	ASSERT_NOT_NULL(filter);
	delete filter;

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_filter_set_int(context_history_filter_h filter, context_history_filter_e filter_type, int val)
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

EXTAPI int context_history_filter_set_string(context_history_filter_h filter, context_history_filter_e filter_type, const char* val)
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

EXTAPI int context_history_get_list(context_history_h handle, context_history_data_e data_type, context_history_filter_h filter, context_history_list_h* list)
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
	ctx::json tmp_list;
	int err = ctx::request_handler::read_sync(data_type_str.c_str(), (filter)? &filter->jfilter : NULL, &req_id, &tmp_list);
	IF_FAIL_RETURN_TAG(err==ERR_NONE, err, _E, "Getting list failed");

	_J("Read response", tmp_list);

	IF_FAIL_RETURN_TAG(tmp_list.array_get_size(NULL, CONTEXT_HISTORY_DATA) > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _D, "No data");

	*list = new(std::nothrow) _cx_history_list_handle();
	ASSERT_ALLOC(*list);
	(*list)->jlist = tmp_list;
	(*list)->current = 0;

	return CONTEXT_HISTORY_ERROR_NONE;
}

// Data object manipulation
EXTAPI int context_history_list_get_count(context_history_list_h list, int* count)
{
	ASSERT_NOT_NULL(list);
	ASSERT_NOT_NULL(count);
	*count = 0;

	int result = list->jlist.array_get_size(NULL, CONTEXT_HISTORY_DATA);
	IF_FAIL_RETURN(result > 0, CONTEXT_HISTORY_ERROR_OPERATION_FAILED);

	*count = result;

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_list_get_current(context_history_list_h list, context_history_record_h* record)
{
	ASSERT_NOT_NULL(list);
	ASSERT_NOT_NULL(record);
	*record = NULL;

	ctx::json tmp_record;
	int error = list->jlist.get_array_elem(NULL, CONTEXT_HISTORY_DATA, list->current, &tmp_record);
	IF_FAIL_RETURN_TAG(error, CONTEXT_HISTORY_ERROR_OPERATION_FAILED, _E, "Record load failed");

	*record = new(std::nothrow) _cx_history_record_handle();
	ASSERT_ALLOC(*record);
	(*record)->jrecord = tmp_record;

	return CONTEXT_HISTORY_ERROR_NONE;
}


EXTAPI int context_history_list_move_first(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	list->current = 0;

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_list_move_next(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	IF_FAIL_RETURN_TAG(list->current+1 < list->jlist.array_get_size(NULL, CONTEXT_HISTORY_DATA), CONTEXT_HISTORY_ERROR_NO_DATA, _D, "End of list");

	list->current++;

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_list_destroy(context_history_list_h list)
{
	ASSERT_NOT_NULL(list);

	delete list;

	return CONTEXT_HISTORY_ERROR_NONE;
}

// Log data manipulation
EXTAPI int context_history_record_create(context_history_record_h* record)
{
	ASSERT_NOT_NULL(record);

	*record = new(std::nothrow) _cx_history_record_handle();
	ASSERT_ALLOC(*record);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_record_get_int(context_history_record_h record, const char* key, int* val)
{
	ASSERT_NOT_NULL(record && val && key);

	// Check key and data type
	IF_FAIL_RETURN_TAG(check_record_key_data_type(TYPE_INT, key), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Data type mismatched");

	std::list<std::string> key_list;
	record->jrecord.get_keys(&key_list);
	IF_FAIL_RETURN_TAG(key_list.size() > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _E, "No data");

	// Check invalid record key
	IF_FAIL_RETURN_TAG(record->jrecord.get(NULL, key, val), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Invalid record key");

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_record_get_string(context_history_record_h record, const char* key, char** val)
{
	ASSERT_NOT_NULL(record && val && key);

	// Check key and data type
	IF_FAIL_RETURN_TAG(check_record_key_data_type(TYPE_STRING, key), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Data type mismatched");

	std::list<std::string> key_list;
	record->jrecord.get_keys(&key_list);
	IF_FAIL_RETURN_TAG(key_list.size() > 0, CONTEXT_HISTORY_ERROR_NO_DATA, _E, "No data");

	// Check Invalid record key
	std::string str;
	IF_FAIL_RETURN_TAG(record->jrecord.get(NULL, key, &str), CONTEXT_HISTORY_ERROR_INVALID_PARAMETER, _E, "Value load failed");

	*val = g_strdup(str.c_str());
	ASSERT_ALLOC(*val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_record_set_int(context_history_record_h record, const char* key, int val)
{
	ASSERT_NOT_NULL(record && key);

	record->jrecord.set(NULL, key, val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_record_set_string(context_history_record_h record, const char* key, const char* val)
{
	ASSERT_NOT_NULL(record && val && key);

	record->jrecord.set(NULL, key, val);

	return CONTEXT_HISTORY_ERROR_NONE;
}

EXTAPI int context_history_record_destroy(context_history_record_h record)
{
	ASSERT_NOT_NULL(record);
	delete record;

	return CONTEXT_HISTORY_ERROR_NONE;
}

// Write
EXTAPI int context_history_record_insert(context_history_record_h record, context_history_event_e event_type)
{
	ASSERT_NOT_NULL(record);
	IF_FAIL_RETURN(event_type > 0, CONTEXT_HISTORY_ERROR_INVALID_PARAMETER);

	std::string event_type_str = convert_event_to_string(event_type);
	if (event_type_str.empty()) {
		return CONTEXT_HISTORY_ERROR_INVALID_PARAMETER;
	}

	record->jrecord.set(NULL, COMMON_ATTR_CLIENT_APP_ID, "");
	int error = ctx::request_handler::write(event_type_str.c_str(), &record->jrecord);

	return error;
}

std::string convert_filter_to_string(context_history_filter_e filter_type)
{
	std::string str;
	switch (filter_type) {
	case CONTEXT_HISTORY_FILTER_TIME_SPAN:
		str = CONTEXT_HISTORY_FILTER_KEY_TIME_SPAN;
		break;
	case CONTEXT_HISTORY_FILTER_RESULT_SIZE:
		str = CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE;
		break;
	case CONTEXT_HISTORY_FILTER_APP_ID:
		str = CONTEXT_HISTORY_FILTER_KEY_APP_ID;
		break;
	case CONTEXT_HISTORY_FILTER_DAY_OF_WEEK:
		str = CONTEXT_HISTORY_FILTER_KEY_DAY_OF_WEEK;
		break;
	case CONTEXT_HISTORY_FILTER_START_TIME:
		str = CONTEXT_HISTORY_FILTER_KEY_START_TIME;
		break;
	case CONTEXT_HISTORY_FILTER_END_TIME:
		str = CONTEXT_HISTORY_FILTER_KEY_END_TIME;
		break;
	case CONTEXT_HISTORY_FILTER_WIFI_BSSID:
		str = CONTEXT_HISTORY_FILTER_KEY_WIFI_BSSID;
		break;
	case CONTEXT_HISTORY_FILTER_AUDIO_JACK:
		str = CONTEXT_HISTORY_FILTER_KEY_AUDIO_JACK;
		break;
	case CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE:
		str = CONTEXT_HISTORY_FILTER_KEY_COMMUNICATION_TYPE;
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
		str = CONTEXT_HISTORY_SUBJECT_RECENTLY_USED_APP;
		break;
	case CONTEXT_HISTORY_FREQUENTLY_USED_APP:
		str = CONTEXT_HISTORY_SUBJECT_FREQUENTLY_USED_APP;
		break;
	case CONTEXT_HISTORY_RARELY_USED_APP:
		str = CONTEXT_HISTORY_SUBJECT_RARELY_USED_APP;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_APP:
		str = CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_APP;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC:
		str = CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_MUSIC;
		break;
	case CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO:
		str = CONTEXT_HISTORY_SUBJECT_PEAK_TIME_FOR_VIDEO;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_APP:
		str = CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_APP;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC:
		str = CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_MUSIC;
		break;
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO:
		str = CONTEXT_HISTORY_SUBJECT_COMMON_SETTING_FOR_VIDEO;
		break;
	case CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS:
		str = CONTEXT_HISTORY_SUBJECT_FREQUENTLY_COMMUNICATED_ADDRESS;
		break;
	default:
		break;
	}
	return str;
}

std::string convert_event_to_string(context_history_event_e event_type)
{
	std::string str;
	switch (event_type) {
	case CONTEXT_HISTORY_START_MUSIC:
		str = CONTEXT_HISTORY_EVENT_START_MUSIC;
		break;
	case CONTEXT_HISTORY_STOP_MUSIC:
		str = CONTEXT_HISTORY_EVENT_STOP_MUSIC;
		break;
	case CONTEXT_HISTORY_START_VIDEO:
		str = CONTEXT_HISTORY_EVENT_START_VIDEO;
		break;
	case CONTEXT_HISTORY_STOP_VIDEO:
		str = CONTEXT_HISTORY_EVENT_STOP_VIDEO;
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
	switch (filter_type) {
	case CONTEXT_HISTORY_FILTER_APP_ID:
	case CONTEXT_HISTORY_FILTER_WIFI_BSSID:
		if (val)
			return true;
		break;
	default:
		return false;
	}
	return false;
}

bool check_invalid_filter(context_history_data_e data_type, context_history_filter_h filter)
{
	static const char* group_common[] = {CONTEXT_HISTORY_FILTER_KEY_TIME_SPAN, NULL};
	static const char* group1[] = {CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE, CONTEXT_HISTORY_FILTER_KEY_START_TIME,
									CONTEXT_HISTORY_FILTER_KEY_END_TIME, CONTEXT_HISTORY_FILTER_KEY_WIFI_BSSID,
									CONTEXT_HISTORY_FILTER_KEY_AUDIO_JACK, NULL};
	static const char* group2[] = {CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE, CONTEXT_HISTORY_FILTER_KEY_START_TIME,
									CONTEXT_HISTORY_FILTER_KEY_END_TIME, NULL};
	static const char* group3[] = {CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE, CONTEXT_HISTORY_FILTER_KEY_START_TIME,
									CONTEXT_HISTORY_FILTER_KEY_END_TIME, CONTEXT_HISTORY_FILTER_KEY_APP_ID,
									CONTEXT_HISTORY_FILTER_KEY_DAY_OF_WEEK, NULL};
	static const char* group4[] = {CONTEXT_HISTORY_FILTER_KEY_START_TIME, CONTEXT_HISTORY_FILTER_KEY_END_TIME,
									CONTEXT_HISTORY_FILTER_KEY_APP_ID, NULL};
	static const char* group5[] = {CONTEXT_HISTORY_FILTER_KEY_RESULT_SIZE, CONTEXT_HISTORY_FILTER_KEY_START_TIME,
									CONTEXT_HISTORY_FILTER_KEY_END_TIME, CONTEXT_HISTORY_FILTER_KEY_COMMUNICATION_TYPE, NULL};
	static const char** const filter_arr[] = {group_common, group1, group2, group3, group4, group5};

	int group;
	switch (data_type) {
	// Group1
	case CONTEXT_HISTORY_RECENTLY_USED_APP:
	case CONTEXT_HISTORY_FREQUENTLY_USED_APP:
		group = GROUP1;
		break;
	// Group2
	case CONTEXT_HISTORY_RARELY_USED_APP:
		group = GROUP2;
		break;
	// Group3
	case CONTEXT_HISTORY_PEAK_TIME_FOR_APP:
	case CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC:
	case CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO:
		group = GROUP3;
		break;
	// Group4
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_APP:
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC:
	case CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO:
		group = GROUP4;
		break;
	// Group5
	case CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS:
		group = GROUP5;
		break;
	default:
		return false;
	}

	bool found = true;
	std::list<std::string> keys;
	filter->jfilter.get_keys(&keys);

	for (std::list<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
		std::string key = (*it);
		found = false;
		for (int i = 0; filter_arr[GROUP_COMMON][i] != NULL; i++) {
			if (key.compare(filter_arr[GROUP_COMMON][i]) == 0) {
				found = true;
				break;
			}
		}
		if (found == true) {
			continue;
		}

		for (int i = 0; filter_arr[group][i] != NULL; i++) {
			if (key.compare(filter_arr[group][i]) == 0) {
				found = true;
				break;
			}
		}
		if (found == false) {
			break;
		}
	}

	return found;
}
