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
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
size_t len;

static bool _parse_endpoint(sock_udp_ep_t *remote,
                            const char *addr_str, uint16_t port)
{
    netif_t *netif;

    /* parse hostname */
    if (netutils_get_ipv6((ipv6_addr_t *)&remote->addr, &netif, addr_str) < 0)
    {
        puts("gcoap_cli: unable to parse destination address");
        return false;
    }

    /* Set the endpoint members */
    remote->netif = netif ? netif_get_id(netif) : SOCK_ADDR_ANY_NETIF;
    remote->family = AF_INET6;

    remote->port = port;
    return true;
}

ssize_t _send_coap_req(uint8_t *buf, size_t len, const char *dest_address,
                       gcoap_resp_handler_t resp_handler, void *context)
{
    sock_udp_ep_t remote;
    if (!_parse_endpoint(&remote, dest_address, COAP_PORT))
    {
        /* unable to parse the endpoint address, exit */
        return 0;
    }
    /* Ask gcoap to send the request */
    return gcoap_req_send(buf, len, &remote, resp_handler, context);
}

/**
 * @brief Send out a coap GET request to the destination IP and endpoint.
 *
 * The supplied callbacks are called when the request is complete.
 *
 * @note Not thread safe
 *
 * @param   dest_ip         Destination address as string
 * @param   endpoint        CoAP endpoint to query
 * @param   resp_handler    Callback for the reply
 * @param   context         Optional context pointer that will be supplied with
 *                          the callback
 */

gcoap_resp_handler_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                                   const sock_udp_ep_t *remote)
{

    (void)remote; /* not interested in the source currently */
    (void)memo;
    // if (memo->state == GCOAP_MEMO_TIMEOUT)
    // {
    //     printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
    //     return;
    // }
    // else if (memo->state == GCOAP_MEMO_RESP_TRUNC)
    // {

    //     printf("gcoap: warning, incomplete response; continuing with the truncated payload\n");
    // }
    // else if (memo->state != GCOAP_MEMO_RESP)
    // {
    //     printf("gcoap: error in response\n");
    //     return;
    // }
    printf("gcoap client printing payload: %d", pdu->payload_len);
    // return ;
}

ssize_t coap_request(const char *dest_ip, const char *endpoint,
                     gcoap_resp_handler_t resp_handler, void *context)
{
    coap_pkt_t pdu;
    /* initialize request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_GET, endpoint);
    /* Set it as confirmable */
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    /* No other options and no payload in a GET request  */
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    return _send_coap_req(buf, len, dest_ip, resp_handler, context);
}

print("%d ", coap_request("2001:db8::1", request_path, _resp_handler, 0));
