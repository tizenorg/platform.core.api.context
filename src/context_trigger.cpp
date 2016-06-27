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

//#include <sstream>
//#include <iomanip>
#include <Types.h>
#include <DBusTypes.h>
#include <Json.h>
#include <DBusClient.h>
#include <app_control_internal.h>
#include <bundle.h>

#ifndef LEGACY_APPFW
#include <bundle_internal.h>
#endif

#include <context_trigger.h>
#include <context_trigger_internal.h>
#include <context_trigger_types_internal.h>
#include <pkgmgr-info.h>
#include "rule_validator.h"

#define INITIAL_RULE "{ \"ID\" : -1, \"DESCRIPTION\" : \"\", \"DETAILS\" : {  } }"
#define INITIAL_ENTRY "{ \"DATA_ARR\" : [ ] }"
#define INITIAL_REF "{ \"option\" : [ ], \"attributes\" : [ ] }"
#define EVENT_DATA_KEY_PREFIX_STR std::string("?")
//#define DOUBLE_PRECISION 2

static ctx::DBusClient __dbusClient;

static int context_trigger_rule_event_create_internal(const char* event_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry, bool is_custom = false);
static int context_trigger_rule_condition_create_internal(const char* condition_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry, bool is_custom = false);
static std::string convert_event_to_string(context_trigger_event_e item);
static std::string convert_condition_to_string(context_trigger_condition_e item);
static std::string convert_logical_type_to_string(context_trigger_logical_type_e logical_type);
static std::string get_custom_item_subject(const char* provider, const char* item);

//static std::string int_to_string(int value);
//static std::string double_to_string(int value);

typedef struct _context_trigger_rule_s {
	ctx::Json jrule;	// rule_id, description, details(event, condition[])
	ctx::Json jref;

	_context_trigger_rule_s() {
		jrule = INITIAL_RULE;
		jref = INITIAL_REF;
	}
} _context_trigger_rule_h;

typedef struct _context_trigger_rule_entry_s {
	ctx::Json jentry;	// key, value, operator
	int type;
	ctx::Json jref;

	_context_trigger_rule_entry_s(int t): type(t) {
		jentry = INITIAL_ENTRY;

		if (t == TYPE_CONDITION) {
			jref = INITIAL_REF;
		}
	}
} _context_trigger_rule_entry_h;

// Add a rule
SO_EXPORT int context_trigger_add_rule(context_trigger_rule_h rule, int* rule_id)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && rule_id);

	// Err: No event
	std::string ename;
	bool ret = (rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_EVENT, CT_RULE_EVENT_ITEM, &ename);
	if (!ret)
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;

	// Err: No action added
	std::string type;
	ret = (rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type);
	if (!ret)
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;

	ctx::Json jrule_id;
	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_ADD, rule->jrule, &jrule_id);

	if (error == ERR_NONE) {
		jrule_id.get(NULL, CT_RULE_ID, rule_id);
		(rule->jrule).set(NULL, CT_RULE_ID, *rule_id);
	}

	return error;
}

// Remove a rule
SO_EXPORT int context_trigger_remove_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::Json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);
	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_REMOVE, jrule_id, NULL);

	if (error == ERR_ALREADY_STARTED) {	// Rule is still enabled.
		return CONTEXT_TRIGGER_ERROR_RULE_ENABLED;
	} else if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

// Enable a rule
SO_EXPORT int context_trigger_enable_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::Json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);

	int req_id;	// Useless in context_trigger
	int error = __dbusClient.subscribe(CONTEXT_TRIGGER_SUBJECT_ENABLE, jrule_id, &req_id, NULL);

	if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

// Disable a rule
SO_EXPORT int context_trigger_disable_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::Json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);
	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_DISABLE, jrule_id, NULL);

	if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

