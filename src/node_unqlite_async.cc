#include "node_unqlite_async.h"
#include "unqlite.h"

namespace node_unqlite {

UnQLiteAsyncWorker::UnQLiteAsyncWorker(Nan::Callback *callback, NodeUnQLite* uql) :
        Nan::AsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK) {
}

void UnQLiteAsyncWorker::SetErrorMessage(const char* message) {
    if (status_ != UNQLITE_OK) {
        std::string errLog;
        unqlite_->get_error_message(errLog);
        std::stringstream ss;
        ss << message << " : ";
        if (errLog.size() > 0) {
            ss << errLog;
        } else {
            ss << status_;
        }
        Nan::AsyncWorker::SetErrorMessage(ss.str().c_str());
    }
}

// OpenWorker
OpenWorker::OpenWorker(Nan::Callback *callback, NodeUnQLite* uql, std::string& filename, int mode) :
        UnQLiteAsyncWorker(callback, uql), filename_(filename), mode_(mode) {
}

void OpenWorker::Execute() {
    status_ = unqlite_->open_db(filename_.c_str(), mode_);
    SetErrorMessage("Failed to open");
}

void OpenWorker::HandleOKCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {Nan::Null()};
    callback->Call(1, argv);
}

// CloseWorker
CloseWorker::CloseWorker(Nan::Callback *callback, NodeUnQLite* uql) :
        UnQLiteAsyncWorker(callback, uql) {
}

void CloseWorker::Execute() {
    status_ = unqlite_->close_db();
    SetErrorMessage("Failed to close");
}

void CloseWorker::HandleOKCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {Nan::Null()};
    callback->Call(1, argv);
}

// AccessWorker

AccessWorker::AccessWorker(Nan::Callback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key) :
        UnQLiteAsyncWorker(callback, uql), type_(type), key_(key), value_() {
}

AccessWorker::AccessWorker(Nan::Callback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key,
        std::string value) :
        UnQLiteAsyncWorker(callback, uql), type_(type), key_(key), value_(value) {
}

void AccessWorker::Execute() {
    if (!unqlite_->is_opened()) {
        std::string err = "Database not opened.";
        Nan::AsyncWorker::SetErrorMessage(err.c_str());
        return;
    }

    switch (type_) {
        case T_UNQLITE_FETCH:
            status_ = unqlite_->fetch_kv(key_, value_);
            SetErrorMessage("Failed to fetch");
            break;
        case T_UNQLITE_STORE:
            status_ = unqlite_->store_kv(key_, value_);
            SetErrorMessage("Failed to store");
            break;
        case T_UNQLITE_APPEND:
            status_ = unqlite_->append_kv(key_, value_);
            SetErrorMessage("Failed to append");
            break;
        case T_UNQLITE_DELETE:
            status_ = unqlite_->delete_kv(key_);
            SetErrorMessage("Failed to delete");
            break;
        default:
            break;
    };
}

void AccessWorker::HandleOKCallback() {
    Nan::HandleScope scope;
    v8::Local <v8::Value> argv[] = {
        Nan::Null(),
        Nan::New<v8::String>(key_.c_str(), key_.size()).ToLocalChecked(),
        Nan::New<v8::String>(value_.c_str(), value_.size()).ToLocalChecked()
    };
    callback->Call(3, argv);
}

} // namespace node_unqlite
