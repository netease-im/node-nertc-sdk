#include "nertc_node_ext.h"
#include "nertc_node_engine.h"
using v8::Object;

// void InitNertc(Local<Object> module)
// {
//     nertc_node::NertcNodeEngine::InitModule(module);
// }
void InitNertc(Local<Object> exports,
                        Local<Value> module,
                        Local<Context> context) {
    nertc_node::NertcNodeEngine::InitModule(exports, module, context);
}

NODE_MODULE_CONTEXT_AWARE(nertc, InitNertc)

