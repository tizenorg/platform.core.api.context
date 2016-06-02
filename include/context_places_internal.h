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

#ifndef __TIZEN_CONTEXT_PLACES_H__
#define __TIZEN_CONTEXT_PLACES_H__

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief		Enumeration for place category.
 * @since_tizen 3.0
 */
typedef enum {
	CONTEXT_PLACES_CATEG_ID_NONE = 0,
	CONTEXT_PLACES_CATEG_ID_HOME = 1,
	CONTEXT_PLACES_CATEG_ID_WORK = 2,
	CONTEXT_PLACES_CATEG_ID_OTHER = 3
} context_places_categ_id_e;

/**
 * @brief		Enumeration for errors.
 * @since_tizen 3.0
 */
typedef enum {
	CONTEXT_PLACES_ERROR_NONE = TIZEN_ERROR_NONE,							/**< Successful */
	CONTEXT_PLACES_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
	CONTEXT_PLACES_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission denied */
	CONTEXT_PLACES_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,			/**< Not supported */
	CONTEXT_PLACES_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,			/**< Memory allocation failed */
	CONTEXT_PLACES_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,						/**< No Data */
	CONTEXT_PLACES_ERROR_OUT_OF_RANGE = (TIZEN_ERROR_CONTEXT | 0x03),		/**< Out of range */
	CONTEXT_PLACES_ERROR_OPERATION_FAILED = (TIZEN_ERROR_CONTEXT | 0x04),	/**< Operation failed */
} context_places_error_e;

/**
 * @brief		Places list handle. It contains one or multiple records.
 * @since_tizen 3.0
 */
typedef struct _context_places_list_handle_s* context_places_list_h;

/**
 * @brief		Place handle.
 * @since_tizen 3.0
 */
typedef struct _context_places_place_handle_s* context_places_place_h;

/**
 * @brief		Place Wifi AP's list handle. It contains one or multiple records.
 * @since_tizen 3.0
 */
typedef struct _context_places_wifi_ap_list_handle_s* context_places_wifi_ap_list_h;

/**
 * @brief		Reads detected/recognized user places list.
 * @since_tizen 3.0
 *
 * @remarks		The @c list must be released using context_places_list_destroy(). \n
 *              %http://tizen.org/privilege/location is needed to retrieve data.
 *
 * @param[out]	list   Places data retrieved. This should be freed after use
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE                Successful
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED    Operation failed
 * @retval		#CONTEXT_PLACES_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval		#CONTEXT_PLACES_ERROR_NO_DATA           Requested data does not exist
 *
 * @post context_places_list_destroy()
 */
int context_places_get_list(context_places_list_h* list);

/**
 * @brief		Retrieves the number of records in a list.
 * @since_tizen 3.0
 *
 * @param[in]	list    The places data list handle
 * @param[out]	count   The count of the data list
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED		Operation failed
 *
 * @pre context_places_get_list()
 * @see context_places_list_move_first()
 * @see context_places_list_move_next()
 */
int context_places_list_get_count(context_places_list_h list, int* count);

/**
 * @brief		Retrieves the current record from the places list.
 * @details		The default current record is the first record.
 * @since_tizen 3.0
 * @remarks		The @c record must be released using context_places_record_destroy().
 *
 * @param[in]	list     The context places list handle
 * @param[out]	record   Places data record retrieved
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_PLACES_ERROR_OUT_OF_MEMORY			Out of memory
 *
 * @pre context_places_get_list()
 * @post context_places_record_destroy()
 */
int context_places_list_get_current(context_places_list_h list, context_places_place_h* record);

/**
 * @brief		Moves a places data list to the first position.
 * @since_tizen 3.0
 *
 * @param[in]	list    The context places list handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 *
 * @pre context_places_get_list()
 * @see context_places_list_get_count()
 * @see context_places_list_move_next()
 */
int context_places_list_move_first(context_places_list_h list);

/**
 * @brief		Moves a places data list to the next position.
 * @since_tizen 3.0
 *
 * @param[in]	list    The context places list handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_NO_DATA			Requested data does not exist
 *
 * @pre context_places_get_list()
 * @see context_places_list_get_count()
 * @see context_places_list_move_first()
 */
int context_places_list_move_next(context_places_list_h list);

/**
 * @brief		Destroys a places list handle and release all its resources.
 * @since_tizen 3.0
 *
 * @param[in]	list    The context places data handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 *
 * @pre context_places_get_list()
 */
int context_places_list_destroy(context_places_list_h list);

