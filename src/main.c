#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// 解释执行source
// source是以'\0'结尾的字符串
static void run(const char* source) {
    initScanner(source);
    int line = -1;
    // 打印Token, 遇到TOKEN_EOF为止
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}

char* readline(const char *prompt) {
    static char line[MAX_LINE_LENGTH];
    printf("%s", prompt);
    fflush(stdout); 
    return fgets(line, MAX_LINE_LENGTH, stdin) ? line : NULL;
}

static void repl() {
    // 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
    // repl是"read evaluate print loop"的缩写
    for (;;) {
        char* line = readline(">");
        if (strcmp(line, "exit()\n") == 0) break;
        run(line);
    }

}

static char* readFile(const char* path) {
    // 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
    // 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Could not open file '%s'\n", path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Not enough memory to read '%s'\n", path);
        exit(1);
    }

    size_t bytesRead = fread(buffer, sizeof(char), length, file);
    if (bytesRead < length) {
        fprintf(stderr, "Could not read file '%s'\n", path);
        exit(1);
    }
    buffer[length] = '\0';

    fclose(file);
    return buffer;

}

static void runFile(const char* path) {
    // 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
    char* source = readFile(path);
    run(source);  // 解释执行source 
    free(source);
}

int main(int argc, const char* argv[]) {
    if (argc == 1) {
        // ./scanner 没有参数,则进入交互式界面
        repl();
    } else if (argc == 2) {
        // ./scanner file 后面跟一个参数,则分析整个文件
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}