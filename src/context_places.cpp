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
	int req_id;
	ctx::Json tmp_list;

	ctx::DBusClient dbusClient;
	int err = dbusClient.readSync(PLACE_DETECTION_SUBJECT, NULL, &req_id, &tmp_list);

	IF_FAIL_RETURN_TAG(err == ERR_NONE, err, _E, "Getting list failed");

	_J("Read response", tmp_list);

	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_get_count(context_places_list_h list, int* count)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_get_current(context_places_list_h list, context_places_place_h* record)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_move_first(context_places_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_move_next(context_places_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_list_destroy(context_places_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_category(context_places_place_h place, context_places_categ_id_e* value, double* confidence)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_name(context_places_place_h place, char** name)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_location(context_places_place_h place, double* latitude, double* longitude, double* accuracy)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_creation_date(context_places_place_h place, time_t* timestamp)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_get_wifi_ap_list(context_places_place_h place, context_places_wifi_ap_list_h* list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_get_count(context_places_wifi_ap_list_h list, int* count)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_get_current(context_places_wifi_ap_list_h list, char** bssid, char** essid)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_move_first(context_places_wifi_ap_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_move_next(context_places_wifi_ap_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_wifi_ap_list_destroy(context_places_wifi_ap_list_h list)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}

SO_EXPORT int context_places_place_destroy(context_places_place_h place)
{
	// TODO: implement

	return CONTEXT_PLACES_ERROR_NONE;
}
