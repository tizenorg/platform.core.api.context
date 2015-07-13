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
			\"name\": \"timer/event\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": 1, \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"timer/state\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": 1, \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] }, { \"key\" : \"DayOfMonth\", \"type\": 1, \"min\": 1, \"max\": 31} ]\
		},\
		{\
			\"name\": \"system/state/battery\",\
			\"attributes\": [ { \"key\" : \"Level\", \"type\": 2, \"acceptable\": [ \"Empty\", \"Critical\", \"Low\", \"Normal\", \"High\", \"Full\" ] }, { \"key\": \"IsCharging\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/charger\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/flight_mode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/gps\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Disabled\", \"Searching\", \"Connected\" ] } ]\
		},\
		{\
			\"name\": \"system/state/headphone\",\
			\"attributes\": [{ \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 }, { \"key\": \"Type\", \"type\": 2, \"acceptable\": [ \"Normal\", \"Headset\", \"Bluetooth\" ] } ]\
		},\
		{\
			\"name\": \"system/state/ps_mode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/silent_mode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/vibration_mode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/usb\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": 1, \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/state/wifi\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Disabled\", \"Unconnected\", \"Connected\" ] }, { \"key\": \"BSSID\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"social/state/call\",\
			\"attributes\": [ { \"key\" : \"Medium\", \"type\": 2, \"acceptable\": [ \"Voice\", \"Video\" ] }, { \"key\" : \"State\", \"type\": 2, \"acceptable\": [ \"Idle\", \"Connecting\", \"Connected\" ] }, { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"social/event/email\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Received\", \"Sent\" ] } ]\
		},\
		{\
			\"name\": \"social/event/message\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Received\" ] } , { \"key\": \"Type\", \"type\": 2, \"acceptable\": [ \"SMS\", \"MMS\" ] }, { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] } ]\
		},\
		{\
			\"name\": \"activity/event/stationary\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/event/walking\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/event/running\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/event/in_vehicle\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": 2, \"acceptable\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"place/event/geofence\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": 2, \"acceptable\": [ \"In\", \"Out\" ] } ],\
			\"option\": [ { \"key\": \"PlaceId\", \"type\": 1, \"min\": 1, \"max\": -1 } ]\
		},\
		{\
			\"name\": \"app/history/use_freq\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": 1, \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"AppId\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"contact/history/comm_freq\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": 1, \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"Address\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"music/history/play_freq\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"video/history/play_freq\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": 1, \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": 2, \"acceptable\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": 2, \"acceptable\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		}\
	]\
}"


namespace ctx {
}	/* namespace ctx */

#endif	/* __CONTEXT_RULE_INFO_H__ */
