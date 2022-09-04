#include "localisation_server.h"

#include <iostream>
#include <functional>
#include <fmt/format.h>

class localisationImpl : public Localisation::Service
{
  public:
    grpc::Status Communicate(
        grpc::ServerContext*                           context,
        grpc::ServerReaderWriter<Command, SensorData>* stream
    ) override
    {
        SensorData data;
        while (stream->Read(&data))
        {
            if ((data.heading_size() != data.radius_size()) ||
                (data.radius_size() != data.time_size()))
            {
                std::cout
                    << "Error: heading/radius/time don't have the same size"
                    << std::endl;
            }

            std::cout << fmt::format("\tadding {} data point", data.time_size())
                      << std::endl;

            for (int i = 0; i < data.time_size(); ++i)
            {
                callback_("", data.time(i), data.radius(i), data.heading(i));
            }
        }
    }

    std::function<void(const std::string&, double, double, double)> callback_;
};

LocalisationServer::LocalisationServer(const std::string& addr) : addr_(addr) {}

void LocalisationServer::append_data(
    const std::string& /*agent_name*/,
    double time,
    double radius,
    double heading
)
{
    time_.push_back(time);
    radius_.push_back(radius);
    heading_.push_back(heading);
}

void LocalisationServer::start_listening()
{
    grpc::ServerBuilder server_builder;
    server_builder.AddListeningPort(addr_, grpc::InsecureServerCredentials());

    localisationImpl service;
    service.callback_ = [this](
                            const std::string& agent_name,
                            double             time,
                            double             radius,
                            double             heading
                        )
    {
        append_data(agent_name, time, radius, heading);
    };

    server_builder.RegisterService(&service);

    server_ = server_builder.BuildAndStart();
    server_->Wait();  // Blocking until server_->Shutdown() is called.

    std::cout << "Server has shutdowned" << std::endl;
}

void LocalisationServer::shutdown()
{
    server_->Shutdown();
}
