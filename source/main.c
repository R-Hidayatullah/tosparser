#include "ipf.h"
#include "hashmap.h"

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

    // Open the IPF file in binary mode
    FILE *ipf_file = fopen(filename, "rb");
    if (!ipf_file)
    {
        perror("Failed to open file"); // Print error message if the file can't be opened
        return EXIT_FAILURE;
    }

    IPF_Root ipf_root = {0}; // Initialize IPF_Root structure to zero

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
    if (extract_data(ipf_file, &ipf_root.ipf_file_table[4], &extracted_data, &extracted_size))
    {
        if (extracted_data) // Ensure extracted_data is valid before using it
        {
            printf("Extraction successful.\n");

            // Print a hex dump of the first 16 bytes of the extracted file
            print_hex_dump(extracted_data, 16, 4);

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