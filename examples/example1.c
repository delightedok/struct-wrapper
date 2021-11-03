#include <json_wrapper/json_wrapper.h>


DEFINE_STRUCT(Son,
    (OBJ(STRING), name)
    (OBJ(INT), age)
    (OBJ(STRING), birthday)
    (OBJ(CHAR), sex)
)

DEFINE_STRUCT(Person,
    (OBJ(STRING), name)
    (OBJ(INT), age)
    (OBJ(STRING), birthday)
    (OBJ(CHAR), sex)
    (OBJ(STRING), couple)
    (OBJ(Son), son)
    (ARRAY(Son, 2), sons)
    (VA_ARRAY(Son), v_sons)
)

int main(int argc, char * argv[])
{
    DECLARE_STRUCT(Person, person);
    char * json = S2J(Person, &person);
    DECLARE_STRUCT(Person, person2);
    J2S(json, Person, &person2);
    COPY_ST(Person, &person, &person2);
    RECYCLE_ST(Person, &person);
    RECYCLE_ST(Person, &person2);
}
