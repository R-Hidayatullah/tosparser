#if !defined(IPF_H)
#define IPF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <zlib.h>

const int64_t IPF_MAGIC = 0x6054B50;
const int64_t HEADER_LOCATION = -24;
const size_t PASSWORD_LEN = 20;

#pragma pack(push, 1) // Set alignment to 1 byte
typedef struct
{
    uint16_t file_count;
    uint32_t file_table_pointer;
    uint16_t padding;
    uint32_t footer_pointer;
    uint32_t magic;
    uint32_t version_to_patch;
    uint32_t new_version;
} IPF_Footer;
#pragma pack(pop) // Restore default alignment

#pragma pack(push, 1) // Set alignment to 1 byte
typedef struct
{
    uint16_t directory_name_length;
    uint32_t crc32;
    uint32_t file_size_compressed;
    uint32_t file_size_uncompressed;
    uint32_t file_pointer;
    uint16_t container_name_length;
} IPF_FileMetadata;
#pragma pack(pop) // Restore default alignment

typedef struct
{
    IPF_FileMetadata metadata;
    char *container_name;
    char *directory_name;
} IPF_FileTable;

typedef struct
{
    IPF_Footer ipf_footer;
    IPF_FileTable *ipf_file_table;
} IPF_Root;

/**
 * Frees allocated memory for IPF file table.
 */
void free_ipf_file_table(IPF_FileTable *file_table, uint16_t file_count)
{
    if (!file_table)
        return;

    for (uint16_t i = 0; i < file_count; i++)
    {
        free(file_table[i].container_name);
        free(file_table[i].directory_name);
    }
    free(file_table);
}

/**
 * Loads IPF file data into IPF_Root.
 */
int load_ipf(IPF_Root *ipf_root, FILE *file)
{
    // Seek to footer location
    fseek(file, HEADER_LOCATION, SEEK_END);
    long footer_pos = ftell(file);
    printf("Seeking to footer location: %ld\n", footer_pos);

    // Read footer
    if (fread(&ipf_root->ipf_footer, sizeof(IPF_Footer), 1, file) != 1)
    {
        perror("Failed to read footer");
        return 0;
    }

    printf("Footer read successfully\n");
    printf("Magic: 0x%X\n", ipf_root->ipf_footer.magic);
    printf("File count: %u\n", ipf_root->ipf_footer.file_count);
    printf("File table pointer: %u\n", ipf_root->ipf_footer.file_table_pointer);

    // Verify magic number
    if (ipf_root->ipf_footer.magic != IPF_MAGIC)
    {
        fprintf(stderr, "Invalid IPF file (wrong magic number: 0x%X)\n", ipf_root->ipf_footer.magic);
        return 0;
    }

    // Allocate memory for file table
    ipf_root->ipf_file_table = calloc(ipf_root->ipf_footer.file_count, sizeof(IPF_FileTable));
    if (!ipf_root->ipf_file_table)
    {
        perror("Memory allocation failed");
        return 0;
    }

    // Seek to file table
    if (fseek(file, ipf_root->ipf_footer.file_table_pointer, SEEK_SET) != 0)
    {
        perror("Failed to seek to file table location");
        return 0;
    }

    long file_table_pos = ftell(file);
    printf("Seeking to file table location: %ld\n", file_table_pos);

    // Read file table metadata
    for (uint16_t i = 0; i < ipf_root->ipf_footer.file_count; i++)
    {
        IPF_FileTable *file_entry = &ipf_root->ipf_file_table[i];

        // Read metadata
        if (fread(&file_entry->metadata, sizeof(IPF_FileMetadata), 1, file) != 1)
        {
            perror("Failed to read file metadata");
            free_ipf_file_table(ipf_root->ipf_file_table, i);
            return 0;
        }

        // Read container name
        file_entry->container_name = malloc(file_entry->metadata.container_name_length + 1);
        if (!file_entry->container_name)
        {
            perror("Memory allocation failed for container_name");
            free_ipf_file_table(ipf_root->ipf_file_table, i);
            return 0;
        }

        if (fread(file_entry->container_name, file_entry->metadata.container_name_length, 1, file) != 1)
        {
            perror("Failed to read container name");
            free_ipf_file_table(ipf_root->ipf_file_table, i);
            return 0;
        }
        file_entry->container_name[file_entry->metadata.container_name_length] = '\0'; // Null-terminate

        // Read directory name
        file_entry->directory_name = malloc(file_entry->metadata.directory_name_length + 1);
        if (!file_entry->directory_name)
        {
            perror("Memory allocation failed for directory_name");
            free_ipf_file_table(ipf_root->ipf_file_table, i);
            return 0;
        }

        if (fread(file_entry->directory_name, file_entry->metadata.directory_name_length, 1, file) != 1)
        {
            perror("Failed to read directory name");
            free_ipf_file_table(ipf_root->ipf_file_table, i);
            return 0;
        }
        file_entry->directory_name[file_entry->metadata.directory_name_length] = '\0'; // Null-terminate
        // printf("Read file %u: Container = %s, Directory = %s, CRC32 = 0x%X, Compressed Size = %u\n",
        //        i, file_entry->container_name, file_entry->directory_name,
        //        file_entry->metadata.crc32, file_entry->metadata.file_size_compressed);
    }

    return 1;
}

