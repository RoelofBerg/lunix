#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roelof");
MODULE_DESCRIPTION("A Simple Hello World module");

static int
hello_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Hello Kernel\n");
    return 0;
}

static int
hello_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = hello_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    proc_create("hello_world", 0, NULL, &hello_proc_fops);
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
    remove_proc_entry("hello_world", NULL);
}

module_init(hello_init);
module_exit(hello_cleanup);

