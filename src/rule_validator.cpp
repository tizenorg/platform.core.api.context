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
#include <string>
#include <map>
#include <types_internal.h>
#include <context_trigger.h>
#include <context_trigger_types_internal.h>
#include "rule_info.h"
#include "rule_validator.h"

#define KEY_TEMPLATE "templates"
#define KEY_NAME "name"
#define KEY_OPTION TYPE_OPTION_STR
#define KEY_ATTR TYPE_ATTR_STR
#define KEY_KEY "key"
#define KEY_TYPE "type"
#define KEY_MIN "min"
#define KEY_MAX "max"
#define KEY_ACCEPTABLE "acceptable"
#define NO_LIMIT -1
#define KEY_REF "ref"

typedef std::map<std::string, ctx::json> template_map_t;
template_map_t template_map;	// <name, template>

static void init(void);
static int string_to_int(std::string str);
static bool check_value_int(ctx::json& tempt, int value);
static bool check_value_string(ctx::json& tempt, std::string value);

int string_to_int(std::string str)
{
	int i;
	std::istringstream convert(str);

	if (!(convert >> i))
		i = 0;

	return i;
}

void init(void)
{
	static bool initialized = false;
	if (initialized)
		return;

	// Load from Data
	ctx::json t = ctx::json(static_cast<const char*>(RULE_INFO));

	ctx::json elem;
	for (int i = 0; t.get_array_elem(NULL, "templates", i, &elem); i++) {
		std::string name;
		elem.get(NULL, "name", &name);

		template_map[name] = elem;
	}

	initialized = true;
}

// called by context_trigger_rule_add_entry()
bool ctx::rule_validator::check_option(ctx::json& item)
{
	init();

	std::string name;
	item.get(NULL, CT_RULE_EVENT_ITEM, &name);

	// No option needed
	if (template_map[name].array_get_size(NULL, KEY_OPTION) <= 0)
		return true;

	// Err: Check if mandatory option is missed
	std::string val_str;
	int val;
	if (name.compare(CT_EVENT_PLACE) == 0) {
		if (!(item.get(CT_RULE_EVENT_OPTION, CONTEXT_TRIGGER_PLACE_ID, &val)))
			return false;
	} else if (name.compare(CT_CONDITION_APP_USE_FREQUENCY) == 0) {
		if (!(item.get(CT_RULE_CONDITION_OPTION, CONTEXT_TRIGGER_APP_ID, &val_str)))
			return false;
	} else if (name.compare(CT_CONDITION_COMMUNICATION_FREQUENCY) == 0) {
		if (!(item.get(CT_RULE_CONDITION_OPTION, CONTEXT_TRIGGER_ADDRESS, &val_str)))
			return false;
	}

	return true;
}

// called by context_trigger_rule_entry_add_option_int()
bool ctx::rule_validator::check_option_int(std::string name, std::string key, int value)
{
	init();

	// Err: Item with no option
	if (template_map[name].array_get_size(NULL, KEY_OPTION) <= 0) {
		return false;
	}

	// Err: Invalid option key or Invalid value type
	bool ret = false;
	ctx::json opt_tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, KEY_OPTION, i, &opt_tempt); i++) {
		std::string t_key;
		int t_type;
		opt_tempt.get(NULL, KEY_KEY, &t_key);
		opt_tempt.get(NULL, KEY_TYPE, &t_type);
		if (t_key.compare(key) == 0 && t_type == TYPE_INT) {
			ret = true;
			break;
		}
	}
	IF_FAIL_RETURN(ret, false);

	// Err: Inappropriate value
	//   a. normal case
	ret = check_value_int(opt_tempt, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_option_string()
bool ctx::rule_validator::check_option_string(std::string name, std::string key, std::string value)
{
	init();

	// Err: Item with no option
	if (template_map[name].array_get_size(NULL, KEY_OPTION) <= 0) {
		return false;
	}

	// Err: Invalid option key or Invalid value type
	bool ret = false;
	ctx::json opt_tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, KEY_OPTION, i, &opt_tempt); i++) {
		std::string t_key;
		int t_type;
		opt_tempt.get(NULL, KEY_KEY, &t_key);
		opt_tempt.get(NULL, KEY_TYPE, &t_type);
		if (t_key.compare(key) == 0 && t_type == TYPE_STRING) {
			ret = true;
			break;
		}
	}
	IF_FAIL_RETURN(ret, false);

	// Err: Inappropriate value
	//   a. spacial case
	if ((name.compare(CT_CONDITION_APP_USE_FREQUENCY) == 0 || name.compare(CT_CONDITION_COMMUNICATION_FREQUENCY) == 0
			|| name.compare(CT_CONDITION_MUSIC_PLAYBACK_FREQUENCY) == 0 || name.compare(CT_CONDITION_VIDEO_PLAYBACK_FREQUENCY) == 0)
			&& key.compare(CONTEXT_TRIGGER_TIME_OF_DAY) == 0) {
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
	ret = check_value_string(opt_tempt, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_comparison_int()
bool ctx::rule_validator::check_comparison_int(std::string name, std::string key, std::string op, int value)
{
	init();

	// Err: Invalid attribute key or Invalid value type
	bool ret = false;
	ctx::json attr_tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, KEY_ATTR, i, &attr_tempt); i++) {
		std::string t_key;
		int t_type;
		attr_tempt.get(NULL, KEY_KEY, &t_key);
		attr_tempt.get(NULL, KEY_TYPE, &t_type);
		if (t_key.compare(key) == 0 && t_type == TYPE_INT) {
			ret = true;
			break;
		}
	}
	IF_FAIL_RETURN(ret, false);

	// Err: Invalid operator for the value

	// Err: Inappropriate value
	//    a. normal case
	ret = check_value_int(attr_tempt, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_comparison_string()
bool ctx::rule_validator::check_comparison_string(std::string name, std::string key, std::string op, std::string value)
{
	init();

	// Err: Invalid attribute key or Invalid value type
	bool ret = false;
	ctx::json attr_tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, KEY_ATTR, i, &attr_tempt); i++) {
		std::string t_key;
		int t_type;
		attr_tempt.get(NULL, KEY_KEY, &t_key);
		attr_tempt.get(NULL, KEY_TYPE, &t_type);
		if (t_key.compare(key) == 0 && t_type == TYPE_STRING) {
			ret = true;
			break;
		}
	}
	IF_FAIL_RETURN(ret, false);

	// Err: Invalid operator for the value

	// Err: Inappropriate value
	//    a. normal case
	ret = check_value_string(attr_tempt, value);
	IF_FAIL_RETURN(ret, false);

	return true;
}

