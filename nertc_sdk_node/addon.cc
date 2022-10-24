#include <napi.h>
#include "nertc_node_engine.h"
#include "nertc_node_channel.h"


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  nertc_node::NertcNodeEngine::Init(env, exports);
  nertc_node::NertcNodeChannel::Init(env, exports);
  return exports;
}

NODE_API_MODULE(nertc, Init)
