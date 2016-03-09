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

#ifndef __TIZEN_CONTEXT_CONTEXT_TRIGGER_H__
#define __TIZEN_CONTEXT_CONTEXT_TRIGGER_H__

/**
 * @addtogroup	CAPI_CONTEXT_TRIGGER_MODULE
 * @{
 */

#include <tizen_error.h>
#include <app_control.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief	The operator "is equal to".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int()
 *			and context_trigger_rule_entry_add_comparison_string().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_EQUAL_TO "=="

/**
 * @brief	The operator "is not equal to".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int()
 *			and context_trigger_rule_entry_add_comparison_string().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_NOT_EQUAL_TO "!="

/**
 * @brief	The operator "is greater than".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_GREATER_THAN ">"

/**
 * @brief	The operator "is greater than or equal to".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_GREATER_THAN_OR_EQUAL_TO ">="

/**
 * @brief	The operator "is less than".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_LESS_THAN "<"

/**
 * @brief	The operator "is less than or equal to".
 * @details	This can be used as operators of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_LESS_THAN_OR_EQUAL_TO "<="

/**
 * @brief	Logical true.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_TRUE 1

/**
 * @brief	Logical false.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_int().
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_FALSE 0

/**
 * @brief	The attribute key denoting "time of day".
 * @details	This can be used as a key of context_trigger_rule_entry_add_option_string(),
 *			or left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			When being used as option, it is time interval of the day. For example, "13-15" denotes "from 1 PM to 3 PM".
 *			When being used with #CONTEXT_TRIGGER_EVENT_TIME or #CONTEXT_TRIGGER_CONDITION_TIME, it's unit is "minute",
 *			the valid range of the corresponding right operands is thus from 0 (12:00 AM) to 1439 (11:59 PM).
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_TIME_OF_DAY "TimeOfDay"

/**
 * @brief	The attribute key denoting "day of week".
 * @details	This can be used as a key of context_trigger_rule_entry_add_option_string(),
 *			or left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			As the right operands of this attribute, one of the following values are allowed:
 *			#CONTEXT_TRIGGER_MON, #CONTEXT_TRIGGER_TUE, #CONTEXT_TRIGGER_WED, #CONTEXT_TRIGGER_THU, #CONTEXT_TRIGGER_FRI,
 *			#CONTEXT_TRIGGER_SAT, #CONTEXT_TRIGGER_SUN, #CONTEXT_TRIGGER_WEEKDAY, and #CONTEXT_TRIGGER_WEEKEND.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_DAY_OF_WEEK "DayOfWeek"

/**
 * @brief	The attribute key denoting "day of month".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			The valid range of the corresponding right operands is from 1 to 31.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_DAY_OF_MONTH "DayOfMonth"

/**
 * @brief	The attribute key denoting boolean states of "is connected".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			As the right operands of this attribute, one of the following values are allowed:
 *			#CONTEXT_TRIGGER_TRUE and #CONTEXT_TRIGGER_FALSE.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_IS_CONNECTED "IsConnected"

/**
 * @brief	The attribute key denoting boolean states of "is charging".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			As the right operands of this attribute, one of the following values are allowed:
 *			#CONTEXT_TRIGGER_TRUE and #CONTEXT_TRIGGER_FALSE.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_IS_CHARGING "IsCharging"

/**
 * @brief	The attribute key denoting boolean states of "is enabled".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			As the right operands of this attribute, one of the following values are allowed:
 *			#CONTEXT_TRIGGER_TRUE and #CONTEXT_TRIGGER_FALSE.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_IS_ENABLED "IsEnabled"

/**
 * @brief	The attribute key denoting "level".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands values.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_LEVEL "Level"

/**
 * @brief	The attribute key denoting "state".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_STATE "State"

/**
 * @brief	The attribute key denoting "BSSID".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_BSSID "BSSID"

/**
 * @brief	The attribute key denoting "type".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_TYPE "Type"

/**
 * @brief	The attribute key denoting "event".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_EVENT "Event"

/**
 * @brief	The attribute key denoting "accuracy".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_ACCURACY "Accuracy"

/**
 * @brief	The attribute key denoting "medium".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_MEDIUM "Medium"

/**
 * @brief	The attribute key denoting "place id".
 * @details	This can be used as a key of context_trigger_rule_entry_add_option_int().@n
 *			See the programming guide to find the corresponding trigger events.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_PLACE_ID "PlaceId"

/**
 * @brief	The attribute key denoting "application id".
 * @details	This can be used as a key of context_trigger_rule_entry_add_option_string().@n
 *			See the programming guide to find the corresponding trigger events.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_APP_ID "AppId"

/**
 * @brief	The attribute key denoting "address".
 * @details	This can be used as a key of context_trigger_rule_entry_add_option_string(),
 *			or context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide and CAPI_CONTEXT_TRIGGER_MODULE_OPTION_FORMAT to find the corresponding items.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_ADDRESS "Address"

/**
 * @brief	The attribute key denoting "rank".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_RANK "Rank"

/**
 * @brief	The attribute key denoting "total count".
 * @details	This can be used as left operands of context_trigger_rule_entry_add_comparison_int().@n
 *			See the programming guide to find available right operands.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_TOTAL_COUNT "TotalCount"

/**
 * @brief	The attribute value denoting Monday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_MON "Mon"

/**
 * @brief	The attribute value denoting Tuesday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_TUE "Tue"

/**
 * @brief	The attribute value denoting Wednesday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_WED "Wed"

/**
 * @brief	The attribute value denoting Thursday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_THU "Thu"

/**
 * @brief	The attribute value denoting Friday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_FRI "Fri"

/**
 * @brief	The attribute value denoting Saturday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_SAT "Sat"

/**
 * @brief	The attribute value denoting Sunday.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_SUN "Sun"

/**
 * @brief	The attribute value denoting Weekdays.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_WEEKDAY "Weekday"

/**
 * @brief	The attribute value denoting Weekends.
 * @details	This can be used as a value of context_trigger_rule_entry_add_option_string()
 *			or right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding option keys or left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_WEEKEND "Weekend"

/**
 * @brief	The attribute value denoting the "empty" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_EMPTY "Empty"

/**
 * @brief	The attribute value denoting the "critical" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_CRITICAL "Critical"

/**
 * @brief	The attribute value denoting the "low" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_LOW "Low"

/**
 * @brief	The attribute value denoting the "normal" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_NORMAL "Normal"

/**
 * @brief	The attribute value denoting the "high" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_HIGH "High"

/**
 * @brief	The attribute value denoting the "full" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_FULL "Full"

/**
 * @brief	The attribute value denoting the "disabled" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_DISABLED "Disabled"

/**
 * @brief	The attribute value denoting the "searching" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_SEARCHING "Searching"

/**
 * @brief	The attribute value denoting the "connecting" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_CONNECTING "Connecting"

/**
 * @brief	The attribute value denoting the "connected" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_CONNECTED "Connected"

/**
 * @brief	The attribute value denoting the "unconnected" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_UNCONNECTED "Unconnected"

/**
 * @brief	The attribute value denoting the "idle" state.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_IDLE "Idle"

/**
 * @brief	The attribute value denoting the "voice" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_VOICE "Voice"

/**
 * @brief	The attribute value denoting the "video" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_VIDEO "Video"

/**
 * @brief	The attribute value denoting the "headset" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_HEADSET "Headset"

/**
 * @brief	The attribute value denoting the "bluetooth" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_BLUETOOTH "Bluetooth"

/**
 * @brief	The attribute value denoting the "received" event.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_RECEIVED "Received"

/**
 * @brief	The attribute value denoting the "sent" event.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_SENT "Sent"

/**
 * @brief	The attribute value denoting the "SMS" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_SMS "SMS"

/**
 * @brief	The attribute value denoting the "MMS" type.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_MMS "MMS"

/**
 * @brief	The attribute value denoting the "detected" event.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_DETECTED "Detected"

/**
 * @brief	The attribute value denoting the "in" event.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_IN "In"

/**
 * @brief	The attribute value denoting the "out" event.
 * @details	This can be used as right operands of context_trigger_rule_entry_add_comparison_string().@n
 *			See the programming guide to find the corresponding left operand attribute keys.
 * @since_tizen 2.4
 */
