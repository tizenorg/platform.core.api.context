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

/**
 * @ingroup		CAPI_CONTEXT_FRAMEWORK
 * @defgroup	CAPI_CONTEXT_TRIGGER_MODULE Contextual Trigger
 *
 * @brief		The contextual trigger API provides a way to define rules,
 *				each of which can trigger a specified action when the rule is satisfied.
 *
 * @section		CAPI_CONTEXT_TRIGGER_MODULE_HEADER Required Header
 *				\#include <context_trigger.h>
 *
 * @section		CAPI_CONTEXT_TRIGGER_MODULE_OVERVIEW Overview
 *
 * The contextual trigger API provides a way to define trigger rules,
 * each of which launches an application or posts a notification if it is satisfied.
 *
 * @subsection CAPI_CONTEXT_TRIGGER_MODULE_CODE_EXAMPLE Trigger Rule Creation and Activation
 *
 * To use this API, an application first needs to assemble a @em rule,
 * which consists of the following three components:
 *
 <ol>
	<li><b>An Event</b>:
		The contextual trigger engine starts to verify if the rule is satisfied, when the event occurs.
		As the name suggests, it is literally an event that can be recognized in the device,
		e.g., changes of setting values or connections of peripherals.
		See #context_trigger_event_e for all available event items.
	<li><b>A Set of Conditions</b>:
		If the event occurs, the trigger engine checks whether the conditions are satisfied or not.
		The application can choose the way of combining conditions, i.e., logical conjunction or disjunction.
		In case of logical conjunction, for example, the set of conditions is @c true if and only if
		all conditions are @c true.
		If no condition is set, i.e., the set of conditions is empty, the trigger engine
		considers the set is satisfied, regardless of the current context.
		See #context_trigger_condition_e for all available condition items.
	<li><b>An Action</b>: If the conditions are satisfied, a designated action will be triggered.
		The action can be an app launching request or a notification posting request.
 </ol>
 *
 * As a case study, the below example code illustrates how to create and activate a rule,
 * which can be described as
 * "Notify if the battery is not charged enough and currently not charing,
 * at 10 and 11 PM" in human language.

 \code
	// Creating a rule handle
	context_trigger_rule_h rule = NULL;
	context_trigger_rule_create(CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &rule);

	// Creating an event
	context_trigger_rule_entry_h event = NULL;
	context_trigger_rule_event_create(CONTEXT_TRIGGER_EVENT_ON_TIME,
			CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &event);
	context_trigger_rule_entry_add_key(event,
			CONTEXT_TRIGGER_LOGICAL_DISJUNCTION, CONTEXT_TRIGGER_TIME_OF_DAY);
	context_trigger_rule_entry_add_comparison_int(event,
			CONTEXT_TRIGGER_TIME_OF_DAY, CONTEXT_TRIGGER_EQUAL_TO, 22);
	context_trigger_rule_entry_add_comparison_int(event,
			CONTEXT_TRIGGER_TIME_OF_DAY, CONTEXT_TRIGGER_EQUAL_TO, 23);

	// Adding the event to the rule and releasing the resource occupied.
	context_trigger_rule_add_entry(rule, event);
	context_trigger_rule_entry_destroy(event);
 \endcode

 * While creating a rule handle, the application chooses the logical operator,
 * <em>logical conjunction</em> or <em>disjunction</em>,
 * which will be applied to the conditions.@n
 * Then some logical conditions can be added to the rule.
 * Note that, adding conditions are not mandatory.

 \code
	// Creating a condition
	context_trigger_rule_entry_h condition = NULL;
	context_trigger_rule_condition_create(CONTEXT_TRIGGER_CONDITION_BATTERY,
			CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, &condition);
	context_trigger_rule_entry_add_key(condition,
			CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, CONTEXT_TRIGGER_LEVEL);
	context_trigger_rule_entry_add_comparison_string(condition,
			CONTEXT_TRIGGER_LEVEL, CONTEXT_TRIGGER_NOT_EQUAL_TO, CONTEXT_TRIGGER_FULL);
	context_trigger_rule_entry_add_comparison_string(condition,
			CONTEXT_TRIGGER_LEVEL, CONTEXT_TRIGGER_NOT_EQUAL_TO, CONTEXT_TRIGGER_HIGH);
	context_trigger_rule_entry_add_key(condition,
			CONTEXT_TRIGGER_LOGICAL_CONJUNCTION, CONTEXT_TRIGGER_IS_CHARGING);
	context_trigger_rule_entry_add_comparison_int(condition,
			CONTEXT_TRIGGER_IS_CHARGING, CONTEXT_TRIGGER_EQUAL_TO, CONTEXT_TRIGGER_FALSE);

	// Adding the condition to the handle
	context_trigger_rule_add_entry(rule, condition);
	context_trigger_rule_entry_destroy(condition);
 \endcode

 * A app launching request or a notification posting request is set as the action,
 * which will be triggered if the rule is satisfied.
 * The below example shows how to register a notification posting request.

 \code
	// Setting a notification posting request
	context_trigger_rule_set_action_notification(rule, "Battery Alert", "Charge your battery please.", NULL, NULL);
 \endcode

 * An app launching request is created and manipulated by the app control API.
 * Please refer to the app control API for more details.@n
 * Regarding the notification posting request, this API supports the very basic
 * form of notifications only. To create a more sophisticated notification,
 * the application needs to implement a service application that posts the notification.@n
 * After creating a rule, to activate it, the rule needs to be registered and enabled explicitly.

 \code
	// Adding the rule to the engine
	int rule_id;
	context_trigger_add_rule(rule, &rule_id);

	// Enabling the rule
	context_trigger_enable_rule(rule_id)
 \endcode

 * Note that, a rule only can be managed by the application that has registered the rule.
 *
 * If the rule is not required anymore, it can be disabled and removed from the engine.
 * Note that, rule activation, deactivation, and removals are only allowed to the
 * owner application, which has registered the rule to the trigger engine.
 * In the below example, the application queries its own rule IDs,
 * deactivates enabled rules, and removes all rules from the trigger engine.

 \code
	context_trigger_disable_rule(rule_id)
	context_trigger_remove_rule(rule_id);
 \endcode

 *
 * @subsection CAPI_CONTEXT_TRIGGER_MODULE_DATA_FORMAT Supported Attribute Keys and Comparison Operators
 *
 * Each event or condition item may support different types of attribute keys,
 * which are used as left operands of comparison operations.
 * The supported attribute keys, their types, and acceptable values are summarized in the table below.
 *

<table>
	<caption>Table: Supported Attribute Keys</caption>
	<tr>
		<th>Event/Condition</th>
		<th>Attribute Key</th>
		<th>Type</th>
		<th>Acceptable Values</th>
	</tr>
	<tr>
		<td rowspan=2>#CONTEXT_TRIGGER_EVENT_ON_TIME</td>
		<td>#CONTEXT_TRIGGER_TIME_OF_DAY</td>
		<td>Integer</td>
		<td>From 0 to 23 (hour)</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_DAY_OF_WEEK</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_MON,<br>
			#CONTEXT_TRIGGER_TUE,<br>
			#CONTEXT_TRIGGER_WED,<br>
			#CONTEXT_TRIGGER_THU,<br>
			#CONTEXT_TRIGGER_FRI,<br>
			#CONTEXT_TRIGGER_SAT,<br>
			#CONTEXT_TRIGGER_SUN
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_CONDITION_TIME_OF_DAY</td>
		<td>#CONTEXT_TRIGGER_TIME_OF_DAY</td>
		<td>Integer</td>
		<td>From 0 to 1439 (min)</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_CONDITION_DAY_OF_WEEK</td>
		<td>#CONTEXT_TRIGGER_DAY_OF_WEEK</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_MON,<br>
			#CONTEXT_TRIGGER_TUE,<br>
			#CONTEXT_TRIGGER_WED,<br>
			#CONTEXT_TRIGGER_THU,<br>
			#CONTEXT_TRIGGER_FRI,<br>
			#CONTEXT_TRIGGER_SAT,<br>
			#CONTEXT_TRIGGER_SUN
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_CONDITION_DAY_OF_MONTH</td>
		<td>#CONTEXT_TRIGGER_DAY_OF_MONTH</td>
		<td>Integer</td>
		<td>From 1 to 31</td>
	</tr>
	<tr>
		<td>
			#CONTEXT_TRIGGER_EVENT_CHARGER<br>
			#CONTEXT_TRIGGER_EVENT_USB<br>
			#CONTEXT_TRIGGER_CONDITION_CHARGER<br>
			#CONTEXT_TRIGGER_CONDITION_USB
		</td>
		<td>#CONTEXT_TRIGGER_IS_CONNECTED</td>
		<td>Integer</td>
		<td>#CONTEXT_TRIGGER_TRUE,<br>#CONTEXT_TRIGGER_FALSE</td>
	</tr>
	<tr>
		<td rowspan=2>
			#CONTEXT_TRIGGER_EVENT_BATTERY<br>
			#CONTEXT_TRIGGER_CONDITION_BATTERY
		</td>
		<td>#CONTEXT_TRIGGER_LEVEL</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_EMPTY,<br>
			#CONTEXT_TRIGGER_CRITICAL,<br>
			#CONTEXT_TRIGGER_LOW,<br>
			#CONTEXT_TRIGGER_NORMAL,<br>
			#CONTEXT_TRIGGER_HIGH,<br>
			#CONTEXT_TRIGGER_FULL
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_IS_CHARGING</td>
		<td>Integer</td>
		<td>#CONTEXT_TRIGGER_TRUE,<br>#CONTEXT_TRIGGER_FALSE</td>
	</tr>
	<tr>
		<td>
			#CONTEXT_TRIGGER_EVENT_FLIGHT_MODE<br>
			#CONTEXT_TRIGGER_EVENT_POWER_SAVING_MODE<br>
			#CONTEXT_TRIGGER_EVENT_SILENT_MODE<br>
			#CONTEXT_TRIGGER_EVENT_VIBRATION_MODE<br>
			#CONTEXT_TRIGGER_CONDITION_FLIGHT_MODE<br>
			#CONTEXT_TRIGGER_CONDITION_POWER_SAVING_MODE<br>
			#CONTEXT_TRIGGER_CONDITION_SILENT_MODE<br>
			#CONTEXT_TRIGGER_CONDITION_VIBRATION_MODE
		</td>
		<td>#CONTEXT_TRIGGER_IS_ENABLED</td>
		<td>Integer</td>
		<td>#CONTEXT_TRIGGER_TRUE,<br>#CONTEXT_TRIGGER_FALSE</td>
	</tr>
	<tr>
		<td>
			#CONTEXT_TRIGGER_EVENT_GPS<br>
			#CONTEXT_TRIGGER_CONDITION_GPS
		</td>
		<td>#CONTEXT_TRIGGER_STATE</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_DISABLED,<br>
			#CONTEXT_TRIGGER_SEARCHING,<br>
			#CONTEXT_TRIGGER_CONNECTED
		</td>
	</tr>
	<tr>
		<td rowspan=2>
			#CONTEXT_TRIGGER_EVENT_HEADPHONE<br>
			#CONTEXT_TRIGGER_CONDITION_HEADPHONE
		</td>
		<td>#CONTEXT_TRIGGER_IS_CONNECTED</td>
		<td>Integer</td>
		<td>#CONTEXT_TRIGGER_TRUE,<br>#CONTEXT_TRIGGER_FALSE</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_TYPE</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_NORMAL,<br>
			#CONTEXT_TRIGGER_HEADSET,<br>
			#CONTEXT_TRIGGER_BLUETOOTH
		</td>
	</tr>
	<tr>
		<td rowspan=2>
			#CONTEXT_TRIGGER_EVENT_WIFI<br>
			#CONTEXT_TRIGGER_CONDITION_WIFI
		</td>
		<td>#CONTEXT_TRIGGER_STATE</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_DISABLED,<br>
			#CONTEXT_TRIGGER_CONNECTED,<br>
			#CONTEXT_TRIGGER_UNCONNECTED
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_BSSID</td>
		<td>String</td>
		<td>WiFi BSSID</td>
	</tr>
	<tr>
		<td rowspan=3>
			#CONTEXT_TRIGGER_EVENT_CALL<br>
			#CONTEXT_TRIGGER_CONDITION_CALL
		</td>
		<td>#CONTEXT_TRIGGER_STATE</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_IDLE,<br>
			#CONTEXT_TRIGGER_CONNECTING,<br>
			#CONTEXT_TRIGGER_CONNECTED
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_MEDIUM</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_VOICE,<br>#CONTEXT_TRIGGER_VIDEO</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_ADDRESS</td>
		<td>String</td>
		<td>Phone number</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_EVENT_EMAIL</td>
		<td>#CONTEXT_TRIGGER_EVENT</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_RECEIVED,<br>#CONTEXT_TRIGGER_SENT</td>
	</tr>
	<tr>
		<td rowspan=3>#CONTEXT_TRIGGER_EVENT_MESSAGE</td>
		<td>#CONTEXT_TRIGGER_EVENT</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_RECEIVED</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_TYPE</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_SMS,<br>#CONTEXT_TRIGGER_MMS</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_ADDRESS</td>
		<td>String</td>
		<td>Phone number</td>
	</tr>
	<tr>
		<td rowspan=2>
			#CONTEXT_TRIGGER_EVENT_ACTIVITY_STATIONARY,<br>
			#CONTEXT_TRIGGER_EVENT_ACTIVITY_WALKING,<br>
			#CONTEXT_TRIGGER_EVENT_ACTIVITY_RUNNING,<br>
			#CONTEXT_TRIGGER_EVENT_ACTIVITY_IN_VEHICLE
		</td>
		<td>#CONTEXT_TRIGGER_EVENT</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_DETECTED</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_ACCURACY</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_HIGH,<br>#CONTEXT_TRIGGER_NORMAL,<br>#CONTEXT_TRIGGER_LOW</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_EVENT_PLACE</td>
		<td>#CONTEXT_TRIGGER_EVENT</td>
		<td>String</td>
		<td>#CONTEXT_TRIGGER_IN,<br>#CONTEXT_TRIGGER_OUT</td>
	</tr>
	<tr>
		<td>
			#CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY
		</td>
		<td>#CONTEXT_TRIGGER_RANK</td>
		<td>Integer</td>
		<td>Positive integer</td>
	</tr>
	<tr>
		<td>
			#CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY
		</td>
		<td>#CONTEXT_TRIGGER_TOTAL_COUNT</td>
		<td>Integer</td>
		<td>Nonnegative integer</td>
	</tr>
</table>

 *
 * For each type of attribute, as operators, the followings are allowed:
 *

<ul>
	<li><i>For <b>integer</b> attributes</i>:<br>
		#CONTEXT_TRIGGER_EQUAL_TO ("=="), #CONTEXT_TRIGGER_NOT_EQUAL_TO ("!="),<br>
		#CONTEXT_TRIGGER_GREATER_THAN (">"), #CONTEXT_TRIGGER_GREATER_THAN_OR_EQUAL_TO (">="),<br>
		#CONTEXT_TRIGGER_LESS_THAN ("<"), #CONTEXT_TRIGGER_LESS_THAN_OR_EQUAL_TO ("<=")
	<li><i>For <b>string</b> attributes</i>:<br>
		#CONTEXT_TRIGGER_EQUAL_TO ("=="), #CONTEXT_TRIGGER_NOT_EQUAL_TO ("!=")
</ul>

 *
 * @subsection CAPI_CONTEXT_TRIGGER_MODULE_OPTION_FORMAT Supported Options Keys
 *
 * Certain events or conditions require to specify option values, which will be used
 * to produce the output attribute values.
 * For example, in case of #CONTEXT_TRIGGER_EVENT_PLACE,
 * the target place id should be specified, by using context_trigger_rule_entry_add_option_int().
 * The table below summarizes the supported option keys and acceptable values for each event or condition.
 *

<table>
	<caption>Table: Supported Option Keys</caption>
	<tr>
		<th>Event/Condition</th>
		<th>Option Key</th>
		<th>Type</th>
		<th>Acceptable Values</th>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_EVENT_PLACE</td>
		<td>#CONTEXT_TRIGGER_PLACE_ID</td>
		<td>Integer</td>
		<td>
			Place ID<br>
			See \ref CAPI_GEOFENCE_MANAGER_MODULE
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY</td>
		<td>#CONTEXT_TRIGGER_APP_ID</td>
		<td>String</td>
		<td>Application ID</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY</td>
		<td>#CONTEXT_TRIGGER_ADDRESS</td>
		<td>String</td>
		<td>Phone number</td>
	</tr>
	<tr>
		<td rowspan=2>
			#CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY,<br>
			#CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY
		</td>
		<td>#CONTEXT_TRIGGER_TIME_OF_DAY</td>
		<td>String</td>
		<td>
			Time interval of day, for example,<br>
			<b>"13-15"</b> denoting "from 1 PM to 3 PM".
		</td>
	</tr>
	<tr>
		<td>#CONTEXT_TRIGGER_DAY_OF_WEEK</td>
		<td>String</td>
		<td>
			#CONTEXT_TRIGGER_MON,<br>
			#CONTEXT_TRIGGER_TUE,<br>
			#CONTEXT_TRIGGER_WED,<br>
			#CONTEXT_TRIGGER_THU,<br>
			#CONTEXT_TRIGGER_FRI,<br>
			#CONTEXT_TRIGGER_SAT,<br>
			#CONTEXT_TRIGGER_SUN,<br>
			#CONTEXT_TRIGGER_WEEKDAY,<br>
			#CONTEXT_TRIGGER_WEEKEND
		</td>
	</tr>
</table>

 *
 * @section CAPI_CONTEXT_TRIGGER_MODULE_FEATURE Related Features
 *
 * Some of the event and condition items in this API are related with one or the following features:@n
 * - http://tizen.org/feature/network.telephony
 * - http://tizen.org/feature/location.gps
 * - http://tizen.org/feature/network.bluetooth
 * - http://tizen.org/feature/network.wifi
 *
 * It is recommended to design feature related code in your application for reliability.@n
 * For your convenience, context_trigger_rule_event_is_supported() and context_trigger_rule_condition_is_supported()
 * are provided. Applications can use these functions to check the availability of a necessary event or condition,
 * before registering a rule, to be sure that the rule is valid in the current device.
 *
 * In general, you can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,
 * thereby controlling the procedure of your application.
 * To ensure your application is only running on the device with specific features,
 * please define the features in your manifest file using the manifest editor in the SDK.
 * More details on featuring your application can be found from
 * <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */
