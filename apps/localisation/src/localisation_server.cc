#include "localisation_server.h"

#include <iostream>
#include <functional>
#include <algorithm>
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

            callback_(
                "",
                std::vector<double>{data.time().begin(), data.time().end()},
                std::vector<double>{data.radius().begin(), data.radius().end()},
                std::vector<double>{
                    data.heading().begin(), data.heading().end()}
            );
        }

        return grpc::Status::OK;
    }

    std::function<
        void(const std::string&, const std::vector<double>&, const std::vector<double>&, const std::vector<double>&)>
        callback_;
};

LocalisationServer::LocalisationServer(const std::string& addr) : addr_(addr) {}

void LocalisationServer::process_data(
    const std::string& /*agent_name*/,
    const std::vector<double>& /*time*/,
    const std::vector<double>& radius,
    const std::vector<double>& heading
)
{
    std::vector<double> x;
    std::vector<double> y;

    x.reserve(radius.size());
    y.reserve(radius.size());

    for (size_t i = 0; i < radius.size(); ++i)
    {
        x.push_back(radius[i] * sin(heading[i]));
        y.push_back(radius[i] * cos(heading[i]));
    }

    vis_.draw(x, y);
}

void LocalisationServer::start_listening()
{
    grpc::ServerBuilder server_builder;
    server_builder.AddListeningPort(addr_, grpc::InsecureServerCredentials());

    localisationImpl service;
    service.callback_ = [this](
                            const std::string&         agent_name,
                            const std::vector<double>& time,
                            const std::vector<double>& radius,
                            const std::vector<double>& heading
                        )
    {
        process_data(agent_name, time, radius, heading);
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
