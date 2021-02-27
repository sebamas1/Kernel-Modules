/*
 * encriptador.c
 *
 *  Created on: 21 ene. 2021
 *      Author: sebastian
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#pragma GCC diagnostic pop

MODULE_LICENSE("Dual BSD/GPL");

#define DEVICE_NAME "TP4"

struct cdev *mydev;
unsigned int major_number;
dev_t dev_num;
int ret;

struct device_data {
	struct cdev cdev;
	char data[100];
/* my data starts here */
//...
} my_device_data;
int device_open(struct inode* inode, struct file *filp){
    //only allow one process to open this device by using a semaphore as mutual exclusive lock-mutex
//    if(down_interruptible(&virtual_device.sem) != 0){
//        printk(KERN_ALERT "soliduscode: could not lock device during open");
//        return -1;
//    }
    printk(KERN_INFO "soliduscode: opened device");
    return 0;
}

ssize_t device_read(struct file* filp, char* bufStoreData,size_t bufCount,loff_t* curOffset){
    //take data from kernel space (device) to user space (process)
    //copy_to_user (destination,source,sizeToTransfer)
    // printk(KERN_INFO "soliduscode: Reading from device");
    // ret = copy_to_user(bufStoreData,virtual_device.data,bufCount);
    // return ret;
    int randomNumber;
    get_random_bytes(&randomNumber,sizeof(randomNumber));
    // printk(KERN_ALERT "TEST: %d",randomNumber);
    return randomNumber;
    // return 999;
}
ssize_t device_write(struct file* filp,const char* bufSourceData,size_t bufCount,loff_t* curOffset){
    //send data from user to kernel
    //copy_from_user (dest, source,count);
    printk(KERN_INFO "soliduscode: writing to device");
    ret = (int) copy_from_user(my_device_data.data, bufSourceData, bufCount);
    return ret;
}
int device_close(struct inode *inode, struct file *filp){
    //by calling up, which is opposite of down for semaphore, we release the mutex that we obtained at device open
    //this has the effect of allowing other process to use the device now
//    up(&virtual_device.sem);
    printk(KERN_INFO "soliduscode: closed device");
    return 0;
}

const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_close,
};


static int driver_entry(void){
    //step (1) use dynamic allocation to assign our device
    // a major number-- alloc_chrdev_region(dev_t*, unit fminor,unit count, char* name)
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret<0) {
        printk(KERN_ALERT "soliduscode: failed to allcate a major number");
        return ret;
    }
    major_number= MAJOR(dev_num); //extracts the major number and store in our variable (MACRO)
    printk(KERN_INFO "soliduscode: major number is %d", major_number);
    printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file",DEVICE_NAME,major_number); //dmesg
    //step(2)
    mydev = cdev_alloc(); //create our cdev structure, initialized our cdev
    mydev->ops = &fops; //struct file_operations
    //now that we create cdev, we have to add it to the kernel
    //int cdev_add(struct cdev* dev, dev_t num, unsigned int count);
    ret = cdev_add(mydev, dev_num, 1);
    if(ret<0) { //always check errors
        printk(KERN_ALERT "soliduscode: unable to add cdev to kernel");
        return ret;
    }
    //(4) Initialize our semaphore
//    sema_init(&virtual_device.sem,1);  //initial value of one

    return 0;
}

static void driver_exit(void){
    //(5) unregister everything in reverse order
    //(a)
    cdev_del(mydev);
    //(b)
    unregister_chrdev_region(dev_num,1);
    printk(KERN_ALERT "soliduscode: unloaded module");
}

module_init(driver_entry);
module_exit(driver_exit);

