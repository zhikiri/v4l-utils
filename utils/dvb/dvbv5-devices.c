#include <stdio.h>
#include <libdvbv5/dvb-dev.h>

int main() {

    struct dvb_device *dvb;
    // struct dvb_dev_list *dvb_dev_list;

    dvb = dvb_dev_alloc();
    dvb_dev_find(dvb, NULL, NULL);
    if (dvb->num_devices == 0) {
        printf("Cannot find any connected devices\n");
        return 0;
    }

    for (int i = 0; i < dvb->num_devices; i++) {
        char *name = dvb->devices[i].sysname;
        printf("DBG : Device sysname %s\n", name);
    }

    // Clean memory
    dvb_dev_free(dvb);
    return 0;
}

