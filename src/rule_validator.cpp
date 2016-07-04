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

#include <sstream>
#include <string>
#include <map>
#include <Types.h>
#include <context_trigger.h>
#include <context_trigger_types_internal.h>
#include "DBusClient.h"
#include "rule_validator.h"

#define KEY_KEY "key"
#define KEY_REF "ref"

typedef std::map<std::string, ctx::Json> template_map_t;
template_map_t template_map;	// <name, template>

static int string_to_int(std::string str);
static bool check_value_int(ctx::Json& tmpl, std::string key, int value);
static bool check_value_string(ctx::Json& tmpl, std::string key, std::string value);
static bool check_value_enum(ctx::Json& tmpl, std::string key, std::string value);
static ctx::Json get_template(std::string name);
static bool check_template_int(ctx::Json& elem);
static bool check_template_string(ctx::Json& elem);
static bool check_template_enum(ctx::Json& elem);
static std::string get_data_type(ctx::Json& elem, std::string& key);
static bool is_equal_type(std::string& type1, std::string& type2);

int string_to_int(std::string str)
{
	int i;
	std::istringstream convert(str);

	if (!(convert >> i))
		i = 0;

	return i;
}

ctx::Json get_template(std::string name)
{
	ctx::rule_validator::request_template(name);
	return template_map[name];
}

int ctx::rule_validator::request_template(std::string name, bool mandatory)
{
	if (!mandatory) {
		template_map_t::iterator it = template_map.find(name);
		IF_FAIL_RETURN(it == template_map.end(), ERR_NONE);
	}

	// Request template
	ctx::Json request;
	request.set(NULL, SUBJECT_STR, name);

	int req_id;
	ctx::Json tmpl;
	ctx::DBusClient dbusClient;
	int error = dbusClient.readSync(CONTEXT_TRIGGER_SUBJECT_GET_TEMPLATE, request, &req_id, &tmpl);
	if (error == ERR_NOT_SUPPORTED) {
		template_map.erase(name);
		_E("Failed to get request: not supported");
		return ERR_NOT_SUPPORTED;
	}
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "Failed to get request");

	template_map[name] = tmpl;

	return ERR_NONE;
}

void ctx::rule_validator::remove_template(std::string name)
{
	template_map.erase(name);
}

// called by context_trigger_rule_add_entry()
bool ctx::rule_validator::check_option(ctx::Json& item)
{
	std::string name;
	item.get(NULL, CT_RULE_EVENT_ITEM, &name);

	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	// No option needed
	ctx::Json opt_tmpl;
	tmpl.get(NULL, TYPE_OPTION_STR, &opt_tmpl);

	std::list<std::string> opt_keys;
	IF_FAIL_RETURN(opt_tmpl.getKeys(&opt_keys), false);
	if (opt_keys.size() <= 0) {
		return true;
	}

	// Err: Check if mandatory option is missed
	std::string val_str;
	int val;
	if (name == CT_EVENT_PLACE) {
		if (!(item.get(CT_RULE_EVENT_OPTION, CONTEXT_TRIGGER_PLACE_ID, &val)))
			return false;
	} else if (name == CT_CONDITION_APP_USE_FREQUENCY) {
		if (!(item.get(CT_RULE_CONDITION_OPTION, CONTEXT_TRIGGER_APP_ID, &val_str)))
			return false;
	} else if (name == CT_CONDITION_COMMUNICATION_FREQUENCY) {
		if (!(item.get(CT_RULE_CONDITION_OPTION, CONTEXT_TRIGGER_ADDRESS, &val_str)))
			return false;
	}

	return true;
}

