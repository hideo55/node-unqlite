#if !defined(NODE_UNQLITE_ASYNC_H)
#define NODE_UNQLITE_ASYNC_H

#include "node_unqlite.h"

namespace node_unqlite {

typedef enum {
    T_UNQLITE_FETCH, T_UNQLITE_STORE, T_UNQLITE_APPEND, T_UNQLITE_DELETE
} UnQLiteAccessType;


/**
 * @brief Base class of UnQLite asynchronous worker
 */
class UnQLiteAsyncWorker : public Nan::AsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     */
    UnQLiteAsyncWorker(Nan::Callback *callback, NodeUnQLite* uql);


protected:
    NodeUnQLite* unqlite_;  /// NodeUnQLite instance
    int status_;            /// File open status

    /**
     * Set error message for async worker
     * @param message[in] Error message
     */
    void SetErrorMessage(const char* message);
};

/**
 * @brief Asynchronous worker for open database
 */
class OpenWorker: public UnQLiteAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param filename[in] Name of the file to open
     * @param mode[in] UnQLite open mode
     */
    OpenWorker(Nan::Callback *callback, NodeUnQLite* uql, std::string& filename, int mode);

    /**
     * @brief Open UnQLite file
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();

private:
    std::string filename_;  /// File name
    int mode_;              /// Open mode
};

/**
 * @brief Asynchronous worker for close database
 */
class CloseWorker: public UnQLiteAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     */
    CloseWorker(Nan::Callback *callback, NodeUnQLite* uql);

    /**
     * @brief Open UnQLite file
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();
};

// Asyncronous worker for database access
class AccessWorker: public UnQLiteAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param type[in] Access type
     * @param key[in] Key
     */
    AccessWorker(Nan::Callback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key);

    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param type[in] Access type
     * @param key[in] Key
     * @param value[in] Value
     */
    AccessWorker(Nan::Callback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key, std::string value);

    /**
     * @brief Access to the UnQLite DB
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();

private:
    UnQLiteAccessType type_;
    std::string key_;
    std::string value_;
};

} // namespace node_unqlite

#endif // NODE_UNQLITE_ASYNC_H
