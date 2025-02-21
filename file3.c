#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/err.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/gpio.h>
/*
#define GPIO_16 16+512
#define GPIO_17 17+512
#define GPIO_18 18+512
#define GPIO_19 19+512
#define GPIO_20 20+512
#define GPIO_21 21+512
#define GPIO_22 22+512
#define GPIO_23 23+512
#define GPIO_24 24+512
#define GPIO_25 25+512
#define GPIO_26 26+512
#define GPIO_27 27+512
*/
#define MAX_GPIO 12
char gpio_name[MAX_GPIO][9] = {"GPIO-16","GPIO-17", "GPIO-18", "GPIO-19", "GPIO-20", "GPIO-21", "GPIO-22", "GPIO-23", "GPIO-24", "GPIO-25", "GPIO-26", "GPIO-27"};
int gpio_lut[MAX_GPIO] = {528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539};
int seg_lut[10] = {0x7E, 0x30, 0x6E, 0x7A, 0x3A, 0x7B, 0x7F, 0x3E, 0x7F, 0x7B};

//initialize
dev_t dev=0;
static struct class *dev_class;
static struct cdev ext_device;
struct gpio_desc *desc[MAX_GPIO];


//function declaration
static int __init device_init(void);
static void __exit device_exit(void);
static int ext_open(struct inode* inode, struct file* file);
static int ext_release(struct inode* inode, struct file* file);
static ssize_t ext_read(struct file* flip, char __user * buf, size_t len, loff_t *off);
static ssize_t ext_write(struct file* flip, const char * buf, size_t len, loff_t *off);

//strut initaialization for device file operations
static struct file_operations fops={
	.owner  = THIS_MODULE,
	.read   = ext_read,
	.write  = ext_write,
	.open   = ext_open,
	.release= ext_release,
};

//Device file open operation
static int ext_open(struct inode* inode, struct file* file){
	printk(KERN_INFO"Driver open function called:)\n");	
	return 0;
}

//Device file release operation
static int ext_release(struct inode* inode, struct file* file){
	printk(KERN_INFO"Driver Release function called:)\n");	
	return 0;
}

//Device file read from kernel operation
static ssize_t ext_read(struct file* flip, char __user * buf, size_t len, loff_t *off){
	int i;
	int gpio_state[MAX_GPIO] = {0};
	
	for(i=0; i < MAX_GPIO; i++)
		gpio_state[i] = gpio_get_value(gpio_lut[i]);

	
	if(copy_to_user(buf, gpio_state, sizeof(gpio_state)))//passing from krenel to user
		printk(KERN_ERR"Data cannot be read form GPIO !!! :(\n");

	for(i=0; i < MAX_GPIO; i++)
		printk(KERN_INFO"%s pin status =%d\n", gpio_name[i], gpio_state[i]);
	return 0;
}

//Device file write into kernel operation
static ssize_t ext_write(struct file* flip, const char * buf, size_t len, loff_t *off){
	int i;
	int8_t rec_buf[MAX_GPIO] = {0};
	if(copy_from_user(rec_buf, buf, len))//getting the value from user
		printk(KERN_ERR"Data cannot be written !!! :( \n");
	
	for(i=0; i < MAX_GPIO; i++)
		printk(KERN_INFO"%s = %d\n",gpio_name[i], rec_buf[i]);
	
	for(i=0; i < MAX_GPIO; i++)
		gpio_set_value(gpio_lut[i], rec_buf[i]);
	
	return len;
}

//Init function 
static int __init device_init(void){
	int i, j;
	//Allocating major and minor number dynamcally
	if(alloc_chrdev_region(&dev,0,1,"ext_dev")<0){
		printk(KERN_ERR"Cannot create MAJOR and MINOR number\n");
		goto r_unreg;
	}else	
		printk(KERN_INFO"MAJOR and MINOR number created \n");

	//Creating cdev structure
	cdev_init(&ext_device, &fops);

	//Adding character device to system
	if(cdev_add(&ext_device, dev, 1) < 0){
		printk(KERN_ERR"Character device cannot be added\n");
		goto r_cdev;
	}	 
	
	//creating class to store the file information in kernel
	if(IS_ERR(dev_class = class_create("ext_class"))){
		printk(KERN_ERR"Class not created\n");
		goto r_class;
	}
	
	//creating the device file
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "ext_device"))){
		printk(KERN_ERR"Device not created\n");
		goto r_device;
	}	

	//checking the valid 12 gpio pin
	for(i=0; i < MAX_GPIO; i++)
	{
		if(!gpio_is_valid(gpio_lut[i])){
			printk(KERN_ERR"ERROR : %s is not valid\n", gpio_name[i]);
			goto r_gpio;
		}
	}
		
	//Requesting the gpio number
	for(j=0; j < MAX_GPIO; j++)
	{
		if(gpio_request(gpio_lut[j], gpio_name[j]) < 0){
			printk(KERN_ERR"ERROR: %s request\n",gpio_name[j]);
			goto r_gpio;	
		}
	}

	for(i=0; i < MAX_GPIO; i++){
		//setting the gpio as output direction
		gpio_direction_output(gpio_lut[i], 0);
		//Export the gpio for user space
		desc[i]  = gpio_to_desc(gpio_lut[i]);
		//second arg 0 means we cannot change the output direction here after
		gpiod_export(desc[i], 0);
	}
	
	printk(KERN_INFO"Kernel module inserted\n");
	return 0;

//Error Handling
r_gpio:
	for(--i; i>=0; i--)
		gpio_free(gpio_lut[i]);
r_device:
	device_destroy(dev_class, dev);
r_class:
	class_destroy(dev_class);
r_cdev:
	cdev_del(&ext_device);
r_unreg:
	unregister_chrdev_region(dev,1);
	return -1;
}

//Exit function
static void __exit device_exit(void){
	int i;
	
	for(i=0; i < MAX_GPIO; i++)
		gpio_set_value(gpio_lut[i], 0);

	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&ext_device);
	unregister_chrdev_region(dev,1);
	for(i=0; i < MAX_GPIO; i++)
		gpiod_unexport(desc[i]);
	for(i=0; i < MAX_GPIO; i++)
		gpio_free(gpio_lut[i]);
	printk(KERN_INFO"Kernel module removed\n");
}

//Module initializtion
module_init(device_init);
module_exit(device_exit);

//Information about this driver programming
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BHUVI<bhuvankmg220@gmail.com>");
MODULE_DESCRIPTION("Real Device Driver");
MODULE_VERSION("2.0");