/**
 * @brief		Gets place category and its categorization confidence.
 * @since_tizen 3.0
 *
 * @param[in]	place		Place handle
 * @param[out]	value		Place category value
 * @param[out]	confidence  Place category confidence
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE				Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_place_get_category(context_places_place_h place, context_places_categ_id_e* value, double* confidence);

/**
 * @brief		Gets a place name.
 * @since_tizen 3.0
 *
 * @param[in]	place		Place handle
 * @param[out]	name		Place name
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE				Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_place_get_name(context_places_place_h place, char** name);

/**
 * @brief		Gets place location.
 * @since_tizen 3.0
 *
 * @param[in]	place		Place handle
 * @param[out]	latitude	Place location latitude
 * @param[out]	longitude	Place location longitude
 * @param[out]	accuracy	Place location accuracy
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE				Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_place_get_location(context_places_place_h place, double* latitude, double* longitude, double* accuracy);

/**
 * @brief		Gets place creation/detection timestamp.
 * @since_tizen 3.0
 *
 * @param[in]	place		Place handle
 * @param[out]	timestamp	Place creation timestamp
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE				Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_place_get_creation_date(context_places_place_h place, time_t* timestamp);



/**
 * @brief		Reads detected/recognized user places list.
 * @since_tizen 3.0
 *
 * @remarks		The @c list must be released using context_places_list_destroy().
 *
 * @param[int]	place  Place handle
 * @param[out]	list   Place Wifi AP's list. This should be freed after use
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE                Successful
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED    Operation failed
 * @retval		#CONTEXT_PLACES_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval		#CONTEXT_PLACES_ERROR_NO_DATA           Requested data does not exist
 *
 * @pre	context_places_list_get_current()
 * @post context_places_wifi_ap_list_destroy()
 */
int context_places_place_get_wifi_ap_list(context_places_place_h place, context_places_wifi_ap_list_h* list);

/**
 * @brief		Retrieves the number of records in a list.
 * @since_tizen 3.0
 *
 * @param[in]	list    Place Wifi AP's list handle
 * @param[out]	count   The count of the data list
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED		Operation failed
 *
 * @pre context_places_place_get_wifi_ap_list()
 * @see context_places_wifi_ap_list_move_first()
 * @see context_places_wifi_ap_list_move_next()
 */
int context_places_wifi_ap_list_get_count(context_places_wifi_ap_list_h list, int* count);

/**
 * @brief		Retrieves the current record from the places list.
 * @details		The default current record is the first record.
 * @since_tizen 3.0
 *
 * @param[in]	list     Wifi AP's list handle
 * @param[out]	bssid    Wifi AP MAC address
 * @param[out]	essid    Wifi AP network name
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_OPERATION_FAILED		Operation failed
 *
 * @pre context_places_place_get_wifi_ap_list()
 */
int context_places_wifi_ap_list_get_current(context_places_wifi_ap_list_h list, char** bssid, char** essid);

/**
 * @brief		Moves a Place Wifi AP's list to the first position.
 * @since_tizen 3.0
 *
 * @param[in]	list    Wifi AP's list handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 *
 * @pre context_places_place_get_wifi_ap_list()
 * @see context_places_wifi_ap_list_get_count()
 * @see context_places_wifi_ap_list_move_next()
 */
int context_places_wifi_ap_list_move_first(context_places_wifi_ap_list_h list);

/**
 * @brief		Moves a Place Wifi AP's list to the next position.
 * @since_tizen 3.0
 *
 * @param[in]	list    Wifi AP's list handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval		#CONTEXT_PLACES_ERROR_NO_DATA			Requested data does not exist
 *
 * @pre context_places_place_get_wifi_ap_list()
 * @see context_places_wifi_ap_list_get_count()
 * @see context_places_wifi_ap_list_move_first()
 */
int context_places_wifi_ap_list_move_next(context_places_wifi_ap_list_h list);

/**
 * @brief		Destroys a Wifi AP's list handle and release all its resources.
 * @since_tizen 3.0
 *
 * @param[in]	list    Wifi AP's list handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER		Invalid parameter
 *
 * @pre context_places_place_get_wifi_ap_list()
 */
int context_places_wifi_ap_list_destroy(context_places_wifi_ap_list_h list);

/**
 * @brief		Destroys a place handle and releases all its resources.
 * @since_tizen 3.0
 *
 * @param[in]	place	Place handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_place_destroy(context_places_place_h place);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
* @}
*/

#endif /* __TIZEN_CONTEXT_PLACES_H__ */
