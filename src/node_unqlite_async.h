#if !defined(NODE_UNQLITE_ASYNC_H)
#define NODE_UNQLITE_ASYNC_H

#include "nan.h"
#include "node_unqlite.h"

namespace node_unqlite {

typedef enum {
    T_UNQLITE_FETCH, T_UNQLITE_STORE, T_UNQLITE_APPEND, T_UNQLITE_DELETE
} UnQLiteAccessType;

/**
 * @brief Asynchronous worker for open database
 */
class OpenWorker: public NanAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param filename[in] Name of the file to open
     * @param mode[in] UnQLite open mode
     */
    OpenWorker(NanCallback *callback, NodeUnQLite* uql, std::string& filename, int mode);

    /**
     * @brief Open UnQLite file
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();

private:
    NodeUnQLite* unqlite_;  /// NodeUnQLite instance
    int status_;            /// File open status
    std::string filename_;  /// File name
    int mode_;              /// Open mode
};

/**
 * @brief Asynchronous worker for close database
 */
class CloseWorker: public NanAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     */
    CloseWorker(NanCallback *callback, NodeUnQLite* uql);

    /**
     * @brief Open UnQLite file
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();

private:
    NodeUnQLite* unqlite_;  /// NodeUnQLite instance
    int status_;            /// File open status
};

// Asyncronous worker for database access
class AccessWorker: public NanAsyncWorker {
public:
    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param type[in] Access type
     * @param key[in] Key
     */
    AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key);

    /**
     * @brief Constructor
     * @param callback[in] Callback functio object
     * @param uql[in] NodeUnQLite instance
     * @param type[in] Access type
     * @param key[in] Key
     * @param value[in] Value
     */
    AccessWorker(NanCallback *callback, NodeUnQLite* uql, UnQLiteAccessType type, std::string key, std::string value);

    /**
     * @brief Access to the UnQLite DB
     */
    void Execute();

    /**
     * @brief Invoke callback function
     */
    void HandleOKCallback();

private:
    NodeUnQLite* unqlite_;
    int status_;
    UnQLiteAccessType type_;
    std::string key_;
    std::string value_;

    void setError(const char* type);
};

} // namespace node_unqlite

#endif // NODE_UNQLITE_ASYNC_H
