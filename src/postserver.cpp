#include <include/interfaces.h>
#include <include/postserver.h>
#include <include/sync.h>

#include <lwip/err.h>
#include "lwip/opt.h"

#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"

#include <stdio.h>
#include <string.h>

#include <map>
#include <memory>



static std::shared_ptr<mw_tools::servers::IPostServer> g_server; 

struct PostContext {
  std::string body;
  std::string uri;
};

static mw_tools::sync::CriticalSection g_cs;
static std::map<void*, std::shared_ptr<PostContext>> g_contexts; 

namespace mw_tools {
namespace servers {

    PostServer::PostServer(){};

    void PostServer::RegisterHandler(const std::string& uri, std::shared_ptr<IPostHandler> handler){
        m_handlers[uri] = handler;
    };

    std::shared_ptr<IPostHandler> PostServer::GetHandler(const std::string& uri) const {
        return m_handlers.at(uri);
    };

    void PostServer::Start() {
        g_server = shared_from_this();
        printf("PostServer initialized\n");
    };

}}

err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                 u16_t http_request_len, int content_len, char *response_uri,
                 u16_t response_uri_len, u8_t *post_auto_wnd)
{
  LWIP_UNUSED_ARG(http_request);
  LWIP_UNUSED_ARG(http_request_len);
  LWIP_UNUSED_ARG(content_len);
  LWIP_UNUSED_ARG(post_auto_wnd);

  printf("httpd_post_begin:\n");
  printf("uri: %s\n", uri);

  std::shared_ptr<PostContext> ctx = std::shared_ptr<PostContext>{new PostContext};
  ctx->uri = uri;
  mw_tools::sync::CriticalSectionGuard guard(g_cs);
  g_contexts[connection] = ctx;
  return ERR_OK;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
    printf("httpd_post_receive_data\n");
    mw_tools::sync::CriticalSectionGuard guard(g_cs);
    auto ctx = g_contexts.at(connection);
    guard.Exit();
    ctx->body.append((char*)(p->payload), p->len);

    pbuf_free(p);
    return ERR_OK;
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
    printf("httpd_post_finished\n");
    mw_tools::sync::CriticalSectionGuard guard(g_cs);
    auto ctx = g_contexts.at(connection);
    guard.Exit();

    printf("URI: %s\n", ctx->uri.c_str());
    printf("JSON: %s\n", ctx->body.c_str());

    auto handler = g_server->GetHandler(ctx->uri);
    if (handler.get() != nullptr) {
        if (handler->ProcessRequest(ctx->body)) {
            snprintf(response_uri, response_uri_len, "/postok.html");
        } else {
           snprintf(response_uri, response_uri_len, "/postfail.html");
        }
    } else {
        snprintf(response_uri, response_uri_len, "/post404.html");    
    } 

    guard.Enter();
    g_contexts.erase(connection);
}