/**
 * Prints the IPF file details.
 */
void print_ipf_details(const IPF_Root *ipf_root)
{
    printf("IPF Version: %u -> %u\n", ipf_root->ipf_footer.version_to_patch, ipf_root->ipf_footer.new_version);
    printf("File Count: %u\n", ipf_root->ipf_footer.file_count);
    printf("File Table Pointer: 0x%X\n\n", ipf_root->ipf_footer.file_table_pointer);

    for (uint16_t i = 0; i < 3; i++)
    {
        printf("File %d:\n", i + 1);
        printf("  CRC32: 0x%X\n", ipf_root->ipf_file_table[i].metadata.crc32);
        printf("  Compressed Size: %u bytes\n", ipf_root->ipf_file_table[i].metadata.file_size_compressed);
        printf("  Uncompressed Size: %u bytes\n", ipf_root->ipf_file_table[i].metadata.file_size_uncompressed);
        printf("  File Pointer: 0x%X\n", ipf_root->ipf_file_table[i].metadata.file_pointer);
        printf("  Container Name: %s\n", ipf_root->ipf_file_table[i].container_name);
        printf("  Directory Name: %s\n\n", ipf_root->ipf_file_table[i].directory_name);
    }
}

static uint32_t CRC32_m_tab[256];

void init_crc32_table(void)
{
    for (uint32_t i = 0; i < 256; ++i)
    {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j)
        {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
        CRC32_m_tab[i] = crc;
    }
}

uint32_t compute_crc32(uint32_t crc, uint8_t b)
{
    return CRC32_m_tab[(crc ^ b) & 0xFF] ^ (crc >> 8);
}

uint8_t extract_byte(uint32_t value, int byte_index)
{
    return (uint8_t)(value >> (byte_index * 8));
}

void keys_update(uint32_t keys[3], uint8_t b)
{
    keys[0] = compute_crc32(keys[0], b);
    keys[1] = 0x8088405 * ((uint8_t)keys[0] + keys[1]) + 1;
    keys[2] = compute_crc32(keys[2], extract_byte(keys[1], 3));
}

void keys_generate(uint32_t keys[3])
{
    const uint8_t password[] = {
        0x6F, 0x66, 0x4F, 0x31, 0x61, 0x30, 0x75,
        0x65, 0x58, 0x41, 0x3F, 0x20, 0x5B, 0xFF,
        0x73, 0x20, 0x68, 0x20, 0x25, 0x3F};

    keys[0] = 0x12345678;
    keys[1] = 0x23456789;
    keys[2] = 0x34567890;

    for (size_t i = 0; i < PASSWORD_LEN; i++)
    {
        keys_update(keys, password[i]);
    }
}

