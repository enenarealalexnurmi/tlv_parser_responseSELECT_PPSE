#include "appUtils.h"

//Разбор двух тэго 4F и 87 для формирования элемента структуры "Приложение"
static t_appItem* newAppItem(t_tagDir *adfTag, t_tagDir *priorityTag)
{
    t_appItem *new;
    if (!(new = malloc(sizeof(t_appItem))) || !(new->data = malloc(sizeof(t_appData))))
    {
        printf("Malloc broken!\n");
    }
    else
    {
        memcpy(new->data->RID, adfTag->data->value, LENGTH_APP_ID);
        if (!(new->data->PIX = malloc(adfTag->data->length - LENGTH_APP_ID + 1)))
        {
            printf("Malloc broken!\n");
        }
        else
        {
            memcpy(new->data->PIX, adfTag->data->value + LENGTH_APP_ID, adfTag->data->length - LENGTH_APP_ID);
            new->data->PIX[adfTag->data->length - LENGTH_APP_ID] = '\0';
        }
        if (priorityTag)
            memcpy(&new->data->priority, priorityTag->data->value, 1);
        new->next = NULL;
    }
    return new;
}

//Добавление по приоритету вместо сортировки собраного списка, сразу разположения элемента приложения по приоритету
//Перед первым элементом приоритетом ниже.
static void addAppFromPriority(t_appItem **list, t_appItem *item)
{
    t_appItem *curr = *list;

    if (curr)
    {
        if (curr->data->priority < item->data->priority)
        {
            item->next = curr;
        }
        else
        {
            while (curr->next)
            {
                if (curr->next->data->priority < item->data->priority)
                {
                    item->next = curr->next;
                    break ;
                }
            }
            curr->next = item;
        }
    }
    else
        *list = item;
}

//Добавление в конец списка - используется для приложений без приоритета.
static void addAppBack(t_appItem **list, t_appItem *item)
{
    t_appItem *curr = *list;

    if (curr)
    {
        while (curr->next)
            curr = curr->next;
        curr->next = item;
    }
    else
        *list = item;
}

//Проверка, является RID выбранного приложения поддерживаемым. Сверка равенства.
static int isSupported(unsigned char *app, unsigned char suppApp[LENGTH_APP_ID])
{
    int ret = 1;
    ssize_t i = -1;

    while (++i < LENGTH_APP_ID)
    {
        ret &= (app[i] == suppApp[i]);
    }
    return ret;
}

//Освобождение элемента "Приложение". Деструктор
static void delItem(t_appItem **item)
{
    free((*item)->data->PIX);
    free((*item)->data);
    free((*item));
}

//Добавление нового приложения в список, если оно из поддерживаемых
static void addAppItem(t_appItem **list, unsigned char suppApps[COUNT_SUPPORTED_APPS][LENGTH_APP_ID], t_tagDir *adfTag, t_tagDir *priorityTag)
{
    t_appItem *item = newAppItem(adfTag, priorityTag);

    ssize_t i = -1;
    while (++i < COUNT_SUPPORTED_APPS)
    {
        if (isSupported(item->data->RID, suppApps[i]))
        {
            if (item->data->priority)
                addAppFromPriority(list, item);
            else
                addAppBack(list, item);
            break ;
        }
        else
            delItem(&item);
    }
}

//Функция перебора курсора в просмотре директорий тэгов
static t_tagDir* lookup(t_tagDir *dir, unsigned short tag)
{
    t_tagDir *ret = NULL;
    while (dir && dir->data->tag != tag)
        dir = dir->next;
    if (dir)
        ret = dir;
    return ret;
}

//Обход по подобию в глубину, коленчатый курсор.
//На структуре вложенных директорий тегов, выделяем нижний элемент на уровне поиска и перебираем в поиске нужного тега.
//При завершении обхода в глубине смещается курсор уровнем выше.
t_appItem* getSupportedApplications(t_tagDir *resp, unsigned char suppApps[COUNT_SUPPORTED_APPS][LENGTH_APP_ID])
{
    t_appItem *list = NULL;

    t_tagDir *cur1 = resp->down;
    while ((cur1 = lookup(cur1, FCI_PROPRIETARY_TAG)))
    {
        if (cur1)
        {
            t_tagDir *cur2 = cur1->down;
            while ((cur2 = lookup(cur2, FCI_IDD_TAG)))
            {
                if (cur2)
                {
                    t_tagDir *cur3 = cur2->down;
                    while ((cur3 = lookup(cur3, FCI_DIRECTORY_ENTRY_TAG)))
                    {
                        if (cur3)
                        {
                            addAppItem(&list, suppApps, lookup(cur3->down, ADF_NAME_TAG), lookup(cur3->down, APP_PRIORITY_TAG));
                            cur3 = cur3->next;
                        }
                    }
                    cur2 = cur2->next;
                }
            }
            cur1 = cur1->next;    
        }  
    }
    return list;
}

//Отображение в консоль списка выбранных приложений
void printAppList(t_appItem *head)
{
    printf(" # |      RID     | PIX  \n");
    printf("=========================\n");
    size_t i = 0;
    while (head)
    {
        printf("%3ld| 0x%.2X%.2X%.2X%.2X%.2X | 0x",
        ++i, head->data->RID[0], head->data->RID[1], head->data->RID[2], head->data->RID[3], head->data->RID[4]);
        unsigned char *s = head->data->PIX;
        while (*s)
        {
            printf("%.2x", *s);
            ++s;
        }
        printf("\n");
        head = head->next;
    }
}

//Деструктов списка выбранных приложений
void        clearAppList(t_appItem **head)
{
    t_appItem *tmp;

    while (*head)
    {
        tmp = (*head)->next;
        delItem(head);
        *head = tmp;
    }
}