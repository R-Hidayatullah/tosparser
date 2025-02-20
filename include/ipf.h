#if !defined(IPF_H)
#define IPF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <zlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Safer constant definitions
    static const uint32_t IPF_MAGIC = 0x6054B50;
    static const int64_t HEADER_LOCATION = -24;
    static const size_t PASSWORD_LEN = 20;

    static const uint32_t CRC32_m_tab[256] = {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

#pragma pack(push, 1) // Ensure structures are packed without padding
    /**
     * Structure representing the footer of an IPF file.
     * This footer is located at the end of the file and contains metadata
     * about the stored files and their locations.
     */
    typedef struct
    {
        uint16_t file_count;         // Number of files in the archive
        uint32_t file_table_pointer; // Offset to the file table in the archive
        uint16_t padding;            // Unused padding (likely for alignment)
        uint32_t footer_pointer;     // Offset to this footer structure
        uint32_t magic;              // Unique identifier (magic number) for validation
        uint32_t version_to_patch;   // Version number before patching
        uint32_t new_version;        // Version number after patching
    } IPF_Footer;
#pragma pack(pop) // Restore default alignment

#pragma pack(push, 1) // Ensure structures are packed without padding
    /**
     * Structure representing metadata for a single file in the IPF archive.
     * This metadata is part of the file table and provides information
     * about each stored file.
     */
    typedef struct
    {
        uint16_t directory_name_length;  // Length of the directory name string
        uint32_t crc32;                  // CRC32 checksum for integrity verification
        uint32_t file_size_compressed;   // File size after compression (in bytes)
        uint32_t file_size_uncompressed; // Original file size before compression (in bytes)
        uint32_t file_pointer;           // Offset to the actual file data in the archive
        uint16_t container_name_length;  // Length of the container name string
    } IPF_FileMetadata;
#pragma pack(pop) // Restore default alignment

    /**
     * Structure representing an entry in the IPF file table.
     * Each entry contains metadata and associated strings for
     * the container name and directory name.
     */
    typedef struct
    {
        IPF_FileMetadata metadata; // Metadata about the file
        char *container_name;      // Pointer to the container name string
        char *directory_name;      // Pointer to the directory name string
    } IPF_FileTable;

    /**
     * Root structure for the IPF archive, containing the footer and file table.
     * This structure is used to manage the archive data in memory.
     */
    typedef struct
    {
        IPF_Footer ipf_footer;         // Footer structure containing archive metadata
        IPF_FileTable *ipf_file_table; // Pointer to the dynamically allocated file table
    } IPF_Root;

    /**
     * Frees allocated memory for IPF file table.
     *
     * @param file_table Pointer to the array of IPF_FileTable structures.
     * @param file_count Number of files in the file table.
     */
    void free_ipf_file_table(IPF_FileTable *file_table, uint16_t file_count)
    {
        if (!file_table)
            return;

        // Loop through each file entry and free allocated name strings
        for (uint16_t i = 0; i < file_count; i++)
        {
            free(file_table[i].container_name); // Free container name
            free(file_table[i].directory_name); // Free directory name
        }

        // Free the entire file table array
        free(file_table);
    }

    /**
     * Loads IPF file data into IPF_Root.
     *
     * @param ipf_root Pointer to the root IPF structure.
     * @param file Pointer to the opened IPF file.
     * @return 1 on success, 0 on failure.
     */
    int load_ipf(IPF_Root *ipf_root, FILE *file)
    {
        // Seek to footer location at the end of the file
        if (fseek(file, HEADER_LOCATION, SEEK_END) != 0)
        {
            perror("Failed to seek to footer");
            return 0;
        }

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

        // Seek to file table location
        if (fseek(file, ipf_root->ipf_footer.file_table_pointer, SEEK_SET) != 0)
        {
            perror("Failed to seek to file table location");
            free(ipf_root->ipf_file_table);
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

    /**
     * Computes the CRC32 value for a single byte using the given CRC32 table.
     *
     * @param crc The current CRC32 value.
     * @param b The byte to process.
     * @return The updated CRC32 value.
     */
    uint32_t compute_crc32(uint32_t crc, uint8_t b)
    {
        return CRC32_m_tab[(crc ^ b) & 0xFF] ^ (crc >> 8);
    }

    /**
     * Extracts a specific byte from a 32-bit integer.
     *
     * @param value The 32-bit integer.
     * @param byte_index The index of the byte to extract (0-3).
     * @return The extracted byte.
     */
    uint8_t extract_byte(uint32_t value, int byte_index)
    {
        return (uint8_t)(value >> (byte_index * 8));
    }

    /**
     * Updates the encryption keys based on the given byte.
     *
     * @param keys A 3-element array representing the encryption keys.
     * @param b The byte used to update the keys.
     */
    void keys_update(uint32_t keys[3], uint8_t b)
    {
        // Update first key using CRC32 computation
        keys[0] = compute_crc32(keys[0], b);

        // Update second key using a custom transformation
        keys[1] = 0x8088405 * ((uint8_t)keys[0] + keys[1]) + 1;

        // Update third key based on the most significant byte of keys[1]
        keys[2] = compute_crc32(keys[2], extract_byte(keys[1], 3));
    }

    /**
     * Generates an initial set of encryption keys based on a predefined password.
     *
     * @param keys A 3-element array to store the generated encryption keys.
     */
    void keys_generate(uint32_t keys[3])
    {
        // Predefined password (20 bytes)
        const uint8_t password[] = {
            0x6F, 0x66, 0x4F, 0x31, 0x61, 0x30, 0x75,
            0x65, 0x58, 0x41, 0x3F, 0x20, 0x5B, 0xFF,
            0x73, 0x20, 0x68, 0x20, 0x25, 0x3F};

        // Initialize keys with predefined values
        keys[0] = 0x12345678;
        keys[1] = 0x23456789;
        keys[2] = 0x34567890;

        // Update keys based on the password
        for (size_t i = 0; i < PASSWORD_LEN; i++)
        {
            keys_update(keys, password[i]);
        }
    }

    /**
     * Decrypts a given buffer using a key-based decryption algorithm.
     *
     * @param buffer The buffer containing encrypted data.
     * @param size The size of the buffer.
     */
    void ipf_decrypt(uint8_t *buffer, size_t size)
    {
        // No operation if the buffer is empty
        if (size == 0)
            return;

        // Generate encryption keys
        uint32_t keys[3] = {0, 0, 0};
        keys_generate(keys);

        // Calculate the number of iterations needed (half of the buffer size, rounded up)
        size_t buffer_size = ((size - 1) >> 1) + 1;

        for (size_t i = 0; i < buffer_size; ++i)
        {
            // Generate a pseudo-random transformation value
            uint32_t v = (keys[2] & 0xFFFD) | 2;

            // Decrypt every second byte
            buffer[i * 2] ^= (uint8_t)((v * (v ^ 1)) >> 8);

            // Update encryption keys
            keys_update(keys, buffer[i * 2]);
        }
    }

    /**
     * Encrypts a given buffer using a key-based encryption algorithm.
     *
     * @param buffer The buffer containing plaintext data.
     * @param size The size of the buffer.
     */
    void ipf_encrypt(uint8_t *buffer, size_t size)
    {
        // No operation if the buffer is empty
        if (size == 0)
            return;

        // Generate encryption keys
        uint32_t keys[3] = {0, 0, 0};
        keys_generate(keys);

        // Calculate the number of iterations needed (half of the buffer size, rounded up)
        size_t buffer_size = ((size - 1) >> 1) + 1;

        for (size_t i = 0; i < buffer_size; ++i)
        {
            // Generate a pseudo-random transformation value
            uint32_t v = (keys[2] & 0xFFFD) | 2;

            // Update encryption keys before encrypting data
            keys_update(keys, buffer[i * 2]);

            // Encrypt every second byte
            buffer[i * 2] ^= (uint8_t)((v * (v ^ 1)) >> 8);
        }
    }

    /**
     * Extracts and decompresses data from an IPF file.
     *
     * @param ipf_file A pointer to the opened IPF file.
     * @param file_entry Metadata about the file within the archive.
     * @param output_data A pointer to store the decompressed data buffer.
     * @param output_size A pointer to store the decompressed data size.
     * @return True if successful, false if an error occurs.
     */
    bool extract_data(FILE *ipf_file, IPF_FileTable *file_entry, uint8_t **output_data, size_t *output_size)
    {
        size_t compressed_size = file_entry->metadata.file_size_compressed;
        size_t uncompressed_size = file_entry->metadata.file_size_uncompressed; // Ensure this value is available

        // Allocate memory for the compressed data
        uint8_t *compressed_data = malloc(compressed_size);
        if (!compressed_data)
        {
            perror("Memory allocation failed");
            return false;
        }

        // Seek to the file position where the compressed data is stored
        fseek(ipf_file, file_entry->metadata.file_pointer, SEEK_SET);

        // Read the compressed data into memory
        if (fread(compressed_data, compressed_size, 1, ipf_file) != 1)
        {
            perror("Failed to read file data");
            free(compressed_data);
            return false;
        }

        // Decrypt the compressed data
        ipf_decrypt(compressed_data, compressed_size);

        // Allocate memory for decompressed data
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

        // Use raw zlib stream (without headers)
        if (inflateInit2(&stream, -MAX_WBITS) != Z_OK)
        {
            perror("Failed to initialize zlib decompression");
            free(compressed_data);
            free(*output_data);
            *output_data = NULL;
            return false;
        }

        // Perform decompression
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

        // Update output size with actual decompressed size
        *output_size = stream.total_out;

        // Free compressed data buffer
        free(compressed_data);

        return true;
    }

    /**
     * Prints a formatted hexadecimal dump of binary data.
     *
     * @param data The pointer to the binary data buffer.
     * @param bytes_per_row Number of bytes to display per row.
     * @param row_size Number of rows to display.
     */
    void print_hex_dump(const uint8_t *data, size_t bytes_per_row, size_t row_size)
    {
        for (size_t i = 0; i < (row_size * bytes_per_row); i++)
        {
            // Start a new line for every row
            if (i % bytes_per_row == 0)
            {
                if (i > 0) // Print ASCII representation after each row
                {
                    printf("| ");
                    for (size_t j = i - bytes_per_row; j < i; j++)
                    {
                        printf("%c", isprint(data[j]) ? data[j] : '.');
                    }
                    printf(" |");
                }
                printf("\n%04zx: ", i); // Print row address
            }

            // Print hex value of each byte
            printf("%02X ", data[i]);
        }

        // Print remaining ASCII characters for the last row
        size_t last_row_start = (row_size * bytes_per_row) - bytes_per_row;
        printf("| ");
        for (size_t j = last_row_start; j < (row_size * bytes_per_row); j++)
        {
            printf("%c", isprint(data[j]) ? data[j] : '.');
        }
        printf(" |\n");
    }

#ifdef __cplusplus
}
#endif

#endif // IPF_H
