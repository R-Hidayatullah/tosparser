#if !defined(XAC_H)
#define XAC_H
#include "shared_formats.h"

#define CHUNK_DATA_SIZE 32

typedef enum
{
    XAC_CHUNK_NODE = 0,
    XAC_CHUNK_MESH = 1,
    XAC_CHUNK_SKINNING_INFO = 2,
    XAC_CHUNK_STANDARD_MATERIAL = 3,
    XAC_CHUNK_STANDARD_MATERIAL_LAYER = 4,
    XAC_CHUNK_FX_MATERIAL = 5,
    XAC_CHUNK_LIMIT = 6,
    XAC_CHUNK_INFO = 7,
    XAC_CHUNK_MESH_LOD_LEVEL = 8,
    XAC_CHUNK_STD_MORPH_TARGET = 9,
    XAC_CHUNK_NODE_GROUPS = 10,
    XAC_CHUNK_NODES = 11,               // XAC_Nodes
    XAC_CHUNK_STD_MORPH_TARGETS = 12,   // XAC_PMorphTargets
    XAC_CHUNK_MATERIAL_INFO = 13,       // XAC_MaterialInfo
    XAC_CHUNK_NODE_MOTION_SOURCES = 14, // XAC_NodeMotionSources
    XAC_CHUNK_ATTACHMENT_NODES = 15,    // XAC_AttachmentNodes
    XAC_FORCE_32BIT = (int)0xFFFFFFFF
} XacChunkType;

typedef enum
{
    XAC_LAYERID_UNKNOWN = 0,           // unknown layer
    XAC_LAYERID_AMBIENT = 1,           // ambient layer
    XAC_LAYERID_DIFFUSE = 2,           // a diffuse layer
    XAC_LAYERID_SPECULAR = 3,          // specular layer
    XAC_LAYERID_OPACITY = 4,           // opacity layer
    XAC_LAYERID_BUMP = 5,              // bump layer
    XAC_LAYERID_SELFILLUM = 6,         // self illumination layer
    XAC_LAYERID_SHINE = 7,             // shininess (for specular)
    XAC_LAYERID_SHINESTRENGTH = 8,     // shine strength (for specular)
    XAC_LAYERID_FILTERCOLOR = 9,       // filter color layer
    XAC_LAYERID_REFLECT = 10,          // reflection layer
    XAC_LAYERID_REFRACT = 11,          // refraction layer
    XAC_LAYERID_ENVIRONMENT = 12,      // environment map layer
    XAC_LAYERID_DISPLACEMENT = 13,     // displacement map layer
    XAC_LAYERID_FORCE_8BIT = (int)0xFF // don't use more than 8 bit values
} XacMaterialLayerType;

typedef enum
{
    LAYERBLENDMODE_NONE = 0,        /**< The foreground texture covers up the background texture entirely. */
    LAYERBLENDMODE_OVER = 1,        /**< The foreground texture is applied like a decal to the background. The shape of the decal is determined by the foreground alpha. */
    LAYERBLENDMODE_IN = 2,          /**< The result is the background texture cut in the shape of the foreground alpha. */
    LAYERBLENDMODE_OUT = 3,         /**< The result is the opposite of In. It is as if the shape of the foreground alpha has been cut out of the background. */
    LAYERBLENDMODE_ADD = 4,         /**< The result color is the foreground color added to the background color as if being projected on the background through a slide projector. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_SUBSTRACT = 5,   /**< The result color is the foreground color subtracted from the background color. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_MULTIPLY = 6,    /**< The result color is the foreground color multiplied by the background color. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_DIFFERENCE = 7,  /**< The result color is the difference between the foreground color and the background color. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_LIGHTEN = 8,     /**< The result color of each pixel is the background color or foreground color, whichever is lighter. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_DARKEN = 9,      /**< The result color of each pixel is the background color or foreground color, whichever is darker. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
    LAYERBLENDMODE_SATURATE = 10,   /**< The result color is the background color with saturation increased in proportion to the foreground color scaled by foreground alpha. If the foreground color is red, for example, the result color will be the background color with more saturated reds. */
    LAYERBLENDMODE_DESATURATE = 11, /**< The result color is the background color with saturation decreased in proportion to the foreground color scaled by foreground alpha. If the foreground color is red, for example, the result color will be the background color with desaturated reds. */
    LAYERBLENDMODE_ILLUMINATE = 12  /**< The result color is the background color mixed with the foreground color, brighter where the foreground is bright and darker where the foreground is dark. It is as if the foreground texture represents the light falling on the background. The result color is then applied over the background color using the foreground alpha to define the opacity of the result. */
} XacBlendModesType;

