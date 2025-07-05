#include "progressBar.h"

void progressBar(double current , double end) {
    char progress_bar_buffer[PROGRESS_BAR_BUFFER_SIZE];
    memset(progress_bar_buffer , 0 , sizeof progress_bar_buffer);
    const char* label = "|/-\\";
    int label_length = strlen(label);
    double rate = current / end;    /* 0 - 1 */

    // 填充进度条
    for (int i = 0; i < (int)(rate * 100); i++) 
        progress_bar_buffer[i] = PROGRESS_BAR_STYLE;

    static int running = 0;
    running %= label_length;
    // 显示进度条
    printf("[%-100s][%.2lf%%][%c]\r" , progress_bar_buffer , rate * 100 , label[running]);

    // 刷新缓冲区
    fflush(stdout);
    running++;
}

void download() {
    double current = 0;
    while (current <= total) {
        progressBar(current , total);
        usleep(7000);
        current += speed;
    }
    printf("\nThe download was successful , total: %.2lfMB\n" , total);
}

