#if !defined(SHARED_FORMATS_H)
#define SHARED_FORMATS_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define IDENTIFIER_LEN 4

typedef enum
{
    SHARED_CHUNK_MOTIONEVENTTABLE = 50,
    SHARED_CHUNK_TIMESTAMP = 51
} SharedChunkId;

typedef enum
{
    MULORDER_SCALE_ROT_TRANS = 0,
    MULORDER_ROT_SCALE_TRANS = 1
} MatrixMulOrder;

#pragma pack(push, 1) // Save current packing and set new alignment

typedef struct
{
    uint32_t chunk_id;
    uint32_t size_in_bytes;
    uint32_t version;
} ChunkData;

typedef struct
{
    float red_color;
    float green_color;
    float blue_color;
    float alpha;
} Color;

typedef struct
{
    float x_axis;
    float y_axis;
    float z_axis;
} Vector3;

typedef struct
{
    uint16_t x_axis;
    uint16_t y_axis;
    uint16_t z_axis;
} ShortVector3;

typedef struct
{
    uint8_t x_axis;
    uint8_t y_axis;
    uint8_t z_axis;
} ByteVector3;

typedef struct
{
    float x_axis;
    float y_axis;
    float z_axis;
    float w_axis;

} Quaternion;

typedef struct
{
    int16_t x_axis;
    int16_t y_axis;
    int16_t z_axis;
    int16_t w_axis;
} ShortQuaternion;

typedef struct
{
    uint16_t year_time;
    int8_t month_time;
    int8_t day_time;
    int8_t hours_time;
    int8_t minutes_time;
    int8_t seconds_time;
} TimeStampData;

typedef struct
{
    float duration;
    uint32_t event_type_index;
    uint32_t parameter_index;
} MotionEvent;

typedef struct
{
    float start_time;
    float end_time;
    uint32_t event_type_index;
    uint32_t parameter_index;
} MotionEventV2;

typedef struct
{
    float start_time;
    float end_time;
    uint32_t event_type_index;
    uint16_t parameter_index;
} MotionEventV3;

typedef struct
{
    uint32_t num_events;
    uint32_t num_type_strings;
    uint32_t num_param_strings;

    // followed by:
    // [num_type_strings] string objects
    // [num_param_strings] string objects
    // MotionEventV3[num_events]
} MotionEventTrack;

typedef struct
{
    uint32_t num_events;
    uint32_t num_type_strings;
    uint32_t num_param_strings;
    uint8_t is_enabled;

    // followed by:
    // String track name
    // [num_type_strings] string objects
    // [num_param_strings] string objects
    // FileMotionEvent3[num_events]
} MotionEventTrackV2;

typedef struct
{
    uint32_t num_tracks;

    // followed by:
    // MotionEventTrackV2[num_tracks]
} MotionEventTable;

typedef struct
{
    uint32_t data_type;
    uint32_t num_bytes;
    uint32_t flags;

    // followed by:
    // uint8 buffer_data[num_bytes];
} AttributeData;

#pragma pack(pop) // Restore default alignment

size_t skip_buffer(const uint8_t **buffer, size_t size, const uint8_t *buffer_end);
size_t read_from_buffer(const uint8_t **buffer, void *dest, size_t size, const uint8_t *buffer_end);
int read_string(const uint8_t **buffer, const uint8_t *buffer_end, char **result_text);

size_t skip_buffer(const uint8_t **buffer, size_t size, const uint8_t *buffer_end)
{
    if (*buffer + size > buffer_end)
    {
        return 0; // Prevent buffer overflow
    }
    *buffer += size;
    return size;
}

size_t read_from_buffer(const uint8_t **buffer, void *dest, size_t size, const uint8_t *buffer_end)
{
    if (*buffer + size > buffer_end)
    {
        printf("Error: Buffer overflow prevented\n");
        return 0;
    }

    memcpy(dest, *buffer, size);
    *buffer += size;

    return size;
}

int read_string(const uint8_t **buffer, const uint8_t *buffer_end, char **result_text)
{
    if (*buffer + sizeof(uint32_t) > buffer_end)
    {
        return EXIT_FAILURE; // Prevent buffer overflow
    }

    // Read string length (ensure correct endianness if necessary)
    uint32_t length;
    memcpy(&length, *buffer, sizeof(uint32_t));
    *buffer += sizeof(uint32_t); // Move past the length field

    // Validate length
    if (length == 0 || *buffer + length > buffer_end || length > 1024 * 1024) // Limiting size to 1MB
    {
        return EXIT_FAILURE; // Prevent buffer overflow or invalid length
    }

    // Allocate memory for the string (+1 for null terminator)
    *result_text = (char *)malloc(length + 1);
    if (!*result_text)
    {
        return EXIT_FAILURE; // Memory allocation failure
    }

    // Copy string data
    memcpy(*result_text, *buffer, length);
    (*result_text)[length] = '\0'; // Null-terminate the string

    *buffer += length; // Move past the string data

    return EXIT_SUCCESS;
}

#endif // SHARED_FORMATS_H