typedef enum
{
    ATTRIB_POSITIONS = 0,     /**< Vertex positions. Typecast to MCore::Vector3. Positions are always exist. */
    ATTRIB_NORMALS = 1,       /**< Vertex normals. Typecast to MCore::Vector3. Normals are always exist. */
    ATTRIB_TANGENTS = 2,      /**< Vertex tangents. Typecast to <b> MCore::Vector4 </b>. */
    ATTRIB_UVCOORDS = 3,      /**< Vertex uv coordinates. Typecast to MCore::Vector2. */
    ATTRIB_COLORS32 = 4,      /**< Vertex colors in 32-bits. Typecast to uint32. */
    ATTRIB_ORGVTXNUMBERS = 5, /**< Original vertex numbers. Typecast to uint32. Original vertex numbers always exist. */
    ATTRIB_COLORS128 = 6,     /**< Vertex colors in 128-bits. Typecast to MCore::RGBAColor. */
    ATTRIB_BITANGENTS = 7     /**< Vertex bitangents (aka binormal). Typecast to MCore::Vector3. When tangents exists bitangents may still not exist! */
} XacAttributeType;

typedef enum
{
    CLONE_MATERIALS = 1 << 0,                                                                                       /**< Clones the materials itself, so do not share them. */
    CLONE_NODEATTRIBUTES = 1 << 1,                                                                                  /**< Clones the node attributes, so do not share them. */
    CLONE_CONTROLLERS = 1 << 2,                                                                                     /**< Clone (duplicate) the controllers. If not set, the controllers will NOT be copied. */
    CLONE_MESHES = 1 << 3,                                                                                          /**< Clones the mesh data, so does not share the mesh data. */
    CLONE_COLLISIONMESHES = 1 << 4,                                                                                 /**< Clones the collision mesh data, so does not share the collision mesh data. */
    CLONE_DEFAULT = (CLONE_NODEATTRIBUTES | CLONE_CONTROLLERS),                                                     /**< The default actor cloning flags, which only copies the node attributes. The rest will be shared. */
    CLONE_ALL = (CLONE_MATERIALS | CLONE_NODEATTRIBUTES | CLONE_CONTROLLERS | CLONE_MESHES | CLONE_COLLISIONMESHES) /**< The actor cloning flags to clone all attributes available. So nothing will be shared. */
} XacCLoneFlags;

typedef enum
{
    REPOSITION_POSITION = 1 << 0, /**< Update the actor position based on the repositioning node. */
    REPOSITION_ROTATION = 1 << 1, /**< Update the actor rotation based on the repositioning node. */
    REPOSITION_SCALE = 1 << 2     /**< [CURRENTLY UNSUPPORTED] Update the actor scale based on the repositioning node. */
} XacRepositioningMask;

typedef enum
{
    SCALE_ROTATION_TRANSLATION = 0, /**< LocalTM = scale * rotation * translation (Maya style). */
    ROTATION_SCALE_TRANSLATION = 1  /**< LocalTM = rotation * scale * translation (3DSMax style) [default]. */
} XacMultiplicationOrder;

typedef struct
{
    float x, y, z;
} Attribute_Vector3;

typedef struct
{
    float x, y, z, w;
} Attribute_Vector4;

typedef struct
{
    float r, g, b, a;
} Attribute_RGBAColor;

typedef struct
{
    float u, v;
} Attribute_Vector2;

typedef struct
{
    union
    {
        Attribute_Vector3 position;
        Attribute_Vector3 normal;
        Attribute_Vector4 tangent;
        Attribute_Vector2 uv;
        uint32_t color32;
        uint32_t originalVertexNumber;
        Attribute_RGBAColor color128;
        Attribute_Vector3 bitangent;
    } data;
} XacAttribute;

typedef struct
{
    int32_t value; // Beware, not unsigned, as negative values are allowed

    char *name; // Pointer to dynamically allocated string (name[])
} XAC_FXIntParameter;

typedef struct
{
    float value;

    char *name; // Pointer to dynamically allocated string (name[])
} XAC_FXFloatParameter;

typedef struct
{
    Color value;

    char *name; // Pointer to dynamically allocated string (name[])
} XAC_FXColorParameter;

typedef struct
{
    Vector3 value;

    char *name; // Pointer to dynamically allocated string (name[])
} XAC_FXVector3Parameter;

typedef struct
{
    uint8_t value; // 0 = no, 1 = yes

    char *name; // Pointer to dynamically allocated string (name[])
} XAC_FXBoolParameter;

typedef struct
{
    uint16_t num_nodes;
    uint8_t disabled_on_default; // 0 = no, 1 = yes

    char *name;             // Pointer to dynamically allocated string (name[])
    uint16_t *node_indices; // Pointer to array of uint16_t[num_nodes]
} XAC_NodeGroup;

typedef struct
{
    uint32_t fourcc;      // Must be "XAC "
    uint8_t high_version; // High version (e.g., 2 for v2.34)
    uint8_t low_version;  // Low version  (e.g., 34 for v2.34)
    uint8_t endian_type;  // Endianness of stored data [0 = little-endian, 1 = big-endian]
    uint8_t mul_order;    // See the enum MULORDER_...
} XAC_Header;

