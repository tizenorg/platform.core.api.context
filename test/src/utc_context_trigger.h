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

#ifndef __CONTEXT_CONTEXT_TRIGGER_API_TEST_H__
#define __CONTEXT_CONTEXT_TRIGGER_API_TEST_H__

void initialize(void);
void release(void);

void utc_context_trigger_create_p(void);
void utc_context_trigger_create_n(void);

void utc_context_trigger_destroy_p(void);
void utc_context_trigger_destroy_n(void);

void utc_context_trigger_set_callback_p(void);
void utc_context_trigger_set_callback_n(void);

void utc_context_trigger_add_rule_p(void);
void utc_context_trigger_add_rule_n(void);

void utc_context_trigger_remove_rule_p(void);
void utc_context_trigger_remove_rule_n(void);

void utc_context_trigger_enable_rule_p(void);
void utc_context_trigger_enable_rule_n(void);

void utc_context_trigger_disable_rule_p(void);
void utc_context_trigger_disable_rule_n(void);

void utc_context_trigger_get_own_rule_ids_p(void);
void utc_context_trigger_get_own_rule_ids_n(void);

void utc_context_trigger_get_rule_by_id_p(void);
void utc_context_trigger_get_rule_by_id_n(void);

void utc_context_trigger_rule_create_p(void);
void utc_context_trigger_rule_create_n(void);

void utc_context_trigger_rule_destroy_p(void);
void utc_context_trigger_rule_destroy_n(void);

void utc_context_trigger_rule_add_entry_p(void);
void utc_context_trigger_rule_add_entry_n(void);

void utc_context_trigger_rule_set_action_app_control_p(void);
void utc_context_trigger_rule_set_action_app_control_n(void);

void utc_context_trigger_rule_set_action_notification_p(void);
void utc_context_trigger_rule_set_action_notification_n(void);

void utc_context_trigger_rule_set_description_p(void);
void utc_context_trigger_rule_set_description_n(void);

void utc_context_trigger_rule_get_description_p(void);
void utc_context_trigger_rule_get_description_n(void);

void utc_context_trigger_rule_event_create_p(void);
void utc_context_trigger_rule_event_create_n(void);

void utc_context_trigger_rule_event_is_supported_p(void);
void utc_context_trigger_rule_event_is_supported_n(void);

void utc_context_trigger_rule_condition_create_p(void);
void utc_context_trigger_rule_condition_create_n(void);

void utc_context_trigger_rule_condition_is_supported_p(void);
void utc_context_trigger_rule_condition_is_supported_n(void);

void utc_context_trigger_rule_entry_destroy_p(void);
void utc_context_trigger_rule_entry_destroy_n(void);

void utc_context_trigger_rule_entry_add_key_p(void);
void utc_context_trigger_rule_entry_add_key_n(void);

void utc_context_trigger_rule_entry_add_comparison_int_p(void);
void utc_context_trigger_rule_entry_add_comparison_int_n(void);
/*
void utc_context_trigger_rule_entry_add_comparison_double_p(void);
void utc_context_trigger_rule_entry_add_comparison_double_n(void);
*/
void utc_context_trigger_rule_entry_add_comparison_string_p(void);
void utc_context_trigger_rule_entry_add_comparison_string_n(void);

#endif /* __CONTEXT_CONTEXT_TRIGGER_API_TEST_H__ */
