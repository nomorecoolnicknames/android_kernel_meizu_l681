#include <linux/module.h>

char camera_f_info[20] = "NO THIS DEVICE";
char camera_b_info[20] = "NO THIS DEVICE";

EXPORT_SYMBOL(camera_f_info);
EXPORT_SYMBOL(camera_b_info);

MODULE_LICENSE("GPL");