#define CONTEXT_TRIGGER_OUT "Out"

/**
 * @brief	Enumeration of error codes for context trigger API.
 * @since_tizen 2.4
 */
typedef enum {
	CONTEXT_TRIGGER_ERROR_NONE				= TIZEN_ERROR_NONE,						/**< Successful */
	CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER,		/**< Invalid function parameter */
	CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		= TIZEN_ERROR_OUT_OF_MEMORY,			/**< Out of memory */
	CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED	= TIZEN_ERROR_PERMISSION_DENIED,		/**< Permission denied */
	CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		= TIZEN_ERROR_NOT_SUPPORTED,			/**< Not supported */
	CONTEXT_TRIGGER_ERROR_NO_DATA			= TIZEN_ERROR_NO_DATA,					/**< No data */
	CONTEXT_TRIGGER_ERROR_OPERATION_FAILED	= (TIZEN_ERROR_CONTEXT | 0x04),			/**< Operation failed */
	CONTEXT_TRIGGER_ERROR_RULE_ENABLED		= (TIZEN_ERROR_CONTEXT | 0X05),			/**< Rule is enabled */
	CONTEXT_TRIGGER_ERROR_RULE_NOT_ENABLED	= (TIZEN_ERROR_CONTEXT | 0X06),			/**< Rule is not enabled */
	CONTEXT_TRIGGER_ERROR_INVALID_RULE		= (TIZEN_ERROR_CONTEXT | 0X07),			/**< Invalid rule */
	CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST	= (TIZEN_ERROR_CONTEXT | 0X08),			/**< Rule does not exist */
	CONTEXT_TRIGGER_ERROR_INVALID_DATA		= CONTEXT_TRIGGER_ERROR_INVALID_RULE,	/**< Invalid data */
	CONTEXT_TRIGGER_ERROR_DATA_EXIST		= (TIZEN_ERROR_CONTEXT | 0X09),			/**< Data exist */
} context_trigger_error_e;

