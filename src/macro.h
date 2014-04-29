#if !defined(NODE_UNQLITE_MACRO_H)
#define NODE_UNQLITE_MACRO_H

#define REQ_ARG_COUNT_AND_TYPE(I, TYPE) \
  if (args.Length() < (I + 1) ) { \
      return ThrowException(Exception::RangeError(String::New("A least " #I " arguments are required"))); \
  } else if (!args[I]->Is##TYPE()) { \
      return ThrowException(Exception::TypeError(String::New("Argument " #I " must be a " #TYPE))); \
  }

// validate the argument type is 'function' or not.
#define REQ_FUN_ARG(I, VAR) \
  REQ_ARG_COUNT_AND_TYPE(I, Function) \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

// validate the argument type is 'string' or not.
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


#endif /* NODE_UNQLITE_MACRO_H */
