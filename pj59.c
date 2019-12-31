#include "lookup.h"

int main(int argc, char ** argv) {
    int status = 0;
    struct heap heap;
    struct lookup lookup;

    if(chdir(argv[1])) {
        status = panic("failed to change directory");
    } else {
        if(heap_create(&heap, 4096)) {
            status = panic("failed to create heap object");
        } else {
            if(lookup_create(&lookup, 65536, &heap)) {
                status = panic("failed to create lookup object");
            } else {
                if(lookup_pet_db_parse(&lookup, "pet_db.yml")) {
                    status = panic("failed to pet db lookup object");
                } else if(lookup_item_db_parse(&lookup, "item_db.txt")) {
                    status = panic("failed to item db lookup object");
                } else if(lookup_item_combo_db_parse(&lookup, "item_combo_db.txt")) {
                    status = panic("failed to item combo db lookup object");
                }
                lookup_destroy(&lookup);
            }
            heap_destroy(&heap);
        }
    }

    return status;
}
