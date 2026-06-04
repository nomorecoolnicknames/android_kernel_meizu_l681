
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "kd_camera_hw.h"
#ifdef CONFIG_COMPAT
#include <linux/fs.h>
#include <linux/compat.h>
#endif
#include "kd_flashlight.h"

int strobe_getPartId(int sensorDev, int strobeId)
{
	int partId;

	/* return 1 or 2 (backup flash part). Other numbers are invalid. */
	if (sensorDev == e_CAMERA_MAIN_SENSOR && strobeId == 1) {
		partId = 1;
	} else if (sensorDev == e_CAMERA_MAIN_SENSOR && strobeId == 2) {
		partId = 1;
	} else if (sensorDev == e_CAMERA_SUB_SENSOR && strobeId == 1) {
		partId = 1;
	} else if (sensorDev == e_CAMERA_SUB_SENSOR && strobeId == 2) {
		partId = 1;
	} else {		/* e_CAMERA_MAIN_2_SENSOR */

		partId = 200;
	}
	pr_info("l681_flashlight: marker=part-id-map sensorDev=%d strobeId=%d partId=%d\n",
		sensorDev, strobeId, partId);
	return partId;
}
