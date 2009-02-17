
#include <stdio.h>
#include <sys/time.h>
#include "../src/alarm/alarm.h"

/**
 * Test of alarm_block to see how it works. We'll create
 * a structure item_t with some data and an alarm_block.
 * Each item will have some  data attached, and also a timeout.
 * When the alarm timesout, the item is freed.
 */

struct item_t;

struct item_t {
    int data;
    struct alarm_block alarm;
};

/**
 * This functions creates a new item_t with the given data and initilizes
 * the alarm to call item_t_timeout() when after the time specified with
 * sec and usec.
 */
void item_t_add(int data, long sec, long usec);

/**
 * This is the callback function which is called when an alarm_block
 * initialized  by item_t_add() timesout.
 */
void item_t_timeout(struct alarm_block *alarm, void *data);

/**
 * Alarm blocks don't work using signals, so we need to actively poll
 * them to check if any alarm has timed out. You can do that by callling
 * this function.
 */
void process_alarms();

void item_t_add(int data, long sec, long usec)
{
    struct item_t *item = malloc(sizeof(struct item_t));
    item->data = data;
    
    init_alarm(&item->alarm, item, item_t_timeout);
    add_alarm(&item->alarm, sec, usec);
}

void item_t_timeout(struct alarm_block *alarm, void *data)
{
    struct item_t *item = (struct item_t *)data;
    struct timeval tv, tmp;

    printf("item->data = %d; now we free the mallocs!\n", item->data);
    free(item);
}

void process_alarms()
{
    static struct timeval next_alarm; 
    static struct timeval *next = NULL;

    // This function works by getting inthe variable "next" how
    // much time is left for the nearest (in time) alarm to timeout:
    // if the alarm timedout, we call to do_alarm_run which in turn
    // calls the callback functions of all the alarms which have timedout.
    // Else, we update the next alarm for the time this function gets called.
    
    if (next != NULL && !timerisset(next))
    {
        next = do_alarm_run(&next_alarm);
    } else {
        
        next = get_next_alarm_run(&next_alarm);
    }
    
    if(next != NULL) {
        printf("next alarm times out in %ld s\t%ld us\n", next->tv_sec, next->tv_usec);
    }
}

int main(int argc, char **argv)
{
    // Here we create a new item with data = 13 and which will time out in 1.5 seconds
    item_t_add(13, 0, 1500000);
    
    // Here we create a new item with data = 2 and which will timeout = 10 seconds
    item_t_add(2, 10, 0);

    while(1) {
        process_alarms();
        
        /* This a simple example, but in reality we will use select with a timeout
         * instead of calling to sleep.
         */
        sleep(1);
    }
    return 0;
}
