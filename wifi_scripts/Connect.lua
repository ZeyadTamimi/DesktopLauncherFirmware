-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
SSID = "**********"
SSID_PASSWORD = "************"

-- configure ESP as a station
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)