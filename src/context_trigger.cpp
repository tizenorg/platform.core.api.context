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

#include <sstream>
//#include <iomanip>
#include <types_internal.h>
#include <json.h>
#include <app_control_internal.h>
#include <bundle.h>
#include <bundle_internal.h>
#include <context_trigger.h>
#include <context_trigger_types_internal.h>
#include <request_handler.h>
#include "rule_validator.h"
#include "priv_util.h"

#define INITIAL_RULE "{ \"ID\" : -1, \"DESCRIPTION\" : \"\", \"DETAILS\" : {  } }"
#define INITIAL_ENTRY "{ \"DATA_ARR\" : [ ] }"
#define INITIAL_REF "{ \"option\" : [ ], \"attributes\" : [ ] }"
#define QUOTATION_MARK_STRING std::string("\"")
#define EVENT_DATA_KEY_PREFIX_STR std::string("?")
//#define DOUBLE_PRECISION 2

static std::string convert_event_to_string(context_trigger_event_e item);
static std::string convert_condition_to_string(context_trigger_condition_e item);
static std::string convert_logical_type_to_string(context_trigger_logical_type_e logical_type);
static int convert_operator(std::string type, std::string op, std::string* converted_op);
static std::string int_to_string(int value);
//static std::string double_to_string(int value);

typedef struct _context_trigger_rule_s {
	ctx::json jrule;	// rule_id, description, details(event, condition[])
	ctx::json jref;

	_context_trigger_rule_s()
	{
		jrule = INITIAL_RULE;
		jref = INITIAL_REF;
	}
} _context_trigger_rule_h;

typedef struct _context_trigger_rule_entry_s {
	ctx::json jentry;	// key, value, operator
	int type;
	ctx::json jref;

	_context_trigger_rule_entry_s(int t): type(t)
	{
		jentry = INITIAL_ENTRY;

		if (t == TYPE_CONDITION) {
			jref = INITIAL_REF;
		}
	}
} _context_trigger_rule_entry_h;

// Add a rule
EXTAPI int context_trigger_add_rule(context_trigger_rule_h rule, int* rule_id)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && rule_id);

	// Err: No event
	std::string ename;
	bool ret = (rule->jrule).get(CT_RULE_DETAILS"."CT_RULE_EVENT, CT_RULE_EVENT_ITEM, &ename);
	if (!ret)
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;

	// Err: No action added
	std::string type;
	ret = (rule->jrule).get(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type);
	if (!ret)
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;

	ctx::json jrule_id;
	int error = ctx::request_handler::write_with_reply(CONTEXT_TRIGGER_SUBJECT_ADD, &(rule->jrule), &jrule_id);

	if (error == ERR_NONE) {
		jrule_id.get(NULL, CT_RULE_ID, rule_id);
		(rule->jrule).set(NULL, CT_RULE_ID, *rule_id);
	}

	return error;
}

// Remove a rule
EXTAPI int context_trigger_remove_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);
	int error = ctx::request_handler::write_with_reply(CONTEXT_TRIGGER_SUBJECT_REMOVE, &jrule_id, NULL);

	if (error == ERR_ALREADY_STARTED) {	// Rule is still enabled.
		return CONTEXT_TRIGGER_ERROR_RULE_ENABLED;
	} else if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

// Enable a rule
EXTAPI int context_trigger_enable_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);

	int req_id;	// Useless in context_trigger
	int error = ctx::request_handler::subscribe(CONTEXT_TRIGGER_SUBJECT_ENABLE, &jrule_id, &req_id, NULL);

	if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

// Disable a rule
EXTAPI int context_trigger_disable_rule(int rule_id)
{
	_D("BEGIN");
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::json jrule_id;
	jrule_id.set(NULL, CT_RULE_ID, rule_id);
	int error = ctx::request_handler::write_with_reply(CONTEXT_TRIGGER_SUBJECT_DISABLE, &jrule_id, NULL);

	if (error == ERR_NO_DATA) {
		return CONTEXT_TRIGGER_ERROR_RULE_NOT_EXIST;
	}

	return error;
}

