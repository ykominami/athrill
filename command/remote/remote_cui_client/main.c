#include "udp/udp_comm.h"
#include "target/target_os_api.h"
#include <string.h>
#include <stdio.h>
#include "cpuemu_config.h"

static int cmd_buffer_len = 0;
static char cmd_buffer[4096];

int main(int argc, const char* argv[])
{
	int i;
	UdpCommConfigType config;
	UdpCommType comm;
	int len;

	if (argc < 2) {
		printf("Usage: athrill_remote <athrill debug command>\n");
		return 1;
	}
	for (i = 1; i < argc; i++) {
		len = strlen(argv[i]);
		if ((cmd_buffer_len + (len + 2)) > UDP_BUFFER_LEN) {
			printf("argument length is too large.len=%d\n", len);
			return 1;
		}
		memcpy(&cmd_buffer[cmd_buffer_len], argv[i], len);
		cmd_buffer_len += (len + 1);
		cmd_buffer[cmd_buffer_len] = ' ';
	}

	config.server_port = CPUEMU_CONFIG_CUI_CLIENT_PORTNO;
	config.client_port = CPUEMU_CONFIG_CUI_EMULATOR_PORTNO;
	config.is_wait = TRUE;

	comm.write_data.len = cmd_buffer_len;
	memcpy(comm.write_data.buffer, cmd_buffer, cmd_buffer_len);
	cmd_buffer[cmd_buffer_len] = '\0';

#ifdef OS_LINUX
#else
	if (winsock_init() < 0) {
		printf("ERROR:winsock_init failed.\n");
		return 1;
	}
#endif

	if (udp_comm_create(&config, &comm) != STD_E_OK) {
		printf("ERROR:udp_comm_create failed.\n");
		return 1;
	}

	if (udp_comm_write(&comm) != STD_E_OK) {
		printf("ERROR:udp_comm_write failed.\n");
		return 1;
	}
	if (udp_comm_read(&comm) != STD_E_OK) {
		printf("ERROR:udp_comm_read failed.\n");
		return 1;
	}
#ifdef OS_LINUX
#else
	winsock_fini();
#endif

	printf("%s", comm.read_data.buffer);
	return 0;
}
