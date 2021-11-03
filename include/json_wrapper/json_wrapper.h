/**
 * @file jason_wrapper.h
 * @author Tan, Bono (jieshentan@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <cJSON.h>


/**
 * @brief The true value in this wrapper
 */
#define TRUE 1

/**
 * @brief The false value in this wrapper
 */
#define FALSE 0


/**************************************** GET_STRUCT_NAME BEGIN ****************************************/
/**
 * @brief Get the struct name that used in this wrapper
 * @param type The type name
 **/
#define GET_STRUCT_NAME(type) type##_JSON
typedef bool BOOL_JSON;
typedef char CHAR_JSON;
typedef int INT_JSON;
typedef char * STRING_JSON;
/**************************************** GET_STRUCT_NAME  END  ****************************************/


/**
 * @brief Hook the memory alloc function
 * 
 * @param alloc_ The alloc function
 */
void json_wrapper_hook_alloc(void * (* alloc_)(size_t size));

/**
 * @brief Hook the memory free function
 * 
 * @param free_ The free function
 */
void json_wrapper_hook_free(void (* free_)(void * ptr));


/**
 * @brief Alloc memory using the hook in json wrapper
 * 
 * @param size The memory size
 * @return void* The pointer of memory
 */
void * json_wrapper_alloc(size_t size);

/**
 * @brief Free a @p ptr using the hook in json wrapper
 * 
 * @param ptr The pointer
 */
void json_wrapper_free(void * ptr);


#define ASSERT_RETURN(exp, rc) if (!(exp)) return rc
#define ASSERT_RETURN_VOID(exp) if (!(exp)) return


#define ASSERT_BREAK(exp) if (!(exp)) break


#define EXPAND_(...) __VA_ARGS__
#define EXPAND(...) EXPAND_(__VA_ARGS__)


#define CONCAT_(A, B) A##B
#define CONCAT(A, B) CONCAT_(A, B)

#define PRE_CONCAT(A, ...) CONCAT_(A, __VA_ARGS__)
#define PRE_CONCAT_(A, ...) A##__VA_ARGS__

#define POST_CONCAT_(end, ...) __VA_ARGS__##end
#define POST_CONCAT(end, ...) POST_CONCAT_(end, __VA_ARGS__)


#define SECOND(a1, a2, ...) a2
#define CHECK(...) SECOND(__VA_ARGS__, 0)
#define PROBE(x) x, 1


/**************************************** EMPTY BEGIN ****************************************/
/**
 * @brief An empty macro, mainly for @link DEFER to use
 **/
#define EMPTY()
/**************************************** EMPTY  END  ****************************************/


/**************************************** DEFER BEGIN ****************************************/
/**
 * @brief Delay to expand the macro
 * @param x The name of the macro to delay to expand
 **/
#define DEFER(x) x EMPTY()
/**************************************** DEFER  END  ****************************************/

/**
 * @brief The base wrapper type
 */
typedef enum GET_STRUCT_NAME(Type)
{
    BOOL,
    CHAR,
    INT,
    STRING,
    ARRAY_,
    VA_ARRAY_,
} GET_STRUCT_NAME(Type);


#define OBJ(type) OBJ, 0, type
#define ARRAY(type, num) ARRAY, num, type
#define VA_ARRAY(type) VA_ARRAY, 0, type


#define GET_VA_ARRAY_NAME(type) PRE_CONCAT(type##_, GET_STRUCT_NAME(VA_ARRAY_))

#define DEFINE_VA_ARRAY(type, name) \
typedef struct GET_VA_ARRAY_NAME(type) \
{ \
    GET_STRUCT_NAME(type) * name; \
    size_t size; \
} GET_VA_ARRAY_NAME(type);


/**************************************** TYPE_2_CJSON_TYPE BEGIN ****************************************/
/**
 * @brief Convert the wrapper type to json type
 * @param t The wrapper type
 **/
#define TYPE_2_CJSON_TYPE(t) t##_2_CJSON_TYPE
#define BOOL_2_CJSON_TYPE Bool
#define CHAR_2_CJSON_TYPE Number
#define INT_2_CJSON_TYPE Number
#define STRING_2_CJSON_TYPE String
/**************************************** TYPE_2_CJSON_TYPE  END  ****************************************/


/**************************************** STANDARD_TYPE BEGIN ****************************************/
/**
 * @brief Convert the wrapper type to standard base type
 * @param t The wrapper type
 **/
#define STANDARD_TYPE(t) t##_2_STANDARD_TYPE
#define BOOL_2_STANDARD_TYPE bool
#define CHAR_2_STANDARD_TYPE char
#define INT_2_STANDARD_TYPE int
#define STRING_2_STANDARD_TYPE char *
/**************************************** STANDARD_TYPE  END  ****************************************/


/**************************************** FIRST_FIELD BEGIN ****************************************/
/**
 * @brief Extract the first field
 * @param has_parentheses If parentheses added into the result, @p TRUE for yes, @p FALSE for no
 * 
 * @note For example:
 *           FIRST_FIELD(TRUE, (int, age) (int height)) -> (int, age)
 *           FIRST_FIELD(FALSE, (int, age) (int height)) -> int, age
 **/
