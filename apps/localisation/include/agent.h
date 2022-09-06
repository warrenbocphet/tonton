#include <queue>
#include <condition_variable>

#include <grpcpp/grpcpp.h>
#include "localisation.grpc.pb.h"
#include "rplidar.h"

class Agent
{
  public:
    Agent(std::shared_ptr<grpc::Channel> channel);

    /// Connect with the server and send new data when it comes in.
    /// This will run on a separate thread and follow a dispatch queue pattern.
    /// The queue in this case would be the data coming from Lidar, of course.
    void communicate_with_server();

    /// Connect with the lidar through RPLidar's SDK
    /// For example,
    ///  addr = "/dev/ttyUSB0"
    ///  baudrate = 115200
    void connect_with_lidar(const std::string& addr, int baudrate);

    /// Read data from LIDAR through RPILidar's SDK.
    void continuously_read_data_from_lidar();

  private:
    std::unique_ptr<Localisation::Stub> stub_;

    std::unique_ptr<sl::ILidarDriver> lidar_driver_;
    std::unique_ptr<sl::IChannel>     lidar_channel_;

    std::queue<double> time_;
    std::queue<double> radius_;
    std::queue<double> heading_;

    std::condition_variable cv_;
    std::mutex              data_mutex_;
};