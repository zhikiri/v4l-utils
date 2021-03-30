#include <stdio.h>
#include <config.h>
#include <time.h>
#include <libdvbv5/dvb-dev.h>

#define MAX_TIME		10	/* 1.0 seconds */

#define xioctl(fh, request, arg...) ({					\
	int __rc;							\
	struct timespec __start, __end;					\
									\
	clock_gettime(CLOCK_MONOTONIC, &__start);			\
	do {								\
		__rc = ioctl(fh, request, ##arg);			\
		if (__rc != -1)						\
			break;						\
		if ((errno != EINTR) && (errno != EAGAIN))		\
			break;						\
		clock_gettime(CLOCK_MONOTONIC, &__end);			\
		if (__end.tv_sec * 10 + __end.tv_nsec / 100000000 >	\
		    __start.tv_sec * 10 + __start.tv_nsec / 100000000 +	\
		    MAX_TIME)						\
			break;						\
	} while (1);							\
									\
	__rc;								\
})

int check_device(struct dvb_device *dvb, struct dvb_dev_list *device) {
    int ret;
    int flags = O_RDONLY & ~O_NONBLOCK;

    int fd = open(device->path, flags, 0);
    if (fd == -1) {
        // Device open is failed
        return 1;
    }

    if (xioctl(fd, FE_GET_INFO, dvb->fe_parms->info) == -1) {
		// Cannot retrieve the device information
        ret = 2;
	}

    // Close opened adapter
    close(fd);
    return ret;
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
        int res = check_device(dvb, &device);
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
