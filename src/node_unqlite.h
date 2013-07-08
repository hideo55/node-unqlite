#if !defined(NODE_UNQLITE_H)
#define NODE_UNQLITE_H

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <string>
#include "macro.h"

extern "C" {
#include "unqlite.h"
}

class NodeUnQLite: node::ObjectWrap {
public:

    struct Baton {
        uv_work_t request;
        v8::Persistent<v8::Function> callback;
        int status;
        NodeUnQLite* unqlite;

        Baton(NodeUnQLite* uql, v8::Handle<v8::Function> cb) :
                status(UNQLITE_OK), unqlite(uql) {
            unqlite->Ref();
            request.data = this;
            callback = v8::Persistent < v8::Function > ::New(__GET_ISOLATE_FOR_NEW cb);
        }
        virtual ~Baton() {
            unqlite->Unref();
            callback.Dispose(__GET_ISOLATE_FOR_DISPOSE);
        }
    };

    struct OpenBaton: Baton {
        std::string filename;
        int mode;
        OpenBaton(NodeUnQLite* uql, v8::Handle<v8::Function> cb, const char* filename_, int mode_) :
                Baton(uql, cb), filename(filename_), mode(mode_) {
        }
    };

    struct ExecBaton: Baton {
        std::string key;
        ExecBaton(NodeUnQLite* uql, v8::Handle<v8::Function> cb, const char* key_) :
                Baton(uql, cb), key(key_) {
        }
    };

    NodeUnQLite();
    virtual ~NodeUnQLite();

    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> StoreKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> AppendKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> DeleteKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> FetchKV(const v8::Arguments& args);

private:
    unqlite* db_;

    static void Work_Open(uv_work_t* req);
    static void Work_Get(uv_work_t* req);
    static void Work_Set(uv_work_t* req);
    static void Work_Append(uv_work_t* req);
    static void Work_Delete(uv_work_t* req);
#if NODE_VERSION_AT_LEAST(0,9,4)
    static void Work_AfterOpen(uv_work_t* req, int status);
    static void Work_AfterGet(uv_work_t* req, int status);
    static void Work_AfterSet(uv_work_t* req, int status);
    static void Work_AfterAppend(uv_work_t* req, int status);
    static void Work_AfterDelete(uv_work_t* req, int status);
#else
    static void Work_AfterOpen(uv_work_t* req);
    static void Work_AfterGet(uv_work_t* req);
    static void Work_AfterSet(uv_work_t* req);
    static void Work_AfterAppend(uv_work_t* req);
    static void Work_AfterDelete(uv_work_t* req);
#endif
};

#endif /* NODE_UNQLITE_H */
