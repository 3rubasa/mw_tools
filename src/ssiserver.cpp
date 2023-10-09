#include "include/interfaces.h"
#include "include/ssiserver.h"

#include <lwip/apps/httpd.h>
#include <pico/cyw43_arch.h>


#include <stdexcept>
#include <memory>
#include <map>

#define RESPONSE_PART_SIZE LWIP_HTTPD_MAX_TAG_INSERT_LEN

namespace mw_tools {
namespace servers {

static std::shared_ptr<ISSIServer> g_server;

struct Context {
    std::string output;
};

extern "C" void* fs_state_init(struct fs_file *file, const char *name)
{
    printf("fs_state_init called: %s\n", name);
    LWIP_UNUSED_ARG(file);
    LWIP_UNUSED_ARG(name);
    void* ret = new Context();
    if (!ret) {
        printf("Failed to allocate memory\n");
    }
    return ret;
}

extern "C" void fs_state_free(struct fs_file *file, void *state)
{
  LWIP_UNUSED_ARG(file);
  printf("fs_state_free called: %d", *((char*)state));
  if (state != NULL) {
    delete (Context *)state;
  }
}

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen, u16_t part, u16_t* next_part, void* connection_state) 
{
    printf("Handler called\n");
    printf("part=%d\n", part);
    printf("nextpart=%d\n", *next_part);
    printf("index = %d\n", iIndex);
    
    Context* ctx = (Context*)connection_state;

    if (part == 0) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); // indiate first part
        std::shared_ptr<ISSIHandler> handler = g_server->GetHandler(iIndex);
        ctx->output = handler->ProcessRequest();
    }

    char toPrint[RESPONSE_PART_SIZE + 1];

    if ((u16_t)(ctx->output.length()) > RESPONSE_PART_SIZE * (part + 1)) {
        *next_part = part+1;
        ctx->output.copy(toPrint, RESPONSE_PART_SIZE, RESPONSE_PART_SIZE * part);
        toPrint[RESPONSE_PART_SIZE]=0;
    } else {
        ctx->output.copy(toPrint, ctx->output.length()-part*RESPONSE_PART_SIZE, part*RESPONSE_PART_SIZE);
        toPrint[ctx->output.length()-part*RESPONSE_PART_SIZE] =0;
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0); // indicate last part
    }

    size_t printed = snprintf(pcInsert, iInsertLen, toPrint);
  
    printf("IsertLen=%d\n", iInsertLen);
    return (u16_t)printed;
    
    return 0;
}

SSIServer::SSIServer() {
}

void SSIServer::RegisterHandler(const std::string& tag, std::shared_ptr<ISSIHandler> handler) {
    if (tag.length() > 8) {
        printf("Tag %s is longer than 8 chars!", tag.c_str());
        return;
    }
    
    m_handlers[tag] = handler;
    
    m_ssi_tags[m_handlers.size() - 1] = new char[tag.length() + 1];
    strcpy(m_ssi_tags[m_handlers.size() - 1], tag.c_str());
    m_ssi_tags[m_handlers.size() - 1][tag.length()] = 0;
    printf("Added to table: %s", m_ssi_tags[m_handlers.size() - 1]);
}

std::shared_ptr<ISSIHandler> SSIServer::GetHandler(const std::string& tag) {
    if (m_handlers.find(tag) == m_handlers.end()) {
        throw std::runtime_error("handler not found");
    }

    return m_handlers.at(tag);
}

std::shared_ptr<ISSIHandler> SSIServer::GetHandler(int idx) {
    if (idx >= static_cast<int>(m_handlers.size())) {
        throw std::runtime_error("tag index is invalid");
    }

    return GetHandler(std::string{m_ssi_tags[idx]});
}

void SSIServer::Start() {
    g_server = shared_from_this();

    http_set_ssi_handler(ssi_handler, (const char **)m_ssi_tags, m_handlers.size());
    printf("SSI Initialized\n");
}

}}