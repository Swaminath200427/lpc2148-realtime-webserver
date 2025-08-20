int main(void) 
{
    net_if_addr6_t addr;

    // Get the default IPv6 address
    int ret = net_if_addr6_get_default(&addr);
    if (ret != 0) {
        printk("Failed to get default IPv6 address: %d\n", ret);
        return -1;
    }

    printk("Default IPv6 address: %s\n", net_addr6_ll_to_str(&addr.addr));

    return 0;
}