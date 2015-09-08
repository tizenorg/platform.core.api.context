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