#define FIRST_FIELD_(has_parentheses, ...) POST_CONCAT(_END, FIRST_FIELD_I_##has_parentheses __VA_ARGS__)
#define FIRST_FIELD(has_parentheses, ...) FIRST_FIELD_(has_parentheses, __VA_ARGS__)  // Convert TRUE/FALSE to 1/0
#define FIRST_FIELD_I_0(...) __VA_ARGS__ FIRST_FIELD_DEL_I
#define FIRST_FIELD_I_1(...) (__VA_ARGS__) FIRST_FIELD_DEL_I
#define FIRST_FIELD_I_0_END
#define FIRST_FIELD_I_1_END
#define FIRST_FIELD_DEL_I(...) FIRST_FIELD_DEL_II
#define FIRST_FIELD_DEL_II(...) FIRST_FIELD_DEL_I
#define FIRST_FIELD_DEL_I_END
#define FIRST_FIELD_DEL_II_END
/**************************************** FIRST_FIELD  END  ****************************************/


/**************************************** IS_EMPTY BEGIN ****************************************/
/**
 * @brief Check if parameters is empty
 * @return @p 0 for not empty, @p 1 for empty
 * 
 * @note For example:
 *           IS_EMPTY(a) -> 0
 *           IS_EMPTY() -> 1
 **/
#define IS_EMPTY_I(x, ...) CHECK(CONCAT(CONCAT_(IS_EMPTY_, x), 0))
#define IS_EMPTY(x, ...) IS_EMPTY_I(x, ##__VA_ARGS__)
#define IS_EMPTY_0 PROBE(1)
/**************************************** IS_EMPTY  END  ****************************************/


/**************************************** IS_EMPTY_FIELD BEGIN ****************************************/
/**
 * @brief Check if there's field
 * @return @p 0 for there's field in the parameter, @p 1 for the empty parameter
 * 
 * @note For example:
 *           IS_EMPTY_FIELD((int, age) (char, sex)) -> 0
 *           IS_EMPTY_FIELD() -> 1
 **/
#define IS_EMPTY_FIELD(...) IS_EMPTY(FIRST_FIELD(FALSE, __VA_ARGS__))
/**************************************** IS_EMPTY_FIELD  END  ****************************************/


/**************************************** OTHER_FIELDS BEGIN ****************************************/
/**
 * @brief Extract other fields except the first one
 * 
 * @note For example:
 *           FIRST_FIELD((int, age) (int height)) -> (int, height)
 **/
#define OTHER_FIELDS(...) CONCAT(OTHER_FIELDS_, IS_EMPTY_FIELD(__VA_ARGS__)) __VA_ARGS__
#define OTHER_FIELDS_0(...)
#define OTHER_FIELDS_1
/**************************************** OTHER_FIELDS  END  ****************************************/


/**************************************** EVAL BEGIN ****************************************/
/**
 * @brief To expand the macro recursively
 **/
#define EVAL(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__
/**************************************** EVAL BEGIN ****************************************/


/**************************************** SPLIT_FIELDS BEGIN ****************************************/
/**
 * @brief Split the fields with comma
 * @param has_parentheses If parentheses added into the result, @p TRUE for yes, @p FALSE for no
 * 
 * @note For example:
 *           SPLIT_FIELDS(TRUE, (int, a) (int, b) (char, c) (double d))
 *               ->  (int, a) , (int, b) , (char, c) , (double d)
 *           SPLIT_FIELDS(FALSE, (int, a) (int, b) (char, c) (double d))
 *               ->  int, a , int, b , char, c , double d
 **/
#define SPLIT_FIELDS_PARAMS(...) CONCAT(SPLIT_FIELDS_PARAMS_, IS_EMPTY_FIELD(__VA_ARGS__))(__VA_ARGS__)
#define SPLIT_FIELDS_PARAMS_1(...)
#define SPLIT_FIELDS_PARAMS_0(...) (__VA_ARGS__)

