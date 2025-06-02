#include <rtdevice.h>
#include <rtthread.h>
#include <mini_shell.h>

#define SHELL_DEVICE_NAME "uart5"
#define SHEEL_SEM_NAME    "serial_sem"
#define SHEEL_THREAD_NAME "serial_rx"

static rt_device_t dev        = RT_NULL;
static rt_sem_t    serial_sem = RT_NULL;

const char *sheel_headtag = "\
\r        _      _        _            _     \r\n\
\r  _ __ (_)_ _ (_)___ __| |_  ___ ___| |    \r\n\
\r | '  \\| | ' \\| |___(_-< ' \\/ -_) -_) | \r\n\
\r |_|_|_|_|_||_|_|   /__/_||_\\___\\___|_|  \r\n\
\r            mini-shell v1.0                \r\n";

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(serial_sem);

    return RT_EOK;
}

static char shell_input_char(void *parameter)
{
    char        ch;
    rt_device_t dev = (rt_device_t)parameter;

    while(rt_device_read(dev, -1, &ch, 1) != 1)
    {
        rt_sem_take(serial_sem, RT_WAITING_FOREVER);
    }

    return ch;
}

void shell_output_char(void *parameter, char buf)
{
    rt_device_t dev = (rt_device_t)parameter;
    if(dev == RT_NULL || buf == RT_NULL)
    {
        return;
    }
    rt_device_write(dev, 0, &buf, 1);
}

static void serial_rx_thread_entry(void *parameter)
{
    while(1)
    {
        shell_servise();
    }
}

int sheel_port_init(void)
{
    dev = rt_device_find(SHELL_DEVICE_NAME);
    if(dev == RT_NULL)
    {
        rt_kprintf("find uart1 failed!\n");
        return 0;
    }

    serial_sem = rt_sem_create(SHEEL_SEM_NAME, 0, RT_IPC_FLAG_FIFO);
    if(serial_sem == RT_NULL)
    {
        rt_kprintf("create serial_sem failed!\n");
        return 0;
    }

    rt_device_set_rx_indicate(dev, uart_input);
    rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    rt_thread_t thread = rt_thread_create(SHEEL_THREAD_NAME, serial_rx_thread_entry, dev, 2048, 10, 20);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }

    shell_init(shell_output_char, dev, shell_input_char, dev, sheel_headtag);

    return 0;
}
INIT_APP_EXPORT(sheel_port_init);
