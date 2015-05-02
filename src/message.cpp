#include "message.hpp"

#include <wild/FreeList.hpp>
#include <wild/SpinLock.hpp>
#include <wild/with_lock.hpp>

#include <cstdlib>

namespace {

using namespace stp;

wild::SpinLock gMutex;
wild::FreeList<Message> gFreeList;

thread_local wild::FreeList<Message> tFreeList;

Message *
allocMessage() {
    auto m = tFreeList.take();
    if (m == nullptr) {
        WITH_LOCK(gMutex) {
            m = gFreeList.take();
        }
        if (m == nullptr) {
            m = static_cast<Message*>(malloc(sizeof(Message)));
        }
    }
    return m;
}

void
deallocMessage(Message *m) {
    if (tFreeList.size() >= 10000) {
        WITH_LOCK(gMutex) {
            gFreeList.push(m);
        }
        return;
    }
    tFreeList.push(m);
}

}

namespace stp {
namespace message {

Message *create(process_t source, session_t session, message::Content content) {
    auto m = allocMessage();
    return new (m) Message{source, session, content};
}

void destroy(Message *msg) {
    msg->~Message();
    deallocMessage(msg);
}

}
}
