#include "node_unqlite_async.h"
#include "unqlite.h"

namespace node_unqlite {

UnQLiteAsyncWorker::UnQLiteAsyncWorker(NanCallback *callback, NodeUnQLite* uql) :
        NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK) {
}

void UnQLiteAsyncWorker::set_error_message(const char* message) {
    std::string errLog;
    unqlite_->get_error_message(errLog);
    std::stringstream ss;
    ss << message << " : ";
    if (errLog.size() > 0) {
        ss << errLog;
    } else {
        ss << status_;
    }
    errmsg = strndup(ss.str().c_str(), ss.str().size());
}

// OpenWorker
OpenWorker::OpenWorker(NanCallback *callback, NodeUnQLite* uql, std::string& filename, int mode) :
        UnQLiteAsyncWorker(callback, uql), filename_(filename), mode_(mode) {
}

void OpenWorker::Execute() {
    status_ = unqlite_->open_db(filename_.c_str(), mode_);
    if (status_ != UNQLITE_OK) {
        set_error_message("Failed to open");
    }
}

void OpenWorker::HandleOKCallback() {
    NanScope();
    v8::Local<v8::Value> argv[] = {NanNewLocal(v8::Null())};
    callback->Call(1, argv);
}

// CloseWorker
CloseWorker::CloseWorker(NanCallback *callback, NodeUnQLite* uql) :
        UnQLiteAsyncWorker(callback, uql) {
}

void CloseWorker::Execute() {
    status_ = unqlite_->close_db();
    if (status_ != UNQLITE_OK) {
        set_error_message("Failed to close");
    }
}

void CloseWorker::HandleOKCallback() {
    NanScope();
    v8::Local<v8::Value> argv[] = {NanNewLocal(v8::Null())};
    callback->Call(1, argv);
}

// AccessWorker

AccessWorker::AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key) :
        UnQLiteAsyncWorker(callback, uql), type_(type), key_(key), value_() {
}

AccessWorker::AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key,
        std::string value) :
        UnQLiteAsyncWorker(callback, uql), type_(type), key_(key), value_(value) {
}

void AccessWorker::Execute() {
    if (!unqlite_->is_opened()) {
        std::string err = "Database not opened.";
        errmsg = strndup(err.c_str(), err.size());
        return;
    }

    switch (type_) {
        case T_UNQLITE_FETCH:
            status_ = unqlite_->fetch_kv(key_, value_);
            set_error_message("fetch");
            break;
        case T_UNQLITE_STORE:
            status_ = unqlite_->store_kv(key_, value_);
            set_error_message("store");
            break;
        case T_UNQLITE_APPEND:
            status_ = unqlite_->append_kv(key_, value_);
            set_error_message("append");
            break;
        case T_UNQLITE_DELETE:
            status_ = unqlite_->delete_kv(key_);
            set_error_message("delete");
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

void AccessWorker::set_error_message(const char* type) {
    if (status_ != UNQLITE_OK) {
        std::stringstream ss;
        ss << "Failed to " << type;
        UnQLiteAsyncWorker::set_error_message(ss.str().c_str());
    }
}

} // namespace node_unqlite
