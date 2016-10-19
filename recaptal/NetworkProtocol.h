#pragma once
#include <stdint.h>
typedef uint8_t u_char;
typedef uint16_t u_short;
typedef uint32_t u_int;
#define	ETHER_ADDR_LEN		6
/*
* define struct of ethernet header , ip address , ip header and tcp header
*/
/* ethernet header */
typedef struct ether_header {
	u_char ether_shost[ETHER_ADDR_LEN]; /* source ethernet address, 8 bytes */
	u_char ether_dhost[ETHER_ADDR_LEN]; /* destination ethernet addresss, 8 bytes */
	u_short ether_type;                 /* ethernet type, 16 bytes */
}ether_header;

/* four bytes ip address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* ipv4 header */
typedef struct ip_header {
	u_char ver_ihl;         /* version and ip header length */
	u_char tos;             /* type of service */
	u_short tlen;           /* total length */
	u_short identification; /* identification */
	u_short flags_fo;       // flags and fragment offset
	u_char ttl;             /* time to live */
	u_char proto;           /* protocol */
	u_short crc;            /* header checksum */
	ip_address saddr;       /* source address */
	ip_address daddr;       /* destination address */
	u_int op_pad;           /* option and padding */
}ip_header;

/* tcp header */
typedef struct tcp_header {
	u_short th_sport;         /* source port */
	u_short th_dport;         /* destination port */
	u_int th_seq;             /* sequence number */
	u_int th_ack;             /* acknowledgement number */
	u_short th_len_resv_code; /* datagram length and reserved code */
	u_short th_window;        /* window */
	u_short th_sum;           /* checksum */
	u_short th_urp;           /* urgent pointer */
}tcp_header;