typedef struct
{
    uint32_t repositioning_mask;
    uint32_t repositioning_node_idx;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
} XAC_InfoMetadata;

typedef struct
{
    XAC_InfoMetadata xac_info_metadata;
    // followed by:
    // string : source application (e.g. "3D Studio MAX 8", "Maya 7.0")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
    char *source_application;
    char *original_filename;
    char *export_date;
    char *actor_name;
} XAC_Info;

typedef struct
{
    uint32_t repositioning_mask;
    uint32_t repositioning_node_idx;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;
} XAC_InfoV2Metadata;

typedef struct
{
    XAC_InfoV2Metadata xac_info_metadata;

    // followed by:
    // string : source application (e.g. "3D Studio MAX 8", "Maya 7.0")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
    char *source_application;
    char *original_filename;
    char *export_date;
    char *actor_name;
} XAC_InfoV2;

typedef struct
{
    uint32_t trajectory_node_idx;
    uint32_t motion_extraction_idx;
    uint32_t motion_extraction_mask;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;
} XAC_InfoV3Metadata;

typedef struct
{
    XAC_InfoV3Metadata xac_info_metadata;

    // followed by:
    // string : source application (e.g. "3D Studio MAX 8", "Maya 7.0")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
    char *source_application;
    char *original_filename;
    char *export_date;
    char *actor_name;
} XAC_InfoV3;

typedef struct
{
    uint32_t num_lods;
    uint32_t trajectory_node_idx;
    uint32_t motion_extraction_idx;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;
} XAC_InfoV4Metadata;

typedef struct
{
    XAC_InfoV4Metadata xac_info_metadata;

    // followed by:
    // string : source application (e.g. "3D Studio MAX 8", "Maya 7.0")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
    char *source_application;
    char *original_filename;
    char *export_date;
    char *actor_name;
} XAC_InfoV4;

// Transformation Node Structures
typedef struct
{
    Quaternion local_quat;  // the local rotation (before hierarchy)
    Quaternion scale_rot;   // scale rotation (space in which to perform scaling)
    Vector3 local_pos;      // the local translation (before hierarchy)
    Vector3 local_scale;    // the local scale (before hierarc
    Vector3 shear;          // shear (x=XY, y=XZ, z=YZ)
    uint32_t skeletal_lods; // each bit representing if the node is active or not, in the give LOD (bit number
    uint32_t parent_idx;    // parent node number, or 0xFFFFFFFF in case of a root node
    char *node_name;
} XAC_Node;

typedef struct
{
    Quaternion local_quat;  // the local rotation (before hierarchy)
    Quaternion scale_rot;   // scale rotation (space in which to perform scaling)
    Vector3 local_pos;      // the local translation (before hierarchy)
    Vector3 local_scale;    // the local scale (before hierarc
    Vector3 shear;          // shear (x=XY, y=XZ, z=YZ)
    uint32_t skeletal_lods; // each bit representing if the node is active or not, in the give LOD (bit number
    uint32_t parent_idx;    // parent node number, or 0xFFFFFFFF in case of a root node
    uint8_t node_flags;
    char *node_name;
} XAC_NodeV2;

typedef struct
{
    Quaternion local_quat;  // the local rotation (before hierarchy)
    Quaternion scale_rot;   // scale rotation (space in which to perform scaling)
    Vector3 local_pos;      // the local translation (before hierarchy)
    Vector3 local_scale;    // the local scale (before hierarc
    Vector3 shear;          // shear (x=XY, y=XZ, z=YZ)
    uint32_t skeletal_lods; // each bit representing if the node is active or not, in the give LOD (bit number
    uint32_t parent_idx;    // parent node number, or 0xFFFFFFFF in case of a root node
    uint8_t node_flags;
    float obb[16];
    char *node_name;
} XAC_NodeV3;

typedef struct
{
    Quaternion local_quat;
    Quaternion scale_rot;
    Vector3 local_pos;
    Vector3 local_scale;
    Vector3 shear;
    uint32_t skeletal_lods;
    uint32_t motion_lods;
    uint32_t parent_idx;
    uint32_t num_children;
    uint8_t node_flags;
    float obb[16];
    float importance_factor;
    char *node_name;
} XAC_NodeV4;

// LOD Model Storage
typedef struct
{
    uint32_t lod_level;
    uint32_t size_in_bytes;
    uint8_t *data; // Dynamically allocated binary model data
} XAC_MeshLODLevel;

typedef struct
{
    float u;
    float v;
} XAC_UV;

typedef struct
{
    uint32_t start_index;  // Index inside the SkinInfluence array
    uint32_t num_elements; // Number of influences for this entry
} XAC_SkinningInfoTableEntry;

