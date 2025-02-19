#if !defined(XAC_H)
#define XAC_H
#include "shared_formats.h"

typedef enum
{
    XAC_CHUNK_NODE = 0,
    XAC_CHUNK_MESH = 1,
    XAC_CHUNK_SKINNINGINFO = 2,
    XAC_CHUNK_STDMATERIAL = 3,
    XAC_CHUNK_STDMATERIALLAYER = 4,
    XAC_CHUNK_FXMATERIAL = 5,
    XAC_CHUNK_LIMIT = 6,
    XAC_CHUNK_INFO = 7,
    XAC_CHUNK_MESHLODLEVELS = 8,
    XAC_CHUNK_STDPROGMORPHTARGET = 9,
    XAC_CHUNK_NODEGROUPS = 10,
    XAC_CHUNK_NODES = 11,             // XAC_Nodes
    XAC_CHUNK_STDPMORPHTARGETS = 12,  // XAC_PMorphTargets
    XAC_CHUNK_MATERIALINFO = 13,      // XAC_MaterialInfo
    XAC_CHUNK_NODEMOTIONSOURCES = 14, // XAC_NodeMotionSources
    XAC_CHUNK_ATTACHMENTNODES = 15,   // XAC_AttachmentNodes
    XAC_FORCE_32BIT = 0xFFFFFFFF
} XacChunkType;

typedef enum
{
    XAC_LAYERID_UNKNOWN = 0,       // unknown layer
    XAC_LAYERID_AMBIENT = 1,       // ambient layer
    XAC_LAYERID_DIFFUSE = 2,       // a diffuse layer
    XAC_LAYERID_SPECULAR = 3,      // specular layer
    XAC_LAYERID_OPACITY = 4,       // opacity layer
    XAC_LAYERID_BUMP = 5,          // bump layer
    XAC_LAYERID_SELFILLUM = 6,     // self illumination layer
    XAC_LAYERID_SHINE = 7,         // shininess (for specular)
    XAC_LAYERID_SHINESTRENGTH = 8, // shine strength (for specular)
    XAC_LAYERID_FILTERCOLOR = 9,   // filter color layer
    XAC_LAYERID_REFLECT = 10,      // reflection layer
    XAC_LAYERID_REFRACT = 11,      // refraction layer
    XAC_LAYERID_ENVIRONMENT = 12,  // environment map layer
    XAC_LAYERID_DISPLACEMENT = 13, // displacement map layer
    XAC_LAYERID_FORCE_8BIT = 0xFF  // don't use more than 8 bit values
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
    uint8_t fourcc[IDENTIFIER_LEN]; // Must be "XAC "
    uint8_t high_version;           // High version (e.g., 2 for v2.34)
    uint8_t low_version;            // Low version  (e.g., 34 for v2.34)
    uint8_t endian_type;            // Endianness of stored data [0 = little-endian, 1 = big-endian]
    uint8_t mul_order;              // See the enum MULORDER_...
} XAC_Header;

typedef struct
{
    uint32_t repositioning_mask;     // Describes transformation components to adjust when repositioning is enabled
    uint32_t repositioning_node_idx; // Node number
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;

    // Followed by:
    // string : Source application (e.g., "3ds Max 8", "Maya 7.0")
    // string : Original filename of the 3dsMax/Maya file
    // string : Compilation date of the exporter
    // string : Actor name
} XAC_Info;

typedef struct
{
    uint32_t repositioning_mask;
    uint32_t repositioning_node_idx;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;

    // followed by:
    // string : source application (e.g. "3D Studio MAX 8", "Maya 7.0")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
} XAC_Info2;

typedef struct
{
    uint32_t trajectory_node_idx;    // Node number of trajectory node used for motion extraction
    uint32_t motion_extraction_idx;  // Node number of trajectory node used for motion extraction
    uint32_t motion_extraction_mask; // Motion extraction mask for transformation adjustments
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;

    // followed by:
    // string : source application (e.g. "3ds Max 2011", "Maya 2011")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
} XAC_Info3;

typedef struct
{
    uint32_t num_lods; // Number of Levels of Detail (LOD)
    uint32_t trajectory_node_idx;
    uint32_t motion_extraction_idx;
    uint8_t exporter_high_version;
    uint8_t exporter_low_version;
    float retarget_root_offset;

    // followed by:
    // string : source application (e.g. "3ds Max 2011", "Maya 2011")
    // string : original filename of the 3dsMax/Maya file
    // string : compilation date of the exporter
    // string : the name of the actor
} XAC_Info4;