SO_EXPORT int context_trigger_get_own_rule_ids(int** enabled_rule_ids, int* enabled_rule_count, int** disabled_rule_ids, int* disabled_rule_count)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(enabled_rule_ids && enabled_rule_count && disabled_rule_ids && disabled_rule_count);

	int req_id;
	ctx::Json data_read;
	int error = __dbusClient.readSync(CONTEXT_TRIGGER_SUBJECT_GET_RULE_IDS, NULL, &req_id, &data_read);

	if (error != ERR_NONE) {
		return error;
	}

	// Enabled rules
	int* e_arr = NULL;
	*enabled_rule_count = data_read.getSize(NULL, CT_RULE_ARRAY_ENABLED);

	if (*enabled_rule_count > 0) {
		e_arr = static_cast<int*>(g_malloc((*enabled_rule_count) * sizeof(int)));
		IF_FAIL_RETURN_TAG(e_arr, CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY, _E, "Memory allocation failed");

		int id;
		for (int i = 0; data_read.getAt(NULL, CT_RULE_ARRAY_ENABLED, i, &id); i++) {
			*(e_arr + i) = id;
		}
	}
	*enabled_rule_ids = e_arr;

	// Disabled rules
	int* d_arr = NULL;
	*disabled_rule_count = data_read.getSize(NULL, CT_RULE_ARRAY_DISABLED);

	if (*disabled_rule_count > 0) {
		d_arr = static_cast<int*>(g_malloc((*disabled_rule_count) * sizeof(int)));
		IF_FAIL_RETURN_TAG(d_arr, CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY, _E, "Memory allocation failed");

		int id;
		for (int i = 0; data_read.getAt(NULL, CT_RULE_ARRAY_DISABLED, i, &id); i++) {
			*(d_arr + i) = id;
		}
	}
	*disabled_rule_ids = d_arr;

	return error;
}

SO_EXPORT int context_trigger_get_rule_by_id(int rule_id, context_trigger_rule_h* rule)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::Json option;
	option.set(NULL, CT_RULE_ID, rule_id);

	int req_id;
	ctx::Json data_read;
	int error = __dbusClient.readSync(CONTEXT_TRIGGER_SUBJECT_GET, option, &req_id, &data_read);

	if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	} else if (error != ERR_NONE) {
		return error;
	}

	*rule = new(std::nothrow) _context_trigger_rule_h();
	(*rule)->jrule = data_read;

	return ERR_NONE;
}

// Rule creation
SO_EXPORT int context_trigger_rule_create(context_trigger_logical_type_e logical_type, context_trigger_rule_h* rule)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);

	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	*rule = new(std::nothrow) _context_trigger_rule_h();
	(*rule)->jrule.set(CT_RULE_DETAILS, CT_RULE_OPERATOR, logical_str);

	return ERR_NONE;
}

// Rule deletion
SO_EXPORT int context_trigger_rule_destroy(context_trigger_rule_h rule)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);
	delete rule;

	return CONTEXT_TRIGGER_ERROR_NONE;
}


SO_EXPORT int context_trigger_rule_add_entry(context_trigger_rule_h rule, context_trigger_rule_entry_h entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && entry);

	std::string des;
	bool ret = (rule->jrule).get(NULL, CT_RULE_DESCRIPTION, &des);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	if (entry->type == TYPE_EVENT) {
		// Err: More than one event
		ctx::Json elem;
		if ((rule->jrule).get(CT_RULE_DETAILS, CT_RULE_EVENT, &elem)) {
			return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
		}

		// Err: Check if all the mandatory options are added
		ret = ctx::rule_validator::check_option(entry->jentry);
		IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

		// Err: If referential conditions are registered priviously, check them
		std::string ename;
		(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &ename);
		ret = ctx::rule_validator::check_referential_data(ename, rule->jref);
		IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

		ctx::Json temp = (entry->jentry).str();
		ret = (rule->jrule).set(CT_RULE_DETAILS, CT_RULE_EVENT, temp);
	} else if (entry->type == TYPE_CONDITION) {
		// Err: Condition without comparison data
		if ((entry->jentry).getSize(NULL, CT_RULE_DATA_ARR) < 1) {
			return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
		}

		ctx::Json elem;
		for (int i = 0; (entry->jentry).getAt(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
			int val_arr_size = elem.getSize(NULL, CT_RULE_DATA_VALUE_ARR);
			int op_arr_size = elem.getSize(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR);

			// Err: Condition without comparison data
			if (val_arr_size != op_arr_size || val_arr_size < 1 || op_arr_size < 1) {
				return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
			}

			// Err: Check if all the mandatory options are added
			ret = ctx::rule_validator::check_option(entry->jentry);
			IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

			// If event is already added ....
			std::string ename;
			ret = (rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_EVENT, CT_RULE_EVENT_ITEM, &ename);
			if (ret) {
				// Err: Check referential information if exists
				ret = ctx::rule_validator::check_referential_data(ename, entry->jref);
				IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);
			} else {
				// If not, copy referential information to rule entry
				ctx::Json info;
				for (int j = 0; (entry->jref).getAt(NULL, TYPE_OPTION_STR, j, &info); j++) {
					(rule->jref).append(NULL, TYPE_OPTION_STR, info);
				}

				for (int j = 0; (entry->jref).getAt(NULL, TYPE_ATTR_STR, j, &info); j++) {
					(rule->jref).append(NULL, TYPE_ATTR_STR, info);
				}
			}
		}

		ctx::Json temp = (entry->jentry).str();
		ret = (rule->jrule).append(CT_RULE_DETAILS, CT_RULE_CONDITION, temp);
	} else {
		// Entry is not created
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_OPERATION_FAILED);
	return CONTEXT_TRIGGER_ERROR_NONE;
}