/**
 * @brief		Enumeration for event types.
 * @since_tizen 2.4
 */
typedef enum {
	CONTEXT_TRIGGER_EVENT_TIME		= 0x10001,	/**< At certain times of day and days of week @n Privilege: http://tizen.org/privilege/alarm.set */
	CONTEXT_TRIGGER_EVENT_BATTERY	= 0x10100,	/**< Battery level changed */
	CONTEXT_TRIGGER_EVENT_CHARGER,				/**< Charger connected/disconnected */
	CONTEXT_TRIGGER_EVENT_GPS,					/**< GPS state changed */
	CONTEXT_TRIGGER_EVENT_HEADPHONE,			/**< Headphone connected/disconnected */
	CONTEXT_TRIGGER_EVENT_USB,					/**< USB connected/disconnected */
	CONTEXT_TRIGGER_EVENT_WIFI,					/**< WiFi state changed @n Privilege: http://tizen.org/privilege/network.get */
	CONTEXT_TRIGGER_EVENT_POWER_SAVING_MODE	= 0x10200,	/**< Power saving mode enabled/disabled */
	CONTEXT_TRIGGER_EVENT_CALL		= 0x10300,	/**< Call state changed @n Privilege: http://tizen.org/privilege/telephony */
	CONTEXT_TRIGGER_EVENT_EMAIL,				/**< Email sent/received */
	CONTEXT_TRIGGER_EVENT_MESSAGE,				/**< Message sent/received @n Privilege: http://tizen.org/privilege/message.read */
	CONTEXT_TRIGGER_EVENT_ACTIVITY_STATIONARY	= 0x10400,	/**< 'Stationary' activity detected */
	CONTEXT_TRIGGER_EVENT_ACTIVITY_WALKING,		/**< 'Walking' activity detected */
	CONTEXT_TRIGGER_EVENT_ACTIVITY_RUNNING,		/**< 'Running' activity detected */
	CONTEXT_TRIGGER_EVENT_ACTIVITY_IN_VEHICLE,	/**< 'In vehicle' activity detected */
	CONTEXT_TRIGGER_EVENT_PLACE		= 0x10500,	/**< Get in/out to/from a specific user place @n Privilege: http://tizen.org/privilege/location */
} context_trigger_event_e;

/**
 * @brief		Enumeration for condition types.
 * @since_tizen 2.4
 */
