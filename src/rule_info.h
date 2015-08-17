/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the \"License\");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an \"AS IS\" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CONTEXT_RULE_INFO_H__
#define __CONTEXT_RULE_INFO_H__

#define RULE_INFO "{ \"templates\": [\
		{\
			\"name\": \"time/alarm\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": 1, \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"time/now\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": 1, \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] }, { \"key\" : \"DayOfMonth\", \"type\": 1, \"min\": 1, \"max\": 31} ]\
		},\
		{\
			\"name\": \"system/battery\",\
			\"attributes\": [ { \"key\" : \"Level\", \"type\": 2, \"acceptable\": [ \"Empty\", \"Critical\", \"Low\", \"Normal\", \"High\", \"Full\" ] }, { \"key\": \"IsCharging\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/charger\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/gps\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Disabled\", \"Searching\", \"Connected\" ] } ]\
		},\
		{\
			\"name\": \"system/headphone\",\
			\"attributes\": [{ \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 }, { \"key\": \"Type\", \"type\": 2, \"acceptable\": [ \"Normal\", \"Headset\", \"Bluetooth\" ] } ]\
		},\
		{\
			\"name\": \"system/psmode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/usb\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/wifi\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Disabled\", \"Unconnected\", \"Connected\" ] }, { \"key\": \"BSSID\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"social/call\",\
			\"attributes\": [ { \"key\" : \"Medium\", \"type\": 2, \"acceptable\": [ \"Voice\", \"Video\" ] }, { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Idle\", \"Connecting\", \"Connected\" ] }, { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"social/email\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Received\", \"Sent\" ] } ]\
		},\
		{\
			\"name\": \"social/message\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Received\" ] } , { \"key\": \"Type\", \"type\": 2, \"acceptable\": [ \"SMS\", \"MMS\" ] }, { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"activity/stationary\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/walking\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/running\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/in_vehicle\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"place/geofence\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"In\", \"Out\" ] } ],\
			\"option\": [ { \"key\": \"PlaceId\", \"type\": 1, \"min\": 1, \"max\": -1 } ]\
		},\
		{\
			\"name\": \"stats/app/frequency\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": 1, \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"AppId\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/contact/frequency\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": 1, \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/music/frequency\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/video/frequency\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		}\
	]\
}"


namespace ctx {
}	/* namespace ctx */

#endif	/* __CONTEXT_RULE_INFO_H__ */
