#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>

size_t capture_screen(char* buffer, size_t buffer_size) {
    memset(buffer, 0, buffer_size);
    CGImageRef img = CGDisplayCreateImage(kCGDirectMainDisplay);
    if (!img) {
        fprintf(stderr, "Screen Capturing Failure\n");
        return 0;
    }

    CFMutableDataRef mutableData = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(mutableData, CFSTR("public.png"), 1, NULL);
    CGImageDestinationAddImage(destination, img, NULL);

    if (!CGImageDestinationFinalize(destination)) {
        fprintf(stderr, "Failed to write image data\n");
        CFRelease(mutableData);
        CFRelease(destination);
        CGImageRelease(img);
        return 0;
    }

    CFIndex data_length = CFDataGetLength(mutableData);

    if ((size_t)data_length > buffer_size) {
        fprintf(stderr, "Buffer size too small\n");
        CFRelease(mutableData);
        CFRelease(destination);
        CGImageRelease(img);
        return 0;
    }

    memcpy(buffer, CFDataGetBytePtr(mutableData), data_length);

    CFRelease(mutableData);
    CFRelease(destination);
    CGImageRelease(img);

    return (size_t)data_length;
}

const char** printS() {
    static const char* bigS[] = {
        "  SSSSS  ",
        " S     S ",
        " S       ",
        "  SSSSS  ",
        "       S ",
        " S     S ",
        "  SSSSS  ",
        NULL
    };
    return bigS;
}

const char** printH() {
    static const char* bigH[] = {
        " H     H ",
        " H     H ",
        " H     H ",
        " HHHHHHH ",
        " H     H ",
        " H     H ",
        " H     H ",
        NULL
    };
    return bigH;
}

const char** printU() {
    static const char* bigU[] = {
        " U     U ",
        " U     U ",
        " U     U ",
        " U     U ",
        " U     U ",
        " U     U ",
        "  UUUUU  ",
        NULL
    };
    return bigU;
}

const char** printT() {
    static const char* bigT[] = {
        " TTTTTTT ",
        "    T    ",
        "    T    ",
        "    T    ",
        "    T    ",
        "    T    ",
        "    T    ",
        NULL
    };
    return bigT;
}

const char** printD() {
    static const char* bigD[] = {
        " DDDDDD  ",
        " D     D ",
        " D     D ",
        " D     D ",
        " D     D ",
        " D     D ",
        " DDDDDD  ",
        NULL
    };
    return bigD;
}

const char** printO() {
    static const char* bigO[] = {
        "  OOOO   ",
        " O    O  ",
        "O      O ",
        "O      O ",
        "O      O ",
        " O    O  ",
        "  OOOO   ",
        NULL
    };
    return bigO;
}

const char** printW() {
    static const char* bigW[] = {
        " W     W ",
        " W     W ",
        " W  W  W ",
        " W  W  W ",
        " W  W  W ",
        " W W W W ",
        "  W   W  ",
        NULL
    };
    return bigW;
}

const char** printN() {
    static const char* bigN[] = {
        " N     N ",
        " NN    N ",
        " N N   N ",
        " N  N  N ",
        " N   N N ",
        " N    NN ",
        " N     N ",
        NULL
    };
    return bigN;
}


void printSHUTDOWN() {
    const char** letters[] = { printS(), printH(), printU(), printT(), printD(), printO(), printW(), printN() };

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) { 
            printf("%s ", letters[j][i]);
        }
        printf("\n");
    }
}
