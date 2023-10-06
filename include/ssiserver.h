#pragma once

#include <memory>
#include <map>

class SSIServer : public ISSIServer, public std::enable_shared_from_this<SSIServer> {
public:
    SSIServer();

    virtual void RegisterHandler(const std::string& tag, std::shared_ptr<ISSIHandler> handler);
    virtual std::shared_ptr<ISSIHandler> GetHandler(const std::string& tag);
    virtual std::shared_ptr<ISSIHandler> GetHandler(int idx);
    virtual void Start();
private:
    std::map<std::string, std::shared_ptr<ISSIHandler>> m_handlers;
    char* m_ssi_tags[10];
    
};

