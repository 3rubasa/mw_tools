#pragma once

#include <string>
#include <memory>

namespace mw_tools {
namespace servers {
class IPostHandler {
    public:
    virtual bool ProcessRequest(const std::string& body) = 0;
};

class IPostServer {
    public:
    virtual void RegisterHandler(const std::string& uri, std::shared_ptr<IPostHandler> handler) = 0;
    virtual std::shared_ptr<IPostHandler> GetHandler(const std::string& uri) const = 0;
    virtual void Start() = 0;
};

class ISSIHandler {
    public:
    virtual std::string ProcessRequest() = 0;
};

class ISSIServer {
    public:
    virtual void RegisterHandler(const std::string& tag, std::shared_ptr<ISSIHandler> handler) = 0;
    virtual std::shared_ptr<ISSIHandler> GetHandler(const std::string& tag) = 0;
    virtual std::shared_ptr<ISSIHandler> GetHandler(int idx) = 0;
    virtual void Start() = 0;
};
}
}

