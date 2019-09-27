#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd, val;

	fd = open("/proc/xenomai/registry/native/pipes/sensor_data", O_RDONLY);
	if(fd == -1)
	{
		perror("failed to open rt pipe.\n");
		_exit(1);
	}
	printf("rt pipe is opened...\n");

	while(read(fd, &val, sizeof(val)) > 0)
	{
		printf("read : %d\n", val);
	}

	close(fd);
	printf("rt pipe is closed...\n");
	return 0;
}


