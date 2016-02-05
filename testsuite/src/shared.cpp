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

#include "shared.h"

static bool __is_mainloop_running = false;

namespace ctx {
	namespace test {
		GMainLoop *mainloop = NULL;
		int callback_count = 0;
		int err = 0;
		int req_id = 0;
		int int_val = 0;
		bool bool_val = false;
		bool skip_tc = false;
		bool newline_after_tcname = false;
		ctx::json json_val = EMPTY_JSON_OBJECT;
		std::string str_val = "";
	}
}

void ctx::test::start_mainloop()
{
	if (!__is_mainloop_running) {
		__is_mainloop_running = true;
		mainloop = g_main_loop_new(NULL, FALSE);
		g_main_loop_run(mainloop);
	}
}

void ctx::test::stop_mainloop()
{
	if (__is_mainloop_running) {
		g_main_loop_quit(mainloop);
		g_main_loop_unref(mainloop);
		ctx::test::skip_tc = true;
		__is_mainloop_running = false;
	}
}

bool ctx::test::is_mainloop_running()
{
	return __is_mainloop_running;
}

void ctx::test::cleanup_vars()
{
	ctx::test::callback_count = 5;
	ctx::test::err = -1;
	ctx::test::req_id = -1;
	ctx::test::json_val = EMPTY_JSON_OBJECT;
	ctx::test::bool_val = false;
	ctx::test::int_val = 0;
	ctx::test::str_val = "";
	ctx::test::skip_tc = false;
}

bool ctx::test::get_input_int(const char *msg)
{
	std::cout << msg;
	std::getline(std::cin, str_val);
	IF_FAIL_RETURN(!str_val.empty(), false);
	int_val = atoi(str_val.c_str());
	return true;
}

bool ctx::test::get_input_str(const char *msg)
{
	std::cout << msg;
	std::getline(std::cin, str_val);
	IF_FAIL_RETURN(!str_val.empty(), false);
	return true;
}

void ctx::test::run_testcase(const char *tc_name, bool (*tc_func)(void))
{
	IF_FAIL_VOID(tc_name && tc_func);

	g_print(CYAN("%s") ": ", tc_name);
	if (newline_after_tcname)
		g_print("\n");

	cleanup_vars();
	bool result = tc_func();
	if (result)
		g_print(GREEN("PASS") "\n");
}
