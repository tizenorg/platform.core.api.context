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
 *
 * @subsection CAPI_CONTEXT_HISTORY_MODULE_Get To get contextual history data
 *The application can read derived contextual history data with <em>read</em> API provided by contextual history engine.
 *The data is mainly categorized to <i>Application Usage</i>, <i>Peak Time of Activity</i> and <i>Common Setting for Activity</i>. @n
 *To read desired data with contextual history API, you should set data type and filters to limit raw data set. Each data type has its corresponding possible filter types and result attributes. @n
 *Each data type also requires different types of privilege.
 *The following is the list of data type. An enumeration type is defined to #context_history_data_e.
 *
 *<ul>
 *	<li>#CONTEXT_HISTORY_RECENTLY_USED_APP @n
 *		Description : Returns recently used application list sorted by time in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/apphistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_WIFI_BSSID </td>
 *			<td> string </td>
 *			<td> Wi-Fi BSSID value to filter </td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_AUDIO_JACK </td>
 *			<td> integer </td>
 *			<td> Audio jack status #context_history_filter_audio_jack_e </td>
 *			<td> X </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Total used count </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_DURATION </td>
 *			<td> integer </td>
 *			<td> Total used time in second </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_TIME </td>
 *			<td> integer </td>
 *			<td> Last used time in epoch time </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_FREQUENTLY_USED_APP @n
 *		Description : Returns frequently used application list sorted by used count in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/apphistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_WIFI_BSSID </td>
 *			<td> string </td>
 *			<td> Wi-Fi BSSID value to filter </td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_AUDIO_JACK </td>
 *			<td> integer </td>
 *			<td> Audio jack status #context_history_filter_audio_jack_e </td>
 *			<td> X </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Total used count </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_DURATION </td>
 *			<td> integer </td>
 *			<td> Total used time in second </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_TIME </td>
 *			<td> integer </td>
 *			<td> Last used time in epoch time </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_RARELY_USED_APP @n
 *		Description : Returns rarely used application list sorted by time in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/apphistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Total used count </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_DURATION </td>
 *			<td> integer </td>
 *			<td> Total used time in second </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_TIME </td>
 *			<td> integer </td>
 *			<td> Last used time in epoch time </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_PEAK_TIME_FOR_APP @n
 *		Description : Returns peak time of application use list in hour sorted by used time in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/apphistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id to filter</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_DAY_OF_WEEK </td>
 *			<td> integer </td>
 *			<td> Weekdays or weekends #context_history_filter_day_of_week_e </td>
 *			<td> X (Default : All days) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_HOUR_OF_DAY </td>
 *			<td> integer </td>
 *			<td> Hour of day value(0~24) based on local time </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Event count </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_PEAK_TIME_FOR_MUSIC @n
 *		Description : Returns peak time of listening music list in hour sorted by used time in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> The player application's id to filter</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_DAY_OF_WEEK </td>
 *			<td> integer </td>
 *			<td> Weekdays or weekends #context_history_filter_day_of_week_e </td>
 *			<td> X (Default : All days) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_HOUR_OF_DAY </td>
 *			<td> integer </td>
 *			<td> Hour of day value(0~24) based on local time </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Event count </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_PEAK_TIME_FOR_VIDEO @n
 *		Description : Returns peak time of watching video list in hour sorted by used time in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> The player application's id to filter</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_DAY_OF_WEEK </td>
 *			<td> integer </td>
 *			<td> Weekdays or weekends #context_history_filter_day_of_week_e </td>
 *			<td> X (Default : All days) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_HOUR_OF_DAY </td>
 *			<td> integer </td>
 *			<td> Hour of day value(0~24) based on local time </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Event count </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_COMMON_SETTING_FOR_APP @n
 *		Description : Returns the most common setting value of application use. This returns always 1 row of record@n
 *		Required privilege : <em> http://tizen.org/privilege/apphistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id to filter</td>
 *			<td> X (Default : All applications) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_AUDIO_JACK </td>
 *			<td> integer </td>
 *			<td> The most common value of audio jack status </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_SYSTEM_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of system volume  </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_MEDIA_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of media volume </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_COMMON_SETTING_FOR_MUSIC @n
 *		Description : Returns the most common setting value of music playing. This returns always 1 row of record@n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id to filter</td>
 *			<td> X (Default : All applications) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_AUDIO_JACK </td>
 *			<td> integer </td>
 *			<td> The most common value of audio jack status </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_SYSTEM_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of system volume  </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_MEDIA_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of media volume </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_COMMON_SETTING_FOR_VIDEO @n
 *		Description : Returns the most common setting value of video watching. This returns always 1 row of record@n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id to filter</td>
  *			<td> X (Default : All applications) </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_AUDIO_JACK </td>
 *			<td> integer </td>
 *			<td> The most common value of audio jack status </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_SYSTEM_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of system volume  </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_MEDIA_VOLUME </td>
 *			<td> integer </td>
 *			<td> The most common value of media volume </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_FREQUENTLY_COMMUNICATED_ADDRESS @n
 *		Description : Returns frequently communicated address list sorted by communicated count in descending order. @n
 *		Required privilege : <em> http://tizen.org/privilege/callhistory.read </em> @n
 *		<table>
 *		<caption> Table: Filters</caption>
 *		<tr>
 *			<th> Filter </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *			<th> Mandatory </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_TIME_SPAN </td>
 *			<td> integer </td>
 *			<td> Time span of data in days from today ( 1 ~ 90 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_RESULT_SIZE </td>
 *			<td> integer </td>
 *			<td> Result size of data set ( 1 ~ 50 ) </td>
 *			<td> O </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_START_TIME </td>
 *			<td> integer </td>
 *			<td> Start time in epoch time to load only data which is inserted after the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_END_TIME </td>
 *			<td> integer </td>
 *			<td> End time in epoch time to load only data which is inserted before the specific time</td>
 *			<td> X </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_FILTER_COMMUNICATION_TYPE </td>
 *			<td> integer </td>
 *			<td> Call or message logs #context_history_filter_communication_type_e </td>
 *			<td> X </td>
 *		</tr>
 *		</table>
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_ADDRESS </td>
 *			<td> string </td>
 *			<td> Address </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_COUNT </td>
 *			<td> integer </td>
 *			<td> Total communication count </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_TOTAL_DURATION </td>
 *			<td> integer </td>
 *			<td> Total communication duration </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_TIME </td>
 *			<td> integer </td>
 *			<td> Last communication time in epoch time </td>
 *		</tr>
 *		</table> @n
 *</ul>
 *
 *Sample code: @n
 *the code below creates a handle and reads the data from contextual history engine.

 \code
// create a handle
context_history_h handle = NULL;
context_history_create(&handle);

// create a filter
context_history_filter_h filter;
context_history_filter_create(&filter);

// set filter values
context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_TIME_SPAN, 30);
context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_RESULT_SIZE, 5);
context_history_filter_set_string(filter, CONTEXT_HISTORY_FILTER_APP_ID, "org.tizen.music");

