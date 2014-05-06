#if !defined(NODE_UNQLITE_CURSOR_H)
#define NODE_UNQLITE_CURSOR_H

#include "nan.h"
#include "unqlite.h"


namespace node_unqlite {

class Cursor : public node::ObjectWrap {
public:
    Cursor();

    void Init();

    int CursorInit(unqlite* uql);

    NAN_METHOD(NewInstance);

    NAN_METHOD(FirstEntry);

    NAN_METHOD(LastEntry);

    NAN_METHOD(ValidEntry);

    NAN_METHOD(Key);

    NAN_METHOD(Value);

    NAN_METHOD(NextEntry);

    NAN_METHOD(PrevENtry);

    NAN_METHOD(Seek);

    NAN_METHOD(DeleteEntry);

private:
    static v8::Persistent<v8::Function> constructor_;
    unqlite* db_;
    unqlite_kv_cursor* cursor_;
};


} // namespace node_unqlite

#endif // NODE_UNQLITE_CURSOR_H