static bool is_call_operation(app_control_h app_control)
{
	char *op = NULL;
	int err = app_control_get_operation(app_control, &op);
	IF_FAIL_RETURN_TAG(err == APP_CONTROL_ERROR_NONE, false, _W, "Getting operation of app control failed");

	bool ret = STR_EQ(op, APP_CONTROL_OPERATION_CALL);
	g_free(op);

	return ret;
}

SO_EXPORT int context_trigger_rule_set_action_app_control(context_trigger_rule_h rule, app_control_h app_control)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && app_control);
	int error;

	// Privilege check
	error = __dbusClient.call(METHOD_CHK_PRIV_APPLAUNCH);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);

	if (is_call_operation(app_control)) {
		error = __dbusClient.call(METHOD_CHK_PRIV_CALL);
		IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);
	}

	// Err: if action arleady exists
	std::string type;
	if ((rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type)) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	// Err: service app
	char* app_id = NULL;
	error = app_control_get_app_id(app_control, &app_id);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to get app id");

	pkgmgrinfo_appinfo_h app_info;
#ifdef LEGACY_APPFW
	error = pkgmgrinfo_appinfo_get_appinfo(app_id, &app_info);
#else
	error = pkgmgrinfo_appinfo_get_usr_appinfo(app_id, getuid(), &app_info);
#endif
	g_free(app_id);
	IF_FAIL_RETURN_TAG(error == PMINFO_R_OK, CONTEXT_TRIGGER_ERROR_INVALID_RULE, _E, "No such app");

