#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <string>
#include <thread>

#include "agent.h"

int main(int argc, char** argv)
{
    CLI::App app("Localisation server CLI");

    std::string addr;
    app.add_option("--addr", addr, "Address")->required();
    std::string port;
    app.add_option("--port", port, "Port")->required();

    CLI11_PARSE(app, argc, argv);

    Agent agent(grpc::CreateChannel(
        fmt::format("{}:{}", addr, port), grpc::InsecureChannelCredentials()
    ));

    agent.connect_with_lidar("/dev/ttyUSB0", 115200);

    auto read_data_thread =
        std::thread([&agent]() { agent.continuously_read_data_from_lidar(); });
    auto communicate_thread =
        std::thread([&agent]() { agent.communicate_with_server(); });

    read_data_thread.join();
    communicate_thread.join();
}