typedef enum {
	CONTEXT_TRIGGER_CONDITION_TIME		= 0x20001,	/**< Time of day */
	CONTEXT_TRIGGER_CONDITION_BATTERY	= 0x20100,	/**< Battery level */
	CONTEXT_TRIGGER_CONDITION_CHARGER,				/**< Charger connection */
	CONTEXT_TRIGGER_CONDITION_GPS,					/**< GPS state */
	CONTEXT_TRIGGER_CONDITION_HEADPHONE,			/**< Headphone connection */
	CONTEXT_TRIGGER_CONDITION_USB,					/**< USB connection */
	CONTEXT_TRIGGER_CONDITION_WIFI,					/**< WiFi state @n Privilege: http://tizen.org/privilege/network.get */
	CONTEXT_TRIGGER_CONDITION_POWER_SAVING_MODE	= 0x20200,	/**< Power saving mode setting */
	CONTEXT_TRIGGER_CONDITION_CALL		= 0x20300,	/**< Call state @n Privilege: http://tizen.org/privilege/telephony */
	CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY	= 0x20600,	/**< Frequency of app use @n Privilege: http://tizen.org/privilege/apphistory.read  */
	CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY,		/**< Frequency of communication via call or message @n Privilege: http://tizen.org/privilege/callhistory.read */
	CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,		/**< Frequency of music playback @n Privilege: http://tizen.org/privilege/mediahistory.read */
	CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY,		/**< Frequency of video playback @n Privilege: http://tizen.org/privilege/mediahistory.read */
} context_trigger_condition_e;

/**
 * @brief		Enumeration for logical operation types.
 * @since_tizen 2.4
 */
typedef enum {
	CONTEXT_TRIGGER_LOGICAL_CONJUNCTION = 1,	/**< Logical conjunction */
	CONTEXT_TRIGGER_LOGICAL_DISJUNCTION,		/**< Logical disjunction */
} context_trigger_logical_type_e;

/**
 * @brief		Handle for editing a trigger rule.
 * @since_tizen 2.4
 */
typedef struct _context_trigger_rule_s* context_trigger_rule_h;

/**
 * @brief		Handle for editing an event or a condition, which is added to a rule.
 * @since_tizen 2.4
 */
typedef struct _context_trigger_rule_entry_s* context_trigger_rule_entry_h;

/**
 * @brief		Registers a rule.
 * @details		Using this, applications can register a rule and get the ID of the registered rule.@n
 *				A rule only can be enabled, disabled, or removed by the application that has registered the rule.
 * @since_tizen 2.4
 *
 * @privlevel	public
 * @privilege	http://tizen.org/privilege/alarm.set @n
 *				http://tizen.org/privilege/telephony @n
 *				http://tizen.org/privilege/message.read @n
 *				http://tizen.org/privilege/network.get
 *
 * @remarks		When registering a new rule,
 *				regarding the event and condition items used to compose the rule,
 *				the application may require one or more privileges.
 *				If the application does not have a necessary privilege,
 *				#CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED is returned.
 *
 * @param[in]	rule		The rule to register
 * @param[out]	rule_id		The ID assigned to the @c rule
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED	Permission denied
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported event/condition contained
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @see			context_trigger_remove_rule()
 */
int context_trigger_add_rule(context_trigger_rule_h rule, int* rule_id);

/**
 * @brief		Removes a rule.
 * @details		This removes the rule designated by the @c rule_id, which is owned by the application.
 * @since_tizen 2.4
 *
 * @remarks		If the rule has been activated, it should be disabled through context_trigger_disable_rule() in advance.
 *
 * @param[in]	rule_id		The ID of the rule to be removed
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_ENABLED			Rule is enabled already
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST		Rule does not exist
 *
 * @see			context_trigger_add_rule()
 */
int context_trigger_remove_rule(int rule_id);

/**
 * @brief		Enables a rule.
 * @details		This activates the rule designated by the @c rule_id, which is owned by the application.
 * @since_tizen 2.4
 *
 * @param[in]	rule_id		The ID of the rule to be enabled
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_NOT_SUPPORTED		Unsupported event/condition contained
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_ENABLED			Rule is enabled already
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST		Rule does not exist
 *
 * @see			context_trigger_disable_rule()
 */
int context_trigger_enable_rule(int rule_id);

/**
 * @brief		Disables a rule.
 * @details		This deactivates the rule designated by the @c rule_id, which is owned by the application.
 * @since_tizen 2.4
 *
 * @param[in]	rule_id		The ID of the rule to be disabled
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_NOT_ENABLED		Rule is not enabled
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST		Rule does not exist
 *
 * @see			context_trigger_enable_rule()
 */
