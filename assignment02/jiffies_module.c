#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LEESEOJUN");
MODULE_DESCRIPTION("/proc/jiffies Kernel Module");
MODULE_VERSION("0.1");

#define PROC_FILENAME "jiffies"

static struct proc_dir_entry *jiffies_entry;

// cat /proc/jiffies
static ssize_t jiffies_read(struct file *file, char __user *buf,
                            size_t count, loff_t *ppos)
{
    char kbuffer[64];
    int len;

    if (*ppos > 0)
        return 0;

    // print current value of jiffies into kbuffer
    len = snprintf(kbuffer, sizeof(kbuffer), "%lu\n", jiffies);

    if (copy_to_user(buf, kbuffer, len))
        return -EFAULT;

    *ppos = len;
    return len;
}

static const struct proc_ops jiffies_proc_ops = {
    .proc_read = jiffies_read,
};

static int __init jiffies_module_init(void)
{
    // create /proc/jiffies
    jiffies_entry = proc_create(PROC_FILENAME, 0444, NULL, &jiffies_proc_ops);
    if (!jiffies_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }
    pr_info("Created /proc/%s\n", PROC_FILENAME);
    return 0;
}

static void __exit jiffies_module_exit(void)
{
    // remove /proc/jiffies
    proc_remove(jiffies_entry);
    pr_info("Removed /proc/%s\n", PROC_FILENAME);
}

module_init(jiffies_module_init);
module_exit(jiffies_module_exit);