typedef struct
{
    float weight;
    uint16_t node_nr;
} XAC_SkinInfluence;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    // for all mesh org num vertices
    //     uint8 numInfluences
    //         XAC_SkinInfluence[numInfluences];
    uint32_t *num_influences;       // Array: Number of influences per vertex
    XAC_SkinInfluence **influences; // Array of influence lists per vertex
} XAC_SkinningInfo;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint32_t num_total_influences; // Total influences across all vertices
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    //	 XAC_SkinInfluence[mNumTotalInfluences]
    //	 XAC_SkinningInfoTableEntry[mesh.GetNumOrgVerts()]
    XAC_SkinInfluence *influences;             // Influence list
    XAC_SkinningInfoTableEntry *table_entries; // Table entries per vertex
} XAC_SkinningInfoV2;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint32_t num_local_bones;      // Number of local bones used by the mesh
    uint32_t num_total_influences; // Total number of influences for all vertices
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    //	 XAC_SkinInfluence[mNumTotalInfluences]
    //	 XAC_SkinningInfoTableEntry[mesh.GetNumOrgVerts()]
    XAC_SkinInfluence *influences;             // Influence list
    XAC_SkinningInfoTableEntry *table_entries; // Table entries per vertex
} XAC_SkinningInfoV3;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint32_t lod;                  // Level of detail
    uint32_t num_local_bones;      // Number of local bones used by the mesh
    uint32_t num_total_influences; // Total number of influences for all vertices
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    //	 XAC_SkinInfluence[mNumTotalInfluences]
    //	 XAC_SkinningInfoTableEntry[mesh.GetNumOrgVerts()]
    XAC_SkinInfluence *influences;             // Influence list
    XAC_SkinningInfoTableEntry *table_entries; // Table entries per vertex
} XAC_SkinningInfoV4;

typedef struct
{
    float amount;             // Amount, between 0 and 1
    float u_offset;           // U offset (horizontal texture shift)
    float v_offset;           // V offset (vertical texture shift)
    float u_tiling;           // Horizontal tiling factor
    float v_tiling;           // Vertical tiling factor
    float rotation_radians;   // Texture rotation in radians
    uint16_t material_number; // Parent material number (0 = first material)
    uint8_t map_type;         // Map type (see enum)

    char *texture_filename; // Texture filename (null-terminated string)
} XAC_StandardMaterialLayer;

typedef struct
{
    float amount;             // Amount, between 0 and 1
    float u_offset;           // U offset (horizontal texture shift)
    float v_offset;           // V offset (vertical texture shift)
    float u_tiling;           // Horizontal tiling factor
    float v_tiling;           // Vertical tiling factor
    float rotation_radians;   // Texture rotation in radians
    uint16_t material_number; // Parent material number (0 = first material)
    uint8_t map_type;         // Map type (see enum)
    uint8_t blend_mode;       // Blend mode for combining texture layers

    char *texture_filename; // Texture filename (null-terminated string)
} XAC_StandardMaterialLayerV2;

typedef struct
{
    Color ambient;             // Ambient color
    Color diffuse;             // Diffuse color
    Color specular;            // Specular color
    Color emissive;            // Self-illumination color
    float shine;               // Shine
    float shine_strength;      // Shine strength
    float opacity;             // Opacity [1.0 = fully opaque, 0.0 = fully transparent]
    float ior;                 // Index of refraction
    uint8_t double_sided;      // Double-sided?
    uint8_t wireframe;         // Render in wireframe?
    uint8_t transparency_type; // F=filter / S=subtractive / A=additive / U=unknown

    char *material_name; // Material name (null-terminated string)
} XAC_StandardMaterial;

typedef struct
{
    Color ambient;             // Ambient color
    Color diffuse;             // Diffuse color
    Color specular;            // Specular color
    Color emissive;            // Self-illumination color
    float shine;               // Shine
    float shine_strength;      // Shine strength
    float opacity;             // Opacity [1.0 = fully opaque, 0.0 = fully transparent]
    float ior;                 // Index of refraction
    uint8_t double_sided;      // Double-sided?
    uint8_t wireframe;         // Render in wireframe?
    uint8_t transparency_type; // F=filter / S=subtractive / A=additive / U=unknown
    uint8_t num_layers;        // Number of material layers

    // followed by:
    // string : material name
    // XAC_StandardMaterialLayer2[ mNumLayers ]
    char *material_name;                 // Material name (null-terminated string)
    XAC_StandardMaterialLayerV2 *layers; // Pointer to material layers array
} XAC_StandardMaterialV2;

typedef struct
{
    uint32_t lod;              // Level of detail
    Color ambient;             // Ambient color
    Color diffuse;             // Diffuse color
    Color specular;            // Specular color
    Color emissive;            // Self-illumination color
    float shine;               // Shine
    float shine_strength;      // Shine strength
    float opacity;             // Opacity [1.0 = fully opaque, 0.0 = fully transparent]
    float ior;                 // Index of refraction
    uint8_t double_sided;      // Double-sided?
    uint8_t wireframe;         // Render in wireframe?
    uint8_t transparency_type; // F=filter / S=subtractive / A=additive / U=unknown
    uint8_t num_layers;        // Number of material layers

    char *material_name;                 // Material name (null-terminated string)
    XAC_StandardMaterialLayerV2 *layers; // Pointer to material layers array
} XAC_StandardMaterialV3;