void ipf_decrypt(uint8_t *buffer, size_t size)
{
    if (size == 0)
        return;

    init_crc32_table();

    uint32_t keys[3] = {0, 0, 0};
    keys_generate(keys);

    size_t buffer_size = ((size - 1) >> 1) + 1;

    for (size_t i = 0; i < buffer_size; ++i)
    {
        uint32_t v = (keys[2] & 0xFFFD) | 2;
        buffer[i * 2] ^= (uint8_t)((v * (v ^ 1)) >> 8);
        keys_update(keys, buffer[i * 2]);
    }
}

void ipf_encrypt(uint8_t *buffer, size_t size)
{
    if (size == 0)
        return;

    init_crc32_table();

    uint32_t keys[3] = {0, 0, 0};
    keys_generate(keys);

    size_t buffer_size = ((size - 1) >> 1) + 1;

    for (size_t i = 0; i < buffer_size; ++i)
    {
        uint32_t v = (keys[2] & 0xFFFD) | 2;
        keys_update(keys, buffer[i * 2]);
        buffer[i * 2] ^= (uint8_t)((v * (v ^ 1)) >> 8);
    }
}

bool extract_data(FILE *ipf_file, IPF_FileTable *file_entry, uint8_t **output_data, size_t *output_size)
{
    size_t compressed_size = file_entry->metadata.file_size_compressed;
    size_t uncompressed_size = file_entry->metadata.file_size_uncompressed; // Ensure this value is available
    uint8_t *compressed_data = malloc(compressed_size);

    if (!compressed_data)
    {
        perror("Memory allocation failed");
        return false;
    }

    fseek(ipf_file, file_entry->metadata.file_pointer, SEEK_SET);
    if (fread(compressed_data, compressed_size, 1, ipf_file) != 1)
    {
        perror("Failed to read file data");
        free(compressed_data);
        return false;
    }

    ipf_decrypt(compressed_data, compressed_size);

    *output_data = malloc(uncompressed_size);
    if (!*output_data)
    {
        perror("Memory allocation failed for decompressed data");
        free(compressed_data);
        return false;
    }

    // Initialize zlib decompression
    z_stream stream = {0};
    stream.next_in = compressed_data;
    stream.avail_in = (uInt)compressed_size;
    stream.next_out = *output_data;
    stream.avail_out = (uInt)uncompressed_size;

    if (inflateInit2(&stream, -MAX_WBITS) != Z_OK)
    {
        perror("Failed to initialize zlib decompression");
        free(compressed_data);
        free(*output_data);
        *output_data = NULL;
        return false;
    }

    int ret = inflate(&stream, Z_FINISH);
    inflateEnd(&stream);

    if (ret != Z_STREAM_END)
    {
        perror("Decompression failed");
        free(compressed_data);
        free(*output_data);
        *output_data = NULL;
        return false;
    }

    *output_size = stream.total_out; // Update with actual decompressed size
    free(compressed_data);
    return true;
}

void print_hex_dump(const uint8_t *data, size_t bytes_per_row, size_t row_size)
{
    for (size_t i = 0; i < (row_size * bytes_per_row); i++)
    {
        if (i % bytes_per_row == 0)
        {
            if (i > 0) // Print ASCII representation after each row
            {
                printf("|");
                for (size_t j = i - bytes_per_row; j < i; j++)
                {
                    printf("%c", isprint(data[j]) ? data[j] : '.');
                }
                printf("|");
            }
            printf("\n%04zx: ", i); // Print row address
        }
        printf("%02X ", data[i]); // Print hex values
    }

    // Print remaining ASCII for the last row
    size_t last_row_start = (row_size * bytes_per_row) - bytes_per_row;
    printf("|");
    for (size_t j = last_row_start; j < (row_size * bytes_per_row); j++)
    {
        printf("%c", isprint(data[j]) ? data[j] : '.');
    }
    printf("|");

    printf("\n");
}

#endif // IPF_H
