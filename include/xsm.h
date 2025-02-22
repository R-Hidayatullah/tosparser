#if !defined(XSM_H)
#define XSM_H
#include "shared_formats.h"

typedef enum
{
    XSM_CHUNK_SUBMOTION = 200,
    XSM_CHUNK_INFO = 201,
    XSM_CHUNK_MOTIONEVENTTABLE = SHARED_CHUNK_MOTIONEVENTTABLE,
    XSM_CHUNK_SUBMOTIONS = 202,
    XSM_CHUNK_WAVELETINFO = 203, // see XSM_WaveletInfo
    XSM_FORCE_32BIT = (int)0xFFFFFFFF
} XsmChunkType;

typedef enum
{
    XSM_WAVELET_HAAR = 0,
    XSM_WAVELET_D4 = 1,
    XSM_WAVELET_CDF97 = 2
} WaveletType;

typedef enum
{
    XSM_COMPRESSOR_HUFFMAN = 0,
    XSM_COMPRESSOR_RICE = 1
} CompressorType;

#pragma pack(push, 1) // Save current packing and set new alignment

typedef struct
{
    Vector3 value_data;
    float time_in_seconds;
} XSM_Vector3Key;

typedef struct
{
    Quaternion value_data;
    float time_in_seconds;
} XSM_QuaternionKey;

typedef struct
{
    ShortQuaternion value_data;
    float time_in_seconds;
} XSM_ShortQuaternionKey;

typedef struct
{
    ShortQuaternion pose_rot;            // initial pose rotation
    ShortQuaternion bind_pose_rot;       // bind pose rotation
    ShortQuaternion pose_scale_rot;      // pose scale rotation
    ShortQuaternion bind_pose_scale_rot; // bind pose scale rotation
    Vector3 pose_pos;                    // initial pose position
    Vector3 pose_scale;                  // initial pose scale
    Vector3 bind_pose_pos;               // bind pose position
    Vector3 bind_pose_scale;             // bind pose scale
    float max_error;                     // maximum error used by the automatic motion LOD system

    char *motion_part_name; // string : motion part name
} XSM_WaveletSkeletalSubMotion;

typedef struct
{
    float rotation_quant_scale;
    float position_quant_scale;
    float scale_quant_scale;
    float start_time;
    uint32_t compressed_rotation_num_bytes;
    uint32_t compressed_position_num_bytes;
    uint32_t compressed_scale_num_bytes;
    uint32_t compressed_position_num_bits;
    uint32_t compressed_rotation_num_bits;
    uint32_t compressed_scale_num_bits;

    uint8_t *compressed_rotation_data; // uint8_t[compressed_rotation_num_bytes]
    uint8_t *compressed_position_data; // uint8_t[compressed_position_num_bytes]
    uint8_t *compressed_scale_data;    // uint8_t[compressed_scale_num_bytes]
} XSM_WaveletChunk;

typedef struct
{
    uint8_t fourcc[IDENTIFIER_LEN]; // Must be "XSM "
    uint8_t high_version;           // High version (e.g., 2 for v2.34)
    uint8_t low_version;            // Low version  (e.g., 34 for v2.34)
    uint8_t endian_type;            // Endianness of stored data [0 = little-endian, 1 = big-endian]
    uint8_t mul_order;              // See the enum MULORDER_...
} XSM_Header;

typedef struct
{
    uint32_t motion_fps;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;

    char *source_app;        // Source application
    char *original_filename; // Original filename
    char *compilation_date;  // Compilation date
    char *motion_name;       // Name of the motion
} XSM_Info;

typedef struct
{
    float importance_factor;
    float max_acceptable_error;
    uint32_t motion_fps;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;

    char *source_app;
    char *original_filename;
    char *compilation_date;
    char *motion_name;
} XSM_InfoV2;

typedef struct
{
    float importance_factor;
    float max_acceptable_error;
    uint32_t motion_fps;
    uint32_t motion_extraction_mask;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;

    char *source_app;
    char *original_filename;
    char *compilation_date;
    char *motion_name;
} XSM_InfoV3;

typedef struct
{
    Quaternion pose_rotation;
    Quaternion bind_pose_rotation;
    Quaternion pose_scale_rotation;
    Quaternion bind_pose_scale_rotation;
    Vector3 pose_position;
    Vector3 pose_scale;
    Vector3 bind_pose_position;
    Vector3 bind_pose_scale;
    uint32_t num_position_keys;
    uint32_t num_rotation_keys;
    uint32_t num_scale_keys;
    uint32_t num_scale_rotation_keys;

    char *motion_part_name;                 // string : motion part name
    XSM_Vector3Key *position_keys;          // XSM_Vector3Key[ num_position_keys ]
    XSM_QuaternionKey *rotation_keys;       // XSM_QuaternionKey[ num_rotation_keys ]
    XSM_Vector3Key *scale_keys;             // XSM_Vector3Key[ num_scale_keys ]
    XSM_QuaternionKey *scale_rotation_keys; // XSM_QuaternionKey[ num_scale_rotation_keys ]
} XSM_SkeletalSubMotion;

