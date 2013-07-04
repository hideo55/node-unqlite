/*!
 * node-unqlite
 * Copyright(c) 2013 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "node_unqlite.h"

using namespace v8;
using namespace node;

void NodeUnQLite::Init(Handle<Object> target) {
    HandleScope scope;

    Local < FunctionTemplate > t = FunctionTemplate::New(New);
    t->SetClassName(String::NewSymbol("UnQLite"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "get", FetchKV);
    NODE_SET_PROTOTYPE_METHOD(t, "set", StoreKV);
    NODE_SET_PROTOTYPE_METHOD(t, "append", AppendKV);
    NODE_SET_PROTOTYPE_METHOD(t, "delete", DeleteKV);

    Persistent < Function > constructor = Persistent < Function > ::New(__GET_ISOLATE_FOR_NEW t->GetFunction());
    target->Set(String::NewSymbol("UnQLite"), constructor);
}

NodeUnQLite::NodeUnQLite() : db_(NULL), filename_("") {
}

NodeUnQLite::~NodeUnQLite(){
    if(db_ != NULL){
        unqlite_close(db_);
    }
}

void Initialize(Handle<Object> target) {
    NodeUnQLite::Init(target);

    // define constants
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_OK, "OK");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_NOMEM, "NOMEM");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_ABORT, "ABORT");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_IOERR, "IOERR");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_CORRUPT, "CORRUPT");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_LOCKED, "LOCKED");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_BUSY, "BUSY");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_DONE, "DONE");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_PERM, "PERM");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_NOTIMPLEMENTED, "NOTIMPLEMENTED");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_NOTFOUND, "NOTFOUND");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_NOOP, "NOOP");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_INVALID, "INVALID");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_EOF, "EOF");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_UNKNOWN, "UNKNOWN");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_LIMIT, "LIMIT");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_EXISTS, "EXISTS");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_EMPTY, "EMPTY");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_COMPILE_ERR, "COMPILE_ERR");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_VM_ERR, "VM_ERR");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_FULL, "FULL");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_CANTOPEN, "CANTOPEN");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_READ_ONLY, "READ_ONLY");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_LOCKERR, "LOCKERR");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_CURSOR_MATCH_EXACT, "CURSOR_MATCH_EXACT");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_CURSOR_MATCH_LE, "CURSOR_MATCH_LE");
    DEFINE_CONSTANT_INTEGER(target, UNQLITE_CURSOR_MATCH_GE, "CURSOR_MATCH_GE");
}

NODE_MODULE(unqlite, Initialize)
