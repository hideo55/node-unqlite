#if !defined(NODE_UNQLITE_MACRO_H)
#define NODE_UNQLITE_MACRO_H

#include "nan.h"
#include <sstream>

#define REQ_ARG_COUNT_AND_TYPE(I, TYPE) \
  if (args.Length() < (I + 1) ) { \
      std::stringstream __ss; \
      __ss << "A least " << I + 1 << " arguments are required"; \
      return NanThrowRangeError(__ss.str().c_str()); \
  } else if (!args[I]->Is##TYPE()) { \
      std::stringstream __ss; \
      __ss << "Argument " << I + 1 << " must be a " #TYPE; \
      return NanThrowTypeError(__ss.str().c_str()); \
  }

// validate the argument type is 'function' or not.
#define REQ_FUN_ARG(I, VAR) \
  REQ_ARG_COUNT_AND_TYPE(I, Function) \
  Local<Function> VAR = args[I].As<Function>();

// validate the argument type is 'string' or not.
#define REQ_STR_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, String)

#define TRY_CATCH_CALL(context, callback, argc, argv)                          \
    TryCatch try_catch;                                                        \
    (callback)->Call((context), (argc), (argv));                               \
    if (try_catch.HasCaught()) {                                               \
        FatalException(try_catch);                                             \
    }

#define DEFINE_CONSTANT_INTEGER(target, constant, name)                        \
    (target)->ForceSet(                                                             \
        NanNew<String>(#name),                                              \
        NanNew<Integer>(constant),                                                \
        static_cast<PropertyAttribute>(ReadOnly | DontDelete)                  \
    );


#endif /* NODE_UNQLITE_MACRO_H */
