int main(struct multiboot *mbootptr)
{
	monitor_clear();
	printf("Hello %s!\n", "world");
	printf("Hello %d\n", 42);
	return 0xDEADBABA;
}
