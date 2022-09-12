#include "localisation.grpc.pb.h"

#include "visualiser.h"

#include <grpcpp/grpcpp.h>

class LocalisationServer : public Localisation::Service
{
  public:
    LocalisationServer(const std::string& addr);

    void process_data(
        const std::string&         agent_name,
        const std::vector<double>& time,
        const std::vector<double>& radius,
        const std::vector<double>& heading
    );

    void start_listening();  // Block until shutdown() is called
    void shutdown();

  private:
    std::unique_ptr<grpc::Server> server_;
    Visualiser                    vis_;

    std::string         addr_;
    std::vector<double> time_;
    std::vector<double> radius_;
    std::vector<double> heading_;
};