int context_trigger_disable_rule(int rule_id);

/**
 * @brief		Gets the IDs of the rules owned by the current application.
 * @since_tizen 2.4
 *
 * @remarks		The arrays @c enabled_rule_ids and @c disabled_rule_ids must be released using @c free().
 *
 * @param[out]	enabled_rule_ids	The IDs of the active rules
 * @param[out]	enabled_rule_count	The number of the active rules
 * @param[out]	disabled_rule_ids	The IDs of the inactive rules
 * @param[out]	disabled_rule_count	The number of the inactive rules
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 *
 */
int context_trigger_get_own_rule_ids(int** enabled_rule_ids, int* enabled_rule_count, int** disabled_rule_ids, int* disabled_rule_count);

/**
 * @brief		Gets a rule stored in the system by rule ID.
 * @since_tizen 2.4
 *
 * @remarks		The @c rule must be released using context_trigger_rule_destroy().
 *
 * @param[in]	rule_id		The ID of the rule to be retrieved
 * @param[out]	rule		The rule retrieved
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST		Rule does not exist
 */
int context_trigger_get_rule_by_id(int rule_id, context_trigger_rule_h* rule);

/**
 * @brief		Creates an empty rule.
 * @details		An empty rule container is created. When using this,
 *				a logical operator, one of #context_trigger_logical_type_e, needs to be designated.@n
 *				In case of #CONTEXT_TRIGGER_LOGICAL_CONJUNCTION,
 *				the rule can be satisfied only if all conditions are true.
 *				Otherwise, in case of #CONTEXT_TRIGGER_LOGICAL_DISJUNCTION,
 *				the rule can be satisfied if at least one is true.
 * @since_tizen 2.4
 *
 * @remarks		The @c rule must be released using context_trigger_rule_destroy().
 *
 * @param[in]	logical_type	The logical operator
 * @param[out]	rule			The rule handle to be initialized
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Memory allocation failed
 *
 * @see			context_trigger_rule_destroy()
 */
int context_trigger_rule_create(context_trigger_logical_type_e logical_type, context_trigger_rule_h* rule);

/**
 * @brief		Releases the resources occupied by a rule handle.
 * @since_tizen 2.4
 *
 * @param[in]	rule		The rule handle to be released
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre			context_trigger_rule_create()
 * @see			context_trigger_get_rule_by_id()
 */
int context_trigger_rule_destroy(context_trigger_rule_h rule);

/**
 * @brief		Adds an event or a condition to a rule.
 * @since_tizen 2.4
 *
 * @param[in]	rule		The rule
 * @param[in]	entry		The event or condition entry to be added to the rule
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 */
int context_trigger_rule_add_entry(context_trigger_rule_h rule, context_trigger_rule_entry_h entry);

/**
 * @brief		Sets a app launching request as the action of a rule.
 * @details		Contextual Trigger accepts an App Control as the action of a rule,
 *				an application thus can be launched when the rule is satisfied.@n
 * @since_tizen 2.4
 *
 * @privlevel	public
 * @privilege	http://tizen.org/privilege/appmanager.launch @n
 *				http://tizen.org/privilege/call
 *
 * @remarks		In addition to the privilege http://tizen.org/privilege/appmanager.launch,
 *				if it is an App Control that makes a call to someone,
 *				the privilege http://tizen.org/privilege/call is also required.
 * @remarks		The launch request of the service application is restricted.
 *				The function will return #CONTEXT_TRIGGER_ERROR_INVALID_RULE,
 *				if the launch request is for the service application.
 *
 * @param[in]	rule			The rule
 * @param[in]	app_control		The App Control, which will be used to launch an application
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED	Permission denied
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 */
int context_trigger_rule_set_action_app_control(context_trigger_rule_h rule, app_control_h app_control);

