#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <modbus/modbus.h>

#define IP	"192.168.0.6"
#define PORT 	1502
#define FCONST	1000.0f

float modbus_get_float_array(int offset, uint16_t *arr);

int main()
{
	int s = -1;
//	int nb = 6;
	double axisX;
	double axisY;
	double axisZ;
	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;

	ctx = modbus_new_tcp(IP, PORT);
	mb_mapping = modbus_mapping_new(0, 0, 100, 0);

	if (mb_mapping == NULL) {
	        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}
	
	printf("-MODBUS SERVER-\n");
	printf("IP:%s\n", IP);
	printf("PORT:%d\n", PORT);
	printf("listen connection....\n");

	s = modbus_tcp_listen(ctx, 1);
	modbus_tcp_accept(ctx, &s);
	printf("Connection OK\n");

	for (;;) {
		uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;

		rc = modbus_receive(ctx, query);

		if (rc > 0) {
			axisX = modbus_get_float_array(0, mb_mapping->tab_registers);
			axisY = modbus_get_float_array(2, mb_mapping->tab_registers);
			axisZ = modbus_get_float_array(4, mb_mapping->tab_registers);
		
			printf("%f m/s2\n", axisX);
			printf("\n");
			printf("%f m/s2\n", axisY);
			printf("\n");
			printf("%f m/s2\n", axisZ);
			/*
			printf("\r%% axisX:%.2f m/s2\n axisY:%.2f m/s2\n axisZ:%.2f m/s2\n  ", axisX,
										         axisY,
                                                                		         axisZ);
			*/
			fflush(stdout);
			modbus_reply(ctx, query, rc, mb_mapping);
		} else if (rc == -1) {
			break;
		}
	}

	if (s != -1) {
		close(s);
	}
    
	modbus_mapping_free(mb_mapping);
    	modbus_close(ctx);
    	modbus_free(ctx);
		
	printf("\n");

    	return 0;
}




float modbus_get_float_array(int offset, uint16_t *arr)
{
	float f;
	int i;
	int nb;
	uint16_t *tmp;

	nb = 2;
	tmp = (uint16_t*)malloc(nb * sizeof(uint16_t));
	memset(tmp,0, nb * sizeof(uint16_t));

	for (i = 0; i < nb; i++) {
		tmp[i] = arr[i + offset];
	}

	f = modbus_get_float_badc(tmp);
	free(tmp);

	return f;
}
