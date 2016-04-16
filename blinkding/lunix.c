#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("willfolk");
MODULE_DESCRIPTION("GPIO IRQ demo");

/* location of GPIO 7 registers */
#define GPIO_7_EDGE_SEL 0x020B401C

/* interrupt mask register */
#define GPIO_7_IMR      0x020B4014

/* interrupt status register */
#define GPIO_7_ISR      0x020B4018

/* number of PIN in register */
#define PIN_INDEX       11

/* IRQ number will be triggerd when an edge occurs */
#define INTERRUPT_NUM 110

/* write a value into an I/O register */
int write_ioreg(uint32_t addr, uint32_t value) {
    void *io_ptr = NULL;
    int res = -1;

    /* tell the kernel we're gonna use the memory region */
    if (request_mem_region(addr, sizeof(value), "lunix") == NULL) {
        printk(KERN_INFO "Couldn't request mem region\n");
        goto out_nothing;
    }

    /* map the physical address into our module's vspace */
    if ((io_ptr = ioremap_nocache(addr, sizeof(value))) == NULL) {
        printk(KERN_INFO "Couldn't map I/O region\n");
        goto out_release_mem;
    }

    /* actually write the value */
    iowrite32(value, io_ptr);
    res = 0;

    /* fress resources */
    iounmap(io_ptr);

out_release_mem:
    release_mem_region(addr, sizeof(value));
out_nothing:
    return res;
}

uint32_t* isr = NULL;

void irq_tasklet(unsigned long data) {
    printk(KERN_INFO "Lunix IRQ!\n");
    
    /* acknowledge the interrupt so we can get it again */
    if (isr != NULL) {
        *isr = (1 << PIN_INDEX);
    }
}
DECLARE_TASKLET(lunix_tasklet, irq_tasklet, 0);

irqreturn_t interrupt_handler(int irq, void *dev_id) {   
    tasklet_schedule(&lunix_tasklet);
    return IRQ_HANDLED;
}
EXPORT_SYMBOL(interrupt_handler);

int setup_interrupt(void) {
    int res;

    if ((isr = ioremap_nocache(GPIO_7_ISR, sizeof(uint32_t))) == NULL) {
        printk(KERN_INFO "Couldn't map ISR region\n");
        return -1;
    }

    /* setup interrupt callback for the edge detect interrupt */
    if ((res = request_irq(INTERRUPT_NUM, interrupt_handler, IRQF_TRIGGER_RISING, "lunix", NULL)) != 0) {
        printk(KERN_INFO "IRQ setup failed: %d\n", res);
        return -1;
    }

    /* configure edge selection: generate interrupts on edge change */
    if (write_ioreg(GPIO_7_EDGE_SEL, (1 << PIN_INDEX)) != 0) {
        printk(KERN_INFO "Setting EDGE_SEL failed\n");
        return -1;
    }
    
    /* enable interrupt in mask register */
    if (write_ioreg(GPIO_7_IMR, (1 << PIN_INDEX)) != 0) {
        printk(KERN_INFO "Setting IMR failed\n");
        return -1;
    }
    
    return 0;
}

static int __init hello_init(void) {
    printk(KERN_INFO "Lunix module loaded\n");

    setup_interrupt();

    return 0;
}

static void __exit hello_cleanup(void) {
    tasklet_kill(&lunix_tasklet);
    if (isr != NULL) {
        iounmap(isr);
        isr = NULL;
    }
    printk(KERN_INFO "Lunix module unloaded\n");
}

module_init(hello_init);
module_exit(hello_cleanup);