/**
 * @brief		Sets a notification posting request as the action of a rule.
 * @details		A basic notification can be posted when the rule is satisfied,
 *				which consists of the title, a content body text, an icon, and an App Control.
 * @since_tizen 2.4
 *
 * @privlevel	public
 * @privilege	http://tizen.org/privilege/notification
 *
 * @remarks		The @c app_control can be @c NULL. In that case, no application will be launched via the notification.
 *
 * @param[in]	rule			The rule
 * @param[in]	title			The title text
 * @param[in]	content			The content body text
 * @param[in]	icon_path		The icon file path
 * @param[in]	app_control		The app control
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_PERMISSION_DENIED	Permission denied
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 */
int context_trigger_rule_set_action_notification(context_trigger_rule_h rule, const char* title, const char* content, const char* icon_path, app_control_h app_control);

/**
 * @brief		Sets the description of a rule.
 * @since_tizen 2.4
 *
 * @param[in]	rule			The rule
 * @param[in]	description		The description to be set
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 *
 * @see			context_trigger_rule_get_description()
 */
int context_trigger_rule_set_description(context_trigger_rule_h rule, const char* description);

/**
 * @brief		Gets the description of a rule.
 * @since_tizen 2.4
 *
 * @remarks		The @c description must be released using @c free().
 *
 * @param[in]	rule			The rule
 * @param[out]	description		The description of the rule
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 *
 * @see			context_trigger_rule_set_description()
 */
int context_trigger_rule_get_description(context_trigger_rule_h rule, char** description);

/**
 * @brief		Creates an event entry.
 * @details		An event of a contextual event item, which will be monitored by the system, is created.
 * @since_tizen 2.4
 *
 * @remarks		The @c entry must be released using context_trigger_rule_entry_destroy().
 *
 * @param[in]	event_item		The contextual event item
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
int context_trigger_rule_event_create(context_trigger_event_e event_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry);

/**
 * @brief		Checks whether a contextual event is supported in the current device.
 * @since_tizen 2.4
 *
 * @param[in]	event_item		The contextual event item
 * @param[out]	supported		If supported, @c true; Otherwise, @c false
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 */
int context_trigger_rule_event_is_supported(context_trigger_event_e event_item, bool* supported);

/**
 * @brief		Creates a condition entry.
 * @details		A condition of a contextual condition item is created.
 * @since_tizen 2.4
 *
 * @remarks		The @c entry must be released using context_trigger_rule_entry_destroy().
 *
 * @param[in]	condition_item	The contextual condition item
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
int context_trigger_rule_condition_create(context_trigger_condition_e condition_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry);

/**
 * @brief		Checks whether a contextual condition is supported in the current device.
 * @since_tizen 2.4
 *
 * @param[in]	condition_item	The contextual condition item
 * @param[out]	supported		If supported, @c true; Otherwise, @c false
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OPERATION_FAILED		Operation failed
 */
int context_trigger_rule_condition_is_supported(context_trigger_condition_e condition_item, bool* supported);

/**
 * @brief		Releases the resource occupied by an entry.
 * @since_tizen 2.4
 *
 * @param[in]	entry		The event or condition entry
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre			context_trigger_rule_event_create()
 * @pre			context_trigger_rule_condition_create()
 */
int context_trigger_rule_entry_destroy(context_trigger_rule_entry_h entry);

/**
 * @brief		Sets an integer type option to an event or condition entry.
 * @details		See the programming guide to find available option keys for each event/condition item.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The event or condition entry
 * @param[in]	option_key		The option key
 * @param[in]	value			The option value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @see			context_trigger_rule_entry_add_option()
 * @see			context_trigger_rule_entry_add_option_string()
 */
int context_trigger_rule_entry_add_option_int(context_trigger_rule_entry_h entry, const char* option_key, int value);

/**
 * @brief		Sets a string type option to an event or condition entry.
 * @details		See the programming guide to find available option keys for each event/condition item.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The event or condition entry
 * @param[in]	option_key		The option key
 * @param[in]	value			The option value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @see			context_trigger_rule_entry_add_option()
 * @see			context_trigger_rule_entry_add_option_int()
 */
int context_trigger_rule_entry_add_option_string(context_trigger_rule_entry_h entry, const char* option_key, const char* value);

/**
 * @brief		Sets an option to a condition entry, which references an attribute that will be extracted from the event.
 * @details		See the programming guide to find available option keys for each condition item.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The condition entry
 * @param[in]	option_key		The option key
 * @param[in]	event_data_key	The event data key of which the corresponding data value will be used as the option parameter
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @see			context_trigger_rule_entry_add_option_int()
 * @see			context_trigger_rule_entry_add_option_string()
 */