typedef struct
{
    uint32_t layer_type_id;      // Type of vertex attribute layer
    uint32_t attrib_size_bytes;  // Size of a single vertex attribute of this type (in bytes)
    uint8_t enable_deformations; // Enable deformations on this layer?
    uint8_t is_scale;            // Is this a scale value? (coordinate system conversion)

    // followed by:
    // (sizeof(mAttribSizeInBytes) * mesh.numVertices) bytes, or mesh.numVertices mDataType objects
    void *data; // Pointer to (attrib_size_bytes * mesh.num_vertices) bytes of data
} XAC_VertexAttributeLayer;

typedef struct
{
    uint32_t num_indices;    // Number of indices
    uint32_t num_vertices;   // Number of vertices
    uint32_t material_index; // Material number (0 = first read material)
    uint32_t num_bones;      // Number of bones used by this submesh

    uint32_t *indices; // Array of indices [num_indices]
    uint32_t *bones;   // Array of bones [num_bones]
} XAC_SubMesh;

typedef struct
{
    uint32_t node_index;       // Node number this mesh belongs to (0 = first node, 1 = second, etc.)
    uint32_t num_org_verts;    // Number of original vertices
    uint32_t total_verts;      // Total number of vertices (all submeshes)
    uint32_t total_indices;    // Total number of indices (all submeshes)
    uint32_t num_sub_meshes;   // Number of submeshes
    uint32_t num_layers;       // Number of vertex attribute layers
    uint8_t is_collision_mesh; // Is this a collision mesh?

    XAC_VertexAttributeLayer *layers; // Pointer to XAC_VertexAttributeLayer[num_layers]
    XAC_SubMesh *submeshes;           // Pointer to XAC_SubMesh[num_sub_meshes]
} XAC_Mesh;

typedef struct
{
    uint32_t node_index;       // Node number this mesh belongs to (0 = first node, 1 = second, etc.)
    uint32_t lod;              // Level of detail
    uint32_t num_org_verts;    // Number of original vertices
    uint32_t total_verts;      // Total number of vertices (all submeshes)
    uint32_t total_indices;    // Total number of indices (all submeshes)
    uint32_t num_sub_meshes;   // Number of submeshes
    uint32_t num_layers;       // Number of vertex attribute layers
    uint8_t is_collision_mesh; // Is this a collision mesh?

    // Followed by:
    // XAC_VertexAttributeLayer[num_layers]
    // XAC_SubMesh[num_sub_meshes]
} XAC_MeshV2;

typedef struct
{
    Vector3 translation_min; // Minimum translation values
    Vector3 translation_max; // Maximum translation values
    Vector3 rotation_min;    // Minimum rotation values
    Vector3 rotation_max;    // Maximum rotation values
    Vector3 scale_min;       // Minimum scale values
    Vector3 scale_max;       // Maximum scale values
    uint8_t limit_flags[9];  // Limit type activation flags
    uint32_t node_number;    // Node number where this limit applies
} XAC_Limit;

typedef struct
{
    uint32_t node_index;
    float min_value;       // Min range value for x, y, z components of compressed position vectors
    float max_value;       // Max range value for x, y, z components of compressed position vectors
    uint32_t num_vertices; // Number of deltas

    ShortVector3 *delta_positions; // Pointer to array of ShortVector3[num_vertices]
    ByteVector3 *delta_normals;    // Pointer to array of ByteVector3[num_vertices]
    ByteVector3 *delta_tangents;   // Pointer to array of ByteVector3[num_vertices]
    uint32_t *vertex_numbers;      // Pointer to array of uint32_t[num_vertices]

} XAC_PMorphTargetMeshDeltas;

typedef struct
{
    uint32_t node_index;       // Node index where the transform belongs
    Quaternion rotation;       // Node rotation
    Quaternion scale_rotation; // Node delta scale rotation
    Vector3 position;          // Node delta position
    Vector3 scale;             // Node delta scale
} XAC_PMorphTargetTransform;

typedef struct
{
    float range_min;                 // Slider minimum
    float range_max;                 // Slider maximum
    uint32_t lod;                    // Level of detail for this morph target
    uint32_t num_mesh_deform_deltas; // Number of mesh deform data objects
    uint32_t num_transformations;    // Number of transformations
    uint32_t phoneme_sets;           // Number of phoneme sets

    char *progressive_morph_target_name;        // Progressive morph target name (null-terminated string)
    XAC_PMorphTargetMeshDeltas *mesh_deltas;    // Pointer to array of XAC_PMorphTargetMeshDeltas[num_mesh_deform_deltas]
    XAC_PMorphTargetTransform *transformations; // Pointer to array of XAC_PMorphTargetTransform[num_transformations]

} XAC_PMorphTarget;

