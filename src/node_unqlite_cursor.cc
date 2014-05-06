#include "node_unqlite_cursor.h"

using namespace v8;
using namespace node;

namespace node_unqlite {

Persistent<Function> Cursor::constructor_;

Cursor::Cursor() : unqlite_(NULL), curosr_(NULL) {

}

int Cursor::CursorInit(unqlite* uql) {
    unqlite_ = uql;
    int status = unqlite_kv_cursor_init(unqlite_, &cursor_);
    return status;
}

NAN_METHOD(NewInstance) {
    NanScope();
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = {args[0]};
    NanReturnValue(constructor_->NewInstance(argc, argv));
}

NAN_METHOD(FirstEntry) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(LastEntry) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(ValidEntry) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(Key) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(Value) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(NextEntry) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(PrevENtry) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(Seek) {
    NanScope();
    NanReturnUndefined();
}

NAN_METHOD(DeleteEntry) {
    NanScope();
    NanReturnUndefined();
}


} // namespace node_unqlite
