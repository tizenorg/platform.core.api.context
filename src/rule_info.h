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
			\"name\": \"device/alarm\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": \"integer\", \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"device/time\",\
			\"attributes\": [ { \"key\": \"TimeOfDay\", \"type\": \"integer\", \"min\": 0, \"max\": 1439}, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] }, { \"key\" : \"DayOfMonth\", \"type\": \"integer\", \"min\": 1, \"max\": 31} ]\
		},\
		{\
			\"name\": \"system/battery\",\
			\"attributes\": [ { \"key\" : \"Level\", \"type\": \"string\", \"values\": [ \"Empty\", \"Critical\", \"Low\", \"Normal\", \"High\", \"Full\" ] }, { \"key\": \"IsCharging\", \"type\": \"integer\", \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/charger\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": \"integer\", \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/gps\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": \"string\", \"values\": [ \"Disabled\", \"Searching\", \"Connected\" ] } ]\
		},\
		{\
			\"name\": \"system/headphone\",\
			\"attributes\": [{ \"key\": \"IsConnected\", \"type\": \"integer\", \"min\": 0, \"max\": 1 }, { \"key\": \"Type\", \"type\": \"string\", \"values\": [ \"Normal\", \"Headset\", \"Bluetooth\" ] } ]\
		},\
		{\
			\"name\": \"system/psmode\",\
			\"attributes\": [ { \"key\": \"IsEnabled\", \"type\": \"integer\", \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/usb\",\
			\"attributes\": [ { \"key\": \"IsConnected\", \"type\": \"integer\", \"min\": 0, \"max\": 1 } ]\
		},\
		{\
			\"name\": \"system/wifi\",\
			\"attributes\": [ { \"key\" : \"State\", \"type\": \"string\", \"values\": [ \"Disabled\", \"Unconnected\", \"Connected\" ] }, { \"key\": \"BSSID\", \"type\": \"string\", \"values\": [ ] } ]\
		},\
		{\
			\"name\": \"social/call\",\
			\"attributes\": [ { \"key\" : \"Medium\", \"type\": \"string\", \"values\": [ \"Voice\", \"Video\" ] }, { \"key\" : \"State\", \"type\": \"string\", \"values\": [ \"Idle\", \"Connecting\", \"Connected\" ] }, { \"key\": \"Address\", \"type\": \"string\", \"values\": [ ] } ]\
		},\
		{\
			\"name\": \"social/email\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Received\", \"Sent\" ] } ]\
		},\
		{\
			\"name\": \"social/message\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Received\" ] } , { \"key\": \"Type\", \"type\": \"string\", \"values\": [ \"SMS\", \"MMS\" ] }, { \"key\": \"Address\", \"type\": \"string\", \"values\": [ ] } ]\
		},\
		{\
			\"name\": \"activity/stationary\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": \"string\", \"values\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/walking\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": \"string\", \"values\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/running\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": \"string\", \"values\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"activity/in_vehicle\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"Detected\" ] }, { \"key\": \"Accuracy\", \"type\": \"string\", \"values\": [ \"High\", \"Normal\", \"Low\" ] } ]\
		},\
		{\
			\"name\": \"place/geofence\",\
			\"attributes\": [ { \"key\": \"Event\", \"type\": \"string\", \"values\": [ \"In\", \"Out\" ] } ],\
			\"option\": [ { \"key\": \"PlaceId\", \"type\": \"integer\", \"min\": 1, \"max\": -1 } ]\
		},\
		{\
			\"name\": \"stats/app/frequency\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": \"integer\", \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": \"integer\", \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"AppId\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/contact/frequency\",\
			\"attributes\": [ { \"key\": \"Rank\", \"type\": \"integer\", \"min\": 1, \"max\": -1 }, { \"key\": \"TotalCount\", \"type\": \"integer\", \"min\": 0, \"max\": -1 } ],\
			\"option\": [  { \"key\": \"Address\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"TimeOfDay\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/music/frequency\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": \"integer\", \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		},\
		{\
			\"name\": \"stats/video/frequency\",\
			\"attributes\": [ { \"key\": \"TotalCount\", \"type\": \"integer\", \"min\": 0, \"max\": -1 } ],\
			\"option\": [ { \"key\": \"TimeOfDay\", \"type\": \"string\", \"values\": [ ] }, { \"key\": \"DayOfWeek\", \"type\": \"string\", \"values\": [ \"Mon\", \"Tue\", \"Wed\", \"Thu\", \"Fri\", \"Sat\", \"Sun\", \"Weekday\", \"Weekend\" ] } ]\
		}\
	]\
}"


namespace ctx {
}	/* namespace ctx */

#endif	/* __CONTEXT_RULE_INFO_H__ */
