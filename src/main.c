int main(struct multiboot *mbootptr)
{
	monitor_clear();
	printf("Hello %s!\n", "world");
	printf("Hello %d\n", 42);
	asm volatile("int $0x3");
	asm volatile("int $0x4");
	return 0xDEADBABA;
}
