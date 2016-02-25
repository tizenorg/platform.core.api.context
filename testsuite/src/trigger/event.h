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

#ifndef __CONTEXT_TEST_TRIGGER_EVENT_H__
#define __CONTEXT_TEST_TRIGGER_EVENT_H__

int test_trigger_event(int *argc, char ***argv);

bool _trigger_event_battery();
bool _trigger_event_charger();
bool _trigger_event_gps();
bool _trigger_event_headphone();
bool _trigger_event_usb();
bool _trigger_event_wifi();
bool _trigger_event_psmode();
bool _trigger_event_call();
bool _trigger_event_time();
bool _trigger_event_email();
bool _trigger_event_message();
bool _trigger_event_activity();
bool _trigger_event_place();

#endif