typedef struct
{
    Quaternion local_quat;  // Local rotation (before hierarchy)
    Quaternion scale_rot;   // Scale rotation (space in which to perform scaling)
    Vector3 local_pos;      // Local translation (before hierarchy)
    Vector3 local_scale;    // Local scale (before hierarchy)
    Vector3 shear;          // Shear (x=XY, y=XZ, z=YZ)
    uint32_t skeletal_lods; // Each bit represents whether the node is active in a given LOD
    uint32_t parent_idx;    // Parent node number, or 0xFFFFFFFF if root node

    // followed by:
    // string : node name (the unique name of the node)
} XAC_Node;

typedef struct
{
    Quaternion local_quat;
    Quaternion scale_rot;
    Vector3 local_pos;
    Vector3 local_scale;
    Vector3 shear;
    uint32_t skeletal_lods;
    uint32_t parent_idx;
    uint8_t node_flags; // Bit #1 specifies whether to include this node in bounds calculation

    // followed by:
    // string : node name (the unique name of the node)
} XAC_Node2;

typedef struct
{
    Quaternion local_quat;
    Quaternion scale_rot;
    Vector3 local_pos;
    Vector3 local_scale;
    Vector3 shear;
    uint32_t skeletal_lods;
    uint32_t parent_idx;
    uint8_t node_flags;
    float obb[16]; // Oriented Bounding Box (OBB)

    // followed by:
    // string : node name (the unique name of the node)
} XAC_Node3;

typedef struct
{
    Quaternion local_quat;
    Quaternion scale_rot;
    Vector3 local_pos;
    Vector3 local_scale;
    Vector3 shear;
    uint32_t skeletal_lods;
    uint32_t motion_lods; // Each bit represents whether the node is active in a given LOD
    uint32_t parent_idx;
    uint32_t num_children; // Number of child nodes
    uint8_t node_flags;
    float obb[16];
    float importance_factor; // Used for automatic motion LOD

    // followed by:
    // string : node name (the unique name of the node)
} XAC_Node4;

typedef struct
{
    uint32_t lod_level;
    uint32_t size_in_bytes;
    // Followed by:
    // uint8_t data[size_in_bytes]; // LOD model memory file
} XAC_MeshLODLevel;

typedef struct
{
    float u;
    float v;
} XAC_UV;

typedef struct
{
    uint32_t node_index;
    uint8_t is_for_collision_mesh;

    // followed by:
    // for all mesh org num vertices
    //     uint8 numInfluences
    //         XAC_SkinInfluence[numInfluences];
} XAC_SkinningInfo;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint32_t num_total_influences; // Total influences across all vertices
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    //	 XAC_SkinInfluence[mNumTotalInfluences]
    //	 XAC_SkinningInfoTableEntry[mesh.GetNumOrgVerts()]
} XAC_SkinningInfo2;

typedef struct
{
    uint32_t node_index;           // Node number in the actor
    uint32_t num_local_bones;      // Number of local bones used by the mesh
    uint32_t num_total_influences; // Total number of influences for all vertices
    uint8_t is_for_collision_mesh; // Is it for a collision mesh?

    // followed by:
    //	 XAC_SkinInfluence[mNumTotalInfluences]
    //	 XAC_SkinningInfoTableEntry[mesh.GetNumOrgVerts()]
} XAC_SkinningInfo3;

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
} XAC_SkinningInfo4;

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

    // followed by:
    // string : material name
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

    // Followed by:
    // char material_name[];                  // Material name (null-terminated string)
    // XAC_StandardMaterialLayer2[num_layers]; // Material layers
} XAC_StandardMaterial2;

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

    // Followed by:
    // char material_name[];                  // Material name (null-terminated string)
    // XAC_StandardMaterialLayer2[num_layers]; // Material layers
} XAC_StandardMaterial3;

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

    // Followed by:
    // char texture_filename[]; // Texture filename (null-terminated string)
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
    // Followed by:
    // char texture_filename[]; // Texture filename (null-terminated string)
} XAC_StandardMaterialLayer2;

typedef struct
{
    uint32_t layer_type_id;      // Type of vertex attribute layer
    uint32_t attrib_size_bytes;  // Size of a single vertex attribute of this type (in bytes)
    uint8_t enable_deformations; // Enable deformations on this layer?
    uint8_t is_scale;            // Is this a scale value? (coordinate system conversion)

    // Followed by:
    // (attrib_size_bytes * mesh.num_vertices) bytes, or mesh.num_vertices DataType objects
} XAC_VertexAttributeLayer;