int context_trigger_rule_entry_add_option(context_trigger_rule_entry_h entry, const char* option_key, const char* event_data_key);

/**
 * @brief		Adds an attribute key to an entry.
 * @details		The key will be used as the left operand of comparisons.
 *				See the programming guide to find available attribute keys for each event/condition item.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The event or condition entry
 * @param[in]	logical_type	The logical operator
 * @param[in]	key				The attribute key
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @post		context_trigger_rule_entry_add_comparison()
 * @post		context_trigger_rule_entry_add_comparison_int()
 * @post		context_trigger_rule_entry_add_comparison_string()
 */
int context_trigger_rule_entry_add_key(context_trigger_rule_entry_h entry, context_trigger_logical_type_e logical_type, const char* key);

/**
 * @brief		Adds a comparison between an attribute key and an integer.
 * @details		The key needs to be registered in advance, via context_trigger_rule_entry_add_key().
 *				As the comparison operator, one of the following operators is allowed:
 *				#CONTEXT_TRIGGER_EQUAL_TO, #CONTEXT_TRIGGER_NOT_EQUAL_TO, #CONTEXT_TRIGGER_LESS_THAN,
 *				#CONTEXT_TRIGGER_GREATER_THAN, #CONTEXT_TRIGGER_LESS_THAN_OR_EQUAL_TO
 *				and #CONTEXT_TRIGGER_GREATER_THAN_OR_EQUAL_TO.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The event or condition entry
 * @param[in]	key				The attribute key, which will be used as the left operand
 * @param[in]	comp_operator	The comparison operator
 * @param[in]	value			The right operand value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_NO_DATA				The key was not added in the entry
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @pre			context_trigger_rule_entry_add_key()
 * @see			context_trigger_rule_entry_add_comparison()
 * @see			context_trigger_rule_entry_add_comparison_string()
 */
int context_trigger_rule_entry_add_comparison_int(context_trigger_rule_entry_h entry, const char* key, const char* comp_operator, int  value);

/**
 * @brief		Adds a comparison between an attribute key and a string.
 * @details		The key needs to be registered in advance, via context_trigger_rule_entry_add_key().
 *				As the comparison operator, one of the following operators is allowed:
 *				#CONTEXT_TRIGGER_EQUAL_TO and #CONTEXT_TRIGGER_NOT_EQUAL_TO.
 * @since_tizen 2.4
 *
 * @param[in]	entry			The event or condition entry
 * @param[in]	key				The attribute key, which will be used as the left operand
 * @param[in]	comp_operator	The comparison operator
 * @param[in]	value			The right operand value
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_NO_DATA				The key was not added in the entry
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @pre			context_trigger_rule_entry_add_key()
 * @see			context_trigger_rule_entry_add_comparison()
 * @see			context_trigger_rule_entry_add_comparison_int()
 */
int context_trigger_rule_entry_add_comparison_string(context_trigger_rule_entry_h entry, const char* key, const char* comp_operator, const char* value);

/**
 * @brief		Adds a comparison between an attribute of a condition and an attribute extracted from the event.
 * @details		The key needs to be registered in advance, via context_trigger_rule_entry_add_key().
 * @since_tizen 2.4
 *
 * @param[in]	entry			The condition entry
 * @param[in]	key				The attribute key of the condition, which will be used as the left operand
 * @param[in]	comp_operator	The comparison operator
 * @param[in]	event_data_key	The event data key of which the corresponding data value will be used as the right operand
 *
 * @return		0 on success, otherwise a negative error value
 * @retval		#CONTEXT_TRIGGER_ERROR_NONE					Successful
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_TRIGGER_ERROR_NO_DATA				The key was not added in the entry
 * @retval		#CONTEXT_TRIGGER_ERROR_INVALID_RULE			Invalid rule
 *
 * @pre			context_trigger_rule_entry_add_key()
 * @see			context_trigger_rule_entry_add_comparison_int()
 * @see			context_trigger_rule_entry_add_comparison_string()
 */
int context_trigger_rule_entry_add_comparison(context_trigger_rule_entry_h entry, const char* key, const char* comp_operator, const char* event_data_key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
* @}
*/

#endif /* __TIZEN_CONTEXT_CONTEXT_TRIGGER_H__ */
