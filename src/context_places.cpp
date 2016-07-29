/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
#include <MyPlaceTypes.h>
#include <Types.h>
#include <context_places_internal.h>
#include "DBusClient.h"

/* XXX: temporary !!!!! Example JSON output:
---------------------------------------------------------------------------------------
{
	"PlacesList":[
	  {
		"CategId":1,
		"CategConfidence":"0.555000007152557",
		"Name":"Home",
		"Location":{"Latitude":"10.9443311","Longitude":"50.8550422","Accuracy":"-1"},
		"WifiAPs":
			{
				"WifiAPs":[
				   {
						"Mac":"00:1f:f3:5b:2b:1f",
						"Network":"first network"
				   },
				   {
						"Mac":"13:34:56:78:9a:ba",
						"Network":"third network"
				   },
				   {
						"Mac":"15:34:56:78:9a:ba",
						"Network":"second network"
				   }
				 ]
			},
		"CreateDate":12132567
	  },
	  {
		"CategId":2,
		"CategConfidence":"0.666000008583069",
		"Name":"Work",
		"Location":
			{
				"Latitude":"10.9623333",
				"Longitude":"50.8430444",
				"Accuracy":"100"
			},
		"WifiAPs":
			{
				"WifiAPs":[
					{
						"Mac":"12:34:56:78:9a:ba",
						"Network":"fourth network"
					}
				]
			},
		"CreateDate":12132889
	  },
	  {
		"CategId":3,
		"CategConfidence":"0.77700001001358",
		"Name":"Other",
		"CreateDate":12132777
	  }
	]
}
---------------------------------------------------------------------------------------
*/

struct _context_places_list_handle_s {
	ctx::Json jList;
	int current;
	_context_places_list_handle_s()
	{
		current = 0;
	}
};

struct _context_places_place_handle_s {
	ctx::Json jPlace;
};

struct _context_places_wifi_ap_list_handle_s {
	ctx::Json jList;
	int current;
	_context_places_wifi_ap_list_handle_s()
	{
		current = 0;
	}
};

