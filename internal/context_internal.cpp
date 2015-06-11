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

#include <string>
#include <map>
#include <json.h>
#include <types_internal.h>
#include <request_handler.h>
#include <context_internal.h>

typedef std::map<int, context_internal_subscribe_cb> callback_map_t;
static callback_map_t callback_map;

static void internal_domain_cb(const char* item, int req_id, int error, ctx::json data)
{
	callback_map_t::iterator cb = callback_map.find(req_id);
	IF_FAIL_VOID_TAG(cb != callback_map.end(), _W, "No callback found");

	// user_data pointer is not supported
	cb->second(item, req_id, error, data.str().c_str(), NULL);
}

EXTAPI int context_internal_read(const char* item, const char* option, char** data)
{
	int req_id;
	ctx::json jdata;
	ctx::json joption = option;

	int error = ctx::request_handler::read_sync(item, &joption, &req_id, &jdata);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "read_sync error");

	*data = jdata.dup_cstr();

	return ERR_NONE;
}

EXTAPI int context_internal_write(const char* item, const char* data, char** result)
{
	ctx::json jdata = data;
	ctx::json jresult;

	int error = ctx::request_handler::write_with_reply(item, &jdata, &jresult);
	IF_FAIL_RETURN_TAG(error == ERR_NONE, error, _E, "write_with_reply error");

	*result = jresult.dup_cstr();

	return ERR_NONE;
}

EXTAPI int context_internal_subscribe(const char* item, const char* option, context_internal_subscribe_cb callback, void *user_data)
{
	ctx::request_handler::register_callback(item, internal_domain_cb);

	int req_id;
	ctx::json jresult;
	ctx::json joption = option;

	int error = ctx::request_handler::subscribe(item, &joption, &req_id, &jresult);
	IF_FAIL_RETURN(error == ERR_NONE, error);

	_J("Request Result:", jresult);

	callback_map[req_id] = callback;
	return req_id;
}

EXTAPI int context_internal_unsubscribe(int req_id)
{
	callback_map.erase(req_id);
	return ctx::request_handler::unsubscribe(req_id);
}
