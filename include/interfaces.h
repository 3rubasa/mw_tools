#pragma once

#include <string>
#include <memory>

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

