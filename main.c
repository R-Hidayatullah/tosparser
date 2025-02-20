
#include "ipf.h"

/**
 * Main function to load and print IPF data.
 */
int main(void)
{

    char *filename = "char_texture.ipf";

    FILE *ipf_file = fopen(filename, "rb");
    if (!ipf_file)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    IPF_Root ipf_root = {0};

    if (!load_ipf(&ipf_root, ipf_file))
    {
        fclose(ipf_file);
        return EXIT_FAILURE;
    }

    print_ipf_details(&ipf_root);

    // Extract the first file
    uint8_t *extracted_data = NULL;
    size_t extracted_size = 0;

    if (extract_data(ipf_file, &ipf_root.ipf_file_table[0], &extracted_data, &extracted_size))
    {
        printf("Extraction successful.\n");
        print_hex_dump(extracted_data, 16, 2);
        free(extracted_data);
    }
    else
    {
        printf("Extraction failed.\n");
    }

    free_ipf_file_table(ipf_root.ipf_file_table, ipf_root.ipf_footer.file_count);
    fclose(ipf_file);

    return EXIT_SUCCESS;
}