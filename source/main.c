#include "ipf.h"
#include "xac.h"
#include "xsm.h"
#include "xpm.h"
#include "hashmap.h"
#include <sys/stat.h>

int file_exists(const char *filename);
void save_extracted_data(const char *output_filename, const uint8_t *buffer, size_t buffer_size);

// Function to check if a file exists
int file_exists(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to save extracted data to a file if it doesn't exist
void save_extracted_data(const char *output_filename, const uint8_t *buffer, size_t buffer_size)
{
    if (file_exists(output_filename))
    {
        printf("File '%s' already exists. Skipping save.\n", output_filename);
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: Unable to create file '%s'.\n", output_filename);
        return;
    }

    fwrite(buffer, 1, buffer_size, output_file);
    fclose(output_file);
    printf("Extracted data saved to '%s'.\n", output_filename);
}

/**
 * Main function to load and print IPF data.
 *
 * This function:
 * 1. Opens the specified IPF file.
 * 2. Loads the IPF archive into memory.
 * 3. Prints the IPF file details.
 * 4. Extracts the first file (if available) and displays a hex dump.
 * 5. Cleans up allocated resources before exiting.
 *
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(void)
{
    const char *filename = "/home/ridwan/Documents/TreeOfSaviorCN/data/char_hi.ipf"; // Name of the IPF file to be loaded
    const char *output_filename = "extracted_data.bin";                              // Output file

    // Open the IPF file in binary mode
    FILE *ipf_file = fopen(filename, "rb");
    if (!ipf_file)
    {
        perror("Failed to open file"); // Print error message if the file can't be opened
        return EXIT_FAILURE;
    }

    // Move the file pointer to the end to get file size
    fseek(ipf_file, 0, SEEK_END);
    long file_size = ftell(ipf_file);
    rewind(ipf_file); // Reset file pointer to the beginning

    // Print filename and size
    printf("Filename: %s\n", filename);
    printf("File Size: %ld bytes\n\n", file_size);

    IPF_Root ipf_root = {0}; // Initialize IPF_Root structure to zero
    XAC_Root xac_root = {0}; // Initialize XAC_Root structure to zero

    // Attempt to load the IPF file
    if (!load_ipf(&ipf_root, ipf_file))
    {
        fprintf(stderr, "Error: Failed to load IPF file.\n");
        fclose(ipf_file);
        return EXIT_FAILURE;
    }

    // Print details about the loaded IPF file
    print_ipf_details(&ipf_root);

    // Ensure there is at least one file before accessing ipf_file_table[0]
    if (ipf_root.ipf_footer.file_count == 0 || ipf_root.ipf_file_table == NULL)
    {
        fprintf(stderr, "Error: No files found in the IPF archive.\n");
        free_ipf_file_table(ipf_root.ipf_file_table, ipf_root.ipf_footer.file_count);
        fclose(ipf_file);
        return EXIT_FAILURE;
    }

    // Variables for extracted data
    uint8_t *extracted_data = NULL;
    size_t extracted_size = 0;

    // Attempt to extract the first file from the archive
    if (extract_data(ipf_file, &ipf_root.ipf_file_table[2], &extracted_data, &extracted_size))
    {
        if (extracted_data) // Ensure extracted_data is valid before using it
        {
            // Call function to save extracted data if needed
            save_extracted_data(output_filename, extracted_data, extracted_size);

            printf("Extraction successful.\n");
            print_buffer_position(extracted_data);
            // Print a hex dump of the first 16 bytes of the extracted file
            print_hex_dump(extracted_data, 16, 4);
            if (extracted_data && extracted_size >= 4)
            {
                if (memcmp(extracted_data, "XAC", 3) == 0)
                {
                    printf("Extracted data is an XAC file.\n");
                    parse_xac_root(&xac_root, extracted_data, extracted_size);
                    free(xac_root.xac_info.version_2.source_application);
                    free(xac_root.xac_info.version_2.original_filename);
                    free(xac_root.xac_info.version_2.export_date);
                    free(xac_root.xac_info.version_2.actor_name);
                }
                else
                {
                    printf("Extracted data is NOT an XAC file.\n");
                }
            }

            free(extracted_data); // Free extracted data memory after use
        }
        else
        {
            fprintf(stderr, "Error: Extracted data is NULL.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error: Extraction failed.\n");
    }

    // Free allocated memory for the IPF file table
    if (ipf_root.ipf_file_table)
    {
        free_ipf_file_table(ipf_root.ipf_file_table, ipf_root.ipf_footer.file_count);
    }

    // Close the file safely
    if (ipf_file)
    {
        fclose(ipf_file);
    }

    return EXIT_SUCCESS; // Indicate successful execution
}