typedef struct
{
    uint32_t num_indices;    // Number of indices
    uint32_t num_vertices;   // Number of vertices
    uint32_t material_index; // Material number (0 = first read material)
    uint32_t num_bones;      // Number of bones used by this submesh

    // Followed by:
    // uint32_t[num_indices]
    // uint32_t[num_bones]
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

    // Followed by:
    // XAC_VertexAttributeLayer[num_layers]
    // XAC_SubMesh[num_sub_meshes]
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
} XAC_Mesh2;

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
    float range_min;                 // Slider minimum
    float range_max;                 // Slider maximum
    uint32_t lod;                    // Level of detail for this morph target
    uint32_t num_mesh_deform_deltas; // Number of mesh deform data objects
    uint32_t num_transformations;    // Number of transformations
    uint32_t phoneme_sets;           // Number of phoneme sets

    // Followed by:
    // char progressive_morph_target_name[];    // Progressive morph target name (null-terminated string)
    // XAC_PMorphTargetMeshDeltas[num_mesh_deform_deltas]
    // XAC_PMorphTargetTransform[num_transformations]
} XAC_PMorphTarget;

typedef struct
{
    uint32_t num_morph_targets; // Number of morph targets to follow
    uint32_t lod;               // LOD level the morph targets are for

    // Followed by:
    // XAC_PMorphTarget[num_morph_targets]
} XAC_PMorphTargets;

typedef struct
{
    uint32_t node_index;
    float min_value;       // Min range value for x, y, z components of compressed position vectors
    float max_value;       // Max range value for x, y, z components of compressed position vectors
    uint32_t num_vertices; // Number of deltas

    // Followed by:
    // File16BitVector3[num_vertices]  (delta position values)
    // File8BitVector3[num_vertices]   (delta normal values)
    // File8BitVector3[num_vertices]   (delta tangent values)
    // uint32_t[num_vertices]          (vertex numbers)
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
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bitmap_params;

    // Followed by:
    // char name[];                     // Material name (null-terminated)
    // char effect_file[];               // Effect file (path excluded, extension included)
    // XAC_FXIntParameter[num_int_params]
    // XAC_FXFloatParameter[num_float_params]
    // XAC_FXColorParameter[num_color_params]
    // [num_bitmap_params]
    //     char param_name[];  // Bitmap param name
    //     char value[];       // Bitmap value
} XAC_FXMaterial;

typedef struct
{
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bool_params;
    uint32_t num_vector3_params;
    uint32_t num_bitmap_params;

    // Followed by:
    // char name[];
    // char effect_file[];
    // char shader_technique[];
    // XAC_FXIntParameter[num_int_params]
    // XAC_FXFloatParameter[num_float_params]
    // XAC_FXColorParameter[num_color_params]
    // XAC_FXBoolParameter[num_bool_params]
    // XAC_FXVector3Parameter[num_vector3_params]
    // [num_bitmap_params]
    //     char param_name[];
    //     char value[];
} XAC_FXMaterial2;

typedef struct
{
    uint32_t lod; // Level of detail
    uint32_t num_int_params;
    uint32_t num_float_params;
    uint32_t num_color_params;
    uint32_t num_bool_params;
    uint32_t num_vector3_params;
    uint32_t num_bitmap_params;

    // Followed by:
    // char name[];
    // char effect_file[];
    // char shader_technique[];
    // XAC_FXIntParameter[num_int_params]
    // XAC_FXFloatParameter[num_float_params]
    // XAC_FXColorParameter[num_color_params]
    // XAC_FXBoolParameter[num_bool_params]
    // XAC_FXVector3Parameter[num_vector3_params]
    // [num_bitmap_params]
    //     char param_name[];
    //     char value[];
} XAC_FXMaterial3;

typedef struct
{
    int32_t value; // Beware, not unsigned, as negative values are allowed

    // Followed by:
    // char name[];
} XAC_FXIntParameter;

typedef struct
{
    float value;

    // Followed by:
    // char name[];
} XAC_FXFloatParameter;

typedef struct
{
    Color value;

    // Followed by:
    // char name[];
} XAC_FXColorParameter;

typedef struct
{
    Vector3 value;

    // Followed by:
    // char name[];
} XAC_FXVector3Parameter;

typedef struct
{
    uint8_t value; // 0 = no, 1 = yes

    // Followed by:
    // char name[];
} XAC_FXBoolParameter;

typedef struct
{
    uint16_t num_nodes;
    uint8_t disabled_on_default; // 0 = no, 1 = yes

    // Followed by:
    // char name[];
    // uint16_t[num_nodes];
} XAC_NodeGroup;

typedef struct
{
    uint32_t num_nodes;
    uint32_t num_root_nodes;

    // Followed by:
    // XAC_Node4[num_nodes];
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
} XAC_MaterialInfo2;

typedef struct
{
    uint32_t num_nodes;

    // Followed by:
    // uint16_t[num_nodes]; // Index per node, indicating the index of the node to extract motion data from if mirroring is enabled.
} XAC_NodeMotionSources;

typedef struct
{
    uint32_t num_nodes;

    // Followed by:
    // uint16_t[num_nodes]; // Index per attachment node.
} XAC_AttachmentNodes;

#endif // XAC_H
