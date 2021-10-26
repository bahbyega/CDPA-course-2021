#include  "mbox.h"

msg_t  msg_init(mbox_t *MailBox)
{
     sem_init(&(MailBox->NotFull), PTHREAD_PROCESS_PRIVATE, 0);
     sem_init(&(MailBox->NotEmpty), PTHREAD_PROCESS_PRIVATE, 1);
     msg_t msg = {0, 0, NULL, NULL};
     MailBox->Msg = msg;
     return msg;
}

msg_t  msg_send(mbox_t *MailBox, msg_t *Value)
{
     sem_wait(&(MailBox->NotFull));
     MailBox->Msg = *Value;
     sem_post(&(MailBox->NotEmpty));
     return  (*Value);
}

msg_t  msg_receive(mbox_t *MailBox, msg_t *Value)
{
     sem_wait(&(MailBox->NotEmpty));
     *Value = MailBox->Msg;
     sem_post(&(MailBox->NotFull));
     return (*Value);
}