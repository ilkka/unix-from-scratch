int main(struct multiboot *mbootptr)
{
	monitor_clear();
	monitor_write("before printf");
	printf("Hello %s!", "world");
	monitor_write("after printf");
	return 0xDEADBABA;
}
