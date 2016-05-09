#include "jz_threadpool.h"

static tp_job_t *job_new(do_job_func func, jz_ptr func_arg)
{
    tp_job_t		*job;

    job = calloc(1, sizeof(tp_job_t));
    jz_assert_return_val(job, NULL);
    job->func = func;
    job->arg = func_arg;
    return job;
}


static void job_destory(tp_job_t *job)
{
    if (job){
        free(job);
    }
}

static void job_queue_destory(struct clist **list)
{
    tp_job_t			*job;
    struct element		*e;

    if (list && *list){
        while (clist_len(*list) > 0){
            e = clist_back(*list);
            job = e->value;
            clist_remove(*list, e);
            if (job){
                job_destory(job);
            }
        }
        free(*list);
        *list = NULL;
    }
}

static tp_worker_t *worker_new(void)
{
    return calloc(1, sizeof(tp_worker_t));
}

static void worker_destory(tp_worker_t *worker)
{
    if (worker){
        pthread_attr_destroy(&worker->attr);
        free(worker);
    }
}

static void worker_queue_destory(struct clist **list)
{
    tp_worker_t			*worker;
    struct element		*e;

    if (list && *list){
        while (clist_len(*list) > 0){
            e = clist_back(*list);
            worker = e->value;
            clist_remove(*list, e);
            if (worker){
                worker_destory(worker);
            }
        }
        free(*list);
        *list = NULL;
    }
}

static void *worker_working(void *jobs)
{
    tp_job_t				*myjob = NULL;
    struct element			*e = NULL;
    tp_job_queue_t			*job = (tp_job_queue_t *)jobs;

    for (;;){
        jz_assert_return_val(job, NULL);
        jz_assert_return_val(job->mask & JOB_QUEUE_READ, NULL);
        pthread_mutex_lock(&job->queue_mutex);
        while (clist_len(job->queue) == 0 && (job->mask & JOB_QUEUE_READ)){
            pthread_cond_wait(&job->queue_cond, &job->queue_mutex);
            continue;
        }
        jz_assert_goto(job->mask & JOB_QUEUE_READ, err_out);
        e = clist_front(job->queue);
        jz_assert_goto(e, err_out);
        myjob = e->value;
        clist_remove(job->queue, e);
        pthread_cond_signal(&job->queue_cond);
        pthread_mutex_unlock(&job->queue_mutex);
        myjob->func(myjob->arg);
        job_destory(myjob);
    }
err_out:
    pthread_cond_signal(&job->queue_cond);
    pthread_mutex_unlock(&job->queue_mutex);
    return NULL;
}



tp_thpool_t *tp_threadpool_create(jz_int worker_max, jz_int job_max)
{
    tp_thpool_t				*pool;
    tp_worker_t				*worker;
    struct element			*e;

    pool = calloc(1, sizeof(tp_thpool_t));
    jz_assert_return_val(pool, NULL);
    pool->pool_state = TP_THREAD_POOL_DEAD;
    pool->worker_max = worker_max;
    pool->alive_workers = 0;
    pool->job.queue_max = job_max;
    pool->job.mask = JOB_QUEUE_READ | JOB_QUEUE_WRITE;
    pthread_mutex_init(&pool->job.queue_mutex, NULL);
    pthread_cond_init(&pool->job.queue_cond, NULL);
    pool->worker_queue = clist_init();
    jz_assert_goto(pool->worker_queue, err_out);

    while (clist_len(pool->worker_queue) != worker_max){
        worker = worker_new();
        jz_assert_goto(worker, err_out);
        e = clist_push_back(pool->worker_queue, worker, sizeof(tp_worker_t));
        jz_assert_goto(e, err_out);
    }
    pool->job.queue = clist_init();
    jz_assert_goto(pool->job.queue, err_out);
    return pool;
err_out:
    pthread_mutex_destroy(&pool->job.queue_mutex);
    pthread_cond_destroy(&pool->job.queue_cond);

    job_queue_destory(&pool->job.queue);
    worker_queue_destory(&pool->worker_queue);
    return NULL;
}


