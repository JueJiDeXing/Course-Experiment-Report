#include <bits/stdc++.h>
#include <semaphore.h>
#include <unistd.h>
#include "../Utils/ColorPrint.h"

using namespace std;
/**
 * 实验二,生产者消费者问题
 */
#define Capacity 3
#define ProducerCnt 6
#define ConsumerCnt 6

int que[Capacity], front = 0, rear = -1, itemCount = 0;// 队列

sem_t sem_empty, sem_full;// 信号量
pthread_mutex_t rMutex;// 互斥锁

void printQueueSizeInfo() {
    if (0 <= itemCount && itemCount <= Capacity) {
        cout << "\t" << YELLOW << "size(que) = " << itemCount;
        for (int i = 0; i < itemCount; i++) cout << "█";
        for (int i = 0; i < Capacity - itemCount; i++) cout << "░";
    } else {// 队列正溢出或负溢出
        cout << RED << "\tsize(que) = %d\n" << itemCount;
    }
    cout << NONE << endl;
}

/**
 * 生产者线程, 生成5个随机数放入共享队列
 */
void *producer(void *arg) {
    int id = *((int *) arg);
    srand(id);//随机数种子
    for (int i = 0; i < 5; i++) {
        int item = rand() % 1000;
        sem_wait(&sem_full); // 等待队列非满

        // 生产者 - 向队列放数字
        pthread_mutex_lock(&rMutex);
        rear = (rear + 1) % Capacity;
        que[rear] = item;
        itemCount++;
        cout << GREEN << "Producer " << id << " -> " << item << NONE;  // 打印信息
        printQueueSizeInfo();
        pthread_mutex_unlock(&rMutex);

        sem_post(&sem_empty);// 放入了一个数据

        usleep(100000);
    }
    return nullptr;
}

/**
 * 消费者线程, 从共享队列取出5个数据
 */
void *consumer(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < 5; i++) {
        sem_wait(&sem_empty);// 等待队列非空

        // 消费者 - 从队列取数字
        pthread_mutex_lock(&rMutex);
        int item = que[front];
        front = (front + 1) % Capacity;
        itemCount--;
        // 打印信息
        cout << BLUE << item << " -> Consumer " << id << NONE;
        printQueueSizeInfo();

        pthread_mutex_unlock(&rMutex);

        sem_post(&sem_full);// 取走了一个数据
        usleep(100000);
    }
    return nullptr;
}

int main() {
    sem_init(&sem_full, 0, Capacity); // 初始值Capacity, 队列满时生产者不能放数据到队列
    sem_init(&sem_empty, 0, 0);// 初始值0, 消费者不能从空队列取数据
    pthread_mutex_init(&rMutex, nullptr); // 互斥锁, 防止同时修改队列, 导致数据不一致

    cout << PURPLE << "ProducerCnt=" << ProducerCnt << ", ConsumerCnt=" << ConsumerCnt
         << ", Capacity=" << Capacity << NONE << endl;
    // 开启线程
    pthread_t producers[ProducerCnt], consumers[ConsumerCnt];
    int pIds[ProducerCnt], cIds[ConsumerCnt];

    for (int i = 0; i < ProducerCnt; i++) {
        pIds[i] = i + 1;
        pthread_create(&producers[i], nullptr, producer, &pIds[i]);
    }

    for (int i = 0; i < ConsumerCnt; i++) {
        cIds[i] = i + 1;
        pthread_create(&consumers[i], nullptr, consumer, &cIds[i]);
    }

    for (auto producer: producers) pthread_join(producer, nullptr);
    for (auto consumer: consumers) pthread_join(consumer, nullptr);

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&rMutex);

    return 0;
}
