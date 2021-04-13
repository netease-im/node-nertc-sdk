#include "nertc_node_ext.h"
#include "nertc_node_engine.h"
using v8::Object;

void InitNertc(Local<Object> module)
{
    nertc_node::NertcNodeEngine::InitModule(module);
}

NODE_MODULE(nertc, InitNertc)
