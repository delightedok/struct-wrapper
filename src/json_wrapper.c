/**
 * @file jason_wrapper.c
 * @author Tan, Bono (jieshentan@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <json_wrapper/json_wrapper.h>


/**
 * @brief The hook functions collection
 */
typedef struct GET_STRUCT_NAME(Hook)
{
    void * (* alloc_)(size_t size);
    void (* free_)(void * ptr);
} GET_STRUCT_NAME(Hook);


/**
 * @brief A static global variable to store the hook functions with the default values initialized
 */
static GET_STRUCT_NAME(Hook) g_hook = {
    .alloc_ = malloc,
    .free_ = free,
};


/**
 * @brief Hook the memory alloc function
 * 
 * @param alloc_ The alloc function
 */
void json_wrapper_hook_alloc(void * (* alloc_)(size_t size))
{
    if (NULL != alloc_)
    {
        g_hook.alloc_ = alloc_;
    } else
    {
        g_hook.alloc_ = malloc;
    }
}

/**
 * @brief Hook the memory free function
 * 
 * @param free_ The free function
 */
void json_wrapper_hook_free(void (* free_)(void * ptr))
{
    if (NULL != free_)
    {
        g_hook.free_ = free_;
    } else
    {
        g_hook.free_ = free;
    }
}


/**
 * @brief Alloc memory using the hook in json wrapper
 * 
 * @param size The memory size
 * @return void* The pointer of memory
 */
void * json_wrapper_alloc(size_t size)
{
    g_hook.alloc_(size);
}

/**
 * @brief Free a @p ptr using the hook in json wrapper
 * 
 * @param ptr The pointer
 */
void json_wrapper_free(void * ptr)
{
    ASSERT_RETURN_VOID(ptr);
    g_hook.free_(ptr);
}


void JSON_2_FUNCTION_NAME(STRING)(cJSON * obj, STANDARD_TYPE(STRING) * dst)
{
    STANDARD_TYPE(STRING) src = (obj)->valuestring;
    // STANDARD_TYPE(STRING) src = CONCAT(PRE_CONCAT(cJSON_Get, TYPE_2_CJSON_TYPE(STRING)), Value)(obj);
    ASSERT_RETURN_VOID(src);
    if (NULL != *dst) g_hook.free_(*dst);
    size_t len = strlen(src);
    *dst = g_hook.alloc_(1 + len);
    ASSERT_RETURN_VOID(*dst);
    memcpy(*dst, src, len);
    (*dst)[len] = '\0';
}


void COPY_FUNCTION_NAME(STRING)(GET_STRUCT_NAME(STRING) * src, GET_STRUCT_NAME(STRING) * dst)
{
    ASSERT_RETURN_VOID(src && *src && dst);
    if (NULL != *dst) g_hook.free_(*dst);
    size_t len = strlen(*src);
    *dst = g_hook.alloc_(len + 1);
    ASSERT_RETURN_VOID(*dst);
    memcpy(*dst, *src, len);
    *dst[len] = '\0';
}


void RECYCLE_FUNCTION_NAME(STRING)(GET_STRUCT_NAME(STRING) * ptr)
{
    ASSERT_RETURN_VOID(ptr);
    if (*ptr)
    {
        g_hook.free_(*ptr);
        *ptr = NULL;
    }
}