#ifndef _ALLOW_SERVICE_APP_TRIGGER_
	char *app_type = NULL;
	pkgmgrinfo_appinfo_get_component_type(app_info, &app_type);
	if (!strcmp(app_type, "svcapp")) {
		_E("Service application is restricted");
		pkgmgrinfo_appinfo_destroy_appinfo(app_info);
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}
#endif
	pkgmgrinfo_appinfo_destroy_appinfo(app_info);

	// Set action type
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, CT_RULE_ACTION_TYPE_APP_CONTROL);

	// Set app control
	bundle* appctl_bundle = NULL;
	error = app_control_to_bundle(app_control, &appctl_bundle);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "App_control to bundle failed");

	bundle_raw* appctl_raw;
	int raw_length;
	error = bundle_encode(appctl_bundle, &appctl_raw, &raw_length);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "Bundle encode failed");

	std::string appctl_str = reinterpret_cast<const char*>(appctl_raw);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_APP_CONTROL, appctl_str);

	bundle_free_encoded_rawdata(&appctl_raw);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_set_action_notification(context_trigger_rule_h rule, const char* title, const char* content, const char* icon_path, app_control_h app_control)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && title && content);

	// Privilege check
	int error = __dbusClient.call(METHOD_CHK_PRIV_NOTIFICATION);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);

	// if action arleady exists
	std::string type;
	if ((rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type)) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	// Err: App control check
	if (app_control) {
		char* app_id = NULL;
		error = app_control_get_app_id(app_control, &app_id);
		IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to get app id");

		pkgmgrinfo_appinfo_h app_info;
#ifdef LEGACY_APPFW
		error = pkgmgrinfo_appinfo_get_appinfo(app_id, &app_info);
#else
		error = pkgmgrinfo_appinfo_get_usr_appinfo(app_id, getuid(), &app_info);
#endif
		g_free(app_id);
		IF_FAIL_RETURN_TAG(error == PMINFO_R_OK, CONTEXT_TRIGGER_ERROR_INVALID_RULE, _E, "No such app");
	}

	// Set title, content
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, CT_RULE_ACTION_TYPE_NOTIFICATION);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_NOTI_TITLE, title);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_NOTI_CONTENT, content);

	// Set icon path
	if (icon_path) {
		(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_NOTI_ICON_PATH, icon_path);
	}

	// Set app control
	if (app_control) {
		bundle* appctl_bundle = NULL;
		error = app_control_to_bundle(app_control, &appctl_bundle);
		IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "App_control to bundle failed");

		bundle_raw* appctl_raw;
		int raw_length;
		error = bundle_encode(appctl_bundle, &appctl_raw, &raw_length);
		IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "Bundle encode failed");

		std::string appctl_str = reinterpret_cast<const char*>(appctl_raw);
		(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_APP_CONTROL, appctl_str);

		bundle_free_encoded_rawdata(&appctl_raw);
	}

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_set_action_dbus_call(context_trigger_rule_h rule,
		const char *bus_name, const char *object_path, const char *interface_name, const char *method_name, GVariant *param)
{
	ASSERT_NOT_NULL(rule && bus_name && object_path && interface_name && method_name);

	/* if action arleady exists */
	std::string type;
	if ((rule->jrule).get(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type))
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;

	/* Set action type */
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_TYPE, CT_RULE_ACTION_TYPE_DBUS_CALL);

	/* Set basic dbus method call info */
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_DBUS_NAME, bus_name);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_DBUS_OBJECT, object_path);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_DBUS_INTERFACE, interface_name);
	(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_DBUS_METHOD, method_name);

	/* Set the parameters */
	if (param)
		(rule->jrule).set(CT_RULE_DETAILS "." CT_RULE_ACTION, CT_RULE_ACTION_DBUS_PARAMETER, param);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Set description
SO_EXPORT int context_trigger_rule_set_description(context_trigger_rule_h rule, const char* description)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);

	(rule->jrule).set(NULL, CT_RULE_DESCRIPTION, description);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Get rule description
SO_EXPORT int context_trigger_rule_get_description(context_trigger_rule_h rule, char** description)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && description);

	std::string val;
	(rule->jrule).get(NULL, CT_RULE_DESCRIPTION, &val);

	*description = strdup(val.c_str());

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Event creation
SO_EXPORT int context_trigger_rule_event_create(context_trigger_event_e event_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);

	std::string eitem_str = convert_event_to_string(event_item);
	if (eitem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = context_trigger_rule_event_create_internal(eitem_str.c_str(), logical_type, entry);
	return error;
}

SO_EXPORT int context_trigger_rule_custom_event_create(const char* event_item, const char* provider, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(event_item && provider && entry);

	// Err: Invalid provider
	pkgmgrinfo_pkginfo_h pkg_info;
#ifdef LEGACY_APPFW
	int error = pkgmgrinfo_pkginfo_get_pkginfo(provider, &pkg_info);
#else
	int error = pkgmgrinfo_pkginfo_get_usr_pkginfo(provider, getuid(), &pkg_info);
#endif
	pkgmgrinfo_pkginfo_destroy_pkginfo(pkg_info);
	IF_FAIL_RETURN_TAG(error == PMINFO_R_OK, CONTEXT_TRIGGER_ERROR_INVALID_DATA, _E, "No such package");

	std::string subject = get_custom_item_subject(provider, event_item);
	error = context_trigger_rule_event_create_internal(subject.c_str(), logical_type, entry, true);

	return error;
}

int context_trigger_rule_event_create_internal(const char* event_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry, bool is_custom)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(event_item);

	// Err: Invalid logical operator
	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = ctx::rule_validator::request_template(event_item, is_custom);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to request template: %#x", error);

	*entry = new(std::nothrow) _context_trigger_rule_entry_h(TYPE_EVENT);
	(*entry)->jentry.set(NULL, CT_RULE_EVENT_ITEM, event_item);
	(*entry)->jentry.set(NULL, CT_RULE_EVENT_OPERATOR, logical_str);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Event availability check
