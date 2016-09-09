/* Standard includes. */
#include "cmsis_os.h"
#include <RTL.h>
#include <string.h>
#include "stm32net.h"
#include "uip.h"
#include "uip_arp.h"



//--------------------------------------------------------------
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
//--------------------------------------------------------------

#define DEF_IP	{192,168,3,248}

u8 HostIP[4] = DEF_IP;


void Thread_uIP_periodic(void const *pvParameters);
void Thread_uIP(void const *pvParameters);
int Init_Thread (void);
void demo_init(void);

osThreadId tid_Thread_uIP_periodic, tid_Thread_uIP;                                          // thread id
osThreadDef(Thread_uIP_periodic, osPriorityBelowNormal, 1, 0);                   // thread object
osThreadDef(Thread_uIP, osPriorityNormal, 1, 0);                   // thread object
int Init_Thread (void) {

  tid_Thread_uIP_periodic = osThreadCreate (osThread(Thread_uIP_periodic), NULL);
  if(!tid_Thread_uIP_periodic) return(-1);
	
  tid_Thread_uIP = osThreadCreate (osThread(Thread_uIP), NULL);
  if(!tid_Thread_uIP) return(-1);
	
	

  return(0);
}
void Thread_uIP_periodic(void const *pvParameters) {
	uint32_t i;
	uint8_t delay_arp = 0;
	volatile uint32_t st;

	for (;;) {
	
//		vTaskDelay(configTICK_RATE_HZ/2); // ??????????
		osDelay (3);
		delay_arp++;

		for (i = 0; i < UIP_CONNS; i++) {
			uip_periodic(i);
			if (uip_len > 0) {
				uip_arp_out();
				enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
			}
		}

#if UIP_UDP
		for(i = 0; i < UIP_UDP_CONNS; i++) {
			uip_udp_periodic(i);
			if(uip_len > 0) {
				uip_arp_out();
				//network_send();

			}
		}
#endif // UIP_UDP

		if (delay_arp >= 50) { // ???? ??? ?? 50 ???????? ?????, ????? 10 ???.
			delay_arp = 0;
			uip_arp_timer();
		}
//		osThreadYield();
//		st = uxTaskGetStackHighWaterMark(NULL);

	}

}
//--------------------------------------------------------------
void Thread_uIP(void const * pvParameters) {
	volatile uint32_t st;


	for (;;) {
	
		uip_len = enc28j60_recv_packet((uint8_t *) uip_buf, UIP_BUFSIZE);

		if (uip_len > 0) {
			if (BUF->type == htons(UIP_ETHTYPE_IP)) {
				uip_arp_ipin();
				uip_input();
				if (uip_len > 0) {
					uip_arp_out();
					enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
				}
			} else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
				uip_arp_arpin();
				if (uip_len > 0) {
					enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
				}
			}
		}
	}
}


int main(void) {
	

	struct uip_eth_addr mac = { { 0x00, 0x01, 0x02, 0x03, 0x04, 0x00 } };
	uip_ipaddr_t ipaddr;

	enc28j60_init(mac.addr);
	uip_init();
	uip_arp_init();

	httpd_init();

	uip_setethaddr(mac);

	uip_ipaddr(ipaddr, HostIP[0], HostIP[1], HostIP[2], HostIP[3]);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192, 168, 3, 1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255, 255, 255, 0);
	uip_setnetmask(ipaddr);


		
	Init_Thread();
//	Init_Timers();


	if (!osKernelRunning ())  {                    // is the kernel running ?
    if (osKernelStart () != osOK)  {             // start the kernel
                                                 // kernel could not be started
    }
  }
	
	
	
	
	/* Will only get here if there was insufficient memory to create the idle
	 task.  The idle task is created within vTaskStartScheduler(). */
	for (;;)
		;

}
/*-----------------------------------------------------------*/
void uip_log(char *msg) {
	while(0);
}

void vApplicationTickHook(void) {

//	TimingDelay_Decrement();
}
/*-----------------------------------------------------------*/

static void HardFault_Handler(void)
{
	char i;
	while(++i)
	{
		++i;
	}
	
}


FRESULT fs_init(card_type_id_t* disk, FATFS* fs)
{
	*disk = mmc_mount();
	if(NO_CARD != *disk)
	{
		return f_mount(fs, _T("0"), 1);
	}
	else
		return FR_DISK_ERR;

}