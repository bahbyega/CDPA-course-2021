#ifndef __MBOX__
#define __MBOX__

#define MAX_PAYLOAD 32

#define MSG_TYPE_IMG 3
#define MSG_TYPE_GO 2
#define MSG_TYPE_EOS 1

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct msg {
  int iFrom;
  int type;
  gchar *filename;
  GdkPixbuf *pixbuf; // message data
} msg_t;

typedef struct mmBox {
  msg_t Msg;
  sem_t NotFull;
  sem_t NotEmpty;
} mbox_t;

msg_t msg_init(mbox_t *);
msg_t msg_send(mbox_t *, msg_t *);
msg_t msg_receive(mbox_t *, msg_t *);

#endif /* __MBOX__ */