jz_int tp_threadpool_add_job(tp_thpool_t *pool, do_job_func func, jz_ptr func_arg)
{
    tp_job_t			*job = NULL;
    struct element		*p;

    jz_assert_return_val(pool && func, -4);
    jz_assert_return_val(pool->job.mask & JOB_QUEUE_WRITE, -1);
    job = job_new(func, func_arg);
    jz_assert_return_val(job, -2);
    pthread_mutex_lock(&pool->job.queue_mutex);
    while (clist_len(pool->job.queue) == pool->job.queue_max &&
           (pool->job.mask & JOB_QUEUE_WRITE)){
        pthread_cond_wait(&pool->job.queue_cond, &pool->job.queue_mutex);
        continue;
    }
    jz_assert_goto(pool->job.mask & JOB_QUEUE_WRITE, err_out);
    p = clist_push_back(pool->job.queue, job, sizeof(tp_job_t));
    jz_assert_goto(p, err_out);
    pthread_cond_signal(&pool->job.queue_cond);
    pthread_mutex_unlock(&pool->job.queue_mutex);
    return 0;
err_out:
    job_destory(job);
    pthread_cond_signal(&pool->job.queue_cond);
    pthread_mutex_unlock(&pool->job.queue_mutex);
    return -3;
}

static void tp_thpool_free(tp_thpool_t *pool)
{
    if (pool){
        worker_queue_destory(&pool->worker_queue);
        job_queue_destory(&pool->job.queue);
        pthread_mutex_destroy(&pool->job.queue_mutex);
        pthread_cond_destroy(&pool->job.queue_cond);
        free(pool);
    }
}

jz_int tp_pthreadpool_destroy(tp_thpool_t *pool, jz_bool wait_done)
{
    tp_worker_t				*worker;
    struct element			*e;

    jz_assert_return_val(pool, -1);

    if (pool->pool_state == TP_THREAD_POOL_RUNNING){
        if (wait_done){
            pool->job.mask &= ~JOB_QUEUE_WRITE;
        }else {
            pool->job.mask &= (JOB_QUEUE_WRITE | JOB_QUEUE_READ);
        }
        pthread_mutex_lock(&pool->job.queue_mutex);
        if (wait_done){
            while (clist_len(pool->job.queue) != 0){
                pthread_cond_wait(&pool->job.queue_cond, &pool->job.queue_mutex);
                continue;
            }
        }
        pool->job.mask &= ~(JOB_QUEUE_WRITE | JOB_QUEUE_READ);
        pthread_cond_broadcast(&pool->job.queue_cond);
        pthread_mutex_unlock(&pool->job.queue_mutex);
        //join;
        for(e = clist_front(pool->worker_queue); e; e = clist_next(e)){
            worker = e->value;
            pthread_join(worker->tid, NULL);
        }
    }
    tp_thpool_free(pool);
    return 0;
}



jz_int tp_threadpool_run(tp_thpool_t *pool)
{
    tp_worker_t				*worker;
    int						ret;
    struct element			*e;

    jz_assert_return_val(pool, -1);
    jz_assert_return_val(pool->pool_state == TP_THREAD_POOL_DEAD, 0);
    e = clist_front(pool->worker_queue);
    for (; e; e = clist_next(e)){
        worker = e->value;
        pthread_attr_init(&worker->attr);
        pthread_attr_setscope(&worker->attr, PTHREAD_SCOPE_SYSTEM);
        ret = pthread_create(&worker->tid, &worker->attr, worker_working, &pool->job);
        jz_assert_return_val(ret == 0, -1);
        pool->alive_workers++;
    }
    pool->pool_state = TP_THREAD_POOL_RUNNING;
    return 0;
}
