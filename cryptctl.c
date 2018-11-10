#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/cred.h>

struct class *cryptClass = NULL;
struct device *cryptDev = NULL;
struct cdev cryptCdev;
int majorNum;
int check;
int crypt_open(struct inode *inode, struct file *fi);
int crypt_release(struct inode *inode, struct file *fi);
MODULE_LICENSE("GPL");

struct file_operations file_ops = {
	.open = crypt_open,
	.release = crypt_release,
};

static int __init hello_init(void){
	dev_t dev;

	cryptClass = class_create(THIS_MODULE, "cryptdev");
	if (IS_ERR(cryptClass)) {
		pr_err("error class create\n");
		return  PTR_ERR(cryptClass);
	}


	check = alloc_chrdev_region(&dev, 0, 100, "crypter");
	if(check < 0){
		pr_err("error in alloc");
		goto err_alloc;
	} 

	majorNum = MAJOR(dev);
	printk(KERN_ALERT "major number: %d\n", majorNum);

	cdev_init(&cryptCdev, &file_ops);
	cryptCdev.owner = THIS_MODULE;

	check = cdev_add(&cryptCdev, MKDEV(majorNum, 0), 1);
	
	if(check < 0){
		pr_err("error in add");
		goto err_cdev;
	}
	
	cryptDev = device_create(cryptClass, NULL, dev, NULL, "cryptctl");
	
	if(IS_ERR(cryptDev)){
		pr_err("err in dev create");
		check = PTR_ERR(cryptDev);
		goto err_dev;
	}

:
	return 0;

err_alloc:
	if (cryptClass)
		class_destroy(cryptClass);

err_cdev:
	unregister_chrdev_region(MKDEV(majorNum, 0), 1);

err_dev:
	cdev_del(&cryptCdev);
	return 0;
}

static void hello_exit(void){
	dev_t dev = MKDEV(majorNum, 0);
	device_destroy(cryptClass, dev);
	cdev_del(&cryptCdev);
	unregister_chrdev_region(MKDEV(majorNum, 0), 1);
	if(cryptClass)
		class_destroy(cryptClass);
}


int crypt_open(struct inode *inode, struct file *fi){
	

	return 0;
}

int crypt_release(struct inode *inode, struct file *fi){

	return 0;
}



module_init(hello_init);
module_exit(hello_exit);
