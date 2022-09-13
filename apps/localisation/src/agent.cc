#include "agent.h"
#include "localisation.grpc.pb.h"

#include <fmt/format.h>
#include <thread>

Agent::Agent(std::shared_ptr<grpc::Channel> channel) :
    stub_(Localisation::NewStub(channel))
{
}

void Agent::communicate_with_server()
{
    std::unique_lock<std::mutex> data_lock(data_mutex_);
    grpc::ClientContext          context;
    auto                         interface = stub_->Communicate(&context);

    while (true)
    {
        SensorData sensor_data;
        // This condition variable will wait until it get notified.
        // If there's data, it will try to acquire the lock and continue.
        cv_.wait(data_lock, [this]() { return time_.size(); });
        while (time_.size())
        {
            sensor_data.add_time(time_.front());
            sensor_data.add_radius(radius_.front());
            sensor_data.add_heading(heading_.front());

            time_.pop();
            radius_.pop();
            heading_.pop();
        }
        interface->Write(sensor_data);
    }
}

void Agent::connect_with_lidar(const std::string& addr, int baudrate)
{
    if (lidar_driver_ || lidar_channel_)
    {
        throw std::runtime_error(
            "connect_with_lidar is unexpectedly called twice."
        );
    }

    sl::ILidarDriver* ldrv = *sl::createLidarDriver();
    lidar_driver_          = std::unique_ptr<sl::ILidarDriver>(ldrv);

    sl::IChannel* channel = *sl::createSerialPortChannel(addr, baudrate);
    lidar_channel_        = std::unique_ptr<sl::IChannel>(channel);

    if (!SL_IS_OK(lidar_driver_->connect(lidar_channel_.get())))
    {
        throw std::runtime_error(fmt::format(
            "Failed to connect with lidar at \n\taddress: {}\n\tbaudrate: {}",
            addr,
            baudrate
        ));
    }
}

void Agent::continuously_read_data_from_lidar()
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
    while (number_of_cotinuous_failure < 3)
    {
        if (SL_IS_OK(lidar_driver_->grabScanDataHq(nodes, node_count)))
        {
            float frequency = -1;
            lidar_driver_->getFrequency(
                scan_mode, nodes, node_count, frequency
            );

            std::cout << fmt::format(
                             "Grabbed {} lidar data, frequency: {}.",
                             node_count,
                             frequency
                         )
                      << std::endl;
            number_of_cotinuous_failure = 0;

            // unlock mutex when exit scope
            std::lock_guard<std::mutex> data_lock(data_mutex_);
            for (int i = 0; i < node_count; ++i)
            {
                time_.push(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    )
                        .count()
                );

                radius_.push(static_cast<double>(nodes[i].dist_mm_q2) / 4.0f);
                heading_.push(
                    static_cast<double>(nodes[i].angle_z_q14) * 1.5708 / 16384.f
                );
            }
            cv_.notify_one();
        }
        else
        {
            ++number_of_cotinuous_failure;
            std::cout << "Failed to grab the scan data" << std::endl;
            // A scan took about 150ms, and if we failed to get the data, just
            // wait for a bit before try again to avoid high CPU usage.
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
}