SO_EXPORT int context_places_get_list(context_places_list_h* list)
{
	ASSERT_NOT_NULL(list);
	*list = NULL;

	ctx::Json option;
	option.set(NULL, PLACE_DETECTION_LIST, -1);

	int req_id;
	ctx::Json tmp_list;
	ctx::DBusClient dbusClient;

	int error = dbusClient.readSync(PLACE_DETECTION_SUBJECT, option, &req_id, &tmp_list);
	IF_FAIL_RETURN_TAG(
			error == ERR_NONE,
			error,
			_E,
			"Getting list failed");

	_J("Read response", tmp_list);

	IF_FAIL_RETURN_TAG(
			tmp_list.getSize(NULL, PLACE_DATA_READ) > 0,
			CONTEXT_PLACES_ERROR_NO_DATA,
			_D,
			"No data");

	*list = new(std::nothrow) _context_places_list_handle_s();
	ASSERT_ALLOC(*list);
	(*list)->jList = tmp_list;
	(*list)->current = 0;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_get_count(context_places_list_h list, int* count)
{
	ASSERT_NOT_NULL(list && count);
	*count = 0;

	int result = list->jList.getSize(NULL, PLACE_DATA_READ);
	IF_FAIL_RETURN(result > 0, CONTEXT_PLACES_ERROR_OPERATION_FAILED);

	*count = result;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_get_current(context_places_list_h list, context_places_place_h* record)
{
	ASSERT_NOT_NULL(list && record);
	*record = NULL;

	ctx::Json tmp_record;
	int error = list->jList.getAt(NULL, PLACE_DATA_READ, list->current, &tmp_record);
	IF_FAIL_RETURN_TAG(
			error,
			CONTEXT_PLACES_ERROR_OPERATION_FAILED,
			_E,
			"Place record load failed");

	*record = new(std::nothrow) _context_places_place_handle_s();
	ASSERT_ALLOC(*record);
	(*record)->jPlace = tmp_record;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_move_first(context_places_list_h list)
{
	ASSERT_NOT_NULL(list);

	list->current = 0;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_move_next(context_places_list_h list)
{
	ASSERT_NOT_NULL(list);

	IF_FAIL_RETURN_TAG(
			list->current+1 < list->jList.getSize(NULL, PLACE_DATA_READ),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_D,
			"End of list");

	list->current++;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_destroy(context_places_list_h list)
{
	ASSERT_NOT_NULL(list);

	delete list;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_category(context_places_place_h place, context_places_categ_id_e* value, double* confidence)
{
	ASSERT_NOT_NULL(place && value && confidence);

	int categId;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_CATEG_ID, &categId),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	double categConfidence;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_CATEG_CONFIDENCE,	&categConfidence),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	*value = static_cast<context_places_categ_id_e>(categId);
	*confidence = categConfidence;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_name(context_places_place_h place, char** name)
{
	ASSERT_NOT_NULL(place && name);

	std::string str;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_NAME, &str),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	*name = g_strdup(str.c_str());
	ASSERT_ALLOC(*name);

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_location(context_places_place_h place, double* latitude, double* longitude, double* accuracy)
{
	ASSERT_NOT_NULL(place && latitude && longitude && accuracy);

	ctx::Json locationJson;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_LOCATION, &locationJson),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	double lat;
	IF_FAIL_RETURN_TAG(
			locationJson.get(NULL, PLACE_LOCATION_LATITUDE, &lat),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	double lon;
	IF_FAIL_RETURN_TAG(
			locationJson.get(NULL, PLACE_LOCATION_LONGITUDE, &lon),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	double acc;
	IF_FAIL_RETURN_TAG(
			locationJson.get(NULL, PLACE_LOCATION_ACCURACY, &acc),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	*latitude = lat;
	*longitude = lon;
	*accuracy = acc;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_creation_date(context_places_place_h place, time_t* timestamp)
{
	ASSERT_NOT_NULL(place && timestamp);

	int64_t tmp_timestamp;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_CREATE_DATE, &tmp_timestamp),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	*timestamp = static_cast<time_t>(tmp_timestamp);

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_destroy(context_places_place_h place)
{
	ASSERT_NOT_NULL(place);

	delete place;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_wifi_ap_list(context_places_place_h place, context_places_wifi_ap_list_h* list)
{
	ASSERT_NOT_NULL(place && list);
	*list = NULL;

	ctx::Json tmp_list;
	IF_FAIL_RETURN_TAG(
			place->jPlace.get(NULL, PLACE_WIFI_APS, &tmp_list),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"No data");

	IF_FAIL_RETURN_TAG(
			tmp_list.getSize(NULL, PLACE_WIFI_APS) > 0,
			CONTEXT_PLACES_ERROR_NO_DATA,
			_D,
			"No data");

	*list = new(std::nothrow) _context_places_wifi_ap_list_handle_s();
	ASSERT_ALLOC(*list);
	(*list)->jList = tmp_list;
	(*list)->current = 0;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_get_count(context_places_wifi_ap_list_h list, int* count)
{
	ASSERT_NOT_NULL(list && count);
	*count = 0;

	int result = list->jList.getSize(NULL, PLACE_WIFI_APS);
	IF_FAIL_RETURN(result > 0, CONTEXT_PLACES_ERROR_OPERATION_FAILED);

	*count = result;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_get_current(context_places_wifi_ap_list_h list, char** bssid, char** essid)
{
	ASSERT_NOT_NULL(list && bssid && essid);

	ctx::Json wifiAPJson;
	IF_FAIL_RETURN_TAG(
			list->jList.getAt(NULL, PLACE_WIFI_APS, list->current, &wifiAPJson),
			CONTEXT_PLACES_ERROR_OPERATION_FAILED,
			_E,
			"Place Wifi AP load failed");

	std::string bssid_str;
	IF_FAIL_RETURN_TAG(
			wifiAPJson.get(NULL, PLACE_WIFI_AP_MAC, &bssid_str),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	std::string essid_str;
	IF_FAIL_RETURN_TAG(
			wifiAPJson.get(NULL, PLACE_WIFI_AP_NETWORK_NAME, &essid_str),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_E,
			"Parameter empty");

	*bssid = g_strdup(bssid_str.c_str());
	ASSERT_ALLOC(*bssid);

	*essid = g_strdup(essid_str.c_str());
	ASSERT_ALLOC(*essid);

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_move_first(context_places_wifi_ap_list_h list)
{
	ASSERT_NOT_NULL(list);

	list->current = 0;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_move_next(context_places_wifi_ap_list_h list)
{
	ASSERT_NOT_NULL(list);

	IF_FAIL_RETURN_TAG(
			list->current+1 < list->jList.getSize(NULL, PLACE_WIFI_APS),
			CONTEXT_PLACES_ERROR_NO_DATA,
			_D,
			"End of list");

	list->current++;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_destroy(context_places_wifi_ap_list_h list)
{
	ASSERT_NOT_NULL(list);

	delete list;

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_is_supported(bool *supported)
{
	_D("");
	ctx::DBusClient dbusClient;
	int error = dbusClient.isSupported(PLACE_DETECTION_SUBJECT);
	if (error == ERR_NONE) {
		*supported = true;
	} else if (error == ERR_NOT_SUPPORTED) {
		*supported = false;
	} else {
		*supported = false;
		_E("Getting supported value failed");
	}
	_D("");
	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_consent(bool consent)
{
	_D("");
	ctx::Json option;
	option.set(NULL, PLACE_DETECTION_CONSENT, consent ? 1 : 0);
	ctx::DBusClient dbusClient;
	int error = dbusClient.write(PLACE_DETECTION_SUBJECT, option);
	IF_FAIL_RETURN_TAG(
			error == ERR_NONE,
			error,
			_E,
			"Setting consent value failed");

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_is_consented(bool *consented)
{
	_D("");
	ctx::Json option;
	option.set(NULL, PLACE_DETECTION_CONSENT, -1);
	int req_id;
	ctx::Json tmp_cons;
	ctx::DBusClient dbusClient;
	int error = dbusClient.readSync(PLACE_DETECTION_SUBJECT, option, &req_id, &tmp_cons);
	IF_FAIL_RETURN_TAG(
			error == ERR_NONE,
			error,
			_E,
			"Getting consent value failed");
	*consented = true;// TODO:
	_D("");
	return CONTEXT_PLACES_ERROR_NONE;
}
