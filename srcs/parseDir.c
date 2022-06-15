#include "parseDir.h"

//Начальная инициализация структуры тэга FCI из бинарного файла.
t_tagDir* getFCIfromBinary(FILE *input)
{
    t_tagDir *takeResponse;
    if (!(takeResponse = malloc(sizeof(t_tagDir))))
        printf("Malloc broken!\n");
    else
    {
        if (!(takeResponse->data = malloc(sizeof(t_tlv))))
        {
            printf("Malloc broken!\n");
            free(takeResponse);
        }
        else
        {
            fread(&(takeResponse->data->tag), 1, 1, input);
            if (takeResponse->data->tag != FCI_TEMPLATE_TAG)
            {
                printf("Template tag wrong!\n");
                free(takeResponse->data);
                free(takeResponse);
            }
            else
            {
                fread(&(takeResponse->data->length), 1, 1, input);
                if (!(takeResponse->data->value = malloc(takeResponse->data->length)))
                {
                    printf("Malloc broken!\n");
                    free(takeResponse->data);
                    free(takeResponse);
                }
                else
                {
                    fread(takeResponse->data->value, takeResponse->data->length, 1, input);
                    takeResponse->next = NULL;
                }
            }
        }
    }
    return (takeResponse);
}

//Получение ноды директории тэга.
static int fillDownFromData(t_tagDir *root)
{
    unsigned int offset = 0;
    int ret = 1;
    t_tlv *data = root->data;
    
    if (!(root->down = malloc(sizeof(t_tagDir))))
    {
        printf("Malloc broken!\n");
        ret = 0;
    }
    else
    {
        root->down->data = get_next_tag(data->value, &offset);
        root->down->down = NULL;
        root->down->next = NULL;
        t_tagDir *temp = root->down;
        while (offset < data->length)
        {
            t_tagDir *next;
            if (!(next = malloc(sizeof(t_tagDir))))
            {
                printf("Malloc broken!\n");
                clearTree(&(root->down));
                ret = 0;
            }
            else
            {
                temp->next = next;
                temp = next;
                temp->data = get_next_tag(data->value, &offset);
                temp->down = NULL;
            }
        }
    }
    return ret;
}

int fillDown(t_tagDir *root)
{
    int ret;

    while (root)
    {
        if (root->data->tag == FCI_TEMPLATE_TAG ||
            root->data->tag == FCI_PROPRIETARY_TAG ||
            root->data->tag == FCI_IDD_TAG ||
            root->data->tag == FCI_DIRECTORY_ENTRY_TAG)
        {
            ret = fillDownFromData(root);
            ret = fillDown(root->down);
        }
        root = root->next;
    }
    return ret;
}

// Очистка дерева директорий тэгов с рекурсивным обходом, так как глубина небольшая вызова.
// Приоритетно очиста в глубину, а не по длине.
void clearTree(t_tagDir **dir)
{
    t_tagDir *curr = *dir;
    if (curr->down)
        clearTree(&(curr->down));
    if (curr->next)
        clearTree(&(curr->next));
    if (curr->data)
        delTlv(&(curr->data));
    free(curr);
}

//Печатать табы для красоты при распечатке дерева директорий тэгов.
static void printTab(int depth)
{
    while (depth--)
        printf("\t");
}

//Функция отображения в консоль разобранной структуры директорий тэгов.
void printTree(t_tagDir **dir, int depth)
{
    int prev_depth;
    t_tagDir *curr = *dir;
    if (curr->data)
    {
        printTab(depth);
        printf("Tag: %x\n", curr->data->tag);
        printTab(depth);
        printf("Len: %u\n", curr->data->length);
        printTab(depth);
        printf("Val: %s\n", curr->data->value);
    }
    else
    {
        printf("NONE\n");
    }
    prev_depth = depth;
    if (curr->down)
    {
        ++depth;
        printTree(&(curr->down), depth);
    }
    depth = prev_depth;
    if (curr->next)
        printTree(&(curr->next), depth);
}