#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "net/gcoap.h"
#include "net/utils.h"
#include "od.h"
#include "nanocbor/nanocbor.h"

#include "gcoap_example.h"

char request_path[] = "/cli/stats_test";
coap_pkt_t pdu;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
size_t len;

/* initialize request */
gcoap_req_init(pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_FETCH, request_path);
/* Optionally add some options */
coap_opt_add_format(pdu, COAP_FORMAT_CBOR);
/* finish the headers and indicate that there is a payload */
len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
/* Copy in the payload, TODO: length check */
memcpy(pdu.payload, payload_buffer, payload_len);

gcoap_req_send(buf, len + payload_len, sock_remote, NULL, NULL);