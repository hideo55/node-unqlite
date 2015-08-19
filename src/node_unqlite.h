#if !defined(NODE_UNQLITE_H)
#define NODE_UNQLITE_H

#include <nan.h>

#include <string>
#include <sstream>
#include "macro.h"
#include <cstring>

extern "C" {
#include "unqlite.h"
}

namespace node_unqlite {

class NodeUnQLite: public node::ObjectWrap {
public:

    /**
     * @brief Constructor
     */
    NodeUnQLite();

    /**
     * @brief Destructor
     */
    virtual ~NodeUnQLite();

    /**
     * @brief Initialize NodeUnQLite class as Node.js addon
     */
    static void Init(v8::Handle<v8::Object> exports);

    static NAN_METHOD (New);

    static NAN_METHOD (Open);

    static NAN_METHOD (Close);

    static NAN_METHOD (StoreKV);

    static NAN_METHOD (AppendKV);

    static NAN_METHOD (DeleteKV);

    static NAN_METHOD (FetchKV);

    /**
     * @brief Open database
     * @param[in] filename
     * @param[in] mode
     * @return status code
     */
    int open_db(const char* filename, int mode);

    /**
     * @brief Close database
     * @return status code
     */
    int close_db();

    /**
     * @brief Return whether the database is opened.
     * @retval true Database is opend.
     * @retval false Database is closed.
     */
    bool is_opened();

    /**
     * @brief Fetch value from the database.
     * @param key[in]
     * @param value[out]
     * @return status code
     */
    int fetch_kv(std::string& key, std::string& value);

    /**
     * @brief Store key and value to the database.
     * @param key[in]
     * @param value[in]
     * @return status code
     */
    int store_kv(std::string& key, std::string& value);

    /**
     * @brief Append value to the database.
     * @param key[in]
     * @param value[in]
     * @return status code
     */
    int append_kv(std::string& key, std::string& value);

    /**
     * @brief Delete key from the database.
     * @param key[in]
     * @return status code
     */
    int delete_kv(std::string& key);

    /**
     * @brief Get error message of status code.
     * @param[in] status Status code
     * @param[out] Error message
     */
    void get_error_message(std::string& message);


private:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    unqlite* db_;
    bool open_;
};

} // namespace node_unqlite

#endif /* NODE_UNQLITE_H */
