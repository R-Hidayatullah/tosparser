#include "ipf.h"
#include "xac.h"
#include "xsm.h"
#include "xpm.h"
#include "hashmap.h"

// Example usage
int main(void)
{
    // Example buffer
    uint8_t buffer[] = {
        0x05, 0x00, 0x00, 0x00, 'H', 'e', 'l', 'l', 'o', // "Hello"
        0x05, 0x00, 0x00, 0x00, 'W', 'o', 'r', 'l', 'd'  // "World"
    };
    size_t index = 0;

    char *str1 = parse_string_from_buffer(buffer, &index);
    char *str2 = parse_string_from_buffer(buffer, &index);

    printf("Buffer String 1: %s\n", str1);
    printf("Buffer String 2: %s\n", str2);

    free(str1);
    free(str2);

    // Example file reading
    FILE *file = fopen("test.dat", "rb");
    if (file)
    {
        char *file_str;
        while ((file_str = parse_string_from_file(file)))
        {
            printf("File String: %s\n", file_str);
            free(file_str);
        }
        fclose(file);
    }

    return 0;
}
