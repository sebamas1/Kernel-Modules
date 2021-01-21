/*
 * hello.c
 *
 *  Created on: 21 dic. 2020
 *      Author: sebastian
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
MODULE_LICENSE("Dual BSD/GPL");

int hello_init(void)
{
	register_chrdev(200, "encriptador", );
    printk(KERN_ALERT "Hello, world\n");
    return 0;
}
void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
