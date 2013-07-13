#if !defined(NODE_UNQLITE_MACRO_H)
#define NODE_UNQLITE_MACRO_H

#define REQ_ARG_COUNT_AND_TYPE(I, TYPE) \
  if (args.Length() < (I + 1) ) { \
      return ThrowException(Exception::RangeError(String::New("A least " #I " arguments are required"))); \
  } else if (!args[I]->Is##TYPE()) { \
      return ThrowException(Exception::TypeError(String::New("Argument " #I " must be a " #TYPE))); \
  }

#define REQ_STR_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, String)

#define TRY_CATCH_CALL(context, callback, argc, argv)                          \
    TryCatch try_catch;                                                        \
    (callback)->Call((context), (argc), (argv));                               \
    if (try_catch.HasCaught()) {                                               \
        FatalException(try_catch);                                             \
    }

#define DEFINE_CONSTANT_INTEGER(target, constant, name)                        \
    (target)->Set(                                                             \
        String::NewSymbol(#name),                                              \
        Integer::New(constant),                                                \
        static_cast<PropertyAttribute>(ReadOnly | DontDelete)                  \
    );

#if NODE_VERSION_AT_LEAST(0,11,0)
#define __GET_ISOLATE_FOR_NEW v8::Isolate::GetCurrent(),
#define __GET_ISOLATE_FOR_DISPOSE v8::Isolate::GetCurrent()
#else
#define __GET_ISOLATE_FOR_NEW
#define __GET_ISOLATE_FOR_DISPOSE
#endif

#endif /* NODE_UNQLITE_MACRO_H */
