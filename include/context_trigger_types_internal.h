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

#ifndef __TIZEN_CONTEXT_CONTEXT_TRIGGER_TYPES_INTERNAL_H__
#define __TIZEN_CONTEXT_CONTEXT_TRIGGER_TYPES_INTERNAL_H__

#define CONTEXT_TRIGGER_SUBJECT_ADD "context_trigger/add"
#define CONTEXT_TRIGGER_SUBJECT_REMOVE "context_trigger/remove"
#define CONTEXT_TRIGGER_SUBJECT_ENABLE "context_trigger/enable"
#define CONTEXT_TRIGGER_SUBJECT_DISABLE "context_trigger/disable"
#define CONTEXT_TRIGGER_SUBJECT_GET "context_trigger/get"
#define CONTEXT_TRIGGER_SUBJECT_GET_RULE_IDS "context_trigger/get_rule_ids"

#define CT_RULE_ARRAY_ENABLED "ARRAY_ENABLED"
#define CT_RULE_ARRAY_DISABLED "ARRAY_DISABLED"

#define CT_RULE_ACTION_TYPE_APP_CONTROL "app_control"
#define CT_RULE_ACTION_TYPE_NOTIFICATION "notification"
#define CT_RULE_ACTION_TYPE_DBUS_CALL "dbus_call"

#define CT_RULE_ID "ID"
#define CT_RULE_DESCRIPTION "DESCRIPTION"
#define CT_RULE_DETAILS "DETAILS"
#define CT_RULE_EVENT "EVENT"
#define CT_RULE_EVENT_ITEM "ITEM_NAME"
#define CT_RULE_EVENT_OPERATOR "ITEM_OPERATOR"
#define CT_RULE_EVENT_OPTION "OPTION"
#define CT_RULE_OPERATOR "OPERATOR"
#define CT_RULE_CONDITION "CONDITION"
#define CT_RULE_CONDITION_ITEM "ITEM_NAME"
#define CT_RULE_CONDITION_OPERATOR "ITEM_OPERATOR"
#define CT_RULE_CONDITION_OPTION "OPTION"
#define CT_RULE_DATA_ARR "DATA_ARR"
#define CT_RULE_DATA_KEY "DATA_KEY"
#define CT_RULE_DATA_KEY_OPERATOR "DATA_KEY_OPERATOR"
#define CT_RULE_DATA_VALUE_ARR "DATA_VALUE_ARR"
#define CT_RULE_DATA_VALUE_OPERATOR_ARR "DATA_VALUE_OPER_ARR"
#define CT_RULE_ACTION "ACTION"
#define CT_RULE_ACTION_TYPE "ACTION_TYPE"
#define CT_RULE_ACTION_APP_CONTROL "ACTION_APP_CONTROL"
#define CT_RULE_ACTION_NOTI_TITLE "ACTION_NOTI_TITLE"
#define CT_RULE_ACTION_NOTI_CONTENT "ACTION_NOTI_CONTENT"
#define CT_RULE_ACTION_NOTI_ICON_PATH "ACTION_NOTI_ICON_PATH"

#define CT_RULE_ACTION_DBUS_NAME		"ACTION_DBUS_NAME"
#define CT_RULE_ACTION_DBUS_OBJECT		"ACTION_DBUS_OBJ"
#define CT_RULE_ACTION_DBUS_INTERFACE	"ACTION_DBUS_IFACE"
#define CT_RULE_ACTION_DBUS_METHOD		"ACTION_DBUS_METHOD"
#define CT_RULE_ACTION_DBUS_USER_DATA	"ACTION_DBUS_USERDATA"

#define CT_RESPONSE "RES"

#define	CT_EVENT_TIME "timer/event"
#define CT_EVENT_BATTERY "system/state/battery"
#define CT_EVENT_CHARGER "system/state/charger"
#define CT_EVENT_FLIGHT_MODE "system/state/flight_mode"
#define CT_EVENT_GPS "system/state/gps"
#define CT_EVENT_HEADPHONE "system/state/headphone"
#define CT_EVENT_POWER_SAVING_MODE "system/state/ps_mode"
#define CT_EVENT_SILENT_MODE "system/state/silent_mode"
#define CT_EVENT_VIBRATION_MODE "system/state/vibration_mode"
#define CT_EVENT_USB "system/state/usb"
#define CT_EVENT_WIFI "system/state/wifi"
#define CT_EVENT_CALL "social/state/call"
#define CT_EVENT_EMAIL "social/event/email"
#define CT_EVENT_MESSAGE "social/event/message"
#define CT_EVENT_ACTIVITY_STATIONARY "activity/event/stationary"
#define CT_EVENT_ACTIVITY_WALKING "activity/event/walking"
#define CT_EVENT_ACTIVITY_RUNNING "activity/event/running"
#define CT_EVENT_ACTIVITY_IN_VEHICLE "activity/event/in_vehicle"
#define CT_EVENT_PLACE "place/event/geofence"
#define CT_CONDITION_TIME "timer/state"
#define CT_CONDITION_BATTERY "system/state/battery"
#define CT_CONDITION_CHARGER "system/state/charger"
#define CT_CONDITION_FLIGHT_MODE "system/state/flight_mode"
#define CT_CONDITION_GPS "system/state/gps"
#define CT_CONDITION_HEADPHONE "system/state/headphone"
#define CT_CONDITION_POWER_SAVING_MODE "system/state/ps_mode"
#define CT_CONDITION_SILENT_MODE "system/state/silent_mode"
#define CT_CONDITION_VIBRATION_MODE "system/state/vibration_mode"
#define CT_CONDITION_USB "system/state/usb"
#define CT_CONDITION_WIFI "system/state/wifi"
#define CT_CONDITION_CALL "social/state/call"
#define CT_CONDITION_APP_USE_FREQUENCY "app/history/use_freq"
#define CT_CONDITION_COMMUNICATION_FREQUENCY "contact/history/comm_freq"
#define CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY "music/history/play_freq"
#define CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY "video/history/play_freq"

#define TYPE_EVENT 1
#define TYPE_CONDITION 2

#define TYPE_OPTION 1
#define TYPE_ATTR 2

#define TYPE_INT 1
#define TYPE_STRING 2

#define TYPE_OPTION_STR "option"
#define TYPE_ATTR_STR "attributes"


#endif /* __TIZEN_CONTEXT_CONTEXT_TRIGGER_TYPES_INTERNAL_H__ */
