#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <mutex>
#include <thread>
#include <algorithm>

std::mutex vector_mtx;
std::mutex cout_mtx;

struct VectorIndex// структура, в которой храняться начало и конец вектора
{
    int start;
    int end;
};

std::vector<VectorIndex> generate_indexes(std::vector<int> vector, int thread_count)
{
    std::vector<VectorIndex> result;
    int slice_size = 0;
    slice_size = vector.size() / thread_count;
    VectorIndex vi;
    for (int i = 0; i < thread_count; i++) {
        vi.start = i * slice_size;
        vi.end = (i + 1) * slice_size;
        if (i == (thread_count - 1))
        {
            vi.end = vector.size();
        }
        result.push_back(vi);
    }
    return result;
}

void find_min(std::vector<int>& vec, VectorIndex vi, int name, int& result)// функция поиска минимального значения ветора
{
    int k = *std::min_element(vec.begin() +vi.start,vec.begin() + vi.end);
    cout_mtx.lock();
    std::cout << "thread " << name << ": " << k << std::endl;
    cout_mtx.unlock();
    result = k;
}


std::vector<int> generate_vector(int size)// функция заполнения вектора
{
    std::srand(std::time(nullptr));
    std::vector<int> result;
    for(long long int i = 0; i<size; ++i)
    {
        result.push_back(rand());
    }
    return result;
}


int main()
{
    const int thread_count = 5;// измение кол-во потоков
    int result_min_value;
    std::vector<int> vector;
    std::vector<VectorIndex> indexes;
    std::thread *threads[thread_count];
    std::vector<int> results;
    std::vector<int> result_values[thread_count];
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point stop_time;
    
    vector = generate_vector(126000000);
    indexes = generate_indexes(vector, thread_count);
    
    for (int i = 0; i < thread_count; i++)
    {
        results.push_back(-1);
    }
    
    start_time = std::chrono::steady_clock::now();
    
    for (int i = 0; i < thread_count; i++)
    {
        threads[i] = new std::thread(find_min, std::ref(vector), indexes[i], i , std::ref(results[i]));
    }
    
    for (auto & thread : threads)
    {
        thread -> join();
    }
    
    stop_time = std::chrono::steady_clock::now();
    
    for (int i = 0; i < thread_count; i++)
    {
        result_values -> push_back(results[i]);
    }
    
    result_min_value = *std::min_element(result_values->begin(), result_values->end());
    std::cout << "result_min_value: " << result_min_value << std::endl;
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time).count() << std::endl;
    for (auto & thread : threads)
    {
       delete thread;
    }
    return 0;
}
