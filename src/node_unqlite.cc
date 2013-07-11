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

NodeUnQLite::NodeUnQLite() : db_(NULL), open(false) {
}

NodeUnQLite::~NodeUnQLite() {
    if (db_ != NULL) {
        unqlite_close(db_);
    }
}

Handle<Value> NodeUnQLite::New(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0)
    std::string filename = *String::Utf8Value(args[0]->ToString());

    int pos = 1;

    int mode;
    if (args.Length() >= pos && args[pos]->IsInt32()) {
        mode = args[pos++]->Int32Value();
    } else {
        mode = UNQLITE_OPEN_CREATE;
    }

    Local < Function > callback;
    if (args.Length() >= pos && args[pos]->IsFunction()) {
        callback = Local < Function > ::Cast(args[pos++]);
    }

    NodeUnQLite* uql = new NodeUnQLite();
    uql->Wrap(args.Holder());
    args.This()->Set(String::NewSymbol("filename"), args[0]->ToString(), ReadOnly);
    args.This()->Set(String::NewSymbol("mode"), Integer::New(mode), ReadOnly);

    OpenBaton* baton = new OpenBaton(uql, callback, filename.c_str(), mode);
    uv_queue_work(uv_default_loop(), &baton->request, Work_Open, Work_AfterOpen);

    return scope.Close(args.This());
}

