#include "../alarm/linux_list.h"
#include <stdio.h>
#include <stdlib.h>

struct cool_entry {
    int value;
    struct list_head list;
};

int main()
{
    struct cool_entry myList;
    struct cool_entry *tmp, *entry;
    //Initialize it
    INIT_LIST_HEAD(&myList.list);

    puts("go across the just initialized list (should be empty):");
    list_for_each_entry(entry, &myList.list, list) {
        printf("entry value: %d\n", entry->value);
    }

    puts("add some entries");
    tmp = calloc(1, sizeof(struct cool_entry));
    tmp->value = 666;
    list_add(&tmp->list, &myList.list);
    tmp = calloc(1, sizeof(struct cool_entry));
    tmp->value = 111;
    list_add(&(tmp->list), &(myList.list));

    puts("go across the list (should show the added entries):");
    list_for_each_entry(entry, &myList.list, list) {
        printf("entry value: %d\n", entry->value);
    }

    puts("free the mallocs:");
    list_for_each_entry_safe(entry, tmp, &myList.list, list) {
        printf("free entry with value: %d\n", entry->value);
        list_del(&entry->list);
        free(entry);
    }

    puts("go across the list (should be empty):");
    list_for_each_entry(entry, &myList.list, list) {
        printf("entry value: %d\n", entry->value);
    }
    return 0;
}

