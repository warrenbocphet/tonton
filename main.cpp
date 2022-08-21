#include <iostream>
#include <fstream>
#include <memory.h>
#include <thread>

#include "sl_lidar.h"
#include "sl_lidar_driver.h"
#include "rplidar.h"

#include <fmt/format.h>

int main(int argc, char** argv)
{
    //  Create a communication channel instance
    std::shared_ptr<sl::IChannel> channel(*sl::createSerialPortChannel("/dev/ttyUSB0", 115200));

    ///  Create a LIDAR driver instance
    std::shared_ptr<sl::ILidarDriver> lidar(*sl::createLidarDriver());

    if (SL_IS_OK(lidar->connect(channel.get())))
    {
        sl_lidar_response_device_info_t deviceInfo;
        if (SL_IS_OK(lidar->getDeviceInfo(deviceInfo)))
        {
            std::cout << fmt::format(
                             "Model: {}\nFirmware version: {}.{}\nHardware version: {}",
                             deviceInfo.model,
                             deviceInfo.firmware_version >> 8,
                             deviceInfo.firmware_version & 0xffu,
                             deviceInfo.hardware_version
                         )
                      << std::endl;
        }
        else
        {
            throw std::runtime_error("Failed to get information from the lidar.");
        }
    }
    else
    {
        throw std::runtime_error("Failed to connect to lidar.");
    }

    sl::LidarScanMode scan_mode;
    if (SL_IS_OK(lidar->startScan(false, true, 0, &scan_mode)))
    {
        std::cout << "Using scan mode: " << scan_mode.scan_mode << std::endl;
    }
    else
    {
        std::cout << "Failed to start a scan." << std::endl;
    }

    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t                                  nodeCount = sizeof(nodes) / sizeof(sl_lidar_response_measurement_node_hq_t);

    if (SL_IS_OK(lidar->grabScanDataHq(nodes, nodeCount)))
    {
        std::cout << fmt::format("Grabbed {} lidar data.", nodeCount) << std::endl;
    }
    else
    {
        std::cout << "Failed to grab the scan data" << std::endl;
    }
}