#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/types.h>   // for dev_t typedef
#include <linux/kdev_t.h>  // for format_dev_t
#include <linux/fs.h>      // for alloc_chrdev_region()
#include <linux/cdev.h>
#include <asm/uaccess.h>

static dev_t mydev;             // (major,minor) value
static char buffer[64];         // optional, for debugging

static char output[] = "Hello world\n";

ssize_t my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_INFO "In chardrv read routine.\n");
    printk(KERN_INFO "Count field is %lu.\n", (unsigned long)count);
    printk(KERN_INFO "Offset is %lu.\n", (unsigned long)*f_pos);

    if (output[*f_pos] == '\0') {
        printk(KERN_INFO "End of string, returning zero.\n");
        return 0;
    }

    if (copy_to_user(buf, &output[*f_pos], 1) == 0) {
      *f_pos += 1;
      return 1;  // returned a single character
    }
    else return 0;
}

struct cdev my_cdev;

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
};

static int __init chardrv_in(void)
{
    printk(KERN_INFO "module chardrv being loaded.\n");

    alloc_chrdev_region(&mydev, 0, 1, "luna");
    printk(KERN_INFO "%s\n", format_dev_t(buffer, mydev));

    cdev_init(&my_cdev, &my_fops);
    my_cdev.owner = THIS_MODULE;
    cdev_add(&my_cdev, mydev, 1);

    return 0;
}

static void __exit chardrv_out(void)
{
    printk(KERN_INFO "module chardrv being unloaded.\n");

    unregister_chrdev_region(mydev, 1);
}

module_init(chardrv_in);
module_exit(chardrv_out);

MODULE_AUTHOR("Robert P. J. Day, http://crashcourse.ca");
MODULE_LICENSE("GPL");