// We use SPLIT_FIELDS_EXPAND to let EXPAND in SPLIT_FIELDS_x_0 work
#define SPLIT_FIELDS_EXPAND(...) SPLIT_FIELDS_EXPAND_(__VA_ARGS__)
#define SPLIT_FIELDS_EXPAND_(...) __VA_ARGS__
#define SPLIT_FIELDS(has_parentheses, ...) SPLIT_FIELDS_(has_parentheses, __VA_ARGS__)
#define SPLIT_FIELDS_(has_parentheses, ...) \
    SPLIT_FIELDS_EXPAND( \
        CONCAT(SPLIT_FIELDS_##has_parentheses##_, IS_EMPTY_FIELD(__VA_ARGS__)) \
            SPLIT_FIELDS_PARAMS(__VA_ARGS__))

#define SPLIT_FIELDS_1_1
#define SPLIT_FIELDS_1_0(...) \
    FIRST_FIELD(TRUE, __VA_ARGS__) \
    POST_CONCAT(_END, EXPAND(SPLIT_FIELDS_1_0_I OTHER_FIELDS(__VA_ARGS__)))
#define SPLIT_FIELDS_1_0_I(...) , (__VA_ARGS__) SPLIT_FIELDS_1_0_II
#define SPLIT_FIELDS_1_0_II(...) , (__VA_ARGS__) SPLIT_FIELDS_1_0_I
#define SPLIT_FIELDS_1_0_I_END
#define SPLIT_FIELDS_1_0_II_END

#define SPLIT_FIELDS_0_1
#define SPLIT_FIELDS_0_0(...) \
    FIRST_FIELD(FALSE, __VA_ARGS__) \
    POST_CONCAT(_END, EXPAND(SPLIT_FIELDS_0_0_I OTHER_FIELDS(__VA_ARGS__)))
#define SPLIT_FIELDS_0_0_I(...) , __VA_ARGS__ SPLIT_FIELDS_0_0_II
#define SPLIT_FIELDS_0_0_II(...) , __VA_ARGS__ SPLIT_FIELDS_0_0_I
#define SPLIT_FIELDS_0_0_I_END
#define SPLIT_FIELDS_0_0_II_END
/**************************************** SPLIT_FIELDS  END  ****************************************/


/**************************************** JOIN_TYPES BEGIN ****************************************/
/**
 * @brief Add the struct name into the field tuples
 * @param st The name of the struct
 * 
 * @note For example:
 *           JOIN_TYPES(Person, (int, age) (char, sex) (char, married) (char, good))
 *               -> (Person, int, age) (Person, char, sex) (Person, char, married) (Person, char, good)
 **/
#define JOIN_TYPES_FOR_EACH(target, st, ...) \
    CONCAT(JOIN_TYPES_FOR_EACH_, IS_EMPTY(SPLIT_FIELDS(FALSE, ##__VA_ARGS__))) (target, st, ##__VA_ARGS__)
#define JOIN_TYPES_FOR_EACH_0_(target, st, ...) \
    target(st, FIRST_FIELD(TRUE, __VA_ARGS__)) \
    DEFER(JOIN_TYPES_FOR_EACH_I)()(target, st, OTHER_FIELDS(__VA_ARGS__))
#define JOIN_TYPES_FOR_EACH_0(...) JOIN_TYPES_FOR_EACH_0_(__VA_ARGS__)
#define JOIN_TYPES_FOR_EACH_1(...)
#define JOIN_TYPES_FOR_EACH_I() JOIN_TYPES_FOR_EACH
#define JOIN_TYPES_TARGET(st, tuple) (st, EXPAND tuple)
#define JOIN_TYPES(st, ...) EVAL(JOIN_TYPES_FOR_EACH(JOIN_TYPES_TARGET, st, ##__VA_ARGS__))
// #define JOIN_TYPES_(st, ...) EVAL(JOIN_TYPES_FOR_EACH(JOIN_TYPES_TARGET, st, ##__VA_ARGS__))
// #define JOIN_TYPES(st, ...) JOIN_TYPES_(st, SPLIT_FIELDS(TRUE, __VA_ARGS__))
/**************************************** JOIN_TYPES  END  ****************************************/


/**************************************** JOIN_TYPES_EX BEGIN ****************************************/
/**
 * @brief Add a tuple of struct names into the field tuples
 * @param st_tuple A tuple of struct names
 * 
 * @note For example:
 *           JOIN_TYPES_EX((json, st), (int, age) (char, sex) (char, married))
 *               -> (json, Person, int, age) (json, Person, char, sex) (json, Person, char, married)
 **/
#define JOIN_TYPES_EX_FOR_EACH(target, st_tuple, ...) \
    CONCAT(JOIN_TYPES_EX_FOR_EACH_, IS_EMPTY(SPLIT_FIELDS(FALSE, ##__VA_ARGS__))) (target, st_tuple, ##__VA_ARGS__)
#define JOIN_TYPES_EX_FOR_EACH_0_(target, st_tuple, ...) \
    target(st_tuple, FIRST_FIELD(TRUE, __VA_ARGS__)) \
    DEFER(JOIN_TYPES_EX_FOR_EACH_I)()(target, st_tuple, OTHER_FIELDS(__VA_ARGS__))
#define JOIN_TYPES_EX_FOR_EACH_0(...) JOIN_TYPES_EX_FOR_EACH_0_(__VA_ARGS__)
#define JOIN_TYPES_EX_FOR_EACH_1(...)
#define JOIN_TYPES_EX_FOR_EACH_I() JOIN_TYPES_EX_FOR_EACH
#define JOIN_TYPES_EX_TARGET(st_tuple, tuple) (EXPAND st_tuple, EXPAND tuple)
#define JOIN_TYPES_EX(st_tuple, ...) EVAL(JOIN_TYPES_EX_FOR_EACH(JOIN_TYPES_EX_TARGET, st_tuple, ##__VA_ARGS__))
/**************************************** JOIN_TYPES_EX  END  ****************************************/


/**************************************** JOIN_JSON BEGIN ****************************************/
/**
 * @brief Add the struct name into the field tuples
 * @param st The name of the struct
 * 
 * @note For example:
 *           JOIN_JSON(json, (st, int, age) (st, char, sex) (st, char, married) (st, char, good))
 *               -> (json, st, int, age) (json, st, char, sex) (json, st, char, married) (json, st, char, good)
 **/
// TODO: How to merge these macros with JOIN_TYPES
#define JOIN_JSON_FOR_EACH(target, json, ...) \
    CONCAT(JOIN_JSON_FOR_EACH_, IS_EMPTY(__VA_ARGS__)) (target, json, __VA_ARGS__)
#define JOIN_JSON_FOR_EACH_0_(target, json, st, t, n, ...) \
    target(json, st, t, n) \
    DEFER(JOIN_JSON_FOR_EACH_I)()(target, json, __VA_ARGS__)
#define JOIN_JSON_FOR_EACH_0(...) JOIN_JSON_FOR_EACH_0_(__VA_ARGS__)
#define JOIN_JSON_FOR_EACH_1(...)
#define JOIN_JSON_FOR_EACH_I() JOIN_JSON_FOR_EACH
#define JOIN_JSON_TARGET(json, st, t, n) (json, st, t, n)
#define JOIN_JSON_(json, ...) EVAL(JOIN_JSON_FOR_EACH(JOIN_JSON_TARGET, json, ##__VA_ARGS__))
#define JOIN_JSON(json, ...) JOIN_JSON_(json, SPLIT_FIELDS(FALSE, __VA_ARGS__))
/**************************************** JOIN_JSON  END  ****************************************/


/**************************************** STRUCT_2 BEGIN ****************************************/
/**
 * @brief Convert field to json object
 * @param type The type of the field
 * @param st The struct object of the field
 * @param n The name of the field
 **/
#define STRUCT_2_FUNCTION_NAME(type) CONCAT(json_wrapper_json_from_, GET_STRUCT_NAME(type))
#define STRUCT_2(type, st, n) STRUCT_2_FUNCTION_NAME(type) (&((st)->n))

static inline cJSON * STRUCT_2_FUNCTION_NAME(BOOL)(STANDARD_TYPE(BOOL) * value);
inline cJSON * STRUCT_2_FUNCTION_NAME(BOOL)(STANDARD_TYPE(BOOL) * value)
{
    return PRE_CONCAT(cJSON_Create, TYPE_2_CJSON_TYPE(BOOL))(*value);
}

static inline cJSON * STRUCT_2_FUNCTION_NAME(CHAR)(STANDARD_TYPE(CHAR) * value);
inline cJSON * STRUCT_2_FUNCTION_NAME(CHAR)(STANDARD_TYPE(CHAR) * value)
{
    return PRE_CONCAT(cJSON_Create, TYPE_2_CJSON_TYPE(CHAR))(*value);
}

static inline cJSON * STRUCT_2_FUNCTION_NAME(INT)(STANDARD_TYPE(INT) * value);
inline cJSON * STRUCT_2_FUNCTION_NAME(INT)(STANDARD_TYPE(INT) * value)
{
    return PRE_CONCAT(cJSON_Create, TYPE_2_CJSON_TYPE(INT))(*value);
}

static inline cJSON * STRUCT_2_FUNCTION_NAME(STRING)(STANDARD_TYPE(STRING) * value);
inline cJSON * STRUCT_2_FUNCTION_NAME(STRING)(STANDARD_TYPE(STRING) * value)
{
    return PRE_CONCAT(cJSON_Create, TYPE_2_CJSON_TYPE(STRING))(*value);
}
/**************************************** STRUCT_2  END  ****************************************/


/**
 * @brief Define a function name about converting json string to struct
 */
#define JSON_STR_2_FUNCTION_NAME(type) CONCAT(json_wrapper_json_str_to_, GET_STRUCT_NAME(type))


/**************************************** JSON_2 BEGIN ****************************************/
/**
 * @brief Convert json object to struct field
 * @param type The type of the struct field
 * @param obj The json object
 * @param dst The struct field
 **/
#define JSON_2_FUNCTION_NAME(type) CONCAT(json_wrapper_json_to_, GET_STRUCT_NAME(type))
#define JSON_2(type, obj, dst) JSON_2_FUNCTION_NAME(type) (obj, dst)

static inline void JSON_2_FUNCTION_NAME(BOOL)(cJSON * obj, STANDARD_TYPE(BOOL) * dst);
inline void JSON_2_FUNCTION_NAME(BOOL)(cJSON * obj, STANDARD_TYPE(BOOL) * dst)
{
    *dst = (obj)->valueint;
    // *dst = CONCAT(PRE_CONCAT(cJSON_Get, TYPE_2_CJSON_TYPE(BOOL)), Value)(obj);
}

static inline void JSON_2_FUNCTION_NAME(CHAR)(cJSON * obj, STANDARD_TYPE(CHAR) * dst);
inline void JSON_2_FUNCTION_NAME(CHAR)(cJSON * obj, STANDARD_TYPE(CHAR) * dst)
{
    *dst = (obj)->valueint;
    // *dst = CONCAT(PRE_CONCAT(cJSON_Get, TYPE_2_CJSON_TYPE(CHAR)), Value)(obj);
}

static inline void JSON_2_FUNCTION_NAME(INT)(cJSON * obj, STANDARD_TYPE(INT) * dst);
inline void JSON_2_FUNCTION_NAME(INT)(cJSON * obj, STANDARD_TYPE(INT) * dst)
{
    *dst = (obj)->valueint;
    // *dst = CONCAT(PRE_CONCAT(cJSON_Get, TYPE_2_CJSON_TYPE(INT)), Value)(obj);
}

void JSON_2_FUNCTION_NAME(STRING)(cJSON * obj, STANDARD_TYPE(STRING) * dst);
/**************************************** JSON_2  END  ****************************************/


/**************************************** DEFINE_VA_ARRAY_TYPES BEGIN ****************************************/
#define DEFINE_VA_ARRAY_TYPE_VA_ARRAY(num, type, name) DEFINE_VA_ARRAY(type, name)
#define DEFINE_VA_ARRAY_TYPE_ARRAY(num, type, name)
#define DEFINE_VA_ARRAY_TYPE_OBJ(num, type, name)
#define DEFINE_VA_ARRAY_TYPE_(t, num, type, name) DEFINE_VA_ARRAY_TYPE_##t(num, type, name)
#define DEFINE_VA_ARRAY_TYPE(t, num, type, name) DEFINE_VA_ARRAY_TYPE_(t, num, type, name)
#define DEFINE_VA_ARRAY_TYPES(...) CONCAT(DEFINE_VA_ARRAY_TYPES_I __VA_ARGS__, _END)
#define DEFINE_VA_ARRAY_TYPES_I(...) DEFINE_VA_ARRAY_TYPE(__VA_ARGS__) DEFINE_VA_ARRAY_TYPES_II
#define DEFINE_VA_ARRAY_TYPES_II(...) DEFINE_VA_ARRAY_TYPE(__VA_ARGS__) DEFINE_VA_ARRAY_TYPES_I
#define DEFINE_VA_ARRAY_TYPES_I_END
#define DEFINE_VA_ARRAY_TYPES_II_END
/**************************************** DEFINE_VA_ARRAY_TYPES  END  ****************************************/


/**************************************** DEFINE_STRUCT_ BEGIN ****************************************/
/**
 * @brief Define the header of the struct
 * 
 * @note For example:
 *           DEFINE_STRUCT_(Person)
 *               -> struct Person_JSON;
 *                  typedef struct Person_JSON Person_JSON;
 *                  struct Person_JSON
 **/
#define DEFINE_STRUCT_(type) \
struct GET_STRUCT_NAME(type); \
typedef struct GET_STRUCT_NAME(type) GET_STRUCT_NAME(type); \
struct GET_STRUCT_NAME(type)
/**************************************** DEFINE_STRUCT_  END  ****************************************/


/**************************************** DEFINE_FIELDS BEGIN ****************************************/
/**
 * @brief Define the fields
 * 
 * @note For example:
 *           (int, age) (char, sex) -> int age; char sex;
 **/
#define DEFINE_FIELD_VA_ARRAY(num, type, name) GET_VA_ARRAY_NAME(type) name;
#define DEFINE_FIELD_ARRAY(num, type, name) GET_STRUCT_NAME(type) name[num];
#define DEFINE_FIELD_OBJ(num, type, name) GET_STRUCT_NAME(type) name;
#define DEFINE_FIELD_(t, num, type, name) DEFINE_FIELD_##t(num, type, name)
#define DEFINE_FIELD(t, num, type, name) DEFINE_FIELD_(t, num, type, name)
#define DEFINE_FIELDS(...) CONCAT(DEFINE_FIELDS_I __VA_ARGS__, _END)
#define DEFINE_FIELDS_I(...) DEFINE_FIELD(__VA_ARGS__) DEFINE_FIELDS_II
#define DEFINE_FIELDS_II(...) DEFINE_FIELD(__VA_ARGS__) DEFINE_FIELDS_I
#define DEFINE_FIELDS_I_END
#define DEFINE_FIELDS_II_END
/**************************************** DEFINE_FIELDS  END  ****************************************/


/**
 * @brief Define a function name to convert wrapper struct to json string
 */
#define STRUCT_2_JSON_STR_FUNCTION_NAME(type) CONCAT(json_wrapper_json_str_from_, GET_STRUCT_NAME(type))


/**************************************** DEFINE_STRUCT_2_JSON BEGIN ****************************************/
/**
 * @brief Define a function to convert the struct to json string
 * @param type The type name of the struct
 **/
#define DEFINE_STRUCT_2_JSON__OBJ(json, st, num, t, n) { \
    cJSON * obj = STRUCT_2(t, st, n); \
    ASSERT_BREAK(obj); \
    cJSON_AddItemToObject(json, #n, obj); \
}
#define DEFINE_STRUCT_2_JSON__ARRAY(json, st, num, t, n) { \
    cJSON * array_ = cJSON_CreateArray(); \
    ASSERT_BREAK(array_); \
    int i = 0; \
    for (; i < num; ++i) \
    { \
        cJSON * obj = STRUCT_2(t, st, n[i]); \
        ASSERT_BREAK(obj); \
        cJSON_AddItemToArray(json, obj); \
    } \
    cJSON_AddItemToObject(json, #n, array_); \
}
#define DEFINE_STRUCT_2_JSON__VA_ARRAY(json, st, num, t, n) { \
    cJSON * array_ = cJSON_CreateArray(); \
    ASSERT_BREAK(array_); \
    if (NULL != (st)->n.n) \
    { \
        int i = 0; \
        for (; i < (st)->n.size; ++i) \
        { \
            cJSON * obj = STRUCT_2(t, st, n.n[i]); \
            ASSERT_BREAK(obj); \
            cJSON_AddItemToArray(json, obj); \
        } \
    } \
    cJSON_AddItemToObject(json, #n, array_); \
}
#define DEFINE_STRUCT_2_JSON__(json, st, type, num, t, n) DEFINE_STRUCT_2_JSON__##type(json, st, num, t, n)
#define DEFINE_STRUCT_2_JSON_(json, st, ...) \
    CONCAT(EXPAND(DEFINE_STRUCT_2_JSON_I JOIN_TYPES_EX((json, st), ##__VA_ARGS__)), _END)
#define DEFINE_STRUCT_2_JSON_I(json, st, type, num, t, n) DEFINE_STRUCT_2_JSON__(json, st, type, num, t, n) DEFINE_STRUCT_2_JSON_II
#define DEFINE_STRUCT_2_JSON_II(json, st, type, num, t, n) DEFINE_STRUCT_2_JSON__(json, st, type, num, t, n) DEFINE_STRUCT_2_JSON_I
#define DEFINE_STRUCT_2_JSON_I_END
#define DEFINE_STRUCT_2_JSON_II_END
#define DEFINE_STRUCT_2_JSON(type, ...) \
static inline cJSON * \
STRUCT_2_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * st); \
inline cJSON * \
STRUCT_2_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * st) \
{ \
    ASSERT_RETURN(st, NULL); \
    cJSON * json = cJSON_CreateObject(); \
    ASSERT_RETURN(json, NULL); \
    do { \
        DEFINE_STRUCT_2_JSON_(json, st, ##__VA_ARGS__) \
    } while (0); \
    return json; \
} \
static inline char * \
STRUCT_2_JSON_STR_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * st); \
inline char * \
STRUCT_2_JSON_STR_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * st) \
{ \
    cJSON * json = STRUCT_2_FUNCTION_NAME(type)(st); \
    ASSERT_RETURN(json, NULL); \
    char * result = cJSON_PrintUnformatted(json); \
    cJSON_Delete(json); \
    return result; \
}
/**************************************** DEFINE_STRUCT_2_JSON  END  ****************************************/


/**************************************** DEFINE_JSON_2_STRUCT BEGIN ****************************************/
/**
 * @brief Define a function to convert the json string to struct
 * @param type The type name of the struct
 **/
#define DEFINE_JSON_2_STRUCT__OBJ(json, st, num, t, n) { \
    cJSON * obj = cJSON_GetObjectItem(json, #n); \
    if (NULL != obj) { \
        JSON_2(t, obj, &((st)->n)); \
    } \
}
#define DEFINE_JSON_2_STRUCT__ARRAY(json, st, num, t, n) { \
    cJSON * obj = cJSON_GetObjectItem(json, #n); \
    if (NULL != obj) { \
        ASSERT_BREAK(cJSON_IsArray(obj)); \
        int n = num > cJSON_GetArraySize(obj) ? cJSON_GetArraySize(obj) : num; \
        int i = 0; \
        for (; i < n; ++i) { \
            cJSON * elem = cJSON_GetArrayItem(obj, i); \
            ASSERT_BREAK(elem); \
            JSON_2(t, elem, &((st)->n[i])); \
        } \
    } \
}
#define DEFINE_JSON_2_STRUCT__VA_ARRAY(json, st, num, t, n) { \
    cJSON * obj = cJSON_GetObjectItem(json, #n); \
    if (NULL != obj) { \
        ASSERT_BREAK(cJSON_IsArray(obj)); \
        int num_ = cJSON_GetArraySize(obj); \
        if (0 == num_) { \
            (st)->n.n = NULL; \
            (st)->n.size = 0; \
        } else { \
            (st)->n.size = num_; \
            (st)->n.n = json_wrapper_alloc(sizeof(GET_STRUCT_NAME(t)) * num_); \
            ASSERT_BREAK((st)->n.n); \
            int i = 0; \
            for (; i < num_; ++i) { \
                cJSON * elem = cJSON_GetArrayItem(obj, i); \
                ASSERT_BREAK(elem); \
                JSON_2(t, elem, &((st)->n.n[i])); \
            } \
        } \
    } \
}
#define DEFINE_JSON_2_STRUCT__(json, st, type, num, t, n) DEFINE_JSON_2_STRUCT__##type(json, st, num, t, n)
#define DEFINE_JSON_2_STRUCT_(json, st, ...) \
    CONCAT(EXPAND(DEFINE_JOSN_2_STRUCT_I JOIN_TYPES_EX((json, st), ##__VA_ARGS__)), _END)
#define DEFINE_JOSN_2_STRUCT_I(json, st, type, num, t, n) DEFINE_JSON_2_STRUCT__(json, st, type, num, t, n) DEFINE_JOSN_2_STRUCT_II
#define DEFINE_JOSN_2_STRUCT_II(json, st, type, num, t, n) DEFINE_JSON_2_STRUCT__(json, st, type, num, t, n) DEFINE_JOSN_2_STRUCT_I
#define DEFINE_JOSN_2_STRUCT_I_END
#define DEFINE_JOSN_2_STRUCT_II_END
#define DEFINE_JSON_2_STRUCT(type, ...) \
static inline int \
JSON_2_FUNCTION_NAME(type) \
    (cJSON * json, GET_STRUCT_NAME(type) * st); \
inline int \
JSON_2_FUNCTION_NAME(type) \
    (cJSON * json, GET_STRUCT_NAME(type) * st) \
{ \
    do { \
        DEFINE_JSON_2_STRUCT_(json, st, ##__VA_ARGS__) \
    } while (0); \
} \
static inline int \
JSON_STR_2_FUNCTION_NAME(type) \
    (char * json_str, GET_STRUCT_NAME(type) * st); \
inline int \
JSON_STR_2_FUNCTION_NAME(type) \
    (char * json_str, GET_STRUCT_NAME(type) * st) \
{ \
    ASSERT_RETURN(json_str && st, -1); \
    cJSON * json = cJSON_Parse(json_str); \
    ASSERT_RETURN(json, -1); \
    int rc = JSON_2_FUNCTION_NAME(type)(json, st); \
    cJSON_Delete(json); \
    return rc; \
}
/**************************************** DEFINE_JSON_2_STRUCT  END  ****************************************/


/**
 * @brief Define a function name about copying struct
 */
#define COPY_FUNCTION_NAME(type) CONCAT(json_wrapper_copy_, GET_STRUCT_NAME(type))


/**************************************** COPY BEGIN ****************************************/
/**
 * @brief Copy value from src to dst
 * @param type The type of the value
 * @param src The src value
 * @param dst The dst value
 **/
#define COPY(type, src, dst) COPY_FUNCTION_NAME(type)(src, dst)
static inline void COPY_FUNCTION_NAME(BOOL)(GET_STRUCT_NAME(BOOL) * src, GET_STRUCT_NAME(BOOL) * dst);
inline void COPY_FUNCTION_NAME(BOOL)(GET_STRUCT_NAME(BOOL) * src, GET_STRUCT_NAME(BOOL) * dst)
{
    ASSERT_RETURN_VOID(src && dst);
    *dst = *src;
}
static inline void COPY_FUNCTION_NAME(CHAR)(GET_STRUCT_NAME(CHAR) * src, GET_STRUCT_NAME(CHAR) * dst);
inline void COPY_FUNCTION_NAME(CHAR)(GET_STRUCT_NAME(CHAR) * src, GET_STRUCT_NAME(CHAR) * dst)
{
    ASSERT_RETURN_VOID(src && dst);
    *dst = *src;
}
static inline void COPY_FUNCTION_NAME(INT)(GET_STRUCT_NAME(INT) * src, GET_STRUCT_NAME(INT) * dst);
inline void COPY_FUNCTION_NAME(INT)(GET_STRUCT_NAME(INT) * src, GET_STRUCT_NAME(INT) * dst)
{
    ASSERT_RETURN_VOID(src && dst);
    *dst = *src;
}
void COPY_FUNCTION_NAME(STRING)(GET_STRUCT_NAME(STRING) * src, GET_STRUCT_NAME(STRING) * dst);
/**************************************** COPY  END  ****************************************/


/**************************************** DEFINE_COPY_STRUCT BEGIN ****************************************/
#define COPY_OBJ(src, dst, num, t, n) COPY(t, &((src)->n), &((dst)->n));
#define COPY_ARRAY(src, dst, num, t, n) { \
    int i = 0; \
    for (; i < num; ++i) \
    { \
        COPY(t, &((src)->n[i]), &((dst)->n[i])); \
    } \
}
#define COPY_VA_ARRAY(src, dst, num, t, n) { \
    int i = 0; \
    for (; i < (src)->n.size; ++i) \
    { \
        COPY(t, &((src)->n.n[i]), &((dst)->n.n[i])); \
    } \
}
#define DEFINE_COPY_STRUCT__(src, dst, type, num, t, n) COPY_##type(src, dst, num, t, n)
#define DEFINE_COPY_STRUCT_(src, dst, ...) \
    CONCAT(EXPAND(DEFINE_COPY_STRUCT_I JOIN_TYPES_EX((src, dst), ##__VA_ARGS__)), _END)
#define DEFINE_COPY_STRUCT_I(src, dst, type, num, t, n) DEFINE_COPY_STRUCT__(src, dst, type, num, t, n) DEFINE_COPY_STRUCT_II
#define DEFINE_COPY_STRUCT_II(src, dst, type, num, t, n) DEFINE_COPY_STRUCT__(src, dst, type, num, t, n) DEFINE_COPY_STRUCT_I
#define DEFINE_COPY_STRUCT_I_END
#define DEFINE_COPY_STRUCT_II_END
#define DEFINE_COPY_STRUCT(type, ...) \
static inline void \
COPY_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * src, GET_STRUCT_NAME(type) * dst); \
inline void \
COPY_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * src, GET_STRUCT_NAME(type) * dst) \
{ \
    ASSERT_RETURN_VOID(src && dst); \
    DEFINE_COPY_STRUCT_(src, dst, ##__VA_ARGS__) \
}
/**************************************** DEFINE_COPY_STRUCT  END  ****************************************/


/**
 * @brief Define a function name about recycling struct
 */
#define RECYCLE_FUNCTION_NAME(type) CONCAT(json_wrapper_recycle_, GET_STRUCT_NAME(type))


/**************************************** RECYCLE BEGIN ****************************************/
/**
 * @brief Recycle a wrapper type object
 **/
#define RECYCLE(type, ptr) RECYCLE_FUNCTION_NAME(type)(ptr)
static inline void RECYCLE_FUNCTION_NAME(BOOL)(GET_STRUCT_NAME(BOOL) * ptr);
inline void RECYCLE_FUNCTION_NAME(BOOL)(GET_STRUCT_NAME(BOOL) * ptr)
{}
static inline void RECYCLE_FUNCTION_NAME(CHAR)(GET_STRUCT_NAME(CHAR) * ptr);
inline void RECYCLE_FUNCTION_NAME(CHAR)(GET_STRUCT_NAME(CHAR) * ptr)
{}
static inline void RECYCLE_FUNCTION_NAME(INT)(GET_STRUCT_NAME(INT) * ptr);
inline void RECYCLE_FUNCTION_NAME(INT)(GET_STRUCT_NAME(INT) * ptr)
{}
void RECYCLE_FUNCTION_NAME(STRING)(GET_STRUCT_NAME(STRING) * ptr);
/**************************************** RECYCLE  END  ****************************************/


/**************************************** DEFINE_RECYCLE_STRUCT BEGIN ****************************************/
#define RECYCLE_OBJ(ptr, num, t, n) RECYCLE(t, &((ptr)->n));
#define RECYCLE_ARRAY(ptr, num, t, n) { \
    int i = 0; \
    for (; i < num; ++i) \
    { \
        RECYCLE(t, &((ptr)->n[i])); \
    } \
}
#define RECYCLE_VA_ARRAY(ptr, num, t, n) { \
    int i = 0; \
    for (; i < (ptr)->n.size; ++i) \
    { \
        RECYCLE(t, &((ptr)->n.n[i])); \
    } \
    json_wrapper_free((ptr)->n.n); \
}
#define DEFINE_RECYCLE_STRUCT__(ptr, type, num, t, n) RECYCLE_##type(ptr, num, t, n)
#define DEFINE_RECYCLE_STRUCT_(ptr, ...) \
    CONCAT(EXPAND(DEFINE_RECYCLE_STRUCT_I JOIN_TYPES_EX((ptr), ##__VA_ARGS__)), _END)
#define DEFINE_RECYCLE_STRUCT_I(ptr, type, num, t, n) DEFINE_RECYCLE_STRUCT__(ptr, type, num, t, n) DEFINE_RECYCLE_STRUCT_II
#define DEFINE_RECYCLE_STRUCT_II(ptr, type, num, t, n) DEFINE_RECYCLE_STRUCT__(ptr, type, num, t, n) DEFINE_RECYCLE_STRUCT_I
#define DEFINE_RECYCLE_STRUCT_I_END
#define DEFINE_RECYCLE_STRUCT_II_END
#define DEFINE_RECYCLE_STRUCT(type, ...) \
static inline void \
RECYCLE_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * ptr); \
inline void \
RECYCLE_FUNCTION_NAME(type) \
    (GET_STRUCT_NAME(type) * ptr) \
{ \
    ASSERT_RETURN_VOID(ptr); \
    DEFINE_RECYCLE_STRUCT_(ptr, ##__VA_ARGS__) \
}
/**************************************** DEFINE_RECYCLE_STRUCT  END  ****************************************/


#define DEFINE_STRUCT(type, ...) \
DEFINE_VA_ARRAY_TYPES(__VA_ARGS__) \
DEFINE_STRUCT_(type) \
{ \
    DEFINE_FIELDS(__VA_ARGS__) \
}; \
DEFINE_STRUCT_2_JSON(type, ##__VA_ARGS__) \
DEFINE_JSON_2_STRUCT(type, ##__VA_ARGS__) \
DEFINE_COPY_STRUCT(type, ##__VA_ARGS__) \
DEFINE_RECYCLE_STRUCT(type, ##__VA_ARGS__)


#define DECLARE_STRUCT(type, obj) \
    GET_STRUCT_NAME(type) obj; \
    memset(&obj, 0, sizeof(GET_STRUCT_NAME(type)))


#define S2J(type, obj_ptr) STRUCT_2_JSON_STR_FUNCTION_NAME(type)(obj_ptr)
#define J2S(json, type, obj_ptr) JSON_STR_2_FUNCTION_NAME(type)(json, obj_ptr)
#define COPY_ST(type, src_ptr, dst_ptr)  COPY_FUNCTION_NAME(type)(src_ptr, dst_ptr)
#define RECYCLE_ST(type, obj_ptr)  RECYCLE_FUNCTION_NAME(type)(obj_ptr)
