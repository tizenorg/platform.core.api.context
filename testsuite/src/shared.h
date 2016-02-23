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

#ifndef __CONTEXT_TEST_SHARED_H__
#define __CONTEXT_TEST_SHARED_H__

#include <cstdlib>
#include <iostream>
#include <string>
#include <glib.h>
#include <types_internal.h>
#include <Json.h>

#define CALLBACK_COUNT 5

#define ASSERT(condition) \
	do { \
		if (!(condition)) { \
			g_print("\n"); \
			if (!ctx::test::skip_tc) \
				g_print(RED("FAIL") ": %s(%d) > %s\n", __FUNCTION__, __LINE__, #condition); \
			return false; \
		} \
	} while (0)

#define ASSERT_CMPINT(left, comp, right) \
	do { \
		if (!((left) comp (right))) { \
			g_print("\n"); \
			if (!ctx::test::skip_tc) \
				g_print(RED("FAIL") ": %s(%d) > %s(%d) %s %s(%d)\n", __FUNCTION__, __LINE__, #left, (left), #comp, #right, (right)); \
			return false; \
		} \
	} while (0)

#define ASSERT_CONTAIN_STR(JSONOBJ, VKEY) \
	do { \
		ctx::test::bool_val = (JSONOBJ).get(NULL, (VKEY), &ctx::test::str_val); \
		if (!ctx::test::bool_val) { \
			g_print("\n"); \
			if (!ctx::test::skip_tc) \
				g_print(RED("FAIL") ": %s(%d) > Getting the string '%s'\n", __FUNCTION__, __LINE__, (VKEY)); \
			return false; \
		} \
		g_print("%s(%s) ", (VKEY), ctx::test::str_val.c_str()); \
	} while (0)

#define ASSERT_CONTAIN_INT(JSONOBJ, VKEY) \
	do { \
		ctx::test::bool_val = (JSONOBJ).get(NULL, (VKEY), &ctx::test::int_val); \
		if (!ctx::test::bool_val) { \
			g_print("\n"); \
			if (!ctx::test::skip_tc) \
				g_print(RED("FAIL") ": %s(%d) > Getting the integer '%s'\n", __FUNCTION__, __LINE__, (VKEY)); \
			return false; \
		} \
		g_print("%s(%d) ", (VKEY), ctx::test::int_val); \
	} while (0)

namespace ctx {
	namespace test {
		extern GMainLoop *mainloop;
		extern int callback_count;
		extern int err;
		extern int req_id;
		extern int int_val;
		extern bool bool_val;
		extern bool skip_tc;
		extern bool newline_after_tcname;
		extern ctx::Json json_val;
		extern std::string str_val;

		void start_mainloop();
		void stop_mainloop();
		bool is_mainloop_running();

		void cleanup_vars();
		bool get_input_int(const char *msg);
		bool get_input_str(const char *msg);

		void run_testcase(const char *tc_name, bool (*tc_func)(void));
	}
}

#endif
