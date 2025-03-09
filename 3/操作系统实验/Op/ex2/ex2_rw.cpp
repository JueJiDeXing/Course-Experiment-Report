#include <pthread.h>
#include <semaphore.h>
#include <bits/stdc++.h>

#include <unistd.h>
#include "../Utils/ColorPrint.h"

using namespace std;
/**
 * 实验二, 读者写者问题
 */
#define WriterCnt 5
#define ReaderCnt 5

int sharedData = 0;// 共享数据
sem_t rMutex, wMutex, Mutex; // 信号量
int readerCount = 0;// 读者数量

void *writer(void *arg) {
    int id = *((int *) arg);
    srand(id);//随机数种子
    for (int i = 0; i < 5; i++) {
        sem_wait(&Mutex); //
        sem_wait(&wMutex); // 等待所有读者完成
        // 写入数据
        sharedData = rand() % 1000;
        cout << GREEN << "Writer " << id << " -> " << sharedData << NONE << endl;
        usleep(100000);

        sem_post(&wMutex);
        sem_post(&Mutex);
        usleep(100000);
    }
    return nullptr;
}

void *reader(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < 5; i++) {
        sem_wait(&Mutex);
        sem_wait(&rMutex);
        if (readerCount == 0) sem_wait(&wMutex);// 如果是第一个读者, 等待当前写者完成
        readerCount++;
        sem_post(&rMutex);
        sem_post(&Mutex);

        // 读取数据
        cout << BLUE << sharedData << " <- Reader " << id << NONE << endl;
        usleep(100000);

        sem_wait(&rMutex);
        readerCount--;
        if (readerCount == 0) sem_post(&wMutex);  // 如果是最后一个读者, 通知写者
        sem_post(&rMutex);

        usleep(100000);
    }
    return nullptr;
}


int main() {
    // 初始化信号量
    sem_init(&rMutex, 0, 1);
    sem_init(&wMutex, 0, 1);
    sem_init(&Mutex, 0, 1);

    cout << PURPLE << "ReaderCnt=" << ReaderCnt << ", WriterCnt=" << WriterCnt << NONE << endl;
    // 开启线程
    pthread_t readers[ReaderCnt], writers[WriterCnt];
    int rIds[ReaderCnt], wIds[WriterCnt];

    for (int i = 0; i < ReaderCnt; i++) {
        rIds[i] = i + 1;
        pthread_create(&readers[i], nullptr, reader, &rIds[i]);
    }

    for (int i = 0; i < WriterCnt; i++) {
        wIds[i] = i + 1;
        pthread_create(&writers[i], nullptr, writer, &wIds[i]);
    }

    for (auto reader : readers) pthread_join(reader, nullptr);
    for (auto writer : writers) pthread_join(writer, nullptr);

    sem_destroy(&rMutex); // 销毁信号量
    sem_destroy(&wMutex);
    sem_destroy(&Mutex);

    return 0;
}