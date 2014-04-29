#include "node_unqlite_async.h"
#include "unqlite.h"

namespace node_unqlite {

// OpenWorker
OpenWorker::OpenWorker(NanCallback *callback, NodeUnQLite* uql, std::string& filename, int mode) :
        NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), filename_(filename), mode_(mode) {
}

void OpenWorker::Execute() {
    status_ = unqlite_->open_db(filename_.c_str(), mode_);
    if (status_ != UNQLITE_OK) {
        std::stringstream ss;
        ss << "Failed to open: " << status_ << std::endl;
        errmsg = new char[ss.str().size() + 1];
        std::memcpy((void*) errmsg, ss.str().c_str(), ss.str().size());
    }
}

void OpenWorker::HandleOKCallback() {
    NanScope();
    v8::Local<v8::Value> argv[] = {NanNewLocal(v8::Null())};
    callback->Call(1, argv);
}

// CloseWorker
// OpenWorker
CloseWorker::CloseWorker(NanCallback *callback, NodeUnQLite* uql) :
        NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK) {
}

void CloseWorker::Execute() {
    status_ = unqlite_->close_db();
    if (status_ != UNQLITE_OK) {
        std::stringstream ss;
        ss << "Failed to close: " << status_ << std::endl;
        errmsg = new char[ss.str().size() + 1];
        std::memcpy((void*) errmsg, ss.str().c_str(), ss.str().size());
    }
}

void CloseWorker::HandleOKCallback() {
    NanScope();
    v8::Local<v8::Value> argv[] = {NanNewLocal(v8::Null())};
    callback->Call(1, argv);
}

// AccessWorker

AccessWorker::AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key) :
        NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), type_(type), key_(key), value_() {
}

AccessWorker::AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key,
        std::string value) :
        NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), type_(type), key_(key), value_(value) {
}

void AccessWorker::Execute() {
    if (!unqlite_->is_opened()) {
        std::string err = "Database not opened.";
        errmsg = new char[err.size() + 1];
        std::memcpy((void*) errmsg, err.c_str(), err.size());
        return;
    }

    switch (type_) {
        case T_UNQLITE_FETCH:
            status_ = unqlite_->fetch_kv(key_, value_);
            setError("fetch");
            break;
        case T_UNQLITE_STORE:
            status_ = unqlite_->store_kv(key_, value_);
            setError("store");
            break;
        case T_UNQLITE_APPEND:
            status_ = unqlite_->append_kv(key_, value_);
            setError("append");
            break;
        case T_UNQLITE_DELETE:
            status_ = unqlite_->delete_kv(key_);
            setError("delete");
            break;
        default:
            break;
    };
}

void AccessWorker::HandleOKCallback() {
    NanScope();
    v8::Local <v8::Value> argv[] = {
        NanNewLocal(v8::Null()),
        NanNewLocal(v8::String::New(key_.c_str(), key_.size())),
        NanNewLocal(v8::String::New(value_.c_str(), value_.size()))
    };
    callback->Call(3, argv);
}

void AccessWorker::setError(const char* type) {
    if (status_ != UNQLITE_OK) {
        std::stringstream ss;
        ss << "Failed to " << type << ": " << status_ << std::endl;
        errmsg = new char[ss.str().size() + 1];
        std::memcpy((void*) errmsg, ss.str().c_str(), ss.str().size());
    }
}

} // namespace unqlite
