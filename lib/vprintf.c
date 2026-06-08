#include <stdio.h>
#include <stdlib.h>
#include <string.h>













int vprintf(const char *fmt, va_list args) {
    char tmp[1024];
    char *s;
    char ch;

    if (!fmt)
        return -1;

    while (*fmt) {
        if (*fmt != '%') {
            putc(*fmt++);
            continue;
        }

        fmt++; // skip '%'

        switch (*fmt) {
            case 'c': {
                ch = (char)va_arg(args, int);
                
                putc(ch);
                
                fmt++;
                break;
            }

            case 's': {
                s = va_arg(args, char *);
                if (!s) {
                    s = "(null)";
                }

                puts(s);
                
                fmt++;
                break;
            }

            case 'd': {
                int d = va_arg(args, int);
                
                itoa(d, tmp, 10);
                
                puts(tmp);
                
                fmt++;
                break;
            }

            case 'u': {
                unsigned int u = va_arg(args, unsigned int);
                
                utoa(u, tmp, 10);
                
                puts(tmp);
                
                fmt++;
                break;
            }

            case 'x': {
                unsigned int x = va_arg(args, unsigned int);
                
                tmp[0] = '0';
                tmp[1] = 'x';
                
                utoa(x, tmp + 2, 16);
                
                puts(tmp);
                
                fmt++;
                break;
            }

            case 'X': {
                unsigned int X = va_arg(args, unsigned int);
                
                tmp[0] = '0';
                tmp[1] = 'x';
                
                utoa(X, tmp + 2, 16);
                
                toupper(tmp + 2);
                
                puts(tmp);
                
                fmt++;
                break;
            }

            case 'o': {
                unsigned int o = va_arg(args, unsigned int);
                
                tmp[0] = '0';
                tmp[1] = 'o';
                
                utoa(o, tmp + 2, 8);
                
                puts(tmp);
                
                fmt++;
                break;
            }

            case 'l': {
                if (*(fmt + 1) == 'l' && *(fmt + 2) == 'x') {
                    unsigned long long llx = va_arg(args, unsigned long long);
                    
                    tmp[0] = '0';
                    tmp[1] = 'x';
                    
                    utoa64(llx, tmp + 2, 16);
                    
                    puts(tmp);
                    
                    fmt += 3;
                } else if (*(fmt + 1) == 'l' && *(fmt + 2) == 'u') {
                    unsigned long long llu = va_arg(args, unsigned long long);
                    
                    utoa64(llu, tmp, 10);
                    
                    puts(tmp);
                    
                    fmt += 3;
                } else if (*(fmt + 1) == 'x') {
                    unsigned long lx = va_arg(args, unsigned long);
                    
                    tmp[0] = '0';
                    tmp[1] = 'x';
                    
                    utoa64(lx, tmp + 2, 16);
                    
                    puts(tmp);
                    
                    fmt += 2;
                } else if (*(fmt + 1) == 'u') {
                    unsigned long lu = va_arg(args, unsigned long);
                    
                    utoa64(lu, tmp, 10);
                    
                    puts(tmp);
                    
                    fmt += 2;
                }

                else {
                    fmt++;
                }

                break;
            }

            case '%':
                putc('%');
                fmt++;
                break;

            default:
                putc('%');
                putc(*fmt++);
                break;
            }
    }   

       return 0;
}