void NodeUnQLite::Work_Open(uv_work_t* req) {
    OpenBaton* baton = static_cast<OpenBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    baton->status  = unqlite_open(&(uql->db_), baton->filename.c_str(), baton->mode);
    if(baton->status != UNQLITE_OK){
        uql->db_ = NULL;
        unqlite_close(uql->db_);
    }
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void NodeUnQLite::Work_AfterOpen(uv_work_t* req, int status) {
#else
void NodeUnQLite::Work_AfterOpen(uv_work_t* req){
#endif
    HandleScope scope;
    OpenBaton* baton = static_cast<OpenBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    Local<Value> argv[1];
    if (baton->status != UNQLITE_OK) {
        Local<Value> exception = Exception::Error(Integer::New(baton->status)->ToString());
        argv[0] = exception;
    }
    else {
        uql->open = true;
        argv[0] = Local<Value>::New(Null());
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(uql->handle_, baton->callback, 1, argv);
    }
    else if (!uql->open) {
        Local<Value> args[] = { String::NewSymbol("error"), argv[0] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }

    if (uql->open) {
        Local<Value> args[] = { String::NewSymbol("open") };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 1, args);
    }

    delete baton;
}

Handle<Value> NodeUnQLite::FetchKV(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(args[0]->ToString());

    int pos = 1;

    Local < Function > callback;
    if (args.Length() >= pos && args[pos]->IsFunction()) {
        callback = Local < Function > ::Cast(args[pos++]);
    }

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(args.Holder());

    ExecBaton* baton = new ExecBaton(uql, callback, key);
    uv_queue_work(uv_default_loop(), &baton->request, Work_FetchKV, Work_AfterFetchKV);

    return scope.Close(args.This());
}

void NodeUnQLite::Work_FetchKV(uv_work_t* req) {
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;
    std::string& key = baton->key;
    unqlite_int64 val_size;

    if(!uql->open){
        return;
    }

    baton->status  = unqlite_kv_fetch(uql->db_, key.c_str(), key.size(), NULL, &val_size);
    if(baton->status != UNQLITE_OK){
        char* buf = new char[val_size];
        baton->status  = unqlite_kv_fetch(uql->db_, key.c_str(), key.size(), buf, &val_size);
        baton->value.assign(buf, val_size);
        delete[] buf;
    }
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void NodeUnQLite::Work_AfterFetchKV(uv_work_t* req, int status) {
#else
void NodeUnQLite::Work_AfterFetchKV(uv_work_t* req){
#endif
    HandleScope scope;
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    Local<Value> argv[2];
    bool hasError =false;

    if(!uql->open){
        Local<Value> exception = Exception::Error(String::New("Not opened"));
        argv[0] = exception;
        argv[1] = Local<Value>::New(Null());
        hasError = true;
    }
    else if (baton->status != UNQLITE_OK) {
        Local<Value> exception = Exception::Error(Integer::New(baton->status)->ToString());
        argv[0] = exception;
        argv[1] = Local<Value>::New(Null());
        hasError = true;
    }
    else {
        argv[0] = Local<Value>::New(Null());
        argv[1] = Local<Value>::New(String::New(baton->value.c_str(), baton->value.size()));
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(uql->handle_, baton->callback, 2, argv);
    }
    else if (hasError) {
        Local<Value> args[] = { String::NewSymbol("error"), argv[0] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }else{
        Local<Value> args[] = { String::NewSymbol("fetch"), argv[1] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }

    delete baton;
}

Handle<Value> NodeUnQLite::StoreKV(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(args[0]->ToString());
    REQ_STR_ARG(1)
    std::string value = *String::Utf8Value(args[0]->ToString());

    int pos = 2;

    Local < Function > callback;
    if (args.Length() >= pos && args[pos]->IsFunction()) {
        callback = Local < Function > ::Cast(args[pos++]);
    }

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(args.Holder());

    ExecBaton* baton = new ExecBaton(uql, callback, key, value);
    uv_queue_work(uv_default_loop(), &baton->request, Work_StoreKV, Work_AfterStoreKV);

    return scope.Close(args.This());
}

void NodeUnQLite::Work_StoreKV(uv_work_t* req) {
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;
    std::string& key = baton->key;
    std::string& val = baton->value;

    if(!uql->open){
        return;
    }

    baton->status  = unqlite_kv_store(uql->db_, key.c_str(), key.size(), val.c_str(), val.size());
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void NodeUnQLite::Work_AfterStoreKV(uv_work_t* req, int status) {
#else
void NodeUnQLite::Work_AfterStoreKV(uv_work_t* req){
#endif
    HandleScope scope;
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    Local<Value> argv[1];
    bool hasError =false;

    if(!uql->open){
        Local<Value> exception = Exception::Error(String::New("Not opened"));
        argv[0] = exception;
        hasError = true;
    }
    else if (baton->status != UNQLITE_OK) {
        Local<Value> exception = Exception::Error(Integer::New(baton->status)->ToString());
        argv[0] = exception;
        hasError = true;
    }
    else {
        argv[0] = Local<Value>::New(Null());
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(uql->handle_, baton->callback, 1, argv);
    }
    else if (hasError) {
        Local<Value> args[] = { String::NewSymbol("error"), argv[0] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }else{
        Local<Value> args[] = { String::NewSymbol("store") };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 1, args);
    }

    delete baton;
}

Handle<Value> NodeUnQLite::AppendKV(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(args[0]->ToString());
    REQ_STR_ARG(1)
    std::string value = *String::Utf8Value(args[0]->ToString());

    int pos = 2;

    Local < Function > callback;
    if (args.Length() >= pos && args[pos]->IsFunction()) {
        callback = Local < Function > ::Cast(args[pos++]);
    }

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(args.Holder());

    ExecBaton* baton = new ExecBaton(uql, callback, key, value);
    uv_queue_work(uv_default_loop(), &baton->request, Work_StoreKV, Work_AfterStoreKV);

    return scope.Close(args.This());
}

void NodeUnQLite::Work_AppendKV(uv_work_t* req) {
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;
    std::string& key = baton->key;
    std::string& val = baton->value;

    if(!uql->open){
        return;
    }

    baton->status  = unqlite_kv_append(uql->db_, key.c_str(), key.size(), val.c_str(), val.size());
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void NodeUnQLite::Work_AfterAppendKV(uv_work_t* req, int status) {
#else
void NodeUnQLite::Work_AfterAppendKV(uv_work_t* req){
#endif
    HandleScope scope;
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    Local<Value> argv[1];
    bool hasError =false;

    if(!uql->open){
        Local<Value> exception = Exception::Error(String::New("Not opened"));
        argv[0] = exception;
        hasError = true;
    }
    else if (baton->status != UNQLITE_OK) {
        Local<Value> exception = Exception::Error(Integer::New(baton->status)->ToString());
        argv[0] = exception;
        hasError = true;
    }
    else {
        argv[0] = Local<Value>::New(Null());
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(uql->handle_, baton->callback, 1, argv);
    }
    else if (hasError) {
        Local<Value> args[] = { String::NewSymbol("error"), argv[0] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }else{
        Local<Value> args[] = { String::NewSymbol("append") };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 1, args);
    }

    delete baton;
}

Handle<Value> NodeUnQLite::DeleteKV(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(args[0]->ToString());

    int pos = 1;

    Local < Function > callback;
    if (args.Length() >= pos && args[pos]->IsFunction()) {
        callback = Local < Function > ::Cast(args[pos++]);
    }

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(args.Holder());

    ExecBaton* baton = new ExecBaton(uql, callback, key);
    uv_queue_work(uv_default_loop(), &baton->request, Work_DeleteKV, Work_AfterDeleteKV);

    return scope.Close(args.This());
}

void NodeUnQLite::Work_DeleteKV(uv_work_t* req) {
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;
    std::string& key = baton->key;

    if(!uql->open){
        return;
    }

    baton->status  = unqlite_kv_delete(uql->db_, key.c_str(), key.size());
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void NodeUnQLite::Work_AfterDeleteKV(uv_work_t* req, int status) {
#else
void NodeUnQLite::Work_AfterDeleteKV(uv_work_t* req){
#endif
    HandleScope scope;
    ExecBaton* baton = static_cast<ExecBaton*>(req->data);
    NodeUnQLite* uql = baton->unqlite;

    Local<Value> argv[1];
    bool hasError =false;

    if(!uql->open){
        Local<Value> exception = Exception::Error(String::New("Not opened"));
        argv[0] = exception;
        hasError = true;
    }
    else if (baton->status != UNQLITE_OK) {
        Local<Value> exception = Exception::Error(Integer::New(baton->status)->ToString());
        argv[0] = exception;
        hasError = true;
    }
    else {
        argv[0] = Local<Value>::New(Null());
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(uql->handle_, baton->callback, 1, argv);
    }
    else if (hasError) {
        Local<Value> args[] = { String::NewSymbol("error"), argv[0] };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 2, args);
    }else{
        Local<Value> args[] = { String::NewSymbol("delete") };
        TRY_CATCH_CALL(uql->handle_, Local<Function>::Cast((uql->handle_)->Get(String::NewSymbol("emit"))), 1, args);
    }

    delete baton;
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

NODE_MODULE(node_unqlite, Initialize)
