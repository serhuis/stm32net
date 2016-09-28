/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd-cgi.c,v 1.2 2006/06/11 21:46:37 adam Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-ajax.h"
#include "httpd-fs.h"



HTTPD_AJAX_CALL(addr, "set-addr", set_addr);

static const struct httpd_ajax_call *ajax_calls[] = {&addr,  NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
httpd_ajax_parser
httpd_ajax(char *name)
{
	const struct httpd_ajax_call **f;	
	
	char xhr_name[16] = {NULL}, xhr_len = 0;
	char *pname = NULL;
  
  for(pname = name+4; *pname != ','; pname++, xhr_len++)
	{
		xhr_name[xhr_len] = *pname;
	}
	/*
	name = strchr(pname, ':') + 1;
	

	argv = strchr(pname, ':') + 1;
	
	if(argv != NULL)
		argc = 1;

	for(pname = argv; *pname != ' '; pname++)
	{
		if(*pname == ':')
		{
			argc++; pname++;
		}
	}
	*/	
	/* Find the matching name in the table, return the function. */
  for(f = ajax_calls; *f != NULL; ++f) {
    if(strncmp((*f)->name, xhr_name, strlen((*f)->name)) == 0) {
      return (*f)->function;
    }

  }
  return nullfunction;
}

static unsigned short
generate_set_addr(void *arg)
{
  char *f = (char *)arg;
	char addr_str[4] = {0};
	unsigned char addr_val[4] = {0}, i=0, j=0;
/*	
	while(*f != ' ')
	{
		addr_str[i++] = *f;
    if(*f == '.')
		{
			addr_str[i-1] = 0;
			addr_val[j++] = atoi(addr_str);
			i = 0;
		}
		if(*f == ' ')
			break;
		f++;
	}

	
	
	
	*/
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", httpd_fs_count(f));
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(set_addr(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
	
	
	

  PSOCK_GENERATOR_SEND(&s->sout, generate_set_addr, strchr(ptr + 4, ':') + 1);
  
  PSOCK_END(&s->sout);
}


//uip_ipaddr_t uip_hostaddr, uip_draddr, uip_netmask;
/*---------------------------------------------------------------------------*/
/** @} */