SO_EXPORT int context_trigger_rule_event_is_supported(context_trigger_event_e event_item, bool* supported)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(supported);

	*supported = false;

	std::string eitem_str = convert_event_to_string(event_item);
	if (eitem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = __dbusClient.isSupported(eitem_str);

	if (error == ERR_NONE)
		*supported = true;

	if (error == ERR_NOT_SUPPORTED)
		return ERR_NONE;

	return error;
}

// Condition creation
SO_EXPORT int context_trigger_rule_condition_create(context_trigger_condition_e condition_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);

	std::string citem_str = convert_condition_to_string(condition_item);
	if (citem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = context_trigger_rule_condition_create_internal(citem_str.c_str(), logical_type, entry);
	return error;
}

SO_EXPORT int context_trigger_rule_custom_condition_create(const char* condition_item, const char* provider, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(condition_item && provider && entry);

	// Err: Invalid provider
	pkgmgrinfo_pkginfo_h pkg_info;
#ifdef LEGACY_APPFW
	int error = pkgmgrinfo_pkginfo_get_pkginfo(provider, &pkg_info);
#else
	int error = pkgmgrinfo_pkginfo_get_usr_pkginfo(provider, getuid(), &pkg_info);
#endif
	pkgmgrinfo_pkginfo_destroy_pkginfo(pkg_info);
	IF_FAIL_RETURN_TAG(error == PMINFO_R_OK, CONTEXT_TRIGGER_ERROR_INVALID_DATA, _E, "No such package");

	std::string subject = get_custom_item_subject(provider, condition_item);
	error = context_trigger_rule_condition_create_internal(subject.c_str(), logical_type, entry, true);
	return error;
}

int context_trigger_rule_condition_create_internal(const char* condition_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry, bool is_custom)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(condition_item);

	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = ctx::rule_validator::request_template(condition_item, is_custom);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to request template: %#x", error);

	*entry = new(std::nothrow) _context_trigger_rule_entry_h(TYPE_CONDITION);
	(*entry)->jentry.set(NULL, CT_RULE_CONDITION_ITEM, condition_item);
	(*entry)->jentry.set(NULL, CT_RULE_CONDITION_OPERATOR, logical_str);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Condition availability check
SO_EXPORT int context_trigger_rule_condition_is_supported(context_trigger_condition_e condition_item, bool* supported)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(supported);

	*supported = false;

	std::string citem_str = convert_condition_to_string(condition_item);
	if (citem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = __dbusClient.isSupported(citem_str);

	if (error == ERR_NONE)
		*supported = true;

	if (error == ERR_NOT_SUPPORTED)
		return ERR_NONE;

	return error;
}

