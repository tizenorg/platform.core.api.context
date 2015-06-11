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

/*
 * This API set is only for testing purposes.
 * Do not use any of this set for other purposes.
 */

#ifndef __TIZEN_CONTEXT_INTERNAL_TEST_API_H__
#define __TIZEN_CONTEXT_INTERNAL_TEST_API_H__

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void(* context_internal_subscribe_cb)(const char* item, int req_id, int error, const char* data, void *user_data);

int context_internal_read(const char* item, const char* option, char** data);

int context_internal_write(const char* item, const char* data, char** result);

int context_internal_subscribe(const char* item, const char* option, context_internal_subscribe_cb callback, void *user_data);

int context_internal_unsubscribe(int req_id);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