EXTAPI int context_trigger_get_own_rule_ids(int** enabled_rule_ids, int* enabled_rule_count, int** disabled_rule_ids, int* disabled_rule_count)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(enabled_rule_ids && enabled_rule_count && disabled_rule_ids && disabled_rule_count);

	int req_id;
	ctx::json data_read;
	int error = ctx::request_handler::read_sync(CONTEXT_TRIGGER_SUBJECT_GET_RULE_IDS, NULL, &req_id, &data_read);

	if (error != ERR_NONE) {
		return error;
	}

	// Enabled rules
	int* e_arr = NULL;
	*enabled_rule_count = data_read.array_get_size(NULL, CT_RULE_ARRAY_ENABLED);

	if (*enabled_rule_count > 0) {
		e_arr = static_cast<int*>(g_malloc((*enabled_rule_count) * sizeof(int)));
		IF_FAIL_RETURN_TAG(e_arr, CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY, _E, "Memory allocation failed");

		int id;
		for (int i = 0; data_read.get_array_elem(NULL, CT_RULE_ARRAY_ENABLED, i, &id); i++) {
			*(e_arr + i) = id;
		}
	}
	*enabled_rule_ids = e_arr;

	// Disabled rules
	int* d_arr = NULL;
	*disabled_rule_count = data_read.array_get_size(NULL, CT_RULE_ARRAY_DISABLED);

	if (*disabled_rule_count > 0) {
		d_arr = static_cast<int*>(g_malloc((*disabled_rule_count) * sizeof(int)));
		IF_FAIL_RETURN_TAG(d_arr, CONTEXT_TRIGGER_ERROR_OUT_OF_MEMORY, _E, "Memory allocation failed");

		int id;
		for (int i = 0; data_read.get_array_elem(NULL, CT_RULE_ARRAY_DISABLED, i, &id); i++) {
			*(d_arr + i) = id;
		}
	}
	*disabled_rule_ids = d_arr;

	return error;
}

EXTAPI int context_trigger_get_rule_by_id(int rule_id, context_trigger_rule_h* rule)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);
	if (rule_id <= 0)
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;

	ctx::json option;
	option.set(NULL, CT_RULE_ID, rule_id);

	int req_id;
	ctx::json data_read;
	int error = ctx::request_handler::read_sync(CONTEXT_TRIGGER_SUBJECT_GET, &option, &req_id, &data_read);

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
EXTAPI int context_trigger_rule_create(context_trigger_logical_type_e logical_type, context_trigger_rule_h* rule)
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
EXTAPI int context_trigger_rule_destroy(context_trigger_rule_h rule)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);
	delete rule;

	return CONTEXT_TRIGGER_ERROR_NONE;
}


EXTAPI int context_trigger_rule_add_entry(context_trigger_rule_h rule, context_trigger_rule_entry_h entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && entry);

	std::string des;
	bool ret = (rule->jrule).get(NULL, CT_RULE_DESCRIPTION, &des);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER);

	if (entry->type == TYPE_EVENT) {
		// Err: More than one event
		ctx::json elem;
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

		ctx::json temp = (entry->jentry).str();
		ret = (rule->jrule).set(CT_RULE_DETAILS, CT_RULE_EVENT, temp);
	} else if (entry->type == TYPE_CONDITION) {
		// Err: Condition without comparison data
		if ((entry->jentry).array_get_size(NULL, CT_RULE_DATA_ARR) < 1) {
			return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
		}

		ctx::json elem;
		for (int i = 0; (entry->jentry).get_array_elem(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
			int val_arr_size = elem.array_get_size(NULL, CT_RULE_DATA_VALUE_ARR);
			int op_arr_size = elem.array_get_size(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR);

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
				ctx::json info;
				for (int j = 0; (entry->jref).get_array_elem(NULL, TYPE_OPTION_STR, j, &info); j++) {
					(rule->jref).array_append(NULL, TYPE_OPTION_STR, info);
				}

				for (int j = 0; (entry->jref).get_array_elem(NULL, TYPE_ATTR_STR, j, &info); j++) {
					(rule->jref).array_append(NULL, TYPE_ATTR_STR, info);
				}
			}
		}

		ctx::json temp = (entry->jentry).str();
		ret = (rule->jrule).array_append(CT_RULE_DETAILS, CT_RULE_CONDITION, temp);
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

EXTAPI int context_trigger_rule_set_action_app_control(context_trigger_rule_h rule, app_control_h app_control)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && app_control);
	int error;

	// Privilege check
	error = ctx::privilege_util::is_allowed("appmanager.launch");
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);

	if (is_call_operation(app_control)) {
		error = ctx::privilege_util::is_allowed("call");
		IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);
	}

	// Err: if action arleady exists
	std::string type;
	if ((rule->jrule).get(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type)) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_TYPE, CT_RULE_ACTION_TYPE_APP_CONTROL);

	// Set app control
	bundle* appctl_bundle = NULL;
	error = app_control_to_bundle(app_control, &appctl_bundle);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "App_control to bundle failed");

	bundle_raw* appctl_raw;
	int raw_length;
	error = bundle_encode(appctl_bundle, &appctl_raw, &raw_length);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER, _E, "Bundle encode failed");

	std::string appctl_str = reinterpret_cast<const char*>(appctl_raw);
	(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_APP_CONTROL, appctl_str);

	bundle_free_encoded_rawdata(&appctl_raw);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

