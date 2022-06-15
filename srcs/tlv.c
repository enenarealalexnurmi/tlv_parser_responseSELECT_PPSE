#include "tlv.h"

//Функция разбора бинарной строки на TLV в структуру.
//value - сырая строка, первым байтом которой идет тэг
//offset - смещение, для получения следующего тэга при разделении.
t_tlv*  get_next_tag(unsigned char *value, unsigned int *offset)
{
    t_tlv *tlv;

    if (!(tlv = malloc(sizeof(t_tlv))))
        printf("Malloc broken!\n");
    else
    {
        if (*(value + *offset) == 0x9F || *(value + *offset) == 0x5F || *(value + *offset) == 0xBF)
        {
            tlv->tag = *(value + *offset) << 8 | *(value + *offset + 1);
            ++(*offset);
        }
        else
            tlv->tag = *(value + *offset);
        ++(*offset);
        tlv->length = *(value + *offset);
        ++(*offset);
        if (!(tlv->value = malloc(tlv->length)))
        {
            printf("Malloc broken!\n");
            free(tlv);
        }
        else
        {
            memcpy(tlv->value, value + *offset, tlv->length);
            *offset += tlv->length;
        }
    }
    return tlv;
}

//Деструктор
void delTlv(t_tlv **data)
{
    if ((*data)->value)
        free((*data)->value);
    free(*data);
}
