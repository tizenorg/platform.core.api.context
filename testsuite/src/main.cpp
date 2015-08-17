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
#include "trigger/condition.h"
#include "trigger/event.h"
#include "history/history.h"

static void signal_handler(int signo)
{
	if (signo == SIGINT && ctx::test::is_mainloop_running()) {
		g_print(PURPLE("\nskipping the testcase..."));
		ctx::test::stop_mainloop();
	} else {
		g_print(PURPLE("\nterminating... (SIGNAL %d)\n"), signo);
		exit(EXIT_SUCCESS);
	}
}

static void print_usage()
{
	g_print("Usage: context-test TESTSUITE\n\n");
	g_print("TESTSUITE can be one of the followings:\n");
	g_print("  h\t for testing all contextual history items\n");
	g_print("  c\t for testing all contextual trigger conditions\n");
	g_print("  e\t for testing all contextual trigger events\n");
}

int main(int argc, char** argv)
{
	signal(SIGINT,  signal_handler);
	signal(SIGHUP,  signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGABRT, signal_handler);

	if (argc < 2) {
		print_usage();
		return EXIT_SUCCESS;
	}

	switch (argv[1][0]) {
	case 'h':
		return test_history(&argc, &argv);
	case 'c':
		return test_trigger_condition(&argc, &argv);
	case 'e':
		return test_trigger_event(&argc, &argv);
	default:
		print_usage();
		return EXIT_SUCCESS;
	}
}
