int main(struct multiboot *mbootptr)
{
	monitor_clear();
	monitor_write("Hello world!");
	return 0xDEADBABA;
}