typedef struct
{
    uint32_t num_morph_targets; // Number of morph targets to follow
    uint32_t lod;               // LOD level the morph targets are for

    XAC_PMorphTarget *morph_targets; // Pointer to array of XAC_PMorphTarget[num_morph_targets]

} XAC_PMorphTargets;

typedef struct
{
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bitmap_params;

    // followed by:
    // string : name
    // string : effect file (path excluded, extension included)
    // XAC_FXIntParameter   [ mNumIntParams ]
    // XAC_FXFloatParameter [ mNumFloatParams ]
    // XAC_FXColorParameter [ mNumColorParams ]
    // [mNumBitmapParams]
    //		string : param name
    //		string : value
    char *name;        // Material name (null-terminated)
    char *effect_file; // Effect file (path excluded, extension included)

    XAC_FXIntParameter *int_params;     // Pointer to array of XAC_FXIntParameter[num_int_params]
    XAC_FXFloatParameter *float_params; // Pointer to array of XAC_FXFloatParameter[num_float_params]
    XAC_FXColorParameter *color_params; // Pointer to array of XAC_FXColorParameter[num_color_params]

    struct
    {
        char *param_name; // Bitmap param name
        char *value;      // Bitmap value
    } *bitmap_params;     // Pointer to array of bitmap parameters [num_bitmap_params]

} XAC_FXMaterial;

typedef struct
{
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bool_params;
    uint32_t num_vector3_params;
    uint32_t num_bitmap_params;

    // followed by:
    // string : name
    // string : effect file (path excluded, extension included)
    // string : shader technique
    // XAC_FXIntParameter		[ mNumIntParams ]
    // XAC_FXFloatParameter		[ mNumFloatParams ]
    // XAC_FXColorParameter		[ mNumColorParams ]
    // XAC_FXBoolParameter		[ mNumBoolParams ]
    // XAC_FXVector3Parameter	[ mNumVector3Params ]
    // [mNumBitmapParams]
    //		string : param name
    //		string : value
    char *name;
    char *effect_file;
    char *shader_technique;

    XAC_FXIntParameter *int_params;
    XAC_FXFloatParameter *float_params;
    XAC_FXColorParameter *color_params;
    XAC_FXBoolParameter *bool_params;
    XAC_FXVector3Parameter *vector3_params;

    struct
    {
        char *param_name;
        char *value;
    } *bitmap_params;

} XAC_FXMaterialV2;

typedef struct
{
    uint32_t lod; // Level of detail
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bool_params;
    uint32_t num_vector3_params;
    uint32_t num_bitmap_params;

    // followed by:
    // string : name
    // string : effect file (path excluded, extension included)
    // string : shader technique
    // XAC_FXIntParameter		[ mNumIntParams ]
    // XAC_FXFloatParameter		[ mNumFloatParams ]
    // XAC_FXColorParameter		[ mNumColorParams ]
    // XAC_FXBoolParameter		[ mNumBoolParams ]
    // XAC_FXVector3Parameter	[ mNumVector3Params ]
    // [mNumBitmapParams]
    //		string : param name
    //		string : value
    char *name;
    char *effect_file;
    char *shader_technique;

    XAC_FXIntParameter *int_params;
    XAC_FXFloatParameter *float_params;
    XAC_FXColorParameter *color_params;
    XAC_FXBoolParameter *bool_params;
    XAC_FXVector3Parameter *vector3_params;

    struct
    {
        char *param_name;
        char *value;
    } *bitmap_params;

} XAC_FXMaterialV3;

typedef struct
{
    uint32_t num_nodes;
    uint32_t num_root_nodes;

    union
    {
        XAC_Node version_1;
        XAC_NodeV2 version_2;
        XAC_NodeV3 version_3;
        XAC_NodeV4 version_4;
    } *nodes; // Pointer to array of XAC_NodeVx[num_nodes]
} XAC_Nodes;

typedef struct
{
    uint32_t num_total_materials;    // Total number of materials to follow (including default/extra materials)
    uint32_t num_standard_materials; // Number of standard materials in the file
    uint32_t num_fx_materials;       // Number of FX materials in the file
} XAC_MaterialInfo;

typedef struct
{
    uint32_t lod;                    // Level of detail
    uint32_t num_total_materials;    // Total number of materials to follow (including default/extra materials)
    uint32_t num_standard_materials; // Number of standard materials in the file
    uint32_t num_fx_materials;       // Number of FX materials in the file
} XAC_MaterialInfoV2;

typedef struct
{
    uint32_t num_nodes; // Number of nodes

    uint16_t *node_indices; // Pointer to array of uint16_t[num_nodes], indicating the index of the node to extract motion data from if mirroring is enabled.
} XAC_NodeMotionSources;

