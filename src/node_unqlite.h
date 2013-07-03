#if !defined(NODE_UNQLITE_H)
#define NODE_UNQLITE_H

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include "unqlite.h"
#include "macro.h"

class NodeUnQLite: node::ObjectWrap {
public:
    NodeUnQLite();
    virtual ~NodeUnQLite();

    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Open(const v8::Arguments& args);
    static v8::Handle<v8::Value> StoreKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> AppendKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> DeleteKV(const v8::Arguments& args);
    static v8::Handle<v8::Value> FetchKV(const v8::Arguments& args);
};

#endif /* NODE_UNQLITE_H */