// called by context_trigger_rule_entry_add_option_int()
bool ctx::rule_validator::check_option_int(std::string name, std::string key, int value)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	ctx::Json opt_tmpl;
	tmpl.get(NULL, TYPE_OPTION_STR, &opt_tmpl);

	// Err: Item with no option
	std::list<std::string> opt_keys;
	IF_FAIL_RETURN(opt_tmpl.getKeys(&opt_keys), false);
	IF_FAIL_RETURN(opt_keys.size() > 0, false);

	// Err: Invalid option key or Invalid value type
	std::string t_type = get_data_type(opt_tmpl, key);
	IF_FAIL_RETURN(t_type == TYPE_INT_STR, false);

	// Err: Inappropriate value
	//   a. normal case
	bool ret = check_value_int(opt_tmpl, key, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_option_string()
bool ctx::rule_validator::check_option_string(std::string name, std::string key, std::string value)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	ctx::Json opt_tmpl;
	tmpl.get(NULL, TYPE_OPTION_STR, &opt_tmpl);

	// Err: ';' for SQL injection
	IF_FAIL_RETURN(value.find(';') == std::string::npos, false);

	// Err: Item with no option
	std::list<std::string> opt_keys;
	IF_FAIL_RETURN(opt_tmpl.getKeys(&opt_keys), false);
	IF_FAIL_RETURN(opt_keys.size() > 0, false);

	// Err: Invalid option key or Invalid value type
	std::string t_type = get_data_type(opt_tmpl, key);
	IF_FAIL_RETURN(t_type == TYPE_STRING_STR || t_type == TYPE_ENUM_STR, false);

	// Err: Inappropriate value
	//   a. spacial case
	if (t_type == TYPE_STRING_STR && (name == CT_CONDITION_APP_USE_FREQUENCY || name == CT_CONDITION_COMMUNICATION_FREQUENCY
			|| name == CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY || name == CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY)
			&& key == CONTEXT_TRIGGER_TIME_OF_DAY) {
		std::size_t found = value.find("-");
		if (found == std::string::npos) {
			return false;
		}

		int t1 = string_to_int(value.substr(0, found-1));
		int t2 = string_to_int(value.substr(found+1, value.length()-1));

		if (!(t1 >= 0 && t1 < 24) || !(t2 >= 0 && t2 < 24)) {
			return false;
		}

		if (t1 >= t2) {
			return false;
		}

		return true;
	}

	//   b. normal case
	bool ret;
	if (t_type == TYPE_STRING_STR) {
		ret = check_value_string(opt_tmpl, key, value);
	} else if (t_type == TYPE_ENUM_STR) {
		ret = check_value_enum(opt_tmpl, key, value);
	}

	return ret;
}

// called by context_trigger_rule_entry_add_comparison_int()
bool ctx::rule_validator::check_comparison_int(std::string name, std::string key, std::string op, int value)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	ctx::Json attr_tmpl;
	tmpl.get(NULL, TYPE_ATTR_STR, &attr_tmpl);

	// Err: Invalid attribute key or Invalid value type
	std::string t_type = get_data_type(attr_tmpl, key);
	IF_FAIL_RETURN(t_type == TYPE_INT_STR, false);

	// Err: Invalid operator for the value
	//    a. normal case
	bool ret = is_valid_operator(TYPE_INT_STR, op);
	IF_FAIL_RETURN(ret, false);
	//    b. special case
	if (name == CT_EVENT_TIME && key == CONTEXT_TRIGGER_TIME_OF_DAY) {
		IF_FAIL_RETURN(op == CONTEXT_TRIGGER_EQUAL_TO, false);
	}

	// Err: Inappropriate value
	//    a. normal case
	ret = check_value_int(attr_tmpl, key, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_comparison_string()
bool ctx::rule_validator::check_comparison_string(std::string name, std::string key, std::string op, std::string value)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != ctx::Json(EMPTY_JSON_OBJECT), false);

	ctx::Json attr_tmpl;
	tmpl.get(NULL, TYPE_ATTR_STR, &attr_tmpl);

	// Err: ';' for SQL injection
	IF_FAIL_RETURN(value.find(';') == std::string::npos, false);

	// Err: Invalid option key or Invalid value type
	std::string t_type = get_data_type(attr_tmpl, key);
	IF_FAIL_RETURN(t_type == TYPE_STRING_STR || t_type == TYPE_ENUM_STR, false);

	// Err: Invalid operator for the value
	bool ret = is_valid_operator(t_type, op);
	IF_FAIL_RETURN(ret, false);

	// Err: Inappropriate value
	//    a. normal case
	if (t_type == TYPE_STRING_STR) {
		ret = check_value_string(attr_tmpl, key, value);
		IF_FAIL_RETURN(ret, false);
	} else if (t_type == TYPE_ENUM_STR) {
		ret = check_value_enum(attr_tmpl, key, value);
		IF_FAIL_RETURN(ret, false);
	}

	return true;
}

bool ctx::rule_validator::check_valid_key(std::string type, std::string name, std::string key)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	// Err: Invalid key
	ctx::Json tmp;
	bool ret = tmpl.get(type.c_str(), key.c_str(), &tmp);
	IF_FAIL_RETURN(ret, false);

	return true;
}

bool check_value_int(ctx::Json& tmpl, std::string key, int value)
{
	int min, max;

	if (tmpl.get(key.c_str(), CT_CUSTOM_MIN, &min)) {
		IF_FAIL_RETURN(value >= min, false);
	}

	if (tmpl.get(key.c_str(), CT_CUSTOM_MAX, &max)) {
		IF_FAIL_RETURN(value <= max, false);
	}

	return true;
}