typedef struct
{
    uint32_t num_nodes; // Number of attachment nodes

    uint16_t *attachment_indices; // Pointer to array of uint16_t[num_nodes], index per attachment node.
} XAC_AttachmentNodes;

typedef struct
{
    XAC_Header header;
    uint32_t chunk_data_size;
    ChunkData chunk_data[CHUNK_DATA_SIZE];

    union
    {
        XAC_Node version_1;
        XAC_NodeV2 version_2;
        XAC_NodeV3 version_3;
        XAC_NodeV4 version_4;
    } xac_node;

    union
    {
        XAC_Mesh version_1;
        XAC_MeshV2 version_2;
    } xac_mesh;
    union
    {
        XAC_Info version_1;
        XAC_InfoV2 version_2;
        XAC_InfoV3 version_3;
        XAC_InfoV4 version_4;
    } xac_info;

    union
    {
        XAC_Nodes version_1;
    } xac_nodes;

    union
    {
        XAC_NodeGroup version_1;
    } xac_node_groups;

    union
    {
        XAC_SkinningInfo version_1;
        XAC_SkinningInfoV2 version_2;
        XAC_SkinningInfoV3 version_3;
        XAC_SkinningInfoV4 version_4;
    } xac_skinning_info;

    union
    {
        XAC_MeshLODLevel version_1;
    } xac_mesh_lod_level;

    union
    {
        XAC_PMorphTarget version_1;
    } xac_std_morph_target;

    union
    {
        XAC_PMorphTargets version_1;
    } xac_std_morph_targets;

    union
    {
        XAC_StandardMaterial version_1;
        XAC_StandardMaterialV2 version_2;
    } xac_standard_material;

    union
    {
        XAC_StandardMaterialLayer version_1;
        XAC_StandardMaterialLayerV2 version_2;
    } xac_standard_material_layer;

    union
    {
        XAC_FXMaterial version_1;
        XAC_FXMaterialV2 version_2;
        XAC_FXMaterialV3 version_3;
    } xac_fx_material;

    union
    {
        XAC_Limit version_1;
    } xac_limit;

    union
    {
        XAC_MaterialInfo version_1;
        XAC_MaterialInfoV2 version_2;
    } xac_material_info;

    union
    {
        XAC_NodeMotionSources version_1;
    } xac_node_motion_sources;

    union
    {
        XAC_AttachmentNodes version_1;
    } xac_attachment_nodes;

} XAC_Root;

