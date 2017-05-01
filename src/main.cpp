// #include "imu.pb.hpp"
// #include <pb_encode.h>
// #include <pb_decode.h>
#include "FreeRTOSmacros.hpp"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"

using namespace cpp_freertos;
using namespace std;



class SignalingThread : public Thread {

    public:

        SignalingThread(Semaphore &sem)
           : Thread("SignalingThread", 100, 1), Sem(sem)
        {
            Start();
        };

    protected:

        virtual void Run() {

            int SignaledCount = 0;

            cout << "[S] Starting Signaling thread " << endl;

            TickType_t DelayInSeconds = 1;

            while (true) {

                TickType_t ticks = Ticks::SecondsToTicks(DelayInSeconds);
                Delay(ticks);
                SignaledCount++;
                cout << "[S] Signaling " << SignaledCount << " times" << endl;
                Sem.Give();
            }
        };

    private:
        Semaphore &Sem;
};


class WaitingThread : public Thread {

    public:

        WaitingThread(Semaphore &sem)
           : Thread("WaitingThread", 100, 1), Sem(sem)
        {
            Start();
        };

    protected:

        virtual void Run() {

            int SignaledCount = 0;

            while (true) {
                Sem.Take();
                SignaledCount++;
                cout << "[W] Got Signal " << SignaledCount << " times" << endl;

                if ((SignaledCount % 5) == 0) {
                    cout << "[W] Simulating long work" << endl;
                    Delay(Ticks::SecondsToTicks(5));
                }
            }
        };

    private:
        Semaphore &Sem;
};


int main (void)
{
    cout << "Testing FreeRTOS C++ wrappers" << endl;
    cout << "Binary Semaphore" << endl;

    BinarySemaphore *Sem;

    try {
        Sem = new BinarySemaphore();
    }
    catch(SemaphoreCreateException &ex) {
        cout << "Caught SemaphoreCreateException" << endl;
        cout << ex.what() << endl;
        configASSERT(!"BinarySemaphore creation failed!");
    }

    SignalingThread sthd(*Sem);
    WaitingThread wthr(*Sem);

    Thread::StartScheduler();

    //
    //  We shouldn't ever get here unless someone calls
    //  Thread::EndScheduler()
    //

    cout << "Scheduler ended!" << endl;

    return 0;
}