bool check_value_string(ctx::Json& tmpl, std::string key, std::string value)
{
	return true;
}

bool check_value_enum(ctx::Json& tmpl, std::string key, std::string value)
{
	std::string t_val;
	for (int i = 0; tmpl.getAt(key.c_str(), CT_CUSTOM_ENUM, i, &t_val); i++) {
		if (t_val == value)
			return true;
	}

	return false;
}

// called by context_trigger_rule_entry_add_comparison()
// called by context_trigger_rule_entry_add_option()
bool ctx::rule_validator::set_ref_info(std::string type, ctx::Json* jref, std::string name, std::string key, std::string ref_data)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, false);

	ctx::Json detailed_tmpl;
	tmpl.get(NULL, type.c_str(), &detailed_tmpl);

	std::string dt = get_data_type(detailed_tmpl, key);
	IF_FAIL_RETURN(dt == TYPE_INT_STR || dt == TYPE_STRING_STR || dt == TYPE_ENUM_STR, false);

	ctx::Json temp;
	temp.set(NULL, CT_CUSTOM_NAME, name);
	temp.set(NULL, KEY_KEY, key);
	temp.set(NULL, CT_CUSTOM_TYPE, dt);
	temp.set(NULL, KEY_REF, ref_data);

	if (type == TYPE_OPTION_STR) {
		jref->append(NULL, TYPE_OPTION_STR, temp);
	} else if (type == TYPE_ATTR_STR) {
		jref->append(NULL, TYPE_ATTR_STR, temp);
	} else {
		return false;
	}

	return true;;
}

std::string get_data_type(ctx::Json& elem, std::string& key)
{
	std::string data_type;
	bool ret = elem.get(key.c_str(), CT_CUSTOM_TYPE, &data_type);
	if (!ret) {
		int size = elem.getSize(key.c_str(), CT_CUSTOM_ENUM);
		IF_FAIL_RETURN(size > 0, "");
		data_type = TYPE_ENUM_STR;
	}
	IF_FAIL_RETURN(data_type == TYPE_INT_STR || data_type == TYPE_STRING_STR || data_type == TYPE_ENUM_STR, "");

	return data_type;
}

// called by context_trigger_rule_entry_add_comparison()
std::string ctx::rule_validator::get_data_type_from_template(std::string type, std::string name, std::string key)
{
	ctx::Json tmpl = get_template(name);
	IF_FAIL_RETURN(tmpl != EMPTY_JSON_OBJECT, "");

	ctx::Json detailed_tmpl;
	tmpl.get(NULL, type.c_str(), &detailed_tmpl);

	return get_data_type(detailed_tmpl, key);
}

// called by context_trigger_rule_add_entry()
bool ctx::rule_validator::check_referential_data(std::string name, ctx::Json& ref_info)
{
	std::map<std::string, std::string> type_map;

	ctx::Json ref_data;
	for (int i = 0; ref_info.getAt(NULL, TYPE_OPTION_STR, i, &ref_data); i++) {
		std::string ref_key;
		ref_data.get(NULL, KEY_REF, &ref_key);
		std::string cond_type;
		ref_data.get(NULL, CT_CUSTOM_TYPE, &cond_type);

		if (type_map.count(ref_key) == 0) {
			type_map[ref_key] = get_data_type_from_template(TYPE_ATTR_STR, name, ref_key); // "", if invalid key
		}

		// Err: Invalid key or Value type not matched
		IF_FAIL_RETURN(is_equal_type(cond_type, type_map[ref_key]), false);
	}

	for (int i = 0; ref_info.getAt(NULL, TYPE_ATTR_STR, i, &ref_data); i++) {
		std::string ref_key;
		ref_data.get(NULL, KEY_REF, &ref_key);
		std::string cond_type;
		ref_data.get(NULL, CT_CUSTOM_TYPE, &cond_type);

		if (type_map.count(ref_key) == 0) {
			type_map[ref_key] = get_data_type_from_template(TYPE_ATTR_STR, name, ref_key); // "", if invalid key
		}

		// Err: Invalid key or Value type not matched
		IF_FAIL_RETURN(is_equal_type(cond_type, type_map[ref_key]), false);
	}

	return true;
}

bool is_equal_type(std::string& type1, std::string& type2)
{
	// This function regards TYPE_ENUM_STR as TYPE_STRING_STR for referential data
	if (type1 == type2) {
		return true;
	}

	if ((type1 == TYPE_STRING_STR || type1 == TYPE_ENUM_STR) && (type2 == TYPE_STRING_STR || type2 == TYPE_ENUM_STR)) {
		return true;
	}

	return false;
}

