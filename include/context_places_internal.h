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

#ifndef __TIZEN_CONTEXT_PLACES_H__
#define __TIZEN_CONTEXT_PLACES_H__

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_CONTEXT_PLACES_MODULE
 * @{
 */

/**
 * @brief	The attribute key denoting "place category"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_CATEG_ID "TypeId"

/**
 * @brief	The attribute key denoting "place categorization confidence"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_CATEG_CONFIDENCE "TypeConfidence"

/**
 * @brief	The attribute key denoting "place name"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_NAME "Name"

/**
 * @brief	The attribute key denoting "location latitude"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_GEO_LATITUDE "GeoLatitude"

/**
 * @brief	The attribute key denoting "location longitude"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_GEO_LONGITUDE "GeoLongitude"

/**
 * @brief	The attribute key denoting "WiFi APs list"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_WIFI_APS "WifiAPs"

/**
 * @brief	The attribute key denoting "creation date"
 * @since_tizen 3.0
 */
#define CONTEXT_PLACES_CREATE_DATE "CreateDate"

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
 * @brief		Places record handle. It contains one or multiple attributes.
 * @since_tizen 3.0
 */
typedef struct _context_places_record_handle_s* context_places_record_h;

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
int context_places_list_get_current(context_places_list_h list, context_places_record_h* record);

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
 * @brief		Gets an integer value from a record.
 * @since_tizen 3.0
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of attribute to get
 * @param[out]	value		The result value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 * @see		context_places_record_get_double()
 */
int context_places_record_get_int(context_places_record_h record,  const char* key, int* value);

/**
 * @brief		Gets a double value from a record.
 * @since_tizen 3.0
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of attribute to get
 * @param[out]	value		The result value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 * @see		context_places_record_get_string()
 */
int context_places_record_get_double(context_places_record_h record,  const char* key, double* value);

/**
 * @brief		Gets a string from a record.
 * @since_tizen 3.0
 * @remarks		@c value must be released using free().
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of attribute to get
 * @param[out]	value		The result value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 * @see		context_places_record_get_int()
 */
int context_places_record_get_string(context_places_record_h record, const char* key, char** value);

/**
 * @brief		Destroys a record handle and releases all its resources.
 * @since_tizen 3.0
 *
 * @param[in]	record		The record handle
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_PLACES_ERROR_NONE					Successful
 * @retval		#CONTEXT_PLACES_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre		context_places_list_get_current()
 */
int context_places_record_destroy(context_places_record_h record);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
* @}
*/

#endif /* __TIZEN_CONTEXT_PLACES_H__ */
