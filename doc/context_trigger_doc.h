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
 * Note that, a rule only can be managed by the application that has registered the rule.
 *
 * If the rule is not required anymore, it can be disabled and removed from the engine.
 * Note that, rule activation, deactivation, and removals are only allowed to the
 * owner application, which has registered the rule to the trigger engine.
 * In the below example, the application queries its own rule IDs,
 * deactivates enabled rules, and removes all rules from the trigger engine.
 *
 * For more details, please see the programming guide and the tutorial.
 *
 *
 * @section CAPI_CONTEXT_TRIGGER_MODULE_FEATURE Related Features
 *
 * Some of the event and condition items in this API are related with one of the following features:@n
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
 * <a href="https://developer.tizen.org/development/tools/native-tools/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 */
