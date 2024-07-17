#include "settings.h"

#include <string.h>

const char *TAG = "KD-Indicator";
const char* settings_txt = "# Configuration File for LED/Button Settings\n\n# Timezone setting (e.g., \"America/New_York\", \"UTC\")\ntimezone=America/Los_Angeles\n\n# Wi-Fi network SSID (Network name)\nssid=your_network_name\n\n# Wi-Fi password\npassword=your_network_password\n\n# LED/Button Pair 1 Settings\n# Function mode options:\n# - manual: LED/button operates manually\n# - offset: LED/button operates with an offset in seconds\n# - set_time: LED/button operates at a set time every x days\n# - set_days: LED/button operates at a set time each day of the week selected\n# - fixed_interval: LED/button operates at fixed intervals since startup in seconds\nled1_function_mode=manual\n\n# Used only if function mode is 'offset'\nled1_offset_seconds=60\n\n# Used only if function mode is 'set_time'\nled1_set_time_days=1\n\n# Used only if function mode is 'set_days'\nled1_set_days=monday,wednesday,friday\n\n# Used by set_time and set_days\n# 24 hour time format\nled1_set_time=12:00\n\n# Used only if function mode is 'fixed_interval'\nled1_fixed_interval_seconds=360\n\n# Display mode options:\n# - solid: LED is solid on\n# - blink: LED blinks at a specified rate\n# - fade: LED fades in and out at a specified rate\nled1_display_mode=solid\n\n# Used only if display mode is 'blink' (in milliseconds)\nled1_blink_rate=500\n\n# Used only if display mode is 'fade' (in milliseconds)\nled1_fade_rate=10\n\n# Brightness setting (0-100%)\nled1_brightness=100\n\n# Repeat the above block for each LED/Button pair (5 total)\n\n# LED/Button Pair 2 Settings\nled2_function_mode=manual\nled2_offset_seconds=60\nled2_set_time_days=1\nled2_set_days=monday,wednesday,friday\nled2_set_time=12:00\nled2_fixed_interval_seconds=360\nled2_display_mode=solid\nled2_blink_rate=500\nled2_fade_rate=10\nled2_brightness=100\n\n# LED/Button Pair 3 Settings\nled3_function_mode=manual\nled3_offset_seconds=60\nled3_set_time_days=1\nled3_set_days=monday,wednesday,friday\nled3_set_time=12:00\nled3_fixed_interval_seconds=360\nled3_display_mode=solid\nled3_blink_rate=500\nled3_fade_rate=10\nled3_brightness=100\n\n# LED/Button Pair 4 Settings\nled4_function_mode=manual\nled4_offset_seconds=60\nled4_set_time_days=1\nled4_set_days=monday,wednesday,friday\nled4_set_time=12:00\nled4_fixed_interval_seconds=360\nled4_display_mode=solid\nled4_blink_rate=500\nled4_fade_rate=10\nled4_brightness=100\n\n# LED/Button Pair 5 Settings\nled5_function_mode=manual\nled5_offset_seconds=60\nled5_set_time_days=1\nled5_set_days=monday,wednesday,friday\nled5_set_time=12:00\nled5_fixed_interval_seconds=360\nled5_display_mode=solid\nled5_blink_rate=500\nled5_fade_rate=10\nled5_brightness=100\n\n\n\n\n#####################\n## TimeZone Options##\n#####################\n# Africa/Abidjan\n# Africa/Accra\n# Africa/Addis_Ababa\n# Africa/Algiers\n# Africa/Asmara\n# Africa/Bamako\n# Africa/Bangui\n# Africa/Banjul\n# Africa/Bissau\n# Africa/Blantyre\n# Africa/Brazzaville\n# Africa/Bujumbura\n# Africa/Cairo\n# Africa/Casablanca\n# Africa/Ceuta\n# Africa/Conakry\n# Africa/Dakar\n# Africa/Dar_es_Salaam\n# Africa/Djibouti\n# Africa/Douala\n# Africa/El_Aaiun\n# Africa/Freetown\n# Africa/Gaborone\n# Africa/Harare\n# Africa/Johannesburg\n# Africa/Juba\n# Africa/Kampala\n# Africa/Khartoum\n# Africa/Kigali\n# Africa/Kinshasa\n# Africa/Lagos\n# Africa/Libreville\n# Africa/Lome\n# Africa/Luanda\n# Africa/Lubumbashi\n# Africa/Lusaka\n# Africa/Malabo\n# Africa/Maputo\n# Africa/Maseru\n# Africa/Mbabane\n# Africa/Mogadishu\n# Africa/Monrovia\n# Africa/Nairobi\n# Africa/Ndjamena\n# Africa/Niamey\n# Africa/Nouakchott\n# Africa/Ouagadougou\n# Africa/Porto-Novo\n# Africa/Sao_Tome\n# Africa/Tripoli\n# Africa/Tunis\n# Africa/Windhoek\n# America/Adak\n# America/Anchorage\n# America/Anguilla\n# America/Antigua\n# America/Araguaina\n# America/Argentina/Buenos_Aires\n# America/Argentina/Catamarca\n# America/Argentina/Cordoba\n# America/Argentina/Jujuy\n# America/Argentina/La_Rioja\n# America/Argentina/Mendoza\n# America/Argentina/Rio_Gallegos\n# America/Argentina/Salta\n# America/Argentina/San_Juan\n# America/Argentina/San_Luis\n# America/Argentina/Tucuman\n# America/Argentina/Ushuaia\n# America/Aruba\n# America/Asuncion\n# America/Atikokan\n# America/Bahia\n# America/Bahia_Banderas\n# America/Barbados\n# America/Belem\n# America/Belize\n# America/Blanc-Sablon\n# America/Boa_Vista\n# America/Bogota\n# America/Boise\n# America/Cambridge_Bay\n# America/Campo_Grande\n# America/Cancun\n# America/Caracas\n# America/Cayenne\n# America/Cayman\n# America/Chicago\n# America/Chihuahua\n# America/Costa_Rica\n# America/Creston\n# America/Cuiaba\n# America/Curacao\n# America/Danmarkshavn\n# America/Dawson\n# America/Dawson_Creek\n# America/Denver\n# America/Detroit\n# America/Dominica\n# America/Edmonton\n# America/Eirunepe\n# America/El_Salvador\n# America/Fortaleza\n# America/Fort_Nelson\n# America/Glace_Bay\n# America/Godthab\n# America/Goose_Bay\n# America/Grand_Turk\n# America/Grenada\n# America/Guadeloupe\n# America/Guatemala\n# America/Guayaquil\n# America/Guyana\n# America/Halifax\n# America/Havana\n# America/Hermosillo\n# America/Indiana/Indianapolis\n# America/Indiana/Knox\n# America/Indiana/Marengo\n# America/Indiana/Petersburg\n# America/Indiana/Tell_City\n# America/Indiana/Vevay\n# America/Indiana/Vincennes\n# America/Indiana/Winamac\n# America/Inuvik\n# America/Iqaluit\n# America/Jamaica\n# America/Juneau\n# America/Kentucky/Louisville\n# America/Kentucky/Monticello\n# America/Kralendijk\n# America/La_Paz\n# America/Lima\n# America/Los_Angeles\n# America/Lower_Princes\n# America/Maceio\n# America/Managua\n# America/Manaus\n# America/Marigot\n# America/Martinique\n# America/Matamoros\n# America/Mazatlan\n# America/Menominee\n# America/Merida\n# America/Metlakatla\n# America/Mexico_City\n# America/Miquelon\n# America/Moncton\n# America/Monterrey\n# America/Montevideo\n# America/Montreal\n# America/Montserrat\n# America/Nassau\n# America/New_York\n# America/Nipigon\n# America/Nome\n# America/Noronha\n# America/North_Dakota/Beulah\n# America/North_Dakota/Center\n# America/North_Dakota/New_Salem\n# America/Nuuk\n# America/Ojinaga\n# America/Panama\n# America/Pangnirtung\n# America/Paramaribo\n# America/Phoenix\n# America/Port-au-Prince\n# America/Port_of_Spain\n# America/Porto_Velho\n# America/Puerto_Rico\n# America/Punta_Arenas\n# America/Rainy_River\n# America/Rankin_Inlet\n# America/Recife\n# America/Regina\n# America/Resolute\n# America/Rio_Branco\n# America/Santarem\n# America/Santiago\n# America/Santo_Domingo\n# America/Sao_Paulo\n# America/Scoresbysund\n# America/Sitka\n# America/St_Barthelemy\n# America/St_Johns\n# America/St_Kitts\n# America/St_Lucia\n# America/St_Thomas\n# America/St_Vincent\n# America/Swift_Current\n# America/Tegucigalpa\n# America/Thule\n# America/Thunder_Bay\n# America/Tijuana\n# America/Toronto\n# America/Tortola\n# America/Vancouver\n# America/Whitehorse\n# America/Winnipeg\n# America/Yakutat\n# America/Yellowknife\n# Antarctica/Casey\n# Antarctica/Davis\n# Antarctica/DumontDUrville\n# Antarctica/Macquarie\n# Antarctica/Mawson\n# Antarctica/McMurdo\n# Antarctica/Palmer\n# Antarctica/Rothera\n# Antarctica/Syowa\n# Antarctica/Troll\n# Antarctica/Vostok\n# Arctic/Longyearbyen\n# Asia/Aden\n# Asia/Almaty\n# Asia/Amman\n# Asia/Anadyr\n# Asia/Aqtau\n# Asia/Aqtobe\n# Asia/Ashgabat\n# Asia/Atyrau\n# Asia/Baghdad\n# Asia/Bahrain\n# Asia/Baku\n# Asia/Bangkok\n# Asia/Barnaul\n# Asia/Beirut\n# Asia/Bishkek\n# Asia/Brunei\n# Asia/Chita\n# Asia/Choibalsan\n# Asia/Colombo\n# Asia/Damascus\n# Asia/Dhaka\n# Asia/Dili\n# Asia/Dubai\n# Asia/Dushanbe\n# Asia/Famagusta\n# Asia/Gaza\n# Asia/Hebron\n# Asia/Ho_Chi_Minh\n# Asia/Hong_Kong\n# Asia/Hovd\n# Asia/Irkutsk\n# Asia/Jakarta\n# Asia/Jayapura\n# Asia/Jerusalem\n# Asia/Kabul\n# Asia/Kamchatka\n# Asia/Karachi\n# Asia/Kathmandu\n# Asia/Khandyga\n# Asia/Kolkata\n# Asia/Krasnoyarsk\n# Asia/Kuala_Lumpur\n# Asia/Kuching\n# Asia/Kuwait\n# Asia/Macau\n# Asia/Magadan\n# Asia/Makassar\n# Asia/Manila\n# Asia/Muscat\n# Asia/Nicosia\n# Asia/Novokuznetsk\n# Asia/Novosibirsk\n# Asia/Omsk\n# Asia/Oral\n# Asia/Phnom_Penh\n# Asia/Pontianak\n# Asia/Pyongyang\n# Asia/Qatar\n# Asia/Qyzylorda\n# Asia/Riyadh\n# Asia/Sakhalin\n# Asia/Samarkand\n# Asia/Seoul\n# Asia/Shanghai\n# Asia/Singapore\n# Asia/Srednekolymsk\n# Asia/Taipei\n# Asia/Tashkent\n# Asia/Tbilisi\n# Asia/Tehran\n# Asia/Thimphu\n# Asia/Tokyo\n# Asia/Tomsk\n# Asia/Ulaanbaatar\n# Asia/Urumqi\n# Asia/Ust-Nera\n# Asia/Vientiane\n# Asia/Vladivostok\n# Asia/Yakutsk\n# Asia/Yangon\n# Asia/Yekaterinburg\n# Asia/Yerevan\n# Atlantic/Azores\n# Atlantic/Bermuda\n# Atlantic/Canary\n# Atlantic/Cape_Verde\n# Atlantic/Faroe\n# Atlantic/Madeira\n# Atlantic/Reykjavik\n# Atlantic/South_Georgia\n# Atlantic/Stanley\n# Atlantic/St_Helena\n# Australia/Adelaide\n# Australia/Brisbane\n# Australia/Broken_Hill\n# Australia/Currie\n# Australia/Darwin\n# Australia/Eucla\n# Australia/Hobart\n# Australia/Lindeman\n# Australia/Lord_Howe\n# Australia/Melbourne\n# Australia/Perth\n# Australia/Sydney\n# Europe/Amsterdam\n# Europe/Andorra\n# Europe/Astrakhan\n# Europe/Athens\n# Europe/Belgrade\n# Europe/Berlin\n# Europe/Bratislava\n# Europe/Brussels\n# Europe/Bucharest\n# Europe/Budapest\n# Europe/Busingen\n# Europe/Chisinau\n# Europe/Copenhagen\n# Europe/Dublin\n# Europe/Gibraltar\n# Europe/Guernsey\n# Europe/Helsinki\n# Europe/Isle_of_Man\n# Europe/Istanbul\n# Europe/Jersey\n# Europe/Kaliningrad\n# Europe/Kiev\n# Europe/Kirov\n# Europe/Lisbon\n# Europe/Ljubljana\n# Europe/London\n# Europe/Luxembourg\n# Europe/Madrid\n# Europe/Malta\n# Europe/Mariehamn\n# Europe/Minsk\n# Europe/Monaco\n# Europe/Moscow\n# Europe/Oslo\n# Europe/Paris\n# Europe/Podgorica\n# Europe/Prague\n# Europe/Riga\n# Europe/Rome\n# Europe/Samara\n# Europe/San_Marino\n# Europe/Sarajevo\n# Europe/Saratov\n# Europe/Simferopol\n# Europe/Skopje\n# Europe/Sofia\n# Europe/Stockholm\n# Europe/Tallinn\n# Europe/Tirane\n# Europe/Ulyanovsk\n# Europe/Uzhgorod\n# Europe/Vaduz\n# Europe/Vatican\n# Europe/Vienna\n# Europe/Vilnius\n# Europe/Volgograd\n# Europe/Warsaw\n# Europe/Zagreb\n# Europe/Zaporozhye\n# Europe/Zurich\n# Indian/Antananarivo\n# Indian/Chagos\n# Indian/Christmas\n# Indian/Cocos\n# Indian/Comoro\n# Indian/Kerguelen\n# Indian/Mahe\n# Indian/Maldives\n# Indian/Mauritius\n# Indian/Mayotte\n# Indian/Reunion\n# Pacific/Apia\n# Pacific/Auckland\n# Pacific/Bougainville\n# Pacific/Chatham\n# Pacific/Chuuk\n# Pacific/Easter\n# Pacific/Efate\n# Pacific/Enderbury\n# Pacific/Fakaofo\n# Pacific/Fiji\n# Pacific/Funafuti\n# Pacific/Galapagos\n# Pacific/Gambier\n# Pacific/Guadalcanal\n# Pacific/Guam\n# Pacific/Honolulu\n# Pacific/Kiritimati\n# Pacific/Kosrae\n# Pacific/Kwajalein\n# Pacific/Majuro\n# Pacific/Marquesas\n# Pacific/Midway\n# Pacific/Nauru\n# Pacific/Niue\n# Pacific/Norfolk\n# Pacific/Noumea\n# Pacific/Pago_Pago\n# Pacific/Palau\n# Pacific/Pitcairn\n# Pacific/Pohnpei\n# Pacific/Port_Moresby\n# Pacific/Rarotonga\n# Pacific/Saipan\n# Pacific/Tahiti\n# Pacific/Tarawa\n# Pacific/Tongatapu\n# Pacific/Wake\n# Pacific/Wallis\n# Etc/GMT\n# Etc/GMT-0\n# Etc/GMT-1\n# Etc/GMT-2\n# Etc/GMT-3\n# Etc/GMT-4\n# Etc/GMT-5\n# Etc/GMT-6\n# Etc/GMT-7\n# Etc/GMT-8\n# Etc/GMT-9\n# Etc/GMT-10\n# Etc/GMT-11\n# Etc/GMT-12\n# Etc/GMT-13\n# Etc/GMT-14\n# Etc/GMT0\n# Etc/GMT+0\n# Etc/GMT+1\n# Etc/GMT+2\n# Etc/GMT+3\n# Etc/GMT+4\n# Etc/GMT+5\n# Etc/GMT+6\n# Etc/GMT+7\n# Etc/GMT+8\n# Etc/GMT+9\n# Etc/GMT+10\n# Etc/GMT+11\n# Etc/GMT+12\n# Etc/UCT\n# Etc/UTC\n# Etc/Greenwich\n# Etc/Universal\n# Etc/Zulu\n";

