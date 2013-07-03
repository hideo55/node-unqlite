/*!
 * node-unqlite
 * Copyright(c) 2013 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "node_unqlite.h"

void NodeUnQLite::Init(Handle<Object> target) {
    HandleScope scope;
    Local < FunctionTemplate > t = FunctionTemplate::New(New);
    t->SetClassName(String::NewSymbol("UnQLite"));
    t->InstanceTemplate()->SetInternalFieldCount(5);

    NODE_SET_PROTOTYPE_METHOD(t, "get", FetchKV);
    NODE_SET_PROTOTYPE_METHOD(t, "set", StoreKV);
    NODE_SET_PROTOTYPE_METHOD(t, "append", AppendKV);
    NODE_SET_PROTOTYPE_METHOD(t, "delete", DeleteKV);

    Persistent < Function > constructor = Persistent < Function > ::New(__GET_ISOLATE_FOR_NEW t->GetFunction());
    target->Set(String::NewSymbol("Database"), constructor);
}

void Initialize(Handle<Object> target) {
    NodeUnQLite::Init(target);
}

NODE_MODULE(unqlite, Initialize)
