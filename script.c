#include "script.h"

#include "script_parser.h"
#include "script_scanner.h"

int script_state_push(struct script *);
void script_state_pop(struct script *);

int script_string_push(struct script *, struct string *);
void script_string_pop(struct script *);

void script_token_print(int, struct script_node *);
int script_parse(struct script *, yyscan_t, scriptpstate *);

int script_create(struct script * script, size_t size, struct heap * heap, struct lookup * lookup) {
    int status = 0;

    script->heap = heap;
    if(!script->heap) {
        status = panic("invalid heap object");
    } else {
        script->lookup = lookup;
        if(!script->lookup) {
            status = panic("invalid lookup object");
        } else {
            if(scriptlex_init_extra(script, &script->scanner)) {
                status = panic("failed to create scanner object");
            } else {
                script->parser = scriptpstate_new();
                if(!script->parser) {
                    status = panic("failed to create parser object");
                } else {
                    if(strbuf_create(&script->strbuf, size)) {
                        status = panic("failed to create strbuf object");
                    } else {
                        if(store_create(&script->store, size)) {
                            status = panic("failed to create store object");
                        } else {
                            script->state = NULL;
                        }
                        if(status)
                            strbuf_destroy(&script->strbuf);
                    }
                    if(status)
                        scriptpstate_delete(script->parser);
                }
                if(status)
                    scriptlex_destroy(script->scanner);
            }
        }
    }

    return status;
}
void script_destroy(struct script * script) {
    store_destroy(&script->store);
    strbuf_destroy(&script->strbuf);
    scriptpstate_delete(script->parser);
    scriptlex_destroy(script->scanner);
}

int script_state_push(struct script * script) {
    int status = 0;
    struct script_state * state;

    state = store_object(&script->store, sizeof(*state));
    if(!state) {
        status = panic("failed to object store object");
    } else {
        state->next = script->state;
        script->state = state;
    }

    return status;
}

void script_state_pop(struct script * script) {
    script->state = script->state->next;

    if(!script->state) {
        store_clear(&script->store);
        strbuf_clear(&script->strbuf);
    }
}

int script_string_push(struct script * script, struct string * string) {
    int status = 0;

    YY_BUFFER_STATE buffer;

    if(strbuf_strcpy(&script->strbuf, string->string, string->length)) {
        status = panic("failed to strcpy strbuf object");
    } else if(strbuf_putcn(&script->strbuf, '\0', 2)) {
        status = panic("failed to putcn strbuf object");
    } else {
        string = strbuf_string(&script->strbuf);
        if(!string) {
            status = panic("failed to string strbuf object");
        } else {
            buffer = script_scan_buffer(string->string, string->length, script->scanner);
            if(!buffer)
                status = panic("failed to scan buffer scanner object");
        }
    }

    return status;
}

void script_string_pop(struct script * script) {
    scriptpop_buffer_state(script->scanner);
}

void script_token_print(int token, struct script_node * node) {
    switch(token) {
        case script_curly_open:     fprintf(stdout, "curly_open "); break;
        case script_curly_close:    fprintf(stdout, "curly_close "); break;
    }
}

int script_parse(struct script * script, yyscan_t scanner, scriptpstate * parser) {
    int status = 0;

    SCRIPTSTYPE value;
    SCRIPTLTYPE location;
    int token;
    int state;

    do {
        value = NULL;
        token = scriptlex(&value, &location, scanner);
        if(token < 0) {
            status = panic("failed to get the next token");
        } else {
            state = scriptpush_parse(parser, token, &value, &location, script);
            if(state && state != YYPUSH_MORE)
                status = panic("failed to parse the current token");
        }
    } while(token && state == YYPUSH_MORE && !status);

    return status;
}

int script_translate(struct script * script, struct string * string) {
    int status = 0;

    if(script_state_push(script)) {
        status = panic("failed to state push script object");
    } else {
        if(script_string_push(script, string)) {
            status = panic("failed to string push script object");
        } else {
            if(script_parse(script, script->scanner, script->parser))
                status = panic("failed to parse script object");
            script_string_pop(script);
        }
        script_state_pop(script);
    }

    return status;
}