bool ctx::rule_validator::is_valid_operator(std::string type, std::string op)
{
	if (op == CONTEXT_TRIGGER_EQUAL_TO || op == CONTEXT_TRIGGER_NOT_EQUAL_TO) {
		return true;
	}

	if (type == TYPE_INT_STR || type == TYPE_DOUBLE_STR) {
		if (op == CONTEXT_TRIGGER_GREATER_THAN || op == CONTEXT_TRIGGER_GREATER_THAN_OR_EQUAL_TO ||
			op == CONTEXT_TRIGGER_LESS_THAN || op == CONTEXT_TRIGGER_LESS_THAN_OR_EQUAL_TO) {
			return true;
		}
	}

	return false;
}

// For custom item template
bool ctx::rule_validator::is_valid_template(ctx::Json& attr_template)
{
	IF_FAIL_RETURN_TAG(attr_template != EMPTY_JSON_OBJECT, false, _E, "Custom template: empty Json");

	bool ret;
	std::list<std::string> keys;
	attr_template.getKeys(&keys);

	for (std::list<std::string>::iterator it = keys.begin(); it != keys.end(); it++) {
		std::string key = *it;

		std::string type = get_data_type(attr_template, key);
		IF_FAIL_RETURN_TAG(type == CT_CUSTOM_INT || type == CT_CUSTOM_STRING || type == CT_CUSTOM_ENUM,
			false, _E, "Custom template: invalid data type");

		ctx::Json elem;
		attr_template.get(NULL, key.c_str(), &elem);
		if (type == CT_CUSTOM_INT) {
			ret = check_template_int(elem);
			IF_FAIL_RETURN(ret, false);
		} else if (type == CT_CUSTOM_STRING) {
			ret = check_template_string(elem);
			IF_FAIL_RETURN(ret, false);
		} else if (type == CT_CUSTOM_ENUM) {
			ret = check_template_enum(elem);
			IF_FAIL_RETURN(ret, false);
		}
	}

	return true;
}

bool check_template_int(ctx::Json& elem)
{
	std::list<std::string> elem_keys;
	elem.getKeys(&elem_keys);

	bool min;
	bool max;
	int min_val;
	int max_val;

	for (std::list<std::string>::iterator it2 = elem_keys.begin(); it2 != elem_keys.end(); it2++) {
		std::string elem_key = *it2;
		if (elem_key == CT_CUSTOM_MIN) {
			min = elem.get(NULL, elem_key.c_str(), &min_val);
			IF_FAIL_RETURN_TAG(min, false, _E, "Custom template: failed to get integer type value");
		} else if (elem_key == CT_CUSTOM_MAX) {
			max = elem.get(NULL, elem_key.c_str(), &max_val);
			IF_FAIL_RETURN_TAG(max, false, _E, "Custom template: failed to get integer type value");
		} else {
			IF_FAIL_RETURN_TAG(elem_key == CT_CUSTOM_TYPE, false, _E, "Custom template: invalid key");
		}
	}

	if (min && max) {
		IF_FAIL_RETURN_TAG(min_val <= max_val, false, _E, "Custom template: invalid min, max value");
	}

	return true;
}

bool check_template_string(ctx::Json& elem)
{
	std::list<std::string> elem_keys;
	elem.getKeys(&elem_keys);

	for (std::list<std::string>::iterator it2 = elem_keys.begin(); it2 != elem_keys.end(); it2++) {
		std::string elem_key = *it2;
		_D("SOM TEST >>> elem key (%s)", elem_key.c_str());
		IF_FAIL_RETURN_TAG(elem_key == CT_CUSTOM_TYPE, false, _E, "Custom template: invalid key");
	}

	return true;
}

bool check_template_enum(ctx::Json& elem)
{
	std::list<std::string> elem_keys;
	elem.getKeys(&elem_keys);

	for (std::list<std::string>::iterator it2 = elem_keys.begin(); it2 != elem_keys.end(); it2++) {
		std::string elem_key = *it2;

		if (elem_key == CT_CUSTOM_ENUM) {
			int size = elem.getSize(NULL, CT_CUSTOM_ENUM);
			IF_FAIL_RETURN_TAG(size > 0, false, _E, "Custom template: invalid enum");

			std::string val_str;
			for (int i = 0; i < size; i++) {
				bool ret = elem.getAt(NULL, CT_CUSTOM_ENUM, i, &val_str);
				IF_FAIL_RETURN_TAG(ret, false, _E, "Custom template: failed to get string type value");
			}
		} else {
			IF_FAIL_RETURN_TAG(elem_key == CT_CUSTOM_TYPE, false, _E, "Custom template: invalid key");
		}
	}

	return true;
}
