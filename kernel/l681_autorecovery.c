/*
 * L681 bring-up autorecovery watchdog.
 *
 * Diagnostic only: if Android does not reach boot_completed and cancel this
 * watchdog, reboot through the platform "recovery" path so evidence survives.
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/reboot.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>

#define L681_AR_PROC "l681_autorecovery"
#define L681_AR_STAGES 4

struct l681_ar_stage {
	const char *name;
	unsigned int timeout_sec;
};

static const struct l681_ar_stage l681_ar_stages[L681_AR_STAGES] = {
	{ "core", 200 },
	{ "postcore", 400 },
	{ "subsys", 600 },
	{ "late", 800 },
};

static DEFINE_MUTEX(l681_ar_lock);
static struct delayed_work l681_ar_work;
static int l681_ar_enabled = 1;
static int l681_ar_stage;
static int l681_ar_fired;
static unsigned int l681_ar_timeout_sec;
static unsigned long l681_ar_armed_jiffies;

static void l681_ar_rearm_locked(int stage)
{
	unsigned long delay;

	if (!l681_ar_enabled || l681_ar_fired)
		return;

	if (stage < 0)
		stage = 0;
	if (stage >= L681_AR_STAGES)
		stage = L681_AR_STAGES - 1;

	l681_ar_stage = stage;
	l681_ar_timeout_sec = l681_ar_stages[stage].timeout_sec;
	l681_ar_armed_jiffies = jiffies;
	delay = msecs_to_jiffies(l681_ar_timeout_sec * 1000);
	cancel_delayed_work(&l681_ar_work);
	schedule_delayed_work(&l681_ar_work, delay);
	pr_warn("L681AR: armed stage=%s timeout=%us\n",
		l681_ar_stages[stage].name, l681_ar_timeout_sec);
}

static void l681_ar_enter_stage(int stage)
{
	mutex_lock(&l681_ar_lock);
	if (stage >= l681_ar_stage)
		l681_ar_rearm_locked(stage);
	mutex_unlock(&l681_ar_lock);
}

static void l681_ar_workfn(struct work_struct *work)
{
	const char *stage_name;
	unsigned int timeout;

	mutex_lock(&l681_ar_lock);
	if (!l681_ar_enabled || l681_ar_fired) {
		mutex_unlock(&l681_ar_lock);
		return;
	}
	l681_ar_fired = 1;
	stage_name = l681_ar_stages[l681_ar_stage].name;
	timeout = l681_ar_timeout_sec;
	mutex_unlock(&l681_ar_lock);

	pr_emerg("L681AR: timeout stage=%s after %us, rebooting recovery\n",
		 stage_name, timeout);
	kernel_restart("recovery");
}

static ssize_t l681_ar_read(struct file *file, char __user *buf,
			    size_t count, loff_t *ppos)
{
	char tmp[160];
	unsigned long elapsed;
	int len;

	mutex_lock(&l681_ar_lock);
	elapsed = jiffies_to_msecs(jiffies - l681_ar_armed_jiffies) / 1000;
	len = snprintf(tmp, sizeof(tmp),
		       "enabled=%d fired=%d stage=%s timeout=%u elapsed=%lu\n",
		       l681_ar_enabled, l681_ar_fired,
		       l681_ar_stages[l681_ar_stage].name,
		       l681_ar_timeout_sec, elapsed);
	mutex_unlock(&l681_ar_lock);

	return simple_read_from_buffer(buf, count, ppos, tmp, len);
}

static ssize_t l681_ar_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	char tmp[16];
	size_t len;

	len = min(count, sizeof(tmp) - 1);
	if (copy_from_user(tmp, buf, len))
		return -EFAULT;
	tmp[len] = '\0';

	mutex_lock(&l681_ar_lock);
	if (tmp[0] == '0') {
		l681_ar_enabled = 0;
		cancel_delayed_work(&l681_ar_work);
		pr_warn("L681AR: disabled by userspace\n");
	} else if (tmp[0] == '1') {
		l681_ar_enabled = 1;
		l681_ar_fired = 0;
		l681_ar_rearm_locked(l681_ar_stage);
		pr_warn("L681AR: enabled by userspace\n");
	}
	mutex_unlock(&l681_ar_lock);

	return count;
}

static const struct file_operations l681_ar_fops = {
	.read = l681_ar_read,
	.write = l681_ar_write,
};

static int __init l681_ar_core_init(void)
{
	struct proc_dir_entry *entry;

	INIT_DELAYED_WORK(&l681_ar_work, l681_ar_workfn);
	entry = proc_create(L681_AR_PROC, 0644, NULL, &l681_ar_fops);
	if (!entry) {
		l681_ar_enabled = 0;
		pr_err("L681AR: failed to create /proc/%s, disabled\n",
		       L681_AR_PROC);
		return 0;
	}
	l681_ar_enter_stage(0);
	return 0;
}
core_initcall(l681_ar_core_init);

static int __init l681_ar_postcore_init(void)
{
	l681_ar_enter_stage(1);
	return 0;
}
postcore_initcall(l681_ar_postcore_init);

static int __init l681_ar_subsys_init(void)
{
	l681_ar_enter_stage(2);
	return 0;
}
subsys_initcall(l681_ar_subsys_init);

static int __init l681_ar_late_init(void)
{
	l681_ar_enter_stage(3);
	return 0;
}
late_initcall(l681_ar_late_init);