typedef struct
{
    Quaternion pose_rotation;
    Quaternion bind_pose_rotation;
    Quaternion pose_scale_rotation;
    Quaternion bind_pose_scale_rotation;
    Vector3 pose_position;
    Vector3 pose_scale;
    Vector3 bind_pose_position;
    Vector3 bind_pose_scale;
    uint32_t num_position_keys;
    uint32_t num_rotation_keys;
    uint32_t num_scale_keys;
    uint32_t num_scale_rotation_keys;
    float max_error;

    char *motion_part_name;                 // string : motion part name
    XSM_Vector3Key *position_keys;          // XSM_Vector3Key[ num_position_keys ]
    XSM_QuaternionKey *rotation_keys;       // XSM_QuaternionKey[ num_rotation_keys ]
    XSM_Vector3Key *scale_keys;             // XSM_Vector3Key[ num_scale_keys ]
    XSM_QuaternionKey *scale_rotation_keys; // XSM_QuaternionKey[ num_scale_rotation_keys ]
} XSM_SkeletalSubMotionV2;

typedef struct
{
    ShortQuaternion pose_rotation;
    ShortQuaternion bind_pose_rotation;
    ShortQuaternion pose_scale_rotation;
    ShortQuaternion bind_pose_scale_rotation;
    Vector3 pose_position;
    Vector3 pose_scale;
    Vector3 bind_pose_position;
    Vector3 bind_pose_scale;
    uint32_t num_position_keys;
    uint32_t num_rotation_keys;
    uint32_t num_scale_keys;
    uint32_t num_scale_rotation_keys;
    float max_error;

    char *motion_part_name;               // string : motion part name
    XSM_Vector3Key *position_keys;        // XSM_Vector3Key[ num_position_keys ]
    ShortQuaternion *rotation_keys;       // XSM_16BitQuaternionKey[ num_rotation_keys ]
    XSM_Vector3Key *scale_keys;           // XSM_Vector3Key[ num_scale_keys ]
    ShortQuaternion *scale_rotation_keys; // XSM_16BitQuaternionKey[ num_scale_rotation_keys ]
} XSM_SkeletalSubMotionV3;

typedef struct
{
    uint32_t num_sub_motions;
    XSM_SkeletalSubMotion *sub_motions; // XSM_SkeletalSubMotion[ num_sub_motions ]
} XSM_SubMotions;

typedef struct
{
    uint32_t num_sub_motions;
    XSM_SkeletalSubMotionV2 *sub_motions; // XSM_SkeletalSubMotionV2[ num_sub_motions ]
} XSM_SubMotionsV2;

typedef struct
{
    uint32_t num_sub_motions;
    XSM_SkeletalSubMotionV3 *sub_motions; // XSM_SkeletalSubMotionV3[ num_sub_motions ]
} XSM_SubMotionsV3;

typedef struct
{
    uint16_t position_index;
    uint16_t rotation_index;
    uint16_t scale_rotation_index;
    uint16_t scale_index;
} XSM_WaveletMapping;

typedef struct
{
    uint32_t num_chunks;
    uint32_t samples_per_chunk;
    uint32_t decompressed_rotation_num_bytes;
    uint32_t decompressed_position_num_bytes;
    uint32_t decompressed_scale_num_bytes;
    uint32_t num_rotation_tracks;
    uint32_t num_scale_rotation_tracks;
    uint32_t num_scale_tracks;
    uint32_t num_position_tracks;
    uint32_t chunk_overhead;
    uint32_t compressed_size;
    uint32_t optimized_size;
    uint32_t uncompressed_size;
    uint32_t scale_rotation_offset;
    uint32_t num_sub_motions;
    float position_quant_factor;
    float rotation_quant_factor;
    float scale_quant_factor;
    float sample_spacing;
    float seconds_per_chunk;
    float max_time;
    uint8_t wavelet_id;    // see the enum with XSM_WAVELET_HAAR etc
    uint8_t compressor_id; // see the enum with XSM_COMPRESSOR_HUFFMAN inside

    XSM_WaveletMapping *wavelet_mappings; // XSM_WaveletMapping[num_sub_motions]
    // XSM_WaveletSubMotion *wavelet_sub_motions; // XSM_WaveletSubMotion[num_sub_motions]
    XSM_WaveletChunk *wavelet_chunks; // XSM_WaveletChunk[num_chunks]
} XSM_WaveletInfo;
#pragma pack(pop) // Restore previous packing alignment

typedef struct
{

    union
    {
        XSM_Info version_1;
        XSM_InfoV2 version_2;
        XSM_InfoV3 version_3;
    } xsm_info;

    union
    {
        XSM_SubMotions version_1;
        XSM_SubMotionsV2 version_2;
        XSM_SubMotionsV3 version_3;
    } xsm_submotions;

    // XSM_WaveletInfo xsm_wavelet_info;

} XSM_Root;

#endif // XSM_H
