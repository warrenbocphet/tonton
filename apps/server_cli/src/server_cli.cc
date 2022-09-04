#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <iostream>
#include <string>

#include "localisation_server.h"

int main(int argc, char** argv)
{
    CLI::App app("Localisation server CLI");

    std::string addr;
    app.add_option("--addr", addr, "Address")->required();
    std::string port;
    app.add_option("--port", port, "Port")->required();

    CLI11_PARSE(app, argc, argv);

    LocalisationServer(fmt::format("{}:{}", addr, port));
}