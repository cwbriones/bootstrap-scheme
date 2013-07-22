#ifndef OBJECT_H
#define OBJECT_H

struct Object {
    static Object true_obj;
    static Object false_obj;
    static Object empty_list_obj;

	enum {
		FIXNUM,
        BOOLEAN,
        CHARACTER,
        STRING,
        EMPTY_LIST
	} type;

	union {
        struct {
            bool value;
        } boolean;
		struct {
			long value;
		} fixnum;
        struct {
            char value;
        } character;
        struct {
            char* value;
        } string;
	} data;

    bool is_false();
    bool is_true();
    bool is_empty_list();
    bool is_fixnum();
    bool is_boolean();
    bool is_character();
    bool is_string();

    static void init_statics();
};

#endif // OBJECT_H
