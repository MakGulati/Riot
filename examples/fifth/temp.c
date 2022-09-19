
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "net/gcoap.h"
#include "net/utils.h"
#include "od.h"

#include "gcoap_example.h"

#define ENABLE_DEBUG 0
#include "debug.h"

#if IS_USED(MODULE_GCOAP_DTLS)
#include "net/dsm.h"
#endif

// static bool _proxied = false;
// static sock_udp_ep_t _proxy_remote;
// static char proxy_uri[64];

/* Retain request path to re-request if response includes block. User must not
 * start a new request (with a new path) until any blockwise transfer
 * completes or times out. */
// #define _LAST_REQ_PATH_MAX (64)
// static char _last_req_path[_LAST_REQ_PATH_MAX];

uint16_t req_count = 0;

/*
 * Response callback.
 */

// static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
//                           const sock_udp_ep_t *remote)
// {
//     (void)remote; /* not interested in the source currently */

//     if (memo->state == GCOAP_MEMO_TIMEOUT)
//     {
//         printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
//         return;
//     }
//     else if (memo->state == GCOAP_MEMO_RESP_TRUNC)
//     {
//         /* The right thing to do here would be to look into whether at least
//          * the options are complete, then to mentally trim the payload to the
//          * next block boundary and pretend it was sent as a Block2 of that
//          * size. */
//         printf("gcoap: warning, incomplete response; continuing with the truncated payload\n");
//     }
//     else if (memo->state != GCOAP_MEMO_RESP)
//     {
//         printf("gcoap: error in response\n");
//         return;
//     }

//     // coap_block1_t block;
//     // if (coap_get_block2(pdu, &block) && block.blknum == 0)
//     // {
//     //     puts("--- blockwise start ---");
//     // }

//     char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS)
//                           ? "Success"
//                           : "Error";
//     printf("gcoap: response %s, code %1u.%02u", class_str,
//            coap_get_code_class(pdu),
//            coap_get_code_detail(pdu));
//     if (pdu->payload_len)
//     {
//         unsigned content_type = coap_get_content_type(pdu);
//         if (content_type == COAP_FORMAT_TEXT || content_type == COAP_FORMAT_LINK || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE)
//         {
//             /* Expecting diagnostic payload in failure cases */
//             printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
//                    (char *)pdu->payload);
//         }
//         else
//         {
//             printf(", %u bytes\n", pdu->payload_len);
//             od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
//         }
//     }
//     else
//     {
//         printf(", empty payload\n");
//     }

//     /* ask for next block if present */
//     // if (coap_get_block2(pdu, &block))
//     // {
//     //     if (block.more)
//     //     {
//     //         unsigned msg_type = coap_get_type(pdu);
//     //         if (block.blknum == 0 && !strlen(_last_req_path))
//     //         {
//     //             puts("Path too long; can't complete blockwise");
//     //             return;
//     //         }

//     //         if (_proxied)
//     //         {
//     //             gcoap_req_init(pdu, (uint8_t *)pdu->hdr, CONFIG_GCOAP_PDU_BUF_SIZE,
//     //                            COAP_METHOD_GET, NULL);
//     //         }
//     //         else
//     //         {
//     gcoap_req_init(pdu, (uint8_t *)pdu->hdr, CONFIG_GCOAP_PDU_BUF_SIZE,
//                    COAP_METHOD_GET, NULL);
//     //         }

//     //         if (msg_type == COAP_TYPE_ACK)
//     //         {
//     //             coap_hdr_set_type(pdu->hdr, COAP_TYPE_CON);
//     //         }
//     //         block.blknum++;
//     //         coap_opt_add_block2_control(pdu, &block);

//     //         if (_proxied)
//     //         {
//     //             coap_opt_add_proxy_uri(pdu, _last_req_path);
//     //         }

//     //         int len = coap_opt_finish(pdu, COAP_OPT_FINISH_NONE);
//     //         gcoap_req_send((uint8_t *)pdu->hdr, len, remote,
//     //                        _resp_handler, memo->context);
//     //     }
//     //     else
//     //     {
//     puts("--- blockwise complete ---");
//     // }
//     // }
// }

static bool _parse_endpoint(sock_udp_ep_t *remote,
                            const char *addr_str, const char *port_str)
{
    netif_t *netif;

    /* parse hostname */
    if (netutils_get_ipv6((ipv6_addr_t *)&remote->addr, &netif, addr_str) < 0)
    {
        puts("gcoap_cli: unable to parse destination address");
        return false;
    }
    remote->netif = netif ? netif_get_id(netif) : SOCK_ADDR_ANY_NETIF;
    remote->family = AF_INET6;

    /* parse port */
    remote->port = atoi(port_str);
    if (remote->port == 0)
    {
        puts("gcoap_cli: unable to parse destination port");
        return false;
    }

    return true;
}
// static size_t _send(uint8_t *buf, size_t len, char *addr_str, char *port_str)
// {
//     size_t bytes_sent;
//     sock_udp_ep_t *remote;
//     sock_udp_ep_t new_remote;

//     // if (_proxied)
//     // {
//     //     remote = &_proxy_remote;
//     // }
//     // else
//     // {
//     if (!_parse_endpoint(&new_remote, addr_str, port_str))
//     {
//         return 0;
//     }
//     remote = &new_remote;
//     // }

//     bytes_sent = gcoap_req_send(buf, len, remote, _resp_handler, NULL);
//     if (bytes_sent > 0)
//     {
//         req_count++;
//     }
//     return bytes_sent;
// }

int main(void)

{
    // sock_udp_ep_t *remote = "cli/stats_test";
    bool ret = _parse_endpoint(&"cli/stats_test", "2001:db8::1", "5683");
    printf("%s", ret ? "true" : "false");

    return 0;
}