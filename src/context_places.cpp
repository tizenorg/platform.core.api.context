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
#include <context_places_internal.h>
#include "request_handler.h"

#define SUBJ_USER_PLACES "place/pattern/personal_poi"

#define CONTEXT_PLACES_DATA "PlacesList"

/*
 * XXX: temporary !!!!!
 *
 * Example JSON output:
 * ------------------------------------------------
 * {
 *	  "PlacesList": [
 *		{
 *		  "TypeId": 2,
 *		  "Name": "Work",
 *		  "GeoLatitude": 10.94433,
 *		  "GeoLongitude": 50.85504,
 *		  "WifiAPs": "00:1f:f3:5b:2b:1f,15:34:56:78:9a:ba,13:34:56:78:9a:ba",
 *		  "CreateDate": 12132567
 *		},
 *		{
 *		  "TypeId": 1,
 *		  "Name": "Home",
 *		  "GeoLatitude": 10.96233,
 *		  "GeoLongitude": 50.84304,
 *		  "WifiAPs": "aa:bb:cc:dd:ee:ff,10:34:56:78:9a:bc",
 *		  "CreateDate": 12132889
 *		},
 *		{
 *		  "TypeId": 3,
 *		  "Name": "Other",
 *		  "GeoLatitude": 10.96553,
 *		  "GeoLongitude": 50.80404,
 *		  "WifiAPs": "12:34:56:78:9a:ba",
 *		  "CreateDate": 12132346
 *		}
 *	  ]
 * }
 */

struct _context_places_list_handle_s {
	ctx::Json jlist;
	int current;
	_context_places_list_handle_s()
	{
		current = 0;
	}
};

struct _context_places_record_handle_s {
	ctx::Json jrecord;
};

EXTAPI int context_places_get_list(context_places_list_h* list)
{
	int req_id;
	ctx::Json tmp_list;

	int err = ctx::request_handler::read_sync(SUBJ_USER_PLACES, NULL, &req_id, &tmp_list);

	IF_FAIL_RETURN_TAG(err == ERR_NONE, err, _E, "Getting list failed");

	_J("Read response", tmp_list);

	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

// Data object manipulation
EXTAPI int context_places_list_get_count(context_places_list_h list, int* count)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_list_get_current(context_places_list_h list, context_places_record_h* record)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}


EXTAPI int context_places_list_move_first(context_places_list_h list)
{
	ASSERT_NOT_NULL(list);

	list->current = 0;

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_list_move_next(context_places_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_list_destroy(context_places_list_h list)
{
	ASSERT_NOT_NULL(list);

	delete list;

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_record_get_int(context_places_record_h record, const char* key, int* val)
{
	ASSERT_NOT_NULL(record && val && key);

	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_record_get_string(context_places_record_h record, const char* key, char** val)
{
	ASSERT_NOT_NULL(record && val && key);

	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

EXTAPI int context_places_record_destroy(context_places_record_h record)
{
	ASSERT_NOT_NULL(record);

	delete record;

	return CONTEXT_PLACES_ERROR_NONE;
}
