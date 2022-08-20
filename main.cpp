#include <iostream>

#include "sl_lidar.h"
#include "sl_lidar_driver.h"

int main(int argc, char** argv)
{
    //  Create a communication channel instance
    sl::IChannel*             _channel;
    sl::Result<sl::IChannel*> channel = sl::createSerialPortChannel("/dev/ttyUSB0", 115200);

    ///  Create a LIDAR driver instance
    sl::ILidarDriver* lidar = *sl::createLidarDriver();
    auto              res   = lidar->connect(*channel);

    if (SL_IS_OK(res))
    {
        sl_lidar_response_device_info_t deviceInfo;
        res = lidar->getDeviceInfo(deviceInfo);
        if (SL_IS_OK(res))
        {
            printf(
                "Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
                deviceInfo.model,
                deviceInfo.firmware_version >> 8,
                deviceInfo.firmware_version & 0xffu,
                deviceInfo.hardware_version
            );
        }
        else { fprintf(stderr, "Failed to get device information from LIDAR %08x\r\n", res); }
    }
    else { fprintf(stderr, "Failed to connect to LIDAR %08x\r\n", res); }

    delete lidar;
    delete _channel;
}