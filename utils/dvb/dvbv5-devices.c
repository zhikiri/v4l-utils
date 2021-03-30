#include <stdio.h>
#include <config.h>
#include <libdvbv5/dvb-dev.h>

int check_device(struct dvb_dev_list *device) {
    int flags = O_RDONLY & ~O_NONBLOCK;

    int fd = open(device->path, flags, 0);
    if (fd == -1) {
        // Device open is failed
        return 1;
    }

    return 0;
}

int main() {
    struct dvb_device *dvb;
    struct dvb_dev_list device;

    dvb = dvb_dev_alloc();
    if (!dvb) {
        printf("{\"error\":\"Cannot allocate memory for DVB\",\"devices\":{}}\n");
        return -1;
    }

    dvb_dev_find(dvb, NULL, NULL);
    if (dvb->num_devices == 0) {
        printf("{\"error\":\"There are no connected devices\",\"devices\":{}}\n");
        dvb_dev_free(dvb);
        return -2;
    }

    printf("{\"error\":\"\",\"devices\":{");

    for (int i = 0; i < dvb->num_devices; i++) {
        device = dvb->devices[i];
        if (device.dvb_type != DVB_DEVICE_FRONTEND)  {
            // Skip not frontend devices
            continue;
        }

        // Prints `"/dev/dvb/adapterX/frontend0": `
        printf("\"%s\":", device.path);

        // Trying to connect to the device frontend
        int res = check_device(&device);
        if (res) {
            // Device cannot be open, mark it as FALSE
            printf("false");
        } else {
            printf("true");
        }

        if (i != (dvb->num_devices - 2)) {
            printf(",");
        }
    }

    printf("}}\n");

    // Clean memory
    dvb_dev_free(dvb);
    return 0;
}
