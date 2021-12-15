#include <napi.h>
#include "nertc_node_engine.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return nertc_node::NertcNodeEngine::Init(env, exports);
}

NODE_API_MODULE(nertc, Init)