// called by context_trigger_rule_entry_add_comparison_string()
bool ctx::rule_validator::check_valid_key(int type, std::string name, std::string key)
{
	init();
	std::string json_key = (type == TYPE_OPTION)? KEY_OPTION : KEY_ATTR;

	// Err: Invalid key
	bool ret = false;
	ctx::json tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, json_key.c_str(), i, &tempt); i++) {
		std::string t_key;
		tempt.get(NULL, KEY_KEY, &t_key);
		if (t_key.compare(key) == 0) {
			ret = true;
			break;
		}
	}
	IF_FAIL_RETURN(ret, false);

	return true;
}

bool check_value_int(ctx::json& tempt, int value)
{
	int min, max;
	tempt.get(NULL, KEY_MIN, &min);
	tempt.get(NULL, KEY_MAX, &max);

	if (min != NO_LIMIT && value < min)
		return false;

	if (max != NO_LIMIT && value > max)
		return false;

	return true;
}

bool check_value_string(ctx::json& tempt, std::string value)
{
	// case1: any value is accepted
	if (tempt.array_get_size(NULL, KEY_ACCEPTABLE) <= 0)
		return true;

	// case2: check acceptable value
	std::string t_val;
	for (int i = 0; tempt.get_array_elem(NULL, KEY_ACCEPTABLE, i, &t_val); i++) {
		if (t_val.compare(value) == 0)
			return true;
	}

	return false;
}

// called by context_trigger_rule_entry_add_comparison()
// called by context_trigger_rule_entry_add_option()
bool ctx::rule_validator::set_ref_info(int type, ctx::json* jref, std::string name, std::string key, std::string ref_data)
{
	init();

	std::string json_key = (type == TYPE_OPTION)? KEY_OPTION : KEY_ATTR;

	ctx::json tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, json_key.c_str(), i, &tempt); i++) {
		std::string k;
		int dt;
		tempt.get(NULL, KEY_KEY, &k);

		if (key.compare(k) == 0) {
			tempt.get(NULL, KEY_TYPE, &dt);

			ctx::json temp;
			temp.set(NULL, KEY_NAME, name);
			temp.set(NULL, KEY_KEY, key);
			temp.set(NULL, KEY_TYPE, dt);
			temp.set(NULL, KEY_REF, ref_data);

			if (type == TYPE_OPTION) {
				jref->array_append(NULL, KEY_OPTION, temp);
			} else if (type == TYPE_ATTR) {
				jref->array_append(NULL, KEY_ATTR, temp);
			} else {
				return false;
			}
			return true;
		}
	}
	return false;
}

// called by context_trigger_rule_entry_add_comparison()
int ctx::rule_validator::get_data_type(int type, std::string name, std::string key)
{
	init();

	std::string json_key = (type == TYPE_OPTION)? KEY_OPTION : KEY_ATTR;

	ctx::json tempt;
	for (int i = 0; template_map[name].get_array_elem(NULL, json_key.c_str(), i, &tempt); i++) {
		std::string k;
		tempt.get(NULL, KEY_KEY, &k);

		if (key.compare(k) == 0) {
			int dt;
			tempt.get(NULL, KEY_TYPE, &dt);
			return dt;
		}
	}

	return -1;
}

// called by context_trigger_rule_add_entry()
bool ctx::rule_validator::check_referential_data(std::string name, ctx::json& ref_info)
{
	std::map<std::string, int> type_map;

	ctx::json ref_data;
	for (int i = 0; ref_info.get_array_elem(NULL, KEY_OPTION, i, &ref_data); i++) {
		std::string ref_key;
		ref_data.get(NULL, KEY_REF, &ref_key);
		int cond_type;
		ref_data.get(NULL, KEY_TYPE, &cond_type);

		if (type_map.count(ref_key) == 0) {
			type_map[ref_key] = get_data_type(TYPE_ATTR, name, ref_key); // -1, if invalid key
		}

		// Err: Invalid key or Value type not matched
		if (cond_type != type_map[ref_key])
			return false;
	}

	for (int i = 0; ref_info.get_array_elem(NULL, KEY_ATTR, i, &ref_data); i++) {
		std::string ref_key;
		ref_data.get(NULL, KEY_REF, &ref_key);
		int cond_type;
		ref_data.get(NULL, KEY_TYPE, &cond_type);

		if (type_map.count(ref_key) == 0) {
			type_map[ref_key] = get_data_type(TYPE_ATTR, name, ref_key); // -1, if invalid key
		}

		// Err: Invalid key or Value type not matched
		if (cond_type != type_map[ref_key])
			return false;
	}

	return true;
}
