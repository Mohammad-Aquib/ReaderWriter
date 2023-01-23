#include <iostream>
#include <thread>
#include <mutex>
std::mutex r_mutex, w_mutex, x, y, z, w;
int total_money; // common resource
int readers_count = 0;
int writers_count = 0;
int readers_entered = 0;
int writers_entered = 0;

void get_balance()
{
    w.lock();
    readers_entered++;
    w.unlock();
    x.lock();

    if (readers_count == 0)
    {
        readers_count++;
        // no writer should enter cs
           while(writers_entered!=0);
             w_mutex.lock(); // equivalent to wait(w_mutex)
           

    }
    else
    {
        readers_count++;
    }
    x.unlock();
    w.lock();
    readers_entered--;
    w.unlock();

    std::cout << "READING by thread id:" << std::this_thread::get_id();
    std::cout << "-->Current balance is:" << total_money << ":writers_count "
              << writers_count << ":readers_count " << readers_count << ":readers_waiting:"
              << readers_entered << ":writers_waiting:" << writers_entered;
    std::cout << "\n";

    x.lock();
    readers_count--;
    if (readers_count == 0)
        w_mutex.unlock(); // equivalent to signal(w_mutex)
    x.unlock();    
}
void update_balance(int amount)
{
    z.lock();
    writers_entered++;
    z.unlock();
    w_mutex.lock();
    writers_count++;
    total_money += amount;
    writers_entered--;
    std::cout << "added:" << amount << " by thread id:" << std::this_thread::get_id();
    std::cout << "-->Updated balance is:" << total_money 
    << "no .of writers" << writers_count << "::no.of readers:" 
    << readers_count << ":readers_waiting:" << readers_entered 
    << ":writers_waiting:" << writers_entered;
    std::cout << "\n";
    writers_count--;
    w_mutex.unlock();
}
int main()
{
    std::thread reader[10];
    std::thread writer[10];

    for (int i = 0; i < 10; i++)
    {
        reader[i] = std::thread(get_balance);
        writer[i] = std::thread(update_balance, i + 1);
    }
    for (auto &th : reader)
        th.join();
    for (auto &th : writer)
        th.join();
    return 0;
}
