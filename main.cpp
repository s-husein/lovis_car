#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <serial.h>

int main(){
    const char* dev_path = "/dev/input/event22";
    int ds4 = open(dev_path, O_RDONLY);

    struct input_event event;
    while(true){
        ssize_t bytes = read(ds4, &event, sizeof(event));
        if (bytes == sizeof(event)){
            if(event.type == EV_KEY){
                std::cout << "key: " << event.code << "value: " << event.value << std::endl;
            }
            if(event.type == EV_ABS){
                if(event.value < 124 || event.value > 136)
                std::cout << "axix: " << event.code << "value: " << event.value << std::endl;
            }
        }
    }
    close(ds4);
    return 0;
}