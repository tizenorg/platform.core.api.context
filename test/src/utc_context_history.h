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

#ifndef __CONTEXT_CONTEXT_HISTORY_API_TEST_H__
#define __CONTEXT_CONTEXT_HISTORY_API_TEST_H__

void utc_context_history_create_p(void);
void utc_context_history_create_n(void);

void utc_context_history_destroy_p(void);
void utc_context_history_destroy_n(void);

void utc_context_history_filter_create_p(void);
void utc_context_history_filter_create_n(void);

void utc_context_history_filter_destroy_p(void);
void utc_context_history_filter_destroy_n(void);

void utc_context_history_filter_set_int_p(void);
void utc_context_history_filter_set_int_n(void);

void utc_context_history_filter_set_string_p(void);
void utc_context_history_filter_set_string_n(void);

void utc_context_history_get_list_p(void);
void utc_context_history_get_list_n(void);

void utc_context_history_list_get_count_p(void);
void utc_context_history_list_get_count_n(void);

void utc_context_history_list_get_current_p(void);
void utc_context_history_list_get_current_n(void);

void utc_context_history_list_move_first_p(void);
void utc_context_history_list_move_first_n(void);

void utc_context_history_list_move_next_p(void);
void utc_context_history_list_move_next_n(void);

void utc_context_history_list_destroy_p(void);
void utc_context_history_list_destroy_n(void);

void utc_context_history_record_create_p(void);
void utc_context_history_record_create_n(void);

void utc_context_history_record_get_int_p(void);
void utc_context_history_record_get_int_n(void);

void utc_context_history_record_get_string_p(void);
void utc_context_history_record_get_string_n(void);

void utc_context_history_record_set_int_p(void);
void utc_context_history_record_set_int_n(void);

void utc_context_history_record_set_string_p(void);
void utc_context_history_record_set_string_n(void);

void utc_context_history_record_destroy_p(void);
void utc_context_history_record_destroy_n(void);

void utc_context_history_record_insert_p(void);
void utc_context_history_record_insert_n(void);

#endif /* __CONTEXT_CONTEXT_HISTORY_API_TEST_H__ */