int parse_xac_node(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_mesh(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_nodes(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_node_groups(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_skinning_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_mesh_lod_level(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_std_morph_target(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_std_morph_targets(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_standard_material(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_standard_material_layer(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_fx_material(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_limit(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_material_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_node_motion_sources(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);
int parse_xac_attachment_nodes(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version);

size_t skip_buffer(const uint8_t **buffer, size_t size, const uint8_t *buffer_end);
size_t read_from_buffer(const uint8_t **buffer, void *dest, size_t size, const uint8_t *buffer_end);
int read_string(const uint8_t **buffer, const uint8_t *buffer_end, char **result_text);
int parse_xac_root(XAC_Root *root, const uint8_t *buffer, size_t buffer_size);

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

int parse_xac_node(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    if (*buffer + chunk_data->size_in_bytes > buffer_end)
    {
        printf("Error: Buffer overflow detected!\n");
        return -1;
    }

    switch (version)
    {
    case 1:
        read_from_buffer(buffer, &root->xac_info.version_1.xac_info_metadata, sizeof(XAC_InfoMetadata), buffer_end);
        skip_buffer(buffer, 2, buffer_end);

        read_string(buffer, buffer_end, &root->xac_info.version_1.source_application);
        read_string(buffer, buffer_end, &root->xac_info.version_1.original_filename);
        read_string(buffer, buffer_end, &root->xac_info.version_1.export_date);
        read_string(buffer, buffer_end, &root->xac_info.version_1.actor_name);

        break;

    case 2:
        read_from_buffer(buffer, &root->xac_info.version_2.xac_info_metadata, sizeof(XAC_InfoV2Metadata), buffer_end);
        // skip_buffer(buffer, 2, buffer_end);

        read_string(buffer, buffer_end, &root->xac_info.version_2.source_application);
        read_string(buffer, buffer_end, &root->xac_info.version_2.original_filename);
        read_string(buffer, buffer_end, &root->xac_info.version_2.export_date);
        read_string(buffer, buffer_end, &root->xac_info.version_2.actor_name);

        break;

    case 3:
        read_from_buffer(buffer, &root->xac_info.version_3, sizeof(XAC_InfoV3Metadata), buffer_end);
        skip_buffer(buffer, 2, buffer_end);

        read_string(buffer, buffer_end, &root->xac_info.version_3.source_application);
        read_string(buffer, buffer_end, &root->xac_info.version_3.original_filename);
        read_string(buffer, buffer_end, &root->xac_info.version_3.export_date);
        read_string(buffer, buffer_end, &root->xac_info.version_3.actor_name);

        break;

    case 4:
        read_from_buffer(buffer, &root->xac_info.version_4.xac_info_metadata, sizeof(XAC_InfoV4Metadata), buffer_end);
        skip_buffer(buffer, 2, buffer_end);

        read_string(buffer, buffer_end, &root->xac_info.version_4.source_application);
        read_string(buffer, buffer_end, &root->xac_info.version_4.original_filename);
        read_string(buffer, buffer_end, &root->xac_info.version_4.export_date);
        read_string(buffer, buffer_end, &root->xac_info.version_4.actor_name);
        break;

    default:
        printf("Error: Unsupported version %u\n", version);
        return -1;
    }
    return 0;
}

int parse_xac_mesh(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_nodes(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_node_groups(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_skinning_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_mesh_lod_level(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_std_morph_target(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_std_morph_targets(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_standard_material(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_standard_material_layer(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_fx_material(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_limit(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_material_info(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_node_motion_sources(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_attachment_nodes(XAC_Root *root, const uint8_t **buffer, const ChunkData *chunk_data, const uint8_t *buffer_end, uint32_t version)
{
    (void)root;    // Mark unused parameter
    (void)version; // Mark unused parameter

    return skip_buffer(buffer, chunk_data->size_in_bytes, buffer_end) == chunk_data->size_in_bytes ? 0 : -1;
}

int parse_xac_root(XAC_Root *root, const uint8_t *buffer, size_t buffer_size)
{
    const uint8_t *buffer_end = buffer + buffer_size;

    // Read the header
    if (read_from_buffer(&buffer, &root->header, sizeof(XAC_Header), buffer_end) != sizeof(XAC_Header))
    {
        return -1; // Error: Not enough data
    }

    printf("\nMagic: 0x%08X, Version: %u.%u\n",
           root->header.fourcc, root->header.high_version, root->header.low_version);

    // Process chunks until we reach the end of the buffer
    while (buffer + sizeof(ChunkData) <= buffer_end)
    {
        ChunkData chunk_data;

        // Read chunk data
        if (read_from_buffer(&buffer, &chunk_data, sizeof(ChunkData), buffer_end) != sizeof(ChunkData))
        {
            return -1; // Error: Not enough data
        }

        printf("Chunk ID: %u, Version: %u, Size: %u bytes\n",
               chunk_data.chunk_id, chunk_data.version, chunk_data.size_in_bytes);

        if (buffer + chunk_data.size_in_bytes > buffer_end)
        {
            printf("Error: Chunk size exceeds buffer limits.\n");
            return -1;
        }

        const uint8_t *after_chunk = buffer + chunk_data.size_in_bytes;

        switch ((XacChunkType)chunk_data.chunk_id)
        {

        case XAC_CHUNK_NODE:
        {
            if (parse_xac_node(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_MESH:
        {
            if (parse_xac_mesh(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_SKINNING_INFO:
        {
            if (parse_xac_skinning_info(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_STANDARD_MATERIAL:
        {
            if (parse_xac_standard_material(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_STANDARD_MATERIAL_LAYER:
        {
            if (parse_xac_standard_material_layer(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_FX_MATERIAL:
        {
            if (parse_xac_fx_material(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_LIMIT:
        {
            if (parse_xac_limit(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_INFO:
        {
            if (parse_xac_info(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_MESH_LOD_LEVEL:
        {
            if (parse_xac_mesh_lod_level(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_STD_MORPH_TARGET:
        {
            if (parse_xac_std_morph_target(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_NODE_GROUPS:
        {
            if (parse_xac_node_groups(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_NODES:
        {
            if (parse_xac_nodes(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_STD_MORPH_TARGETS:
        {
            if (parse_xac_std_morph_targets(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_MATERIAL_INFO:
        {
            if (parse_xac_material_info(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_NODE_MOTION_SOURCES:
        {
            if (parse_xac_node_motion_sources(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        case XAC_CHUNK_ATTACHMENT_NODES:
        {
            if (parse_xac_attachment_nodes(root, &buffer, &chunk_data, buffer_end, chunk_data.version) != 0)
            {
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }

        default:
        {
            printf("Skipping unknown chunk type: %u\n", chunk_data.chunk_id);
            if (skip_buffer(&buffer, chunk_data.size_in_bytes, buffer_end) != chunk_data.size_in_bytes)
            {
                printf("Error: Failed to skip buffer.\n");
                return -1;
            }
            root->chunk_data[root->chunk_data_size] = chunk_data;
            root->chunk_data_size += 1;
            break;
        }
        }

        if (buffer != after_chunk)
        {
            printf("Error: Buffer position mismatch! Expected %p, but got %p\n",
                   (void *)after_chunk, (void *)buffer);
            return -1;
        }
    }

    printf("Chunk data size : %d\n", root->chunk_data_size);
    return 0;
}

#endif // XAC_H
