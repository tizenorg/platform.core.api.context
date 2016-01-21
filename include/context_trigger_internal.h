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

#ifndef __TIZEN_CONTEXT_CONTEXT_TRIGGER_INTERNAL_H__
#define __TIZEN_CONTEXT_CONTEXT_TRIGGER_INTERNAL_H__

#include <glib.h>
#include <context_trigger.h>

/**
 * @brief		Add custom event/condition item with its template.
 * @since_tizen 3.0
 *
 * @param[in]	name			The name of custom item to register
 * @param[in]	attr_template	The attribute template in json style
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_DATA			Invalid data
 * @retval		#CONTEXT_TRIGGER_ERROR_DATA_EXIST			Data exist
 *
 * @see			context_trigger_remove_custom_item()
 */
int context_trigger_add_custom_item(const char* name, const char* attr_template);

/**
 * @brief		Remove custom event/condition item.
 * @since_tizen 3.0
 *
 * @param[in]	name			The name of custom item to be removed
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported event/condition item
 *
 * @see			context_trigger_add_custom_item()
 */
int context_trigger_remove_custom_item(const char* name);

/**
 * @brief		Publish custom event/condition data.
 * @since_tizen 3.0
 *
 * @param[in]	name			The name of custom item to provide data
 * @param[in]	fact			The fact data in json style to publish
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported event/condition contained
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_DATA			Invalid data
 */
int context_trigger_publish_custom_item(const char* name, const char* fact);

/**
 * @brief		Creates an event entry which uses custom event.
 * @details		An event of a contextual event item, which will be provided by an application/system, is created.
 * @since_tizen 3.0
 *
 * @remarks		The @c entry must be released using context_trigger_rule_entry_destroy().
 *
 * @param[in]	event_item		The contextual event item
 * @param[in]	provider		The id of the package which provide the event item
 * @param[in]	logical_type	The logical operator
 * @param[out]	entry			The event entry to be initialized
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Memory allocation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported event contained
 *
 * @see			context_trigger_rule_entry_destroy()
 */
int context_trigger_rule_custom_event_create(const char* event_item, const char* provider,
		context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry);

/**
 * @brief		Creates a condition entry which uses custom condition.
 * @details		A condition of a contextual condition item is created.
 * @since_tizen 3.0
 *
 * @remarks		The @c entry must be released using context_trigger_rule_entry_destroy().
 *
 * @param[in]	condition_item	The contextual condition item
 * @param[in]	provider		The id of the package which provide the condition item
 * @param[in]	logical_type	The logical operator
 * @param[out]	entry			The condition entry to be initialized
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Memory allocation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported condition contained
 *

 * @see			context_trigger_rule_entry_destroy()
 */
int context_trigger_rule_custom_condition_create(const char* condition_item, const char* provider,
		context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry);

/**
 * @brief		Sets a D-Bus method call as the action of a rule.
 * @since_tizen 2.4
 *
 * @param[in]	rule			The rule to set the action on
 * @param[in]	bus_name		Destination remote bus name
 * @param[in]	object_path		Path of the remote object
 * @param[in]	interface_name	D-Bus interface to invoke method on
 * @param[in]	method_name		The name of the method to invoke
 * @param[in]	param			Tuple with parameters for the method,
 *								or @c NULL if not passing parameters.
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED	Permission denied
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 */
int context_trigger_rule_set_action_dbus_call(context_trigger_rule_h rule,
		const char *bus_name, const char *object_path, const char *interface_name,
		const char *method_name, GVariant *param);

#endif
