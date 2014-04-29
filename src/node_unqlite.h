#if !defined(NODE_UNQLITE_H)
#define NODE_UNQLITE_H

#include "nan.h"

#include <string>
#include <sstream>
#include "macro.h"
#include <memory>

extern "C" {
#include "unqlite.h"
}

class NodeUnQLite: public node::ObjectWrap {
public:

    NodeUnQLite();
    virtual ~NodeUnQLite();

    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD (New);
    static NAN_METHOD (Open);
    static NAN_METHOD (StoreKV);
    static NAN_METHOD (AppendKV);
    static NAN_METHOD (DeleteKV);
    static NAN_METHOD (FetchKV);

private:

    typedef enum {
        T_UNQLITE_FETCH, T_UNQLITE_STORE, T_UNQLITE_APPEND, T_UNQLITE_DELETE
    } UnQLiteAccessType;

    class OpenWorker: public NanAsyncWorker {
    public:
        /**
         * @brief Constructor
         * @param callback[in] Callback functio object
         * @param uql[in] NodeUnQLite instance
         * @param filename[in] Name of the file to open
         * @param mode[in] UnQLite open mode
         */
        OpenWorker(NanCallback *callback, NodeUnQLite* uql, std::string& filename, int mode) :
                NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), filename_(filename), mode_(mode) {
        }

        /**
         * @brief Open UnQLite file
         */
        void Execute() {
            status_ = unqlite_open(&(unqlite_->db_), filename_.c_str(), mode_);
            if (status_ != UNQLITE_OK) {
                unqlite_close(unqlite_->db_);
                unqlite_->db_ = NULL;
                std::stringstream ss;
                ss << "Failed to open: " << status_ << std::endl;
                errmsg = ss.str().c_str();
            } else {
                unqlite_->open_ = true;
            }
        }

        /**
         * @brief Invoke callback function
         */
        void HandleOKCallback() {
            NanScope();
            v8::Local<v8::Value> argv[] = { NanNewLocal(v8::Null()) };
            callback->Call(1, argv);
        }

    private:
        NodeUnQLite* unqlite_;  /// NodeUnQLite instance
        int status_;            /// File open status
        std::string filename_;  /// File name
        int mode_;              /// Open mode
    };

    class AccessWorker: public NanAsyncWorker {
    public:
        /**
         * @brief Constructor
         * @param callback[in] Callback functio object
         * @param uql[in] NodeUnQLite instance
         * @param type[in] Access type
         * @param key[in] Key
         */
        AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key) :
                NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), type_(type), key_(key), value_() {
        }

        /**
         * @brief Constructor
         * @param callback[in] Callback functio object
         * @param uql[in] NodeUnQLite instance
         * @param type[in] Access type
         * @param key[in] Key
         * @param value[in] Value
         */
        AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key,
                std::string value) :
                NanAsyncWorker(callback), unqlite_(uql), status_(UNQLITE_OK), type_(type), key_(key), value_(value) {
        }

        /**
         * @brief Access to the UnQLite DB
         */
        void Execute() {
            if (!unqlite_->open_) {
                errmsg = "Not opened";
                return;
            }

            switch (type_) {
                case T_UNQLITE_FETCH:
                    Fetch();
                    break;
                case T_UNQLITE_STORE:
                    Store();
                    break;
                case T_UNQLITE_APPEND:
                    Append();
                    break;
                case T_UNQLITE_DELETE:
                    Delete();
                    break;
                default:
                    break;
            }
        }

        /**
         * @brief Invoke callback function
         */
        void HandleOKCallback() {
            NanScope();
            v8::Local <v8::Value> argv[] = {
                    NanNewLocal(v8::Null()),
                    NanNewLocal(v8::String::New(key_.c_str(), key_.size())),
                    NanNewLocal(v8::String::New(value_.c_str(), value_.size()))
            };
            callback->Call(3, argv);
        }

    private:
        NodeUnQLite* unqlite_;
        int status_;
        UnQLiteAccessType type_;
        std::string key_;
        std::string value_;

        void Fetch() {
            unqlite_int64 val_size;
            status_ = unqlite_kv_fetch(unqlite_->db_, key_.c_str(), key_.size(), NULL, &val_size);
            if (status_ == UNQLITE_OK) {
                char* buf = new char[val_size];
                status_ = unqlite_kv_fetch(unqlite_->db_, key_.c_str(), key_.size(), buf, &val_size);
                value_.assign(buf, val_size);
                delete[] buf;
            }
            setError("fetch");
        }

        void Store() {
            status_ = unqlite_kv_store(unqlite_->db_, key_.c_str(), key_.size(), value_.c_str(), value_.size());
            setError("store");

        }

        void Append() {
            status_ = unqlite_kv_append(unqlite_->db_, key_.c_str(), key_.size(), value_.c_str(), value_.size());
            setError("append");
        }

        void Delete() {
            key_ = "hoge";
            unqlite_kv_store(unqlite_->db_, key_.c_str(), key_.size(), value_.c_str(), value_.size());
            status_ = unqlite_kv_delete(unqlite_->db_, key_.c_str(), key_.size());
            value_ = "";
            setError("delete");
        }

        void setError(const char* type) {
            if (status_ != UNQLITE_OK) {
                std::stringstream ss;
                ss << "Failed to " << type << ": " << status_ << std::endl;
                errmsg = new char[ss.str().size() + 1];
                std::memcpy((void*)errmsg, ss.str().c_str(), ss.str().size());
            }
        }
    };

    static v8::Persistent<v8::FunctionTemplate> constructor_template;
    unqlite* db_;
    bool open_;
};

#endif /* NODE_UNQLITE_H */
