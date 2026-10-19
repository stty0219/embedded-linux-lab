#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEV_NAME "mychar"
#define BUF_SIZE 1024

static dev_t devno;
static struct cdev mychar_cdev;
static struct class *mychar_class;

static char kbuf[BUF_SIZE];
static size_t kbuf_len;

static int mychar_open(struct inode *inode, struct file *filp)
{
	pr_info("mychar: open\n");
	return 0;
}

static int mychar_release(struct inode *inode, struct file *filp)
{
	pr_info("mychar: release\n");
	return 0;
}

static ssize_t mychar_read(struct file *filp, char __user *ubuf,
			   size_t count, loff_t *ppos)
{
	/* offset 已經到尾端：回傳 0 代表 EOF，cat 才會結束 */
	if (*ppos >= kbuf_len)
		return 0;

	if (count > kbuf_len - *ppos)
		count = kbuf_len - *ppos;

	/* 回傳值是「沒搬成功的位元組數」，非 0 就是失敗 */
	if (copy_to_user(ubuf, kbuf + *ppos, count))
		return -EFAULT;

	*ppos += count;
	pr_info("mychar: read %zu bytes\n", count);
	return count;
}

static ssize_t mychar_write(struct file *filp, const char __user *ubuf,
			    size_t count, loff_t *ppos)
{
	/* 超過 buffer 就拒絕，不截斷，避免呼叫端誤以為寫完 */
	if (count > BUF_SIZE)
		return -ENOSPC;

	if (copy_from_user(kbuf, ubuf, count))
		return -EFAULT;

	kbuf_len = count;	/* 每次 write 都覆蓋舊內容 */
	pr_info("mychar: write %zu bytes\n", count);
	return count;
}

static const struct file_operations mychar_fops = {
	.owner   = THIS_MODULE,
	.open    = mychar_open,
	.release = mychar_release,
	.read    = mychar_read,
	.write   = mychar_write,
};

static int __init mychar_init(void)
{
	struct device *dev;
	int ret;

	ret = alloc_chrdev_region(&devno, 0, 1, DEV_NAME);
	if (ret)
		return ret;

	cdev_init(&mychar_cdev, &mychar_fops);
	mychar_cdev.owner = THIS_MODULE;
	ret = cdev_add(&mychar_cdev, devno, 1);
	if (ret)
		goto err_region;

	mychar_class = class_create(DEV_NAME);
	if (IS_ERR(mychar_class)) {
		ret = PTR_ERR(mychar_class);
		goto err_cdev;
	}

	dev = device_create(mychar_class, NULL, devno, NULL, DEV_NAME);
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto err_class;
	}

	pr_info("mychar: registered, major=%d minor=%d\n",
		MAJOR(devno), MINOR(devno));
	return 0;

err_class:
	class_destroy(mychar_class);
err_cdev:
	cdev_del(&mychar_cdev);
err_region:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit mychar_exit(void)
{
	/* 順序與 init 相反 */
	device_destroy(mychar_class, devno);
	class_destroy(mychar_class);
	cdev_del(&mychar_cdev);
	unregister_chrdev_region(devno, 1);
	pr_info("mychar: unregistered\n");
}

module_init(mychar_init);
module_exit(mychar_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Minimal character driver demo");