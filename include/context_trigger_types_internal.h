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

#define CONTEXT_TRIGGER_SUBJECT_ADD "trigger/add"
#define CONTEXT_TRIGGER_SUBJECT_REMOVE "trigger/remove"
#define CONTEXT_TRIGGER_SUBJECT_ENABLE "trigger/enable"
#define CONTEXT_TRIGGER_SUBJECT_DISABLE "trigger/disable"
#define CONTEXT_TRIGGER_SUBJECT_GET "trigger/get"
#define CONTEXT_TRIGGER_SUBJECT_GET_RULE_IDS "trigger/get_rule_ids"
#define CONTEXT_TRIGGER_SUBJECT_GET_TEMPLATE "trigger/tepmlate/get"
#define CONTEXT_TRIGGER_SUBJECT_CUSTOM_ADD "custom/add"
#define CONTEXT_TRIGGER_SUBJECT_CUSTOM_REMOVE "custom/remove"
#define CONTEXT_TRIGGER_SUBJECT_CUSTOM_PUBLISH "custom/publish"

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
#define CT_RULE_ACTION_DBUS_PARAMETER	"ACTION_DBUS_PARAM"

#define CT_RESPONSE "RES"

#define CT_EVENT_TIME "device/alarm"
#define CT_EVENT_BATTERY "system/battery"
#define CT_EVENT_CHARGER "system/charger"
#define CT_EVENT_GPS "system/gps"
#define CT_EVENT_HEADPHONE "system/headphone"
#define CT_EVENT_POWER_SAVING_MODE "system/psmode"
#define CT_EVENT_USB "system/usb"
#define CT_EVENT_WIFI "system/wifi"
#define CT_EVENT_CALL "social/call"
#define CT_EVENT_EMAIL "social/email"
#define CT_EVENT_MESSAGE "social/message"
#define CT_EVENT_ACTIVITY_STATIONARY "activity/stationary"
#define CT_EVENT_ACTIVITY_WALKING "activity/walking"
#define CT_EVENT_ACTIVITY_RUNNING "activity/running"
#define CT_EVENT_ACTIVITY_IN_VEHICLE "activity/in_vehicle"
#define CT_EVENT_PLACE "place/geofence"
#define CT_CONDITION_TIME "device/time"
#define CT_CONDITION_BATTERY "system/battery"
#define CT_CONDITION_CHARGER "system/charger"
#define CT_CONDITION_GPS "system/gps"
#define CT_CONDITION_HEADPHONE "system/headphone"
#define CT_CONDITION_POWER_SAVING_MODE "system/psmode"
#define CT_CONDITION_USB "system/usb"
#define CT_CONDITION_WIFI "system/wifi"
#define CT_CONDITION_CALL "social/call"
#define CT_CONDITION_APP_USE_FREQUENCY "stats/app/frequency"
#define CT_CONDITION_COMMUNICATION_FREQUENCY "stats/contact/frequency"
#define CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY "stats/music/frequency"
#define CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY "stats/video/frequency"

#define CT_CUSTOM_NAME "name"
#define CT_CUSTOM_ATTRIBUTES "attributes"
#define CT_CUSTOM_FACT "fact"
#define CT_CUSTOM_TYPE "type"
#define CT_CUSTOM_INT "integer"
#define CT_CUSTOM_STRING "string"
#define CT_CUSTOM_MIN "min"
#define CT_CUSTOM_MAX "max"
#define CT_CUSTOM_VALUES "values"
#define CT_CUSTOM_SUBJECT "subject"
#define CT_CUSTOM_DELIMITER std::string("::")

#define TYPE_EVENT 1
#define TYPE_CONDITION 2

#define TYPE_INT_STR "integer"
#define TYPE_STRING_STR "string"
#define TYPE_DOUBLE_STR "double"

#define TYPE_OPTION_STR "options"
#define TYPE_ATTR_STR "attributes"
#define SUBJECT_STR "subject"

#endif /* __TIZEN_CONTEXT_CONTEXT_TRIGGER_TYPES_INTERNAL_H__ */
