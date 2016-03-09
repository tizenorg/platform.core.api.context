/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#ifndef __CONTEXT_LIB_REQUEST_HANDLER_H__
#define __CONTEXT_LIB_REQUEST_HANDLER_H__

#include <Json.h>

namespace ctx { namespace request_handler {

	/*
	 */
	typedef void(* subject_response_cb)(const char* subject, int req_id, int error, ctx::Json response);

	/*
	 */
	bool register_callback(const char* subject, subject_response_cb callback);

	/*
	 */
	int subscribe(const char* subject, ctx::Json* option, int* req_id, ctx::Json* request_result = NULL);

	/*
	 */
	int unsubscribe(const char* subject, int req_id);

	/*
	 */
	int read(const char* subject, ctx::Json* option, int* req_id, ctx::Json* request_result = NULL);

	/*
	 */
	int read_sync(const char* subject, ctx::Json* option, int* req_id, ctx::Json* data_read);

	/*
	 */
	int write(const char* subject, ctx::Json* data);

	/*
	 */
	int write_with_reply(const char* subject, ctx::Json* data, ctx::Json* request_result = NULL);

	/*
	 */
	int is_supported(const char* subject);

} }	/* namespace ctx::request_handler */

#endif // __CONTEXT_LIB_REQUEST_HANDLER_H__
