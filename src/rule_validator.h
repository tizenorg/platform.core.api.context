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

#ifndef __CONTEXT_RULE_VALIDATOR_H__
#define __CONTEXT_RULE_VALIDATOR_H__

#include <json.h>

namespace ctx {
	namespace rule_validator {

		bool check_option(ctx::json &item);
		bool check_option_int(std::string name, std::string key, int value);
		bool check_option_string(std::string name, std::string key, std::string value);
		bool check_option_reference(std::string event, ctx::json &item);
		bool check_comparison_int(std::string name, std::string key, std::string op, int value);
		bool check_comparison_string(std::string name, std::string key, std::string op, std::string value);
		bool check_valid_key(int type, std::string name, std::string key);

		bool set_ref_info(int type, ctx::json *jref, std::string name, std::string key, std::string ref_key);
		std::string get_data_type(int type, std::string name, std::string key);
		bool check_referential_data(std::string name, ctx::json &ref_info);
		bool is_valid_operator(std::string type, std::string op);

	}
}	/* namespace ctx */

#endif	/* __CONTEXT_RULE_VALIDATOR_H__ */
