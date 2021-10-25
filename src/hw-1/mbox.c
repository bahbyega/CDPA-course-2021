#include  "mbox.h"

msg_t  msg_init(mbox_t *MailBox)
{
     sem_init(&(MailBox->NotFull), PTHREAD_PROCESS_PRIVATE, 1);  // producer
     sem_init(&(MailBox->NotEmpty), PTHREAD_PROCESS_PRIVATE, 0); // consumer
     msg_t data = {0, 0, NULL};
     MailBox->Data = data;
     return data;
}

msg_t  msg_send(mbox_t *MailBox, msg_t *Value)
{
     sem_wait(&(MailBox->NotFull));
     MailBox->Data = *Value;
     sem_post(&(MailBox->NotEmpty));
     return  (*Value);
}

msg_t  msg_receive(mbox_t *MailBox, msg_t *Value)
{
     sem_wait(&(MailBox->NotEmpty));
     *Value = MailBox->Data;
     sem_post(&(MailBox->NotFull));
     return (*Value);
}