EXTAPI int context_trigger_rule_set_action_notification(context_trigger_rule_h rule, const char* title, const char* content, const char* icon_path, app_control_h app_control)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && title && content);

	// Privilege check
	int error = ctx::privilege_util::is_allowed("notification");
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Privilege checking failed (%#x)", error);

	// if action arleady exists
	std::string type;
	if ((rule->jrule).get(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_TYPE, &type)) {
		return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
	}

	// Set title, content
	(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_TYPE, CT_RULE_ACTION_TYPE_NOTIFICATION);
	(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_NOTI_TITLE, title);
	(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_NOTI_CONTENT, content);

	// Set icon path
	if (icon_path) {
		(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_NOTI_ICON_PATH, icon_path);
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
		(rule->jrule).set(CT_RULE_DETAILS"."CT_RULE_ACTION, CT_RULE_ACTION_APP_CONTROL, appctl_str);

		bundle_free_encoded_rawdata(&appctl_raw);
	}

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Set description
EXTAPI int context_trigger_rule_set_description(context_trigger_rule_h rule, const char* description)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule);

	(rule->jrule).set(NULL, CT_RULE_DESCRIPTION, description);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Get rule description
EXTAPI int context_trigger_rule_get_description(context_trigger_rule_h rule, char** description)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(rule && description);

	std::string val;
	(rule->jrule).get(NULL, CT_RULE_DESCRIPTION, &val);

	*description = strdup(val.c_str());

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Event creation
EXTAPI int context_trigger_rule_event_create(context_trigger_event_e event_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);

	std::string eitem_str = convert_event_to_string(event_item);
	if (eitem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	*entry = new(std::nothrow) _context_trigger_rule_entry_h(TYPE_EVENT);
	(*entry)->jentry.set(NULL, CT_RULE_EVENT_ITEM, eitem_str);
	(*entry)->jentry.set(NULL, CT_RULE_EVENT_OPERATOR, logical_str);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Event availability check
EXTAPI int context_trigger_rule_event_is_supported(context_trigger_event_e event_item, bool* supported)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(supported);

	if (event_item == CONTEXT_TRIGGER_EVENT_TIME) {
		*supported = true;
		return CONTEXT_TRIGGER_ERROR_NONE;
	}

	*supported = false;

	std::string eitem_str = convert_event_to_string(event_item);
	if (eitem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = ctx::request_handler::is_supported(eitem_str.c_str());

	if (error == ERR_NONE)
		*supported = true;

	if (error == ERR_NOT_SUPPORTED)
		return ERR_NONE;

	return error;
}

// Condition creation
EXTAPI int context_trigger_rule_condition_create(context_trigger_condition_e condition_item, context_trigger_logical_type_e logical_type, context_trigger_rule_entry_h* entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);

	std::string citem_str = convert_condition_to_string(condition_item);
	if (citem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	std::string logical_str = convert_logical_type_to_string(logical_type);
	if (logical_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	*entry = new(std::nothrow) _context_trigger_rule_entry_h(TYPE_CONDITION);
	(*entry)->jentry.set(NULL, CT_RULE_CONDITION_ITEM, citem_str);
	(*entry)->jentry.set(NULL, CT_RULE_CONDITION_OPERATOR, logical_str);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

// Condition availability check
EXTAPI int context_trigger_rule_condition_is_supported(context_trigger_condition_e condition_item, bool* supported)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(supported);

	if (condition_item == CONTEXT_TRIGGER_CONDITION_TIME) {
		*supported = true;
		return CONTEXT_TRIGGER_ERROR_NONE;
	}

	*supported = false;

	std::string citem_str = convert_condition_to_string(condition_item);
	if (citem_str.empty()) {
		return CONTEXT_TRIGGER_ERROR_INVALID_PARAMETER;
	}

	int error = ctx::request_handler::is_supported(citem_str.c_str());

	if (error == ERR_NONE)
		*supported = true;

	if (error == ERR_NOT_SUPPORTED)
		return ERR_NONE;

	return error;
}

// Rule data deletion
EXTAPI int context_trigger_rule_entry_destroy(context_trigger_rule_entry_h entry)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry);
	delete entry;

	return CONTEXT_TRIGGER_ERROR_NONE;
}