const KeyValuePair timezone_dict[] = {{"Africa/Abidjan", "GMT0"}, {"Africa/Accra", "GMT0"}, {"Africa/Addis_Ababa", "EAT-3"}, {"Africa/Algiers", "CET-1"}, {"Africa/Asmara", "EAT-3"}, {"Africa/Bamako", "GMT0"}, {"Africa/Bangui", "WAT-1"}, {"Africa/Banjul", "GMT0"}, {"Africa/Bissau", "GMT0"}, {"Africa/Blantyre", "CAT-2"}, {"Africa/Brazzaville", "WAT-1"}, {"Africa/Bujumbura", "CAT-2"}, {"Africa/Cairo", "EET-2EEST,M4.5.5/0,M10.5.4/24"}, {"Africa/Casablanca", "<+01>-1"}, {"Africa/Ceuta", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Africa/Conakry", "GMT0"}, {"Africa/Dakar", "GMT0"}, {"Africa/Dar_es_Salaam", "EAT-3"}, {"Africa/Djibouti", "EAT-3"}, {"Africa/Douala", "WAT-1"}, {"Africa/El_Aaiun", "<+01>-1"}, {"Africa/Freetown", "GMT0"}, {"Africa/Gaborone", "CAT-2"}, {"Africa/Harare", "CAT-2"}, {"Africa/Johannesburg", "SAST-2"}, {"Africa/Juba", "CAT-2"}, {"Africa/Kampala", "EAT-3"}, {"Africa/Khartoum", "CAT-2"}, {"Africa/Kigali", "CAT-2"}, {"Africa/Kinshasa", "WAT-1"}, {"Africa/Lagos", "WAT-1"}, {"Africa/Libreville", "WAT-1"}, {"Africa/Lome", "GMT0"}, {"Africa/Luanda", "WAT-1"}, {"Africa/Lubumbashi", "CAT-2"}, {"Africa/Lusaka", "CAT-2"}, {"Africa/Malabo", "WAT-1"}, {"Africa/Maputo", "CAT-2"}, {"Africa/Maseru", "SAST-2"}, {"Africa/Mbabane", "SAST-2"}, {"Africa/Mogadishu", "EAT-3"}, {"Africa/Monrovia", "GMT0"}, {"Africa/Nairobi", "EAT-3"}, {"Africa/Ndjamena", "WAT-1"}, {"Africa/Niamey", "WAT-1"}, {"Africa/Nouakchott", "GMT0"}, {"Africa/Ouagadougou", "GMT0"}, {"Africa/Porto-Novo", "WAT-1"}, {"Africa/Sao_Tome", "GMT0"}, {"Africa/Tripoli", "EET-2"}, {"Africa/Tunis", "CET-1"}, {"Africa/Windhoek", "CAT-2"}, {"America/Adak", "HST10HDT,M3.2.0,M11.1.0"}, {"America/Anchorage", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/Anguilla", "AST4"}, {"America/Antigua", "AST4"}, {"America/Araguaina", "<-03>3"}, {"America/Argentina/Buenos_Aires", "<-03>3"}, {"America/Argentina/Catamarca", "<-03>3"}, {"America/Argentina/Cordoba", "<-03>3"}, {"America/Argentina/Jujuy", "<-03>3"}, {"America/Argentina/La_Rioja", "<-03>3"}, {"America/Argentina/Mendoza", "<-03>3"}, {"America/Argentina/Rio_Gallegos", "<-03>3"}, {"America/Argentina/Salta", "<-03>3"}, {"America/Argentina/San_Juan", "<-03>3"}, {"America/Argentina/San_Luis", "<-03>3"}, {"America/Argentina/Tucuman", "<-03>3"}, {"America/Argentina/Ushuaia", "<-03>3"}, {"America/Aruba", "AST4"}, {"America/Asuncion", "<-04>4<-03>,M10.1.0/0,M3.4.0/0"}, {"America/Atikokan", "EST5"}, {"America/Bahia", "<-03>3"}, {"America/Bahia_Banderas", "CST6"}, {"America/Barbados", "AST4"}, {"America/Belem", "<-03>3"}, {"America/Belize", "CST6"}, {"America/Blanc-Sablon", "AST4"}, {"America/Boa_Vista", "<-04>4"}, {"America/Bogota", "<-05>5"}, {"America/Boise", "MST7MDT,M3.2.0,M11.1.0"}, {"America/Cambridge_Bay", "MST7MDT,M3.2.0,M11.1.0"}, {"America/Campo_Grande", "<-04>4"}, {"America/Cancun", "EST5"}, {"America/Caracas", "<-04>4"}, {"America/Cayenne", "<-03>3"}, {"America/Cayman", "EST5"}, {"America/Chicago", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Chihuahua", "CST6"}, {"America/Costa_Rica", "CST6"}, {"America/Creston", "MST7"}, {"America/Cuiaba", "<-04>4"}, {"America/Curacao", "AST4"}, {"America/Danmarkshavn", "GMT0"}, {"America/Dawson", "MST7"}, {"America/Dawson_Creek", "MST7"}, {"America/Denver", "MST7MDT,M3.2.0,M11.1.0"}, {"America/Detroit", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Dominica", "AST4"}, {"America/Edmonton", "MST7MDT,M3.2.0,M11.1.0"}, {"America/Eirunepe", "<-05>5"}, {"America/El_Salvador", "CST6"}, {"America/Fortaleza", "<-03>3"}, {"America/Fort_Nelson", "MST7"}, {"America/Glace_Bay", "AST4ADT,M3.2.0,M11.1.0"}, {"America/Godthab", "<-02>2<-01>,M3.5.0/-1,M10.5.0/0"}, {"America/Goose_Bay", "AST4ADT,M3.2.0,M11.1.0"}, {"America/Grand_Turk", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Grenada", "AST4"}, {"America/Guadeloupe", "AST4"}, {"America/Guatemala", "CST6"}, {"America/Guayaquil", "<-05>5"}, {"America/Guyana", "<-04>4"}, {"America/Halifax", "AST4ADT,M3.2.0,M11.1.0"}, {"America/Havana", "CST5CDT,M3.2.0/0,M11.1.0/1"}, {"America/Hermosillo", "MST7"}, {"America/Indiana/Indianapolis", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Indiana/Knox", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Indiana/Marengo", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Indiana/Petersburg", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Indiana/Tell_City", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Indiana/Vevay", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Indiana/Vincennes", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Indiana/Winamac", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Inuvik", "MST7MDT,M3.2.0,M11.1.0"}, {"America/Iqaluit", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Jamaica", "EST5"}, {"America/Juneau", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/Kentucky/Louisville", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Kentucky/Monticello", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Kralendijk", "AST4"}, {"America/La_Paz", "<-04>4"}, {"America/Lima", "<-05>5"}, {"America/Los_Angeles", "PST8PDT,M3.2.0,M11.1.0"}, {"America/Lower_Princes", "AST4"}, {"America/Maceio", "<-03>3"}, {"America/Managua", "CST6"}, {"America/Manaus", "<-04>4"}, {"America/Marigot", "AST4"}, {"America/Martinique", "AST4"}, {"America/Matamoros", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Mazatlan", "MST7"}, {"America/Menominee", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Merida", "CST6"}, {"America/Metlakatla", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/Mexico_City", "CST6"}, {"America/Miquelon", "<-03>3<-02>,M3.2.0,M11.1.0"}, {"America/Moncton", "AST4ADT,M3.2.0,M11.1.0"}, {"America/Monterrey", "CST6"}, {"America/Montevideo", "<-03>3"}, {"America/Montreal", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Montserrat", "AST4"}, {"America/Nassau", "EST5EDT,M3.2.0,M11.1.0"}, {"America/New_York", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Nipigon", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Nome", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/Noronha", "<-02>2"}, {"America/North_Dakota/Beulah", "CST6CDT,M3.2.0,M11.1.0"}, {"America/North_Dakota/Center", "CST6CDT,M3.2.0,M11.1.0"}, {"America/North_Dakota/New_Salem", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Nuuk", "<-02>2<-01>,M3.5.0/-1,M10.5.0/0"}, {"America/Ojinaga", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Panama", "EST5"}, {"America/Pangnirtung", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Paramaribo", "<-03>3"}, {"America/Phoenix", "MST7"}, {"America/Port-au-Prince", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Port_of_Spain", "AST4"}, {"America/Porto_Velho", "<-04>4"}, {"America/Puerto_Rico", "AST4"}, {"America/Punta_Arenas", "<-03>3"}, {"America/Rainy_River", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Rankin_Inlet", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Recife", "<-03>3"}, {"America/Regina", "CST6"}, {"America/Resolute", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Rio_Branco", "<-05>5"}, {"America/Santarem", "<-03>3"}, {"America/Santiago", "<-04>4<-03>,M9.1.6/24,M4.1.6/24"}, {"America/Santo_Domingo", "AST4"}, {"America/Sao_Paulo", "<-03>3"}, {"America/Scoresbysund", "<-02>2<-01>,M3.5.0/-1,M10.5.0/0"}, {"America/Sitka", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/St_Barthelemy", "AST4"}, {"America/St_Johns", "NST3:30NDT,M3.2.0,M11.1.0"}, {"America/St_Kitts", "AST4"}, {"America/St_Lucia", "AST4"}, {"America/St_Thomas", "AST4"}, {"America/St_Vincent", "AST4"}, {"America/Swift_Current", "CST6"}, {"America/Tegucigalpa", "CST6"}, {"America/Thule", "AST4ADT,M3.2.0,M11.1.0"}, {"America/Thunder_Bay", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Tijuana", "PST8PDT,M3.2.0,M11.1.0"}, {"America/Toronto", "EST5EDT,M3.2.0,M11.1.0"}, {"America/Tortola", "AST4"}, {"America/Vancouver", "PST8PDT,M3.2.0,M11.1.0"}, {"America/Whitehorse", "MST7"}, {"America/Winnipeg", "CST6CDT,M3.2.0,M11.1.0"}, {"America/Yakutat", "AKST9AKDT,M3.2.0,M11.1.0"}, {"America/Yellowknife", "MST7MDT,M3.2.0,M11.1.0"}, {"Antarctica/Casey", "<+08>-8"}, {"Antarctica/Davis", "<+07>-7"}, {"Antarctica/DumontDUrville", "<+10>-10"}, {"Antarctica/Macquarie", "AEST-10AEDT,M10.1.0,M4.1.0/3"}, {"Antarctica/Mawson", "<+05>-5"}, {"Antarctica/McMurdo", "NZST-12NZDT,M9.5.0,M4.1.0/3"}, {"Antarctica/Palmer", "<-03>3"}, {"Antarctica/Rothera", "<-03>3"}, {"Antarctica/Syowa", "<+03>-3"}, {"Antarctica/Troll", "<+00>0<+02>-2,M3.5.0/1,M10.5.0/3"}, {"Antarctica/Vostok", "<+05>-5"}, {"Arctic/Longyearbyen", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Asia/Aden", "<+03>-3"}, {"Asia/Almaty", "<+05>-5"}, {"Asia/Amman", "<+03>-3"}, {"Asia/Anadyr", "<+12>-12"}, {"Asia/Aqtau", "<+05>-5"}, {"Asia/Aqtobe", "<+05>-5"}, {"Asia/Ashgabat", "<+05>-5"}, {"Asia/Atyrau", "<+05>-5"}, {"Asia/Baghdad", "<+03>-3"}, {"Asia/Bahrain", "<+03>-3"}, {"Asia/Baku", "<+04>-4"}, {"Asia/Bangkok", "<+07>-7"}, {"Asia/Barnaul", "<+07>-7"}, {"Asia/Beirut", "EET-2EEST,M3.5.0/0,M10.5.0/0"}, {"Asia/Bishkek", "<+06>-6"}, {"Asia/Brunei", "<+08>-8"}, {"Asia/Chita", "<+09>-9"}, {"Asia/Choibalsan", "<+08>-8"}, {"Asia/Colombo", "<+0530>-5:30"}, {"Asia/Damascus", "<+03>-3"}, {"Asia/Dhaka", "<+06>-6"}, {"Asia/Dili", "<+09>-9"}, {"Asia/Dubai", "<+04>-4"}, {"Asia/Dushanbe", "<+05>-5"}, {"Asia/Famagusta", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Asia/Gaza", "EET-2EEST,M3.4.4/50,M10.4.4/50"}, {"Asia/Hebron", "EET-2EEST,M3.4.4/50,M10.4.4/50"}, {"Asia/Ho_Chi_Minh", "<+07>-7"}, {"Asia/Hong_Kong", "HKT-8"}, {"Asia/Hovd", "<+07>-7"}, {"Asia/Irkutsk", "<+08>-8"}, {"Asia/Jakarta", "WIB-7"}, {"Asia/Jayapura", "WIT-9"}, {"Asia/Jerusalem", "IST-2IDT,M3.4.4/26,M10.5.0"}, {"Asia/Kabul", "<+0430>-4:30"}, {"Asia/Kamchatka", "<+12>-12"}, {"Asia/Karachi", "PKT-5"}, {"Asia/Kathmandu", "<+0545>-5:45"}, {"Asia/Khandyga", "<+09>-9"}, {"Asia/Kolkata", "IST-5:30"}, {"Asia/Krasnoyarsk", "<+07>-7"}, {"Asia/Kuala_Lumpur", "<+08>-8"}, {"Asia/Kuching", "<+08>-8"}, {"Asia/Kuwait", "<+03>-3"}, {"Asia/Macau", "CST-8"}, {"Asia/Magadan", "<+11>-11"}, {"Asia/Makassar", "WITA-8"}, {"Asia/Manila", "PST-8"}, {"Asia/Muscat", "<+04>-4"}, {"Asia/Nicosia", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Asia/Novokuznetsk", "<+07>-7"}, {"Asia/Novosibirsk", "<+07>-7"}, {"Asia/Omsk", "<+06>-6"}, {"Asia/Oral", "<+05>-5"}, {"Asia/Phnom_Penh", "<+07>-7"}, {"Asia/Pontianak", "WIB-7"}, {"Asia/Pyongyang", "KST-9"}, {"Asia/Qatar", "<+03>-3"}, {"Asia/Qyzylorda", "<+05>-5"}, {"Asia/Riyadh", "<+03>-3"}, {"Asia/Sakhalin", "<+11>-11"}, {"Asia/Samarkand", "<+05>-5"}, {"Asia/Seoul", "KST-9"}, {"Asia/Shanghai", "CST-8"}, {"Asia/Singapore", "<+08>-8"}, {"Asia/Srednekolymsk", "<+11>-11"}, {"Asia/Taipei", "CST-8"}, {"Asia/Tashkent", "<+05>-5"}, {"Asia/Tbilisi", "<+04>-4"}, {"Asia/Tehran", "<+0330>-3:30"}, {"Asia/Thimphu", "<+06>-6"}, {"Asia/Tokyo", "JST-9"}, {"Asia/Tomsk", "<+07>-7"}, {"Asia/Ulaanbaatar", "<+08>-8"}, {"Asia/Urumqi", "<+06>-6"}, {"Asia/Ust-Nera", "<+10>-10"}, {"Asia/Vientiane", "<+07>-7"}, {"Asia/Vladivostok", "<+10>-10"}, {"Asia/Yakutsk", "<+09>-9"}, {"Asia/Yangon", "<+0630>-6:30"}, {"Asia/Yekaterinburg", "<+05>-5"}, {"Asia/Yerevan", "<+04>-4"}, {"Atlantic/Azores", "<-01>1<+00>,M3.5.0/0,M10.5.0/1"}, {"Atlantic/Bermuda", "AST4ADT,M3.2.0,M11.1.0"}, {"Atlantic/Canary", "WET0WEST,M3.5.0/1,M10.5.0"}, {"Atlantic/Cape_Verde", "<-01>1"}, {"Atlantic/Faroe", "WET0WEST,M3.5.0/1,M10.5.0"}, {"Atlantic/Madeira", "WET0WEST,M3.5.0/1,M10.5.0"}, {"Atlantic/Reykjavik", "GMT0"}, {"Atlantic/South_Georgia", "<-02>2"}, {"Atlantic/Stanley", "<-03>3"}, {"Atlantic/St_Helena", "GMT0"}, {"Australia/Adelaide", "ACST-9:30ACDT,M10.1.0,M4.1.0/3"}, {"Australia/Brisbane", "AEST-10"}, {"Australia/Broken_Hill", "ACST-9:30ACDT,M10.1.0,M4.1.0/3"}, {"Australia/Currie", "AEST-10AEDT,M10.1.0,M4.1.0/3"}, {"Australia/Darwin", "ACST-9:30"}, {"Australia/Eucla", "<+0845>-8:45"}, {"Australia/Hobart", "AEST-10AEDT,M10.1.0,M4.1.0/3"}, {"Australia/Lindeman", "AEST-10"}, {"Australia/Lord_Howe", "<+1030>-10:30<+11>-11,M10.1.0,M4.1.0"}, {"Australia/Melbourne", "AEST-10AEDT,M10.1.0,M4.1.0/3"}, {"Australia/Perth", "AWST-8"}, {"Australia/Sydney", "AEST-10AEDT,M10.1.0,M4.1.0/3"}, {"Europe/Amsterdam", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Andorra", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Astrakhan", "<+04>-4"}, {"Europe/Athens", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Belgrade", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Berlin", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Bratislava", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Brussels", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Bucharest", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Budapest", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Busingen", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Chisinau", "EET-2EEST,M3.5.0,M10.5.0/3"}, {"Europe/Copenhagen", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Dublin", "IST-1GMT0,M10.5.0,M3.5.0/1"}, {"Europe/Gibraltar", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Guernsey", "GMT0BST,M3.5.0/1,M10.5.0"}, {"Europe/Helsinki", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Isle_of_Man", "GMT0BST,M3.5.0/1,M10.5.0"}, {"Europe/Istanbul", "<+03>-3"}, {"Europe/Jersey", "GMT0BST,M3.5.0/1,M10.5.0"}, {"Europe/Kaliningrad", "EET-2"}, {"Europe/Kiev", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Kirov", "MSK-3"}, {"Europe/Lisbon", "WET0WEST,M3.5.0/1,M10.5.0"}, {"Europe/Ljubljana", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/London", "GMT0BST,M3.5.0/1,M10.5.0"}, {"Europe/Luxembourg", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Madrid", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Malta", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Mariehamn", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Minsk", "<+03>-3"}, {"Europe/Monaco", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Moscow", "MSK-3"}, {"Europe/Oslo", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Paris", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Podgorica", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Prague", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Riga", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Rome", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Samara", "<+04>-4"}, {"Europe/San_Marino", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Sarajevo", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Saratov", "<+04>-4"}, {"Europe/Simferopol", "MSK-3"}, {"Europe/Skopje", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Sofia", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Stockholm", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Tallinn", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Tirane", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Ulyanovsk", "<+04>-4"}, {"Europe/Uzhgorod", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Vaduz", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Vatican", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Vienna", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Vilnius", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Volgograd", "MSK-3"}, {"Europe/Warsaw", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Zagreb", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Europe/Zaporozhye", "EET-2EEST,M3.5.0/3,M10.5.0/4"}, {"Europe/Zurich", "CET-1CEST,M3.5.0,M10.5.0/3"}, {"Indian/Antananarivo", "EAT-3"}, {"Indian/Chagos", "<+06>-6"}, {"Indian/Christmas", "<+07>-7"}, {"Indian/Cocos", "<+0630>-6:30"}, {"Indian/Comoro", "EAT-3"}, {"Indian/Kerguelen", "<+05>-5"}, {"Indian/Mahe", "<+04>-4"}, {"Indian/Maldives", "<+05>-5"}, {"Indian/Mauritius", "<+04>-4"}, {"Indian/Mayotte", "EAT-3"}, {"Indian/Reunion", "<+04>-4"}, {"Pacific/Apia", "<+13>-13"}, {"Pacific/Auckland", "NZST-12NZDT,M9.5.0,M4.1.0/3"}, {"Pacific/Bougainville", "<+11>-11"}, {"Pacific/Chatham", "<+1245>-12:45<+1345>,M9.5.0/2:45,M4.1.0/3:45"}, {"Pacific/Chuuk", "<+10>-10"}, {"Pacific/Easter", "<-06>6<-05>,M9.1.6/22,M4.1.6/22"}, {"Pacific/Efate", "<+11>-11"}, {"Pacific/Enderbury", "<+13>-13"}, {"Pacific/Fakaofo", "<+13>-13"}, {"Pacific/Fiji", "<+12>-12"}, {"Pacific/Funafuti", "<+12>-12"}, {"Pacific/Galapagos", "<-06>6"}, {"Pacific/Gambier", "<-09>9"}, {"Pacific/Guadalcanal", "<+11>-11"}, {"Pacific/Guam", "ChST-10"}, {"Pacific/Honolulu", "HST10"}, {"Pacific/Kiritimati", "<+14>-14"}, {"Pacific/Kosrae", "<+11>-11"}, {"Pacific/Kwajalein", "<+12>-12"}, {"Pacific/Majuro", "<+12>-12"}, {"Pacific/Marquesas", "<-0930>9:30"}, {"Pacific/Midway", "SST11"}, {"Pacific/Nauru", "<+12>-12"}, {"Pacific/Niue", "<-11>11"}, {"Pacific/Norfolk", "<+11>-11<+12>,M10.1.0,M4.1.0/3"}, {"Pacific/Noumea", "<+11>-11"}, {"Pacific/Pago_Pago", "SST11"}, {"Pacific/Palau", "<+09>-9"}, {"Pacific/Pitcairn", "<-08>8"}, {"Pacific/Pohnpei", "<+11>-11"}, {"Pacific/Port_Moresby", "<+10>-10"}, {"Pacific/Rarotonga", "<-10>10"}, {"Pacific/Saipan", "ChST-10"}, {"Pacific/Tahiti", "<-10>10"}, {"Pacific/Tarawa", "<+12>-12"}, {"Pacific/Tongatapu", "<+13>-13"}, {"Pacific/Wake", "<+12>-12"}, {"Pacific/Wallis", "<+12>-12"}, {"Etc/GMT", "GMT0"}, {"Etc/GMT-0", "GMT0"}, {"Etc/GMT-1", "<+01>-1"}, {"Etc/GMT-2", "<+02>-2"}, {"Etc/GMT-3", "<+03>-3"}, {"Etc/GMT-4", "<+04>-4"}, {"Etc/GMT-5", "<+05>-5"}, {"Etc/GMT-6", "<+06>-6"}, {"Etc/GMT-7", "<+07>-7"}, {"Etc/GMT-8", "<+08>-8"}, {"Etc/GMT-9", "<+09>-9"}, {"Etc/GMT-10", "<+10>-10"}, {"Etc/GMT-11", "<+11>-11"}, {"Etc/GMT-12", "<+12>-12"}, {"Etc/GMT-13", "<+13>-13"}, {"Etc/GMT-14", "<+14>-14"}, {"Etc/GMT0", "GMT0"}, {"Etc/GMT+0", "GMT0"}, {"Etc/GMT+1", "<-01>1"}, {"Etc/GMT+2", "<-02>2"}, {"Etc/GMT+3", "<-03>3"}, {"Etc/GMT+4", "<-04>4"}, {"Etc/GMT+5", "<-05>5"}, {"Etc/GMT+6", "<-06>6"}, {"Etc/GMT+7", "<-07>7"}, {"Etc/GMT+8", "<-08>8"}, {"Etc/GMT+9", "<-09>9"}, {"Etc/GMT+10", "<-10>10"}, {"Etc/GMT+11", "<-11>11"}, {"Etc/GMT+12", "<-12>12"}, {"Etc/UCT", "UTC0"}, {"Etc/UTC", "UTC0"}, {"Etc/Greenwich", "GMT0"}, {"Etc/Universal", "UTC0"}, {"Etc/Zulu", "UTC0"}};

const int timezone_dict_size = sizeof(timezone_dict) / sizeof(timezone_dict[0]);

const char* get_timezone(const char* key) {
    for (int i = 0; i < timezone_dict_size; ++i) {
        if (strcmp(timezone_dict[i].key, key) == 0) {
            return timezone_dict[i].value;
        }
    }
    return NULL;
}
