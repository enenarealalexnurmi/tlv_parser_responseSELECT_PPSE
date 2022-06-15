#ifndef TLV_H
#define TLV_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FCI_TEMPLATE 0x6F

typedef struct s_tlv
{
    unsigned short tag;
    unsigned short length;
    unsigned char *value;
}   t_tlv;

t_tlv*  get_next_tag(unsigned char *value, unsigned int *offset);
void    delTlv(t_tlv **data);

#endif //TLV_H