#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>

static char *mode = "w";
module_param(mode, charp, 0);

static char *filename = NULL;
module_param(filename, charp, 0);

#define BUF_LEN 255
#define DEFNAME "/etc/yumex.profiles.conf"

static char buff[BUF_LEN + 1] = DEFNAME;

static int __init kread_init( void ) 
{
  struct file *f;
  ssize_t n;

  if (filename != NULL)
    strcpy(buff, filename);

  if (strcmp(mode, "r") == 0)
  {
    printk("*** openning file: %s\n", buff);
    printk("*** mode is %s\n", mode);

    f = filp_open(buff, O_RDONLY, 0);
    
    if (IS_ERR(f))
    {
      printk("*** file open failed: %s\n", buff);
      return -ENOENT;
    }

    n = kernel_read(f, buff, BUF_LEN, 0);

    if (n) 
    {
      printk("*** read first %d bytes:\n", n);
      buff[n] = '\0';
      printk("%s\n", buff);
    } 
    else
    {
      printk("*** kernel_read failed\n");
      return -EIO;
    }

    printk("*** close file\n");
    filp_close(f, NULL);
  }
  else if (strcmp(mode, "w") == 0)
  {
    char *string = "TEST STRING!\n";

    printk("*** openning file: %s\n", buff);
    printk("*** mode is %s\n", mode);
   
    f = filp_open(buff, O_RDWR | O_APPEND | O_CREAT, 0666);
       
    if (IS_ERR(f))
    {
      printk("*** file open failed: %s\n", buff);
      return -ENOENT;
    }

    n = kernel_write(f, string, 13, 0);

    if (n)
      printk("*** kernel_write successfully wrote %d bytes\n", n);
    else
    {
      printk("*** kernel_write failed\n");
      return -EIO;
    }

    printk("*** close file\n");
    filp_close(f, NULL);
  }
 
  return -EPERM;
}

module_init(kread_init);
MODULE_LICENSE("GPL");

