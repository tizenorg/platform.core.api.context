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

/**
 * @ingroup		CAPI_CONTEXT_FRAMEWORK
 * @defgroup	CAPI_CONTEXT_HISTORY_MODULE Contextual History
 *
 * @brief		The contextual history API provides functions for reading contextual history data.@n
 *				This API allows you to query statistics and patterns derived from contextual history data.
 *
 * @section		CAPI_CONTEXT_HISTORY_MODULE_HEADER Required Header
 *				\#include <context_history.h>
 *
 * @section		CAPI_CONTEXT_HISTORY_MODULE_OVERVIEW Overview
 *
 * An application can retrieve the user's device usages statistics and patterns via the contextual history API.
 * This API mainly provides three categories of statistical usage patterns, including
 * <i>application usage</i>, <i>peak time of activities</i>, and <i>common setting for activities</i>.
 * See #context_history_data_e for all available statistic items.
 * After choosing a statistic item to retrieve, a @em filter can be set to narrow down to the data you need.@n
 * For more details about the available filter keys, and the data attributes provided by each item,
 * please refer the programming guide and the tutorial.
 *
 */
