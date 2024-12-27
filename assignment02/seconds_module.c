#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LEESEOJUN");
MODULE_DESCRIPTION("/proc/seconds Kernel Module");
MODULE_VERSION("0.1");

#define PROC_FILENAME "seconds"

static struct proc_dir_entry *seconds_entry;
static unsigned long start_jiffies;

// cat /proc/seconds
static ssize_t seconds_read(struct file *file, char __user *buf,
                            size_t count, loff_t *ppos)
{
    char kbuffer[64];
    int len;
    unsigned long elapsed_jiffies;
    unsigned long elapsed_seconds;

    if (*ppos > 0)
        return 0;

    // elapsed jiffies and convert to seconds
    elapsed_jiffies = jiffies - start_jiffies;
    elapsed_seconds = elapsed_jiffies / HZ;

    len = snprintf(kbuffer, sizeof(kbuffer), "%lu\n", elapsed_seconds);

    if (copy_to_user(buf, kbuffer, len))
        return -EFAULT;

    *ppos = len;
    return len;
}

static const struct proc_ops seconds_proc_ops = {
    .proc_read = seconds_read,
};

static int __init seconds_module_init(void)
{
    start_jiffies = jiffies;

    // create /proc/seconds
    seconds_entry = proc_create(PROC_FILENAME, 0444, NULL, &seconds_proc_ops);
    if (!seconds_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }
    pr_info("Created /proc/%s\n", PROC_FILENAME);
    return 0;
}

static void __exit seconds_module_exit(void)
{
    // remove /proc/seconds
    proc_remove(seconds_entry);
    pr_info("Removed /proc/%s\n", PROC_FILENAME);
}

module_init(seconds_module_init);
module_exit(seconds_module_exit);
