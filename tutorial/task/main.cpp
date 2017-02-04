#include <iostream>
#include "thirdparty/mu/base/task_queue_i.h"
#include "thirdparty/mu/base/task_queue_impl.h"
#include "thirdparty/mu/base/thread.h"

using namespace mu;
using namespace std;

typedef int (*TT)();

int C0()
{
    cout << "----t1"<<endl;
    return 0;
}

void C1(void* p)
{
    cout << "-----CFunc1"<<endl;
}

void C2()
{
    cout << "-----c2"<<endl;
}

void C3(int a)
{
    cout << "------c3 " << a <<endl;
}
void C4(int a, float b)
{
    cout << "------c4 " << a << b <<endl;
}

void C5(int a1, int a2, int a3)
{
    cout << "------c5 " << a1 << a2 << a3 <<endl;
}

void C6(int a1, int a2, int a3, int a4)
{
    cout << "------c6 " << a1 << a2 << a3<<a4 <<endl;
}

void C7(int a1, int a2, int a3, int a4,int a5)
{
    cout << "------c7 " << a1 << a2 << a3<<a4<<a5 <<endl;
}

void C8(int a1, int a2, int a3, int a4,int a5,int a6)
{
    cout << "------c8 " << a1 << a2 << a3<<a4<<a5<<a6 <<endl;
}

class TF
{
public:
    void F0()
    {
        cout << __FUNCTION__ <<endl;
    }
    void F1(int a)
    {
        cout << __FUNCTION__ <<"|"<<a<<endl;
    }
    void F2(int a,int a2)
    {
        cout << __FUNCTION__ <<"|"<<a<<a2<<endl;
    }
    void F3(int a,int a2,int a3)
    {
        cout << __FUNCTION__ <<"|"<<a<<a2<<a3<<endl;
    }
    void F4(int a,int a2,int a3,int a4)
    {
        cout << __FUNCTION__ <<"|"<<a<<a2<<a3<<a4<<endl;
    }
    void F5(int a,int a2,int a3,int a4,int a5)
    {
        cout << __FUNCTION__ <<"|"<<a<<a2<<a3<<a4<<a5<<endl;
    }
    void F6(int a,int a2,int a3,int a4,int a5,int a6)
    {
        cout << __FUNCTION__ <<"|"<<a<<a2<<a3<<a4<<a5<<a6<<endl;
    }
private:
    
};

void TestCplus()
{
    TF tf;
    Task t0 = TaskBinder::Gen(&TF::F0,&tf);    
    t0.Run();
    Task t1 = TaskBinder::Gen(&TF::F1,&tf,1);    
    t1.Run();
    Task t2 = TaskBinder::Gen(&TF::F2,&tf,1,2);    
    t2.Run();
    Task t3 = TaskBinder::Gen(&TF::F3,&tf,1,2,3);    
    t3.Run();
    Task t4 = TaskBinder::Gen(&TF::F4,&tf,1,2,3,4);    
    t4.Run();
    Task t5 = TaskBinder::Gen(&TF::F5,&tf,1,2,3,4,5);    
    t5.Run();
    Task t6 = TaskBinder::Gen(&TF::F6,&tf,1,2,3,4,5,6);    
    t6.Run();
}

void TestC()
{
    Task t1 = TaskBinder::Gen(C1,(void *)&C0);
    t1.Run();
    Task t2 = TaskBinder::Gen(C2);
    t2.Run();
    Task t3 = TaskBinder::Gen(C3,1);
    t3.Run();
    Task t4 = TaskBinder::Gen(C4,1,1.5);
    t4.Run();
    Task t5 = TaskBinder::Gen(C5,1,2,3);
    t5.Run();
    Task t6 = TaskBinder::Gen(C6,1,2,3,4);
    t6.Run();
    Task t7 = TaskBinder::Gen(C7,1,2,3,4,5);
    t7.Run();
    Task t8 = TaskBinder::Gen(C8,1,2,3,4,5,6);
    t8.Run();
}

void TestQueue()
{
    TaskQueue* tq = new TaskQueue();
    Task t1 = TaskBinder::Gen(C1,(void *)&C0);
    Task t2 = TaskBinder::Gen(C2);
    Task t3 = TaskBinder::Gen(C3,1);
    Task t4 = TaskBinder::Gen(C4,1,1.5);
    Task t5 = TaskBinder::Gen(C5,1,2,3);
    Task t6 = TaskBinder::Gen(C6,1,2,3,4);
    Task t7 = TaskBinder::Gen(C7,1,2,3,4,5);
    Task t8 = TaskBinder::Gen(C8,1,2,3,4,5,6);
    tq->Produce(t1);
    tq->Produce(t2);
    tq->Produce(t3);
    tq->Produce(t4);
    tq->Produce(t5);
    tq->Produce(t6);
    tq->Produce(t7);
    tq->Produce(t8);
    //tq->Run();
    tq->BatchRun();
}

void TestThread()
{
    Task t8 = TaskBinder::Gen(C8,1,2,3,4,5,6);
    Thread* thread = new Thread();
    thread->CreateThread(t8,3);
    sleep(1);
    thread->Join();
}

void TestThreadPool()
{
    Thread* thread = new Thread();
    TaskQueuePool* tq = new TaskQueuePool(2);
    thread->CreateThread(TaskQueuePool::GenTask(tq), 2);
    TaskQueueI* tq1 =  tq->Alloc(0);
    Task t3 = TaskBinder::Gen(C3,1);
    Task t4 = TaskBinder::Gen(C4,1,1.5);
    tq1->Produce(t3);
    tq1->Produce(t4);
    TaskQueueI* tq2 =  tq->Alloc(1);
    Task t5 = TaskBinder::Gen(C5,1,2,3);
    Task t6 = TaskBinder::Gen(C6,1,2,3,4);
    tq2->Produce(t5);
    tq2->Produce(t6);
    sleep(5);
    tq->Close();
    thread->Join();
}

int main(int argc, char** argv)
{
    //TestC();
    //TestCplus();
    //TestQueue();
    //TestThread();
    TestThreadPool();
    return 0;
}
