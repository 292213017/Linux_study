#ifndef LOG_H
#define LOG_H

 #include <stdio.h>
 #include <string.h>
#include <time.h>


class Log{
    public:
        Log();
        ~Log();

        static Log *get_instance()
        {
            static Log instance;
            return &instance;
        }

        bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);


    private:
        char dir_name[128]; //路径名
        char log_name[128]; //log文件名
        int m_split_lines;  //日志最大行数
        int m_log_buf_size; //日志缓冲区大小
        long long m_count;  //日志行数记录
        int m_today;        //因为按天分类,记录当前时间是那一天
        FILE *m_fp;         //打开log的文件指针
        char *m_buf;
        // block_queue<string> *m_log_queue; //阻塞队列
        bool m_is_async;                  //是否同步标志位
        // locker m_mutex;
        int m_close_log; //关闭日志
};

#endif