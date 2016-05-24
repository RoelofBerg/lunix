#include <stdio.h>
#include <unistd.h>

void writeVal(int val) {
    FILE *dev = fopen("/sys/class/gpio/gpio22/value", "w");
    if (dev == NULL) {
        perror("fopen");
        return;
    }
    
    fprintf(dev, "%d\n", val);
    fclose(dev);
}

void dit() {
    writeVal(1);
    usleep(200 * 1000);
    writeVal(0);
    usleep(1000 * 60);
}

void dah() {
    writeVal(1);
    usleep(400 * 1000);
    writeVal(0);
    usleep(1000 * 60);
}

int main(int argc, char **argv) {
    dit();
    dah();
    dit();
    dit();
    
    usleep(1000 * 100);
    
    dit();
    dit();
    dah();

    usleep(1000 * 100);
    
    dah();
    dit();
    
    usleep(1000 * 100);
    
    dit();
    dah();
    
    
    return 0;
}