// Rule data deletion
SO_EXPORT int context_trigger_rule_entry_destroy(context_trigger_rule_entry_h entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);
	delete entry;

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_entry_add_option_int(context_trigger_rule_entry_h entry, const char* option_key, int value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && option_key);

	bool ret = true;
	std::string name;
	(entry->jentry).get(NULL, CT_RULE_CONDITION_ITEM, &name);
	ret = ctx::rule_validator::check_option_int(name, option_key, value);
	if (!ret) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	(entry->jentry).set(CT_RULE_CONDITION_OPTION, option_key, value);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_entry_add_option_string(context_trigger_rule_entry_h entry, const char* option_key, const char* value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && option_key && value);

	bool ret = true;
	std::string name;
	(entry->jentry).get(NULL, CT_RULE_CONDITION_ITEM, &name);
	ret = ctx::rule_validator::check_option_string(name, option_key, value);
	if (!ret) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	(entry->jentry).set(CT_RULE_CONDITION_OPTION, option_key, value);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_entry_add_option(context_trigger_rule_entry_h entry, const char* option_key, const char* event_data_key)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && option_key && event_data_key);

	// Err: Only conditin can reference data from event
	if (entry->type != TYPE_CONDITION) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_CONDITION_ITEM, &name);

	// Err: Check if key is valid
	bool ret = ctx::rule_validator::check_valid_key(TYPE_OPTION_STR, name, option_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	// Set reference information
	ret = ctx::rule_validator::set_ref_info(TYPE_OPTION_STR, &(entry->jref), name, option_key, event_data_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_OPERATION_FAILED);

	(entry->jentry).set(CT_RULE_CONDITION_OPTION, option_key, EVENT_DATA_KEY_PREFIX_STR + std::string(event_data_key));

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_entry_add_key(context_trigger_rule_entry_h entry, context_trigger_logical_type_e logical_type, const char* key)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key);

	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	// Err: Check if key is valid
	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);
	bool ret = ctx::rule_validator::check_valid_key(TYPE_ATTR_STR, name, key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	ctx::Json elem;
	for (int i = 0; (entry->jentry).getAt(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
		std::string elem_item;
		elem.get(NULL, CT_RULE_DATA_KEY, &elem_item);
		// Err: Comparison key is already added
		if (elem_item.compare(key) == 0) {
			return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
		}
	}

	ctx::Json data;
	data.set(NULL, CT_RULE_DATA_KEY, key);
	data.set(NULL, CT_RULE_DATA_KEY_OPERATOR, logical_str);
	(entry->jentry).append(NULL, CT_RULE_DATA_ARR, data);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

static int context_trigger_rule_entry_add_comparison_string_internal(context_trigger_rule_entry_h entry, const char* key, std::string op, std::string value)
{
	ctx::Json elem;
	for (int i = 0; (entry->jentry).getAt(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
		std::string elem_item;
		elem.get(NULL, CT_RULE_DATA_KEY, &elem_item);

		if (elem_item.compare(key) == 0) {
			std::string elem_val;
			std::string elem_op;
			for (int j = 0; elem.getAt(NULL, CT_RULE_DATA_VALUE_ARR, j, &elem_val); j++) {
				elem.getAt(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, j, &elem_op);

				// Err: Duplicated <operator, value>
				if (elem_val.compare(value) == 0 && elem_op.compare(op) == 0) {
					return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
				}
			}
			elem.append(NULL, CT_RULE_DATA_VALUE_ARR, value.c_str());
			elem.append(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, op);
			(entry->jentry).setAt(NULL, CT_RULE_DATA_ARR, i, elem);

			return CONTEXT_TRIGGER_ERROR_NONE;
		}
	}

	// Comparison key not exist
	return CONTEXT_TRIGGER_ERROR_NO_DATA;
}

static int context_trigger_rule_entry_add_comparison_int_internal(context_trigger_rule_entry_h entry, const char* key, std::string op, int value)
{
	ctx::Json elem;
	for (int i = 0; (entry->jentry).getAt(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
		std::string elem_item;
		elem.get(NULL, CT_RULE_DATA_KEY, &elem_item);

		if (elem_item.compare(key) == 0) {
			int elem_val;
			std::string elem_op;
			for (int j = 0; elem.getAt(NULL, CT_RULE_DATA_VALUE_ARR, j, &elem_val); j++) {
				elem.getAt(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, j, &elem_op);

				// Err: Duplicated <operator, value>
				if (elem_val == value && elem_op.compare(op) == 0) {
					return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
				}
			}
			elem.append(NULL, CT_RULE_DATA_VALUE_ARR, value);
			elem.append(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, op);
			(entry->jentry).setAt(NULL, CT_RULE_DATA_ARR, i, elem);

			return CONTEXT_TRIGGER_ERROR_NONE;
		}
	}

	// Comparison key not exist
	return CONTEXT_TRIGGER_ERROR_NO_DATA;
}

SO_EXPORT int context_trigger_rule_entry_add_comparison(context_trigger_rule_entry_h entry, const char* key, const char* op, const char* event_data_key)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op && event_data_key);

	// Err: Only condition can reference data from event
	if (entry->type != TYPE_CONDITION) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_CONDITION_ITEM, &name);

	// Err: Check if key is valid
	bool ret = ctx::rule_validator::check_valid_key(TYPE_ATTR_STR, name, key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	// Err: Invalid operator
	std::string type = ctx::rule_validator::get_data_type(TYPE_ATTR_STR, name, key);
	ret = ctx::rule_validator::is_valid_operator(type, op);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	int error = context_trigger_rule_entry_add_comparison_string_internal(entry, key, op, EVENT_DATA_KEY_PREFIX_STR + std::string(event_data_key));
	IF_FAIL_RETURN(error == ERR_NONE, error);

	// Set reference information
	ret = ctx::rule_validator::set_ref_info(TYPE_ATTR_STR, &(entry->jref), name, key, event_data_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_OPERATION_FAILED);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

SO_EXPORT int context_trigger_rule_entry_add_comparison_int(context_trigger_rule_entry_h entry, const char* key, const char* op, int value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op);

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);

	bool ret = ctx::rule_validator::check_comparison_int(name, key, op, value);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	int error = context_trigger_rule_entry_add_comparison_int_internal(entry, key, op, value);
	return error;
}
/*
SO_EXPORT int context_trigger_rule_entry_add_comparison_double(context_trigger_rule_entry_h entry, const char* key, const char* op, double value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op);

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);

	// TODO: check_comparison_double()
	bool ret = ctx::rule_validator::check_comparison_double(name, key, op, value);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	int error = context_trigger_rule_entry_add_comparison_internal(entry, key, op, double_to_string(value));
	return error;
}
*/
SO_EXPORT int context_trigger_rule_entry_add_comparison_string(context_trigger_rule_entry_h entry, const char* key, const char* op, const char* value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op && value);

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);

	bool ret = ctx::rule_validator::check_comparison_string(name, key, op, value);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	int error = context_trigger_rule_entry_add_comparison_string_internal(entry, key, op, value);
	return error;
}

