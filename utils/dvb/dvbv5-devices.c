#include <stdio.h>
#include <config.h>
#include <libdvbv5/dvb-dev.h>

int main() {
    struct dvb_device *dvb;
    struct dvb_open_descriptor *dev_desc;
    struct dvb_dev_list device;

    dvb = dvb_dev_alloc();
    dvb_dev_find(dvb, NULL, NULL);

    if (dvb->num_devices == 0) {
        // Return a JSON with error `{"error": "There are not connected devices", "devices": {}}`
        printf("{\"error\": \"There are no connected devices\", \"devices\": {}}");
        return 1;
    }

    printf("{\"error\": \"There are no connected devices\", \"devices\": {");

    for (int i = 0; i < dvb->num_devices; i++) {
        device = dvb->devices[i];
        if (device.dvb_type != DVB_DEVICE_FRONTEND)  {
            // Skip not frontend devices
            continue;
        }

        // Prints `"/dev/dvb/adapterX/frontend0": `
        printf("{\"%s\": ", device.path);

        // Trying to connect to the device frontend
        dev_desc = dvb_dev_open(dvb, device.sysname, O_RDONLY);
        if (!dev_desc) {
            // Device cannot be open, mark it as FALSE
            printf("true");
            continue;
        }

        printf("false");

        if (i != (dvb->num_devices - 1)) {
            printf(",");
        }

        // Close device connection
        dvb_dev_close(dev_desc);
    }

    // Clean memory
    dvb_dev_free(dvb);
    return 0;
}