// set start and end time to query only data from 6:00 to 12:00

i18n_ucalendar_h ucal = NULL;
char *loc_default = NULL;
i18n_udate time;
int start_time_in_sec, end_time_in_sec;

i18n_ulocale_get_default((const char **)&loc_default);
i18n_ucalendar_create(NULL, -1, loc_default, I18N_UCALENDAR_GREGORIAN, &ucal);

i18n_ucalendar_set(ucal, I18N_UCALENDAR_HOUR_OF_DAY, 6);
i18n_ucalendar_set(ucal, I18N_UCALENDAR_MINUTE, 0);
i18n_ucalendar_set(ucal, I18N_UCALENDAR_SECOND, 0);
i18n_ucalendar_set(ucal, I18N_UCALENDAR_MILLISECOND, 0);
i18n_ucalendar_get_milliseconds(ucal, &time);
start_time_in_sec = time/1000;

i18n_ucalendar_add(ucal, I18N_UCALENDAR_HOUR_OF_DAY, 6);
i18n_ucalendar_get_milliseconds(ucal, &time);
end_time_in_sec = time/1000;

i18n_ucalendar_destroy(ucal);

context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_START_TIME, start_time_in_sec);
context_history_filter_set_int(filter, CONTEXT_HISTORY_FILTER_END_TIME, end_time_in_sec);

