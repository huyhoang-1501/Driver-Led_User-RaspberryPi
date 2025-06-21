#include<asm/io.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/string.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>

#define DRIVER_NAME "driver"
#define DRIVER_CLASS "class"
#define BCM2711_GPIO_ADDRESS 0xFE200000

static dev_t dev_no;
static struct cdev dev;
static struct class *cls;
static unsigned int *gpio_reg = NULL;
static void gpio_set(void)
{
    unsigned int *gpio_sel_reg = (unsigned int*)((char*) gpio_reg + 0x00);
    unsigned int *gpio_set_reg = (unsigned int*)((char*) gpio_reg + 0x1c);
    *gpio_sel_reg = 0x01;
    *gpio_set_reg = 0x01;
}

static void gpio_clr(void)
{
    unsigned int *gpio_clr_reg = (unsigned int*)((char*) gpio_reg + 0x28);
    *gpio_clr_reg = 0x01;
}

static int device_open(struct inode *device_file, struct file *instance)
{
    printk("Device open was called \n");
    return 0;
}

static int device_close(struct inode *device_file, struct file *instance)
{
    printk("Device close was called \n");
    return 0;
}

static ssize_t device_read(struct file *file, char *user_buffer, size_t size, loff_t *offs)
{
    return copy_to_user(user_buffer, "Hello user \n", 11) ? 0 : 11;
}

static ssize_t device_write(struct file *file, const char *user_buffer, size_t size, loff_t *offs)
{
    char user_data[10];
    memset(user_data, 0, 10);
    if(copy_from_user(user_data, user_buffer, size))
        return 0;
    printk("User write: %s \n", user_data);

    if(strncmp(user_data, "bat", 3) == 0)
    {
        printk("Write bat");
        gpio_set();
    }

    if(strncmp(user_data, "tat", 3) == 0)
    {
        printk("Write tat");
        gpio_clr();
    }
    return size;
}
static struct file_operations fops =
{
    .owner   = THIS_MODULE,
    .read    = device_read,
    .write   = device_write,
    .open    = device_open,
    .release = device_close,
};

static int device_driver_init(void)
{
    if(alloc_chrdev_region(&dev_no, 0, 1, DRIVER_NAME) < 0)
    {
        printk("Device number cannot be allocated \n");
        return -1; 
    }
    printk("Driver number: Major - %d, Minor - %d \n", MAJOR(dev_no), MINOR(dev_no));
    if((cls = class_create(DRIVER_CLASS)) == NULL) // S?a l?i ? ?ây
    {
        printk("Device class cannot be created \n");
        unregister_chrdev_region(dev_no, 1);
        return -1;
    }

    if(device_create(cls, NULL, dev_no, NULL, DRIVER_NAME) == NULL)
    {
        printk("Device file cannot be created \n"); 
        class_destroy(cls);
        return -1;
    }
    
    cdev_init(&dev, &fops);
    if(cdev_add(&dev, dev_no, 1) == 1)
    {
        printk("Device registration failed \n");
        device_destroy(cls, dev_no);
        return -1;
    }

    gpio_reg = (int*)ioremap(BCM2711_GPIO_ADDRESS, PAGE_SIZE);
    if(gpio_reg == NULL) 
    {
        printk("Fail to map gpio \n");
    }
    else
    {
        printk("Map gpio! \n");
    }
printk("Hello device \n");
    
    return 0;
}
static void device_driver_exit(void)
{
    cdev_del(&dev);
    device_destroy(cls, dev_no);
    class_destroy(cls);
    unregister_chrdev_region(dev_no, 1);
    printk("Goodbye device \n");
}

module_init(device_driver_init);
module_exit(device_driver_exit);

MODULE_LICENSE("GPL");

