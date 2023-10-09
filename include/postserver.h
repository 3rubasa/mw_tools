#pragma once

#include <memory>
#include <map>

namespace mw_tools {
    namespace servers {
        class PostServer : public IPostServer, public std::enable_shared_from_this<PostServer> {
public:
    PostServer();

    virtual void RegisterHandler(const std::string& uri, std::shared_ptr<IPostHandler> handler);
    virtual std::shared_ptr<IPostHandler> GetHandler(const std::string& uri) const;
    virtual void Start();
private:
    std::map<std::string, std::shared_ptr<IPostHandler>> m_handlers;
};

}}

