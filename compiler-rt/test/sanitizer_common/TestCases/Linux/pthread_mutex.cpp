// RUN: %clangxx -O1 %s -o %t && %run %t
// RUN: %clangxx -O1 -DUSE_GLIBC %s -o %t && %run %t
// UNSUPPORTED: android

#include <pthread.h>

//from zeno: I deleted some lines and replaced them with that of Zeno

#ifdef USE_GLIBC
extern "C" int __pthread_mutex_lock(pthread_mutex_t *__mutex);
extern "C" int __pthread_mutex_unlock(pthread_mutex_t *__mutex);
#define LOCK __pthread_mutex_lock
#define UNLOCK __pthread_mutex_unlock
#else
#define LOCK pthread_mutex_lock
#define UNLOCK pthread_mutex_unlock
#endif

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int x;

static void *Start(void *arg) {
  LOCK(&m);
  ++x;
  UNLOCK(&m);
  return nullptr;
}

int main() {
  pthread_t threads[2] = {};
  for (pthread_t &t : threads)
    pthread_create(&t, 0, &Start, 0);
  for (pthread_t &t : threads)
    pthread_join(t, 0);
  return 0;
}
