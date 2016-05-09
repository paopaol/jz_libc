#ifndef JZ_THREADPOOL_
#define JZ_THREADPOOL_
#include <stdlib.h>
#include <pthread.h>
#include <clist.h>
#include "jz_type.h"

typedef void (*do_job_func)(jz_ptr arg);

typedef struct tp_job {
    jz_uint		id;
    jz_ptr		arg;
    do_job_func	func;
}tp_job_t;

struct tp_thread {
    tp_job_t	job;
};

typedef struct tp_worker {
    pthread_t				tid;
    pthread_attr_t			attr;
}tp_worker_t;


typedef struct tp_job_queue {
#define JOB_QUEUE_READ	0x01
#define JOB_QUEUE_WRITE 0x02
    jz_int				mask;
    //how much jobs threadpool can hold
    jz_int				queue_max;
    pthread_mutex_t		queue_mutex;
    pthread_cond_t		queue_cond;
    struct clist		*queue;
}tp_job_queue_t;

typedef enum {
    TP_THREAD_POOL_RUNNING,
    TP_THREAD_POOL_DEAD
}pool_state_t;

typedef struct tp_threadpool {
    //how much thread run at one time
    pool_state_t		pool_state;
    jz_int				worker_max;
    int					alive_workers;
    struct clist		*worker_queue;
    tp_job_queue_t		job;
}tp_thpool_t;

extern tp_thpool_t *tp_threadpool_create(jz_int worker_max, jz_int job_max);
extern jz_int tp_pthreadpool_destroy(tp_thpool_t *pool, jz_bool wait_done);
extern jz_int tp_threadpool_run(tp_thpool_t *pool);
extern jz_int tp_threadpool_add_job(tp_thpool_t *pool, do_job_func func, jz_ptr func_arg);
#endif // JZ_THREADPOOL_

