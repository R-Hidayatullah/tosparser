#if !defined(XPM_H)
#define XPM_H

#include "shared_formats.h"

typedef enum
{
    XPM_CHUNK_SUBMOTION = 100,
    XPM_CHUNK_INFO = 101,
    XPM_CHUNK_MOTIONEVENTTABLE = SHARED_CHUNK_MOTIONEVENTTABLE,
    XPM_CHUNK_SUBMOTIONS = 102,
    XPM_FORCE_32BIT = (int)0xFFFFFFFF
} XPM_ChunkType;

typedef struct
{
    float time;  // the time, in seconds
    float value; // the value
} XPM_FloatKey;

typedef struct
{
    float time;     // the time in seconds
    uint16_t value; // the value
} XPM_UnsignedShortKey;

typedef struct
{
    uint8_t fourcc[IDENTIFIER_LEN]; // must be "XPM "
    uint8_t hi_version;             // high version (2 in case of v2.34)
    uint8_t lo_version;             // low version (34 in case of v2.34)
    uint8_t endian_type;            // the endian in which the data is saved [0=little, 1=big]
    uint8_t mul_order;              // see the enum MULORDER_....
} XPM_Header;

typedef struct
{
    uint32_t motion_fps; // the motion frame rate
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;

    char *source_application; // pointer to string: source application (e.g. "3D Studio MAX 7", "Maya 6.5")
    char *original_filename;  // pointer to string: original filename of the 3DSMAX/Maya file
    char *compilation_date;   // pointer to string: compilation date of the exporter
    char *motion_name;        // pointer to string: the name of the motion
} XPM_Info;

typedef struct
{
    float pose_weight;    // pose weight to use in case no animation data is present
    float min_weight;     // minimum allowed weight value (used for unpacking the keyframe weights)
    float max_weight;     // maximum allowed weight value (used for unpacking the keyframe weights)
    uint32_t phoneme_set; // the phoneme set of the submotion, 0 if this is a normal progressive morph target submotion
    uint32_t num_keys;    // number of keyframes to follow

    char *name; // pointer to string: name (the name of this motion part)

    XPM_UnsignedShortKey *keys; // pointer to array of XPM_UnsignedShortKey[num_keys]
} XPM_ProgressiveSubMotion;

typedef struct
{
    uint32_t num_sub_motions; // number of sub-motions to follow

    XPM_ProgressiveSubMotion *sub_motions; // pointer to array of XPM_ProgressiveSubMotion[num_sub_motions]
} XPM_SubMotions;

typedef struct
{
    XPM_Header xpm_header;
    XPM_SubMotions xpm_submotions;
} XPM_Root;

#endif // XPM_H
