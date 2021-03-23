#include <stdio.h>
#include <config.h>
#include <libdvbv5/dvb-dev.h>

# define _(string) string

int main() {

    struct dvb_device *dvb;
    struct dvb_open_descriptor *dev_desc;
    struct dvb_v5_fe_parms *parms;

    struct dvb_dev_list device;

    dvb = dvb_dev_alloc();
    dvb_dev_find(dvb, NULL, NULL);
    parms = dvb->fe_parms;

    if (dvb->num_devices == 0) {
        dvb_log(_("There are no connected devices %d"), dvb->num_devices);
        return 1;
    }

    dvb_log(_("Found #%d connected devices"), dvb->num_devices);

    for (int i = 0; i < dvb->num_devices; i++) {
        device = dvb->devices[i];
        if (device.dvb_type != DVB_DEVICE_FRONTEND)  {
            // Skip not frontend devices
            continue;
        }

        // Trying to connect to the device frontend
        dev_desc = dvb_dev_open(dvb, device.sysname, O_RDONLY);
        if (!dev_desc) {
            dvb_log(_("Device (path: %s) - FAIL"), device.path);
            continue;
        }

        dvb_log(_("Device (path: %s) - OK"), device.path);
        // Close device connection
        dvb_dev_close(dev_desc);
    }

    // Clean memory
    dvb_dev_free(dvb);
    return 0;
}

