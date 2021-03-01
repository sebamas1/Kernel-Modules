/*
 * desencriptador.c
 *
 *  Created on: 28 feb. 2021
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

#define DEVICE_NAME "desencriptador"
#define BUFFER_SIZE 1000

struct cdev *mydev;
unsigned int major_number;
dev_t dev_num;
int ret;

struct device_data {
	char data[BUFFER_SIZE];
	struct semaphore sem;
} my_device_data;

int device_open(__attribute__((unused))     struct inode *inode,
		__attribute__((unused))     struct file *filp) {
	//only allow one process to open this device by using a semaphore as mutual exclusive lock-mutex
	if (down_interruptible(&my_device_data.sem) != 0) {
		printk(KERN_ALERT "encriptador: could not lock device during open");
		return -1;
	}
	printk(KERN_INFO "desencriptador: opened device\n");
	return 0;
}

ssize_t device_read(__attribute__((unused))     struct file *filp,
		__attribute__((unused)) char *bufStoreData,
		__attribute__((unused))     size_t bufCount,
		__attribute__((unused))     loff_t *curOffset) {
	printk(KERN_INFO "desencriptador: Reading from device\n");

	ret = (int) copy_to_user(bufStoreData, my_device_data.data, bufCount);
	return ret;
}
ssize_t device_write(__attribute__((unused))    struct file *filp,
		const char *bufSourceData, size_t bufCount,
		__attribute__((unused))    loff_t *curOffset) {

	printk(KERN_INFO "desencriptador: writing to device\n");

	ret = (int) copy_from_user(my_device_data.data, bufSourceData, bufCount);

	for (int i = 0; i < strlen(bufSourceData); i++) {
		my_device_data.data[i] = (char) ((int) my_device_data.data[i] - 3);
	}

	return ret;
}
int device_close(__attribute__((unused))    struct inode *inode,
		__attribute__((unused))    struct file *filp) {
	up(&my_device_data.sem);
	printk(KERN_INFO "desencriptador: closed device\n");
	return 0;
}

const struct file_operations fops = { .owner = THIS_MODULE, .open = device_open,
		.read = device_read, .write = device_write, .release = device_close, };

static int driver_entry(void) {

	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ALERT "desencriptador: no se pudo alocar el major number\n");
		return ret;
	}
	major_number = MAJOR(dev_num); //extracts the major number and store in our variable (MACRO)
	printk(KERN_INFO "desencriptador: major number is %d \n", major_number);
	printk(KERN_INFO "\tuse \"sudo mknod /dev/%s c %d 0\" for device file\n", DEVICE_NAME, major_number);

	mydev = cdev_alloc(); //create our cdev structure, initialized our cdev
	mydev->ops = &fops; //struct file_operations
	//now that we create cdev, we have to add it to the kernel
	//int cdev_add(struct cdev* dev, dev_t num, unsigned int count);
	ret = cdev_add(mydev, dev_num, 1);
	if (ret < 0) {
		printk(KERN_ALERT "desencriptador: unable to add cdev to kernel\n");
		return ret;
	}
//	Initialize our semaphore
	sema_init(&my_device_data.sem, 1);  //initial value of one

	return 0;
}

static void driver_exit(void) {
	cdev_del(mydev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "desencriptador: unloaded module\n");
}

module_init( driver_entry);
module_exit( driver_exit);