context_history_list_h list = NULL;

// read data from contextual history engine
context_history_get_list(handle, CONTEXT_HISTORY_COMMON_SETTING_FOR_APP, filter, &list);

int size = 0;
int media_volume;
int audio_jack;
context_history_list_get_count(list, &size);

context_history_record_h record = NULL;

// get current record
context_history_list_get_current(list, &record);

// get attributes
context_history_record_get_int(record, CONTEXT_HISTORY_MEDIA_VOLUME, &media_volume);
context_history_record_get_int(record, CONTEXT_HISTORY_AUDIO_JACK, &audio_jack);

context_history_record_destroy(record);

// to next position of the list
context_history_list_move_next(list);

// ...

// destroy
context_history_list_destroy(list);
context_history_destroy(handle);
context_history_filter_destroy(filter);
 \endcode

 * @internal
 * <h2>To insert contextual event data to contextual history database</h2>
 * The application can insert contextual event data into the contextual history database when a meaningful user event is generated.
 * An <i>event data</i> usually contains a set of information regarding user's activities.
 * It is used by contextual history engine to analyze user's behavioral patterns or characteristics. Each event type has different attributes and requires different privileges.@n
 * The following is the list of event type can be used. An enumeration type is defined to #context_history_event_e.
 *
 *<ul>
 *	<li>#CONTEXT_HISTORY_START_MUSIC @n
 *		Description : The user started to listen to music. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.admin </em> @n
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_URI </td>
 *			<td> string </td>
 *			<td> Uri of media file </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id that inserted event data </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_POSITION </td>
 *			<td> integer </td>
 *			<td> The last played position of media file </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_STOP_MUSIC @n
 *		Description : The user stopped to listen to music. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.admin </em> @n
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_URI </td>
 *			<td> string </td>
 *			<td> Uri of media file </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id that inserted event data </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_POSITION </td>
 *			<td> integer </td>
 *			<td> The last played position of media file </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_START_VIDEO @n
 *		Description : The user started to watch a video. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.admin </em> @n
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_URI </td>
 *			<td> string </td>
 *			<td> Uri of media file </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id that inserted event data </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_POSITION </td>
 *			<td> integer </td>
 *			<td> The last played position of media file </td>
 *		</tr>
 *		</table> @n
 *	<li>#CONTEXT_HISTORY_STOP_VIDEO @n
 *		Description : The user stopped to watch the video. @n
 *		Required privilege : <em> http://tizen.org/privilege/mediahistory.admin </em> @n
 *		<table>
 *		<caption> Table: Attributes</caption>
 *		<tr>
 *			<th> Attribute </th>
 *			<th> Type </th>
 *			<th> Value </th>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_URI </td>
 *			<td> string </td>
 *			<td> Uri of media file </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_APP_ID </td>
 *			<td> string </td>
 *			<td> Application id that inserted event data </td>
 *		</tr>
 *		<tr>
 *			<td> #CONTEXT_HISTORY_LAST_POSITION </td>
 *			<td> integer </td>
 *			<td> The last played position of media file </td>
 *		</tr>
 *		</table> @n
 *</ul>
 *
 *Sample code: @n
 *the code below creates a record and inserts it into contextual history engine.


 \code
// create a record
context_history_record_h record = NULL;
context_history_record_create(&record);

// set attributes to the record
context_history_record_set_string(record, "Uri", media_info->uri);
context_history_record_set_int(record, "LastPosition", media_info->last_position);

// insert record to contextual history
context_history_record_insert(record, CONTEXT_HISTORY_START_MUSIC);

// destroy
context_history_record_destroy(record);
 \endcode

 */