SO_EXPORT int context_trigger_custom_register(const char* name, const char* attr_template)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(name && attr_template);

	// Err: Invalid Json
	ctx::Json jattr_template = attr_template;
	IF_FAIL_RETURN_TAG(jattr_template.valid(), CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "Failed to parse template");

	// Err: Invalid template
	bool ret = ctx::rule_validator::is_valid_template(jattr_template);
	IF_FAIL_RETURN_TAG(ret, CONTEXT_TRIGGER_ERROR_INVALID_DATA, _E, "Invalid template");

	ctx::Json data;
	data.set(NULL, CT_CUSTOM_REQ, CT_CUSTOM_ADD);
	data.set(NULL, CT_CUSTOM_NAME, name);
	data.set(NULL, CT_CUSTOM_ATTRIBUTES, jattr_template);

	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_CUSTOM, data, NULL);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to add custom item: %#x", error);

	return error;
}

SO_EXPORT int context_trigger_custom_unregister(const char* name)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(name);

	ctx::Json data;
	data.set(NULL, CT_CUSTOM_REQ, CT_CUSTOM_REMOVE);
	data.set(NULL, CT_CUSTOM_NAME, name);

	ctx::Json subj;
	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_CUSTOM, data, &subj);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to remove custom item: %#x", error);

	std::string subject;
	subj.get(NULL, CT_CUSTOM_SUBJECT, &subject);
	ctx::rule_validator::remove_template(subject);

	return error;
}

SO_EXPORT int context_trigger_custom_publish(const char* name, const char* fact)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(name && fact);

	// Err: Invalid Json
	ctx::Json jfact = fact;
	IF_FAIL_RETURN_TAG(jfact.valid(), CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "Cannot parse fact Json");

	ctx::Json data;
	data.set(NULL, CT_CUSTOM_REQ, CT_CUSTOM_PUBLISH);
	data.set(NULL, CT_CUSTOM_NAME, name);
	data.set(NULL, CT_CUSTOM_FACT, jfact);

	int error = __dbusClient.write(CONTEXT_TRIGGER_SUBJECT_CUSTOM, data, NULL);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to publish custom data");

	return error;
}

