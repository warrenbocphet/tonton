#include "localisation.grpc.pb.h"

#include <grpcpp/grpcpp.h>

class localisationServer : public Localisation::Service
{
  public:
    localisationServer(const std::string& addr);

    void append_data(
        const std::string& agent_name,
        double             time,
        double             radius,
        double             heading
    );

    void start_listening();  // Block until shutdown() is called
    void shutdown();

  private:
    std::unique_ptr<grpc::Server> server_;

    std::string         addr_;
    std::vector<double> time_;
    std::vector<double> radius_;
    std::vector<double> heading_;
};