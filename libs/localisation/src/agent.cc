#include "agent.h"

#include <fmt/format.h>

Agent::Agent(std::shared_ptr<grpc::Channel> channel) :
    stub_(Localisation::NewStub(channel))
{
}

void Agent::communicate_with_server() {}

void Agent::connect_with_lidar(const std::string& addr, int baudrate)
{
    lidar_driver_ =
        std::make_unique<sl::ILidarDriver>(*sl::createLidarDriver());
    if (!SL_IS_OK(
            lidar_driver_->connect(*sl::createSerialPortChannel(addr, baudrate))
        ))
    {
        throw std::runtime_error(fmt::format(
            "Failed to connect with lidar at \n\taddress: {}\n\tbaudrate: {}",
            addr,
            baudrate
        ));
    }
}

void Agent::read_data_from_lidar()
{
    sl::LidarScanMode scan_mode;
    if (SL_IS_OK(lidar_driver_->startScan(false, true, 0, &scan_mode)))
    {
        std::cout << "Using scan mode: " << scan_mode.scan_mode << std::endl;
    }
    else
    {
        throw std::runtime_error("Failed to start a scan.");
    }

    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t                                  node_count =
        sizeof(nodes) / sizeof(sl_lidar_response_measurement_node_hq_t);

    int number_of_cotinuous_failure = 0;
    if (SL_IS_OK(lidar_driver_->grabScanDataHq(nodes, node_count)))
    {
        std::cout << fmt::format("Grabbed {} lidar data.", node_count)
                  << std::endl;
        number_of_cotinuous_failure = 0;
        for (int i = 0; i < node_count; ++i)
        {
            time_.push_back(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                )
                    .count()
            );

            radius_.push_back(nodes[i].dist_mm_q2 / (1 << 2));
            angle_.push_back(nodes[i].angle_z_q14 / (1 << 14));
        }
    }
    else
    {
        std::cout << "Failed to grab the scan data" << std::endl;
    }
}