std::string convert_event_to_string(context_trigger_event_e item)
{
	std::string str;
	switch (item) {
	case CONTEXT_TRIGGER_EVENT_TIME:
		str = CT_EVENT_TIME;
		break;
	case CONTEXT_TRIGGER_EVENT_BATTERY:
		str = CT_EVENT_BATTERY;
		break;
	case CONTEXT_TRIGGER_EVENT_CHARGER:
		str = CT_EVENT_CHARGER;
		break;
	case CONTEXT_TRIGGER_EVENT_GPS:
		str = CT_EVENT_GPS;
		break;
	case CONTEXT_TRIGGER_EVENT_HEADPHONE:
		str = CT_EVENT_HEADPHONE;
		break;
	case CONTEXT_TRIGGER_EVENT_POWER_SAVING_MODE:
		str = CT_EVENT_POWER_SAVING_MODE;
		break;
	case CONTEXT_TRIGGER_EVENT_USB:
		str = CT_EVENT_USB;
		break;
	case CONTEXT_TRIGGER_EVENT_WIFI:
		str = CT_EVENT_WIFI;
		break;
	case CONTEXT_TRIGGER_EVENT_CALL:
		str = CT_EVENT_CALL;
		break;
	case CONTEXT_TRIGGER_EVENT_EMAIL:
		str = CT_EVENT_EMAIL;
		break;
	case CONTEXT_TRIGGER_EVENT_MESSAGE:
		str = CT_EVENT_MESSAGE;
		break;
	case CONTEXT_TRIGGER_EVENT_CONTACTS:
		str = CT_EVENT_CONTACTS;
		break;
	case CONTEXT_TRIGGER_EVENT_ACTIVITY_STATIONARY:
		str = CT_EVENT_ACTIVITY_STATIONARY;
		break;
	case CONTEXT_TRIGGER_EVENT_ACTIVITY_WALKING:
		str = CT_EVENT_ACTIVITY_WALKING;
		break;
	case CONTEXT_TRIGGER_EVENT_ACTIVITY_RUNNING:
		str = CT_EVENT_ACTIVITY_RUNNING;
		break;
	case CONTEXT_TRIGGER_EVENT_ACTIVITY_IN_VEHICLE:
		str = CT_EVENT_ACTIVITY_IN_VEHICLE;
		break;
	case CONTEXT_TRIGGER_EVENT_PLACE:
		str = CT_EVENT_PLACE;
		break;
	default:
		break;
	}
	return str;
}

std::string convert_condition_to_string(context_trigger_condition_e item)
{
	std::string str;
	switch (item) {
	case CONTEXT_TRIGGER_CONDITION_TIME:
		str = CT_CONDITION_TIME;
		break;
	case CONTEXT_TRIGGER_CONDITION_BATTERY:
		str = CT_CONDITION_BATTERY;
		break;
	case CONTEXT_TRIGGER_CONDITION_CHARGER:
		str = CT_CONDITION_CHARGER;
		break;
	case CONTEXT_TRIGGER_CONDITION_GPS:
		str = CT_CONDITION_GPS;
		break;
	case CONTEXT_TRIGGER_CONDITION_HEADPHONE:
		str = CT_CONDITION_HEADPHONE;
		break;
	case CONTEXT_TRIGGER_CONDITION_POWER_SAVING_MODE:
		str = CT_CONDITION_POWER_SAVING_MODE;
		break;
	case CONTEXT_TRIGGER_CONDITION_USB:
		str = CT_CONDITION_USB;
		break;
	case CONTEXT_TRIGGER_CONDITION_WIFI:
		str = CT_CONDITION_WIFI;
		break;
	case CONTEXT_TRIGGER_CONDITION_CALL:
		str = CT_CONDITION_CALL;
		break;
	case CONTEXT_TRIGGER_CONDITION_APP_USE_FREQUENCY:
		str = CT_CONDITION_APP_USE_FREQUENCY;
		break;
	case CONTEXT_TRIGGER_CONDITION_COMMUNICATION_FREQUENCY:
		str = CT_CONDITION_COMMUNICATION_FREQUENCY;
		break;
	case CONTEXT_TRIGGER_CONDITION_MUSIC_PLAYBACK_FREQUENCY:
		str = CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY;
		break;
	case CONTEXT_TRIGGER_CONDITION_VIDEO_PLAYBACK_FREQUENCY:
		str = CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY;
		break;
	default:
		break;
	}
	return str;
}

std::string convert_logical_type_to_string(context_trigger_logical_type_e logical_type)
{
	std::string str;
	switch (logical_type) {
	case CONTEXT_TRIGGER_LOGICAL_CONJUNCTION:
		str = "and";
		break;
	case CONTEXT_TRIGGER_LOGICAL_DISJUNCTION:
		str = "or";
		break;
	default:
		break;
	}
	return str;
}

std::string get_custom_item_subject(const char* provider, const char* item)
{
	std::string subject_name = CT_CUSTOM + std::string("/") + provider + std::string("/") + item;
	return subject_name;
}

/*
std::string int_to_string(int value)
{
	std::ostringstream ostr;
	ostr << value;
	return ostr.str();
}

std::string double_to_string(int value)
{
	std::ostringstream ostr;
	ostr.imbue(std::locale("C"));
	ostr << std::setprecision(DOUBLE_PRECISION) << std::fixed << value;
	return ostr.str();
}*/