EXTAPI int context_trigger_rule_entry_add_option_int(context_trigger_rule_entry_h entry, const char* option_key, int value)
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

EXTAPI int context_trigger_rule_entry_add_option_string(context_trigger_rule_entry_h entry, const char* option_key, const char* value)
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

EXTAPI int context_trigger_rule_entry_add_option(context_trigger_rule_entry_h entry, const char* option_key, const char* event_data_key)
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
	bool ret = ctx::rule_validator::check_valid_key(TYPE_OPTION, name, option_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	// Set reference information
	ret = ctx::rule_validator::set_ref_info(TYPE_OPTION, &(entry->jref), name, option_key, event_data_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_OPERATION_FAILED);

	(entry->jentry).set(CT_RULE_CONDITION_OPTION, option_key, EVENT_DATA_KEY_PREFIX_STR + std::string(event_data_key));

	return CONTEXT_TRIGGER_ERROR_NONE;
}

EXTAPI int context_trigger_rule_entry_add_key(context_trigger_rule_entry_h entry, context_trigger_logical_type_e logical_type, const char* key)
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
	bool ret = ctx::rule_validator::check_valid_key(TYPE_ATTR, name, key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	ctx::json elem;
	for (int i = 0; (entry->jentry).get_array_elem(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
		std::string elem_item;
		elem.get(NULL, CT_RULE_DATA_KEY, &elem_item);
		// Err: Comparison key is already added
		if (elem_item.compare(key) == 0) {
			return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
		}
	}

	ctx::json data;
	data.set(NULL, CT_RULE_DATA_KEY, key);
	data.set(NULL, CT_RULE_DATA_KEY_OPERATOR, logical_str);
	(entry->jentry).array_append(NULL, CT_RULE_DATA_ARR, data);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

static int context_trigger_rule_entry_add_comparison_internal(context_trigger_rule_entry_h entry, const char* key, std::string op, std::string value)
{
	ctx::json elem;
	for (int i = 0; (entry->jentry).get_array_elem(NULL, CT_RULE_DATA_ARR, i, &elem); i++) {
		std::string elem_item;
		elem.get(NULL, CT_RULE_DATA_KEY, &elem_item);

		if (elem_item.compare(key) == 0) {
			std::string elem_val;
			std::string elem_op;
			for (int j = 0; elem.get_array_elem(NULL, CT_RULE_DATA_VALUE_ARR, j, &elem_val); j++) {
				elem.get_array_elem(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, j, &elem_op);

				// Err: Duplicated <operator, value>
				if (elem_val.compare(value) == 0 && elem_op.compare(op) == 0) {
					return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
				}
			}
			elem.array_append(NULL, CT_RULE_DATA_VALUE_ARR, value.c_str());
			elem.array_append(NULL, CT_RULE_DATA_VALUE_OPERATOR_ARR, op);
			(entry->jentry).array_set_at(NULL, CT_RULE_DATA_ARR, i, elem);

			return CONTEXT_TRIGGER_ERROR_NONE;
		}
	}

	// Comparison key not exist
	return CONTEXT_TRIGGER_ERROR_NO_DATA;
}

EXTAPI int context_trigger_rule_entry_add_comparison(context_trigger_rule_entry_h entry, const char* key, const char* op, const char* event_data_key)
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
	bool ret = ctx::rule_validator::check_valid_key(TYPE_ATTR, name, key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	int type = ctx::rule_validator::get_data_type(TYPE_ATTR, name, key);
	std::string type_str = (type == TYPE_INT)? "int" : "string";

	int error;
	std::string converted_op;
	error = convert_operator(type_str, op, &converted_op);	// Err: Invalid operator
	if (error != ERR_NONE){
		return error;
	}

	error = context_trigger_rule_entry_add_comparison_internal(entry, key, converted_op, EVENT_DATA_KEY_PREFIX_STR + std::string(event_data_key));
	if (error != ERR_NONE)
		return error;

	// Set reference information
	ret = ctx::rule_validator::set_ref_info(TYPE_ATTR, &(entry->jref), name, key, event_data_key);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_OPERATION_FAILED);

	return CONTEXT_TRIGGER_ERROR_NONE;
}

EXTAPI int context_trigger_rule_entry_add_comparison_int(context_trigger_rule_entry_h entry, const char* key, const char* op, int value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op);

	int error;
	std::string converted_op;

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);

	bool ret = ctx::rule_validator::check_comparison_int(name, key, op, value);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	error = convert_operator("int", op, &converted_op);	// Err: Invalid operator
	if (error != ERR_NONE){
		return error;
	}

	error = context_trigger_rule_entry_add_comparison_internal(entry, key, converted_op, int_to_string(value));
	return error;
}
/*
EXTAPI int context_trigger_rule_entry_add_comparison_double(context_trigger_rule_entry_h entry, const char* key, const char* op, double value)
{
	ASSERT_NOT_NULL(entry && key && op);

	int error;
	std::string converted_op;

	error = convert_operator("double", op, &converted_op);	// Err: Invalid operator
	if (error != ERR_NONE){
		return error;
	}

	error = context_trigger_rule_entry_add_comparison_internal(entry, key, converted_op, double_to_string(value));
	return error;
}
*/
EXTAPI int context_trigger_rule_entry_add_comparison_string(context_trigger_rule_entry_h entry, const char* key, const char* op, const char* value)
{
	_D("BEGIN");
	ASSERT_NOT_NULL(entry && key && op && value);

	int error;
	std::string converted_op;

	std::string name;
	(entry->jentry).get(NULL, CT_RULE_EVENT_ITEM, &name);

	bool ret = ctx::rule_validator::check_comparison_string(name, key, op, value);
	IF_FAIL_RETURN(ret, CONTEXT_TRIGGER_ERROR_INVALID_RULE);

	error = convert_operator("string", op, &converted_op);	// Err: Invalid operator
	if (error != ERR_NONE){
		return error;
	}

	error = context_trigger_rule_entry_add_comparison_internal(entry, key, converted_op, QUOTATION_MARK_STRING + value + QUOTATION_MARK_STRING);
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

int convert_operator(std::string type, std::string op, std::string* converted_op)
{
	if (op.compare("==") == 0) {
		*converted_op = "eq";
		return CONTEXT_TRIGGER_ERROR_NONE;
	} else if(op.compare("!=") == 0) {
		*converted_op = "neq";
		return CONTEXT_TRIGGER_ERROR_NONE;
	}

	if (type.compare("int") == 0 || type.compare("double") == 0) {
		if (op.compare("<") == 0 || op.compare("<=") == 0 || op.compare(">") == 0 || op.compare(">=") == 0) {
			*converted_op = op;
			return CONTEXT_TRIGGER_ERROR_NONE;
		}
	}

	return CONTEXT_TRIGGER_ERROR_INVALID_RULE;
}

std::string int_to_string(int value)
{
	std::ostringstream ostr;
	ostr << value;
	return ostr.str();
}
/*
std::string double_to_string(int value)
{
	std::ostringstream ostr;
	ostr.imbue(std::locale("C"));
	ostr << std::setprecision(DOUBLE_PRECISION) << std::fixed << value;
	return ostr.str();
}*/
