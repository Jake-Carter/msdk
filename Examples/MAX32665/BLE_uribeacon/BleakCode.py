'''
python script to scan for BLE packets, do lexical analyses and launch the website

three arguments could be passed to the script, 
serviceuuid, deviceName, launchWebsite.

Each comes with a default value and could be overridden by Command Line arguments

'''


import asyncio
from bleak import BleakScanner
import webbrowser
import argparse

Uri_Service_UUId_16_bit = "fed8"
Bit_base_uuid_128_bit = "0000xxxx-0000-1000-8000-00805f9b34fb"
uriBeaconPrefixes = [ "http://www.", "https://www.", "http://", "https://", "urn:uuid:" ]
uriBeaconCodes = [ ".com/", ".org/", ".edu/", ".net/", ".info/", ".biz/", ".gov/", ".com",  ".org",  ".edu",  ".net",  ".info",  ".biz",  ".gov"]
target_device_name = "UriBcn"
scan_wait_time_in_seconds = 10
start_index_for_url = 3
beaconPrefixIndex = 2
launchWebsite = 1
websites_received = []

def decodeURI(uridata):
    uriString =  str(uriBeaconPrefixes[(uridata[beaconPrefixIndex])]) + (uridata[start_index_for_url:len(uridata)-1].decode()) + str(uriBeaconCodes[(uridata[len(uridata)-1])])
    print("\n\n\n")
    print("URL Received over BLE is " + uriString)
    print("\n\n\n")
    if (launchWebsite):
        webbrowser.open(uriString)


async def discover_devices_by_name(device_name):
    scanner = BleakScanner()
    def advertisement_callback(device, advertisement_data):
        global websites_received
        if advertisement_data.local_name == device_name:
            Uri_Service_UUId_128_bit = Bit_base_uuid_128_bit.replace("xxxx", Uri_Service_UUId_16_bit)
            for i in range(len(advertisement_data.service_uuids)):
                if(advertisement_data.service_uuids[i] == Uri_Service_UUId_128_bit):
                    dataReceived = advertisement_data.service_data[advertisement_data.service_uuids[i]] 
                    if dataReceived not in websites_received:
                        websites_received.append(dataReceived)
                        decodeURI(dataReceived)

    scanner.register_detection_callback(advertisement_callback)
    while True:
        await scanner.start()
        await asyncio.sleep(scan_wait_time_in_seconds)
        await scanner.stop()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Arguments for URI Beacon')
    parser.add_argument('--serviceUUID', type = str, default='fed8', help='Service UUID required for the beacon')
    parser.add_argument('--deviceName', type = str, default='UriBcn', help='Name of the Beacon device')
    parser.add_argument('--launchWebsite', type = int, default=1, help='Name of the Beacon device')

    args = parser.parse_args()

    target_device_name = args.deviceName
    Uri_Service_UUId_16_bit = args.serviceUUID
    launchWebsite = args.launchWebsite

    loop = asyncio.get_event_loop()
    loop.run_until_complete(discover_devices_by_name(target_device_name))
