Hello!!!

    Link to Youtube Demo: https://www.youtube.com/watch?v=DEcY42X0384
    My implementation has 1 file: presidential_debate.c.
Requirements and where they are filled:
    a) Binary semaphores are used properly to protect critical regions of code.
        Semaphore connected_lock is used in void* phonecall to 
        protect int connected when they are incremented and decremented during phone calls.
    b) Binary semaphores are used properly not to protect non-critical regions of code.
        Yes, I only protect data with a binary semaphore when a local variable in a thread is being
        incremented.
    c) A counting semaphore is used properly to restrict the use of resources (operators).
        I use the operators semaphore in void* phonecall to lock a sleep(3) call, as specified 
        by instructions.
    d) All semaphores are correctly initialized and destroyed.
        Yes, connected_lock and operators semaphores are initialized at the beginning and destroyed
        at the end of int main().
    e) A thread function exists and is implemented properly.
        Yes, void* phonecall() and void* timerThread() are thread functions that are passed in
        as arguments to a pthread_create() in int main().
    f) Threads are created, detached, and joined properly.
        Threads are created with pthread_create() in int main(). phonecall() threads use 
        pthread_detach(), and int main() calls pthread_join() on timerThread().
    g) A global variable next_id exists and is properly updated in the thread function and used to set the caller’s id.
        Yes, I have a static global variable called next_id. I set a local variable in phonecall()
        to it inside a binary semaphore.
    h) The phonecall thread properly updates the shared state for the number of connected callers in a critical section.
        Yes, phonecall() increments the global static int connected inside a critical region. 
    i) The program prints properly formatted outputs with the caller’s id.
        I followed the formatting output shown in the project documentation.
    j) The static modifier is used properly for both thread local variables as well as any global variables.
        Global int next_id is static, but local var caller_ID in phonecall() is not. This is because we want each thread not
        to share their caller id number. 
