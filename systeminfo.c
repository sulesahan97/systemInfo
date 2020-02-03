#include <linux/module.h>  // Needed by all modules
#include <linux/kernel.h>  // Needed for KERN_INFO
#include <linux/fs.h>      // Needed by filp
#include <asm/uaccess.h>   // Needed by segment descriptors





//initial values
static int p=0;

static int all=0;

//I learned this methods from this website http://linuxkernel51.blogspot.com/2011/03/how-to-pass-command-line-arguments-to.html

//In here it takes p parameter
module_param(p, int, 0);
//In here it takes all parameter
module_param(all, int, 0);



// This function reads file in kernel module... I found this function from https://www.howtoforge.com/reading-files-from-the-linux-kernel-space-module-driver-fedora-14.
//I used same read functions all of the real file operations...
int cpuinfo(void)
{
    // Create variables
    struct file *f;
    char buf[128];
    mm_segment_t fs;



    int i;
    // Init the buffer with 0
    for(i=0;i<128;i++)
        buf[i] = 0;

    // To see in /var/log/messages that the module is operating
    printk(KERN_INFO "My module is loaded\n");
    printk(KERN_INFO "---------CPU INFORMATIONS-----------");

    // I used /proc/cpuinfo file to read in this function.
    f = filp_open("/proc/cpuinfo", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 128, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);
    return 0;
}

int loadavginfo(void)
{
    // Create variables
    struct file *f;
    char buf[128];
    mm_segment_t fs;


    int i;
    // Init the buffer with 0
    for(i=0;i<128;i++)
        buf[i] = 0;
    // To see in /var/log/messages that the module is operating


    printk(KERN_INFO "My module is loaded\n");
    printk(KERN_INFO "----------SYSTEM LOAD INFORMATIONS----------");

    // I used /proc/loadavg file to read in this function.

    f = filp_open("/proc/loadavg", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 128, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);
    return 0;
}



int uptimeinfo(void)
{
    // Create variables
    struct file *f;
    char buf[128];
    mm_segment_t fs;


    int i;
    // Init the buffer with 0
    for(i=0;i<128;i++)
        buf[i] = 0;
    // To see in /var/log/messages that the module is operating
    printk(KERN_INFO "My module is loaded\n");
    printk(KERN_INFO "The length of time since the system was booted..");
    // I used /proc/uptime file to read in this function.
    
    f = filp_open("/proc/uptime", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 128, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);
    return 0;
}


//this function takes parameter pid and find it if it is exists....
int processinfo(int pid)
{
    // Create variables
    struct file *f;
    char buf[128];
    mm_segment_t fs;


    int i;
    // Init the buffer with 0
    for(i=0;i<128;i++)
        buf[i] = 0;
    // To see in /var/log/messages that the module is operating
    printk(KERN_INFO "My module is loaded\n");
    printk(KERN_INFO "PROCESS INFORMATION WHICH IS YOU SELECTED...");
    // I used /proc/uptime file to read in this function.
    

    // It fills the array with 0's.It empty the array.....
    char filnameptr[128];
    for(i = 0; i < 128; i++){
        filnameptr[i] = 0;
    }

    //It takes array and pid...I used sprintf function...
    sprintf(filnameptr , "/proc/%d/status" , pid);

    f = filp_open(filnameptr, O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 128, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);
    return 0;
}

//initial module
int init_module(void)
{
   //It prints CPU informations unconditionally...
   cpuinfo();


  //if all different than zero it prints "load average informations" and "uptime informations"..
   if(all!=0)
   {
   loadavginfo();
   uptimeinfo();
   }

  //If process id bigger than zero and if it is exists, it prints the informations of process...
   if(p>0)
   {
       processinfo(p);

   }

   return 0;

}

//exit module
void exit_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}
