/*!
 * node-unqlite
 * Copyright(c) 2013 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "node_unqlite.h"
#include "node_unqlite_async.h"
#include <iostream>

using namespace v8;
using namespace node;

namespace node_unqlite {

Nan::Persistent<FunctionTemplate> NodeUnQLite::constructor_template;

void NodeUnQLite::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    Local < FunctionTemplate > t = Nan::New<FunctionTemplate>(NodeUnQLite::New);
    constructor_template.Reset(t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(Nan::New<String>("Database").ToLocalChecked());

    Nan::SetPrototypeMethod(t, "open", Open);
    Nan::SetPrototypeMethod(t, "close", Close);
    Nan::SetPrototypeMethod(t, "fetch", FetchKV);
    Nan::SetPrototypeMethod(t, "store", StoreKV);
    Nan::SetPrototypeMethod(t, "append", AppendKV);
    Nan::SetPrototypeMethod(t, "delete", DeleteKV);

    exports->Set(Nan::New<String>("Database").ToLocalChecked(), t->GetFunction());
}

NodeUnQLite::NodeUnQLite() :
        db_(NULL), open_(false) {
}

NodeUnQLite::~NodeUnQLite() {
    if (db_ != NULL) {
        close_db();
    }
}

NAN_METHOD(NodeUnQLite::New){
    Nan::HandleScope scope;

    REQ_STR_ARG(0)
    std::string filename = *String::Utf8Value(info[0]->ToString());

    NodeUnQLite* uql = new NodeUnQLite();
    uql->Wrap(info.Holder());
    Nan::ForceSet(info.This(), Nan::New<String>("filename").ToLocalChecked(), info[0]->ToString(), ReadOnly);

    info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(NodeUnQLite::Open){
    Nan::HandleScope scope;

    int pos = 0;

    int mode;
    if (info.Length() >= pos && info[pos]->IsInt32()) {
        mode = info[pos++]->Int32Value();
    } else {
        mode = UNQLITE_OPEN_CREATE;
    }

    REQ_FUN_ARG(pos, cb);

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());
    Nan::ForceSet(info.This(), Nan::New<String>("mode").ToLocalChecked(), Nan::New<Integer>(mode), ReadOnly);
    std::string filename = *String::Utf8Value(info.This()->Get(Nan::New<String>("filename").ToLocalChecked()));

    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new OpenWorker(callback, uql, filename, mode));
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodeUnQLite::Close) {
    Nan::HandleScope scope;
    REQ_FUN_ARG(0, cb);
    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());
    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new CloseWorker(callback, uql));
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodeUnQLite::FetchKV){
    Nan::HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(info[0]->ToString());
    REQ_FUN_ARG(1, cb);

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());

    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new AccessWorker(callback, uql, T_UNQLITE_FETCH, key));

    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodeUnQLite::StoreKV){
    Nan::HandleScope scope;
    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(info[0]->ToString());
    REQ_STR_ARG(1)
    std::string value = *String::Utf8Value(info[1]->ToString());

    REQ_FUN_ARG(2, cb);

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());

    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new AccessWorker(callback, uql, T_UNQLITE_STORE, key, value));

    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodeUnQLite::AppendKV){
    Nan::HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(info[0]->ToString());
    REQ_STR_ARG(1)
    std::string value = *String::Utf8Value(info[1]->ToString());

    REQ_FUN_ARG(2, cb);

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());

    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new AccessWorker(callback, uql, T_UNQLITE_APPEND, key, value));

    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodeUnQLite::DeleteKV){
    Nan::HandleScope scope;

    REQ_STR_ARG(0)
    std::string key = *String::Utf8Value(info[0]->ToString());

    REQ_FUN_ARG(1, cb);

    NodeUnQLite* uql = Unwrap<NodeUnQLite>(info.Holder());

    Nan::Callback *callback = new Nan::Callback(cb);
    Nan::AsyncQueueWorker(new AccessWorker(callback, uql, T_UNQLITE_DELETE, key));

    info.GetReturnValue().SetUndefined();
}

int NodeUnQLite::open_db(const char* filename, int mode) {
    int status = unqlite_open(&(db_), filename, mode);
    if (status != UNQLITE_OK) {
        close_db();
        open_ = false;
    } else {
        open_ = true;
    }
    return status;
}

int NodeUnQLite::close_db() {
    if(!open_){
        return UNQLITE_OK;
    }
    int status = unqlite_close(db_);
    if (status == UNQLITE_OK) {
        db_ = NULL;
        open_ = false;
    }
    return status;
}

bool NodeUnQLite::is_opened() {
    return open_;
}

int NodeUnQLite::fetch_kv(std::string& key, std::string& value) {
    unqlite_int64 val_size;
    int status = unqlite_kv_fetch(db_, key.c_str(), key.size(), NULL, &val_size);
    if (status == UNQLITE_OK) {
        char* buf = new char[val_size];
        status = unqlite_kv_fetch(db_, key.c_str(), key.size(), buf, &val_size);
        value.assign(buf, val_size);
        delete[] buf;
    }
    return status;
}

int NodeUnQLite::store_kv(std::string& key, std::string& value) {
    return unqlite_kv_store(db_, key.c_str(), key.size(), value.c_str(), value.size());
}

int NodeUnQLite::append_kv(std::string& key, std::string& value) {
    return unqlite_kv_append(db_, key.c_str(), key.size(), value.c_str(), value.size());
}

int NodeUnQLite::delete_kv(std::string& key) {
    return unqlite_kv_delete(db_, key.c_str(), key.size());
}

void NodeUnQLite::get_error_message(std::string& message) {
    const char* buf;
    int bufLen;
    unqlite_config(db_, UNQLITE_CONFIG_ERR_LOG, &buf, &bufLen);
    if(bufLen > 0){
        message.assign(buf, bufLen);
    }
}

} // namespace node_unqlite

void Initialize(Handle<Object> exports) {
    node_unqlite::NodeUnQLite::Init(exports);

    // define constants
    // status code
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OK, OK);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_NOMEM, NOMEM);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_ABORT, ABORT);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_IOERR, IOERR);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_CORRUPT, CORRUPT);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_LOCKED, LOCKED);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_BUSY, BUSY);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_DONE, DONE);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_PERM, PERM);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_NOTIMPLEMENTED, NOTIMPLEMENTED);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_NOTFOUND, NOTFOUND);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_NOOP, NOOP);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_INVALID, INVALID);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_EOF, EOF);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_UNKNOWN, UNKNOWN);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_LIMIT, LIMIT);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_EXISTS, EXISTS);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_EMPTY, EMPTY);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_COMPILE_ERR, COMPILE_ERR);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_VM_ERR, VM_ERR);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_FULL, FULL);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_CANTOPEN, CANTOPEN);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_READ_ONLY, READ_ONLY);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_LOCKERR, LOCKERR);

    // Cursor seek position
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_CURSOR_MATCH_EXACT, CURSOR_MATCH_EXACT);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_CURSOR_MATCH_LE, CURSOR_MATCH_LE);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_CURSOR_MATCH_GE, CURSOR_MATCH_GE);

    // open mode
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_CREATE, OPEN_CREATE);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_READWRITE, OPEN_READWRITE);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_READONLY, OPEN_READONLY);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_MMAP, OPEN_MMAP);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_EXCLUSIVE, OPEN_EXCLUSIVE);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_TEMP_DB, OPEN_TEMP);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_IN_MEMORY, OPEN_IN_MEMORY);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_OMIT_JOURNALING, OPEN_OMIT_JOURNALING);
    DEFINE_CONSTANT_INTEGER(exports, UNQLITE_OPEN_NOMUTEX, OPEN_NOMUTEX);
}

NODE_MODULE(node_unqlite, Initialize)
