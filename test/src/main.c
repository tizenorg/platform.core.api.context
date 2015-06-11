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

#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>
#include <glib/gprintf.h>

#include "utc_context_trigger.h"
#include "utc_context_history.h"

#define RED(X) "\033[1;31m"X"\033[0m"

static void signal_handler(int signo)
{
	g_print(RED("\nSIGNAL %d received\n"), signo);
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
	signal(SIGINT,  signal_handler);
	signal(SIGHUP,  signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGABRT, signal_handler);

	g_print("\n");
	g_print("***********************************\n");
	g_print("* Context Service Unit Test Cases *\n");
	g_print("***********************************\n");
	g_print("\n");

#if !defined(GLIB_VERSION_2_36)
	g_type_init();
#endif
	g_test_init(&argc, &argv, NULL);

	g_test_add_func("/context_trigger/initialize", initialize);
	g_test_add_func("/context_trigger/utc_context_trigger_create_p", utc_context_trigger_create_p);
	g_test_add_func("/context_trigger/utc_context_trigger_create_n", utc_context_trigger_create_n);
	g_test_add_func("/context_trigger/utc_context_trigger_destroy_p", utc_context_trigger_destroy_p);
	g_test_add_func("/context_trigger/utc_context_trigger_destroy_n", utc_context_trigger_destroy_n);
	g_test_add_func("/context_trigger/utc_context_trigger_set_callback_p", utc_context_trigger_set_callback_p);
	g_test_add_func("/context_trigger/utc_context_trigger_set_callback_n", utc_context_trigger_set_callback_n);
	g_test_add_func("/context_trigger/utc_context_trigger_add_rule_p", utc_context_trigger_add_rule_p);
	g_test_add_func("/context_trigger/utc_context_trigger_add_rule_n", utc_context_trigger_add_rule_n);
	g_test_add_func("/context_trigger/utc_context_trigger_remove_rule_p", utc_context_trigger_remove_rule_p);
	g_test_add_func("/context_trigger/utc_context_trigger_remove_rule_n", utc_context_trigger_remove_rule_n);
	g_test_add_func("/context_trigger/utc_context_trigger_enable_rule_p", utc_context_trigger_enable_rule_p);
	g_test_add_func("/context_trigger/utc_context_trigger_enable_rule_n", utc_context_trigger_enable_rule_n);
	g_test_add_func("/context_trigger/utc_context_trigger_disable_rule_p", utc_context_trigger_disable_rule_p);
	g_test_add_func("/context_trigger/utc_context_trigger_disable_rule_n", utc_context_trigger_disable_rule_n);
	g_test_add_func("/context_trigger/utc_context_trigger_get_own_rule_ids_p", utc_context_trigger_get_own_rule_ids_p);
	g_test_add_func("/context_trigger/utc_context_trigger_get_own_rule_ids_n", utc_context_trigger_get_own_rule_ids_n);
	g_test_add_func("/context_trigger/utc_context_trigger_get_rule_by_id_p", utc_context_trigger_get_rule_by_id_p);
	g_test_add_func("/context_trigger/utc_context_trigger_get_rule_by_id_n", utc_context_trigger_get_rule_by_id_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_create_p", utc_context_trigger_rule_create_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_create_n", utc_context_trigger_rule_create_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_destroy_p", utc_context_trigger_rule_destroy_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_destroy_n", utc_context_trigger_rule_destroy_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_add_entry_p", utc_context_trigger_rule_add_entry_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_add_entry_n", utc_context_trigger_rule_add_entry_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_action_app_control_p", utc_context_trigger_rule_set_action_app_control_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_action_app_control_n", utc_context_trigger_rule_set_action_app_control_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_action_notification_p", utc_context_trigger_rule_set_action_notification_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_action_notification_n", utc_context_trigger_rule_set_action_notification_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_description_p", utc_context_trigger_rule_set_description_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_set_description_n", utc_context_trigger_rule_set_description_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_get_description_p", utc_context_trigger_rule_get_description_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_get_description_n", utc_context_trigger_rule_get_description_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_event_create_p", utc_context_trigger_rule_event_create_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_event_create_n", utc_context_trigger_rule_event_create_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_event_is_supported_p", utc_context_trigger_rule_event_is_supported_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_event_is_supported_n", utc_context_trigger_rule_event_is_supported_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_condition_create_p", utc_context_trigger_rule_condition_create_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_condition_create_n", utc_context_trigger_rule_condition_create_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_condition_is_supported_p", utc_context_trigger_rule_condition_is_supported_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_condition_is_supported_n", utc_context_trigger_rule_condition_is_supported_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_destroy_p", utc_context_trigger_rule_entry_destroy_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_destroy_n", utc_context_trigger_rule_entry_destroy_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_key_p", utc_context_trigger_rule_entry_add_key_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_key_n", utc_context_trigger_rule_entry_add_key_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_int_p", utc_context_trigger_rule_entry_add_comparison_int_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_int_n", utc_context_trigger_rule_entry_add_comparison_int_n);
//	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_double_p", utc_context_trigger_rule_entry_add_comparison_double_p);
//	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_double_n", utc_context_trigger_rule_entry_add_comparison_double_n);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_string_p", utc_context_trigger_rule_entry_add_comparison_string_p);
	g_test_add_func("/context_trigger/utc_context_trigger_rule_entry_add_comparison_string_n", utc_context_trigger_rule_entry_add_comparison_string_n);
	g_test_add_func("/context_trigger/release", release);

	g_test_add_func("/context_history/utc_context_history_create_p", utc_context_history_create_p);
	g_test_add_func("/context_history/utc_context_history_create_n", utc_context_history_create_n);
	g_test_add_func("/context_history/utc_context_history_destroy_p", utc_context_history_destroy_p);
	g_test_add_func("/context_history/utc_context_history_destroy_n", utc_context_history_destroy_n);
	g_test_add_func("/context_history/utc_context_history_filter_create_p", utc_context_history_filter_create_p);
	g_test_add_func("/context_history/utc_context_history_filter_create_n", utc_context_history_filter_create_n);
	g_test_add_func("/context_history/utc_context_history_filter_destroy_p", utc_context_history_filter_destroy_p);
	g_test_add_func("/context_history/utc_context_history_filter_destroy_n", utc_context_history_filter_destroy_n);
	g_test_add_func("/context_history/utc_context_history_filter_set_int_p", utc_context_history_filter_set_int_p);
	g_test_add_func("/context_history/utc_context_history_filter_set_int_n", utc_context_history_filter_set_int_n);
	g_test_add_func("/context_history/utc_context_history_filter_set_string_p", utc_context_history_filter_set_string_p);
	g_test_add_func("/context_history/utc_context_history_filter_set_string_n", utc_context_history_filter_set_string_n);
	g_test_add_func("/context_history/utc_context_history_get_list_p", utc_context_history_get_list_p);
	g_test_add_func("/context_history/utc_context_history_get_list_n", utc_context_history_get_list_n);
	g_test_add_func("/context_history/utc_context_history_list_get_count_p", utc_context_history_list_get_count_p);
	g_test_add_func("/context_history/utc_context_history_list_get_count_n", utc_context_history_list_get_count_n);
	g_test_add_func("/context_history/utc_context_history_list_get_current_p", utc_context_history_list_get_current_p);
	g_test_add_func("/context_history/utc_context_history_list_get_current_n", utc_context_history_list_get_current_n);
	g_test_add_func("/context_history/utc_context_history_list_move_first_p", utc_context_history_list_move_first_p);
	g_test_add_func("/context_history/utc_context_history_list_move_first_n", utc_context_history_list_move_first_n);
	g_test_add_func("/context_history/utc_context_history_list_move_next_p", utc_context_history_list_move_next_p);
	g_test_add_func("/context_history/utc_context_history_list_move_next_n", utc_context_history_list_move_next_n);
	g_test_add_func("/context_history/utc_context_history_list_destroy_p", utc_context_history_list_destroy_p);
	g_test_add_func("/context_history/utc_context_history_list_destroy_n", utc_context_history_list_destroy_n);
	g_test_add_func("/context_history/utc_context_history_record_create_p", utc_context_history_record_create_p);
	g_test_add_func("/context_history/utc_context_history_record_create_n", utc_context_history_record_create_n);
	g_test_add_func("/context_history/utc_context_history_record_get_int_p", utc_context_history_record_get_int_p);
	g_test_add_func("/context_history/utc_context_history_record_get_int_n", utc_context_history_record_get_int_n);
	g_test_add_func("/context_history/utc_context_history_record_get_string_p", utc_context_history_record_get_string_p);
	g_test_add_func("/context_history/utc_context_history_record_get_string_n", utc_context_history_record_get_string_n);
	g_test_add_func("/context_history/utc_context_history_record_set_int_p", utc_context_history_record_set_int_p);
	g_test_add_func("/context_history/utc_context_history_record_set_int_n", utc_context_history_record_set_int_n);
	g_test_add_func("/context_history/utc_context_history_record_set_string_p", utc_context_history_record_set_string_p);
	g_test_add_func("/context_history/utc_context_history_record_set_string_p", utc_context_history_record_set_string_n);
	g_test_add_func("/context_history/utc_context_history_record_destroy_p", utc_context_history_record_destroy_p);
	g_test_add_func("/context_history/utc_context_history_record_destroy_n", utc_context_history_record_destroy_n);
	g_test_add_func("/context_history/utc_context_history_record_insert_p", utc_context_history_record_insert_p);
	g_test_add_func("/context_history/utc_context_history_record_insert_n", utc_context_history_record_insert_n);

	return g_test_run();
}
