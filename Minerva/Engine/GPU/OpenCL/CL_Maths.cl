typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
	float x;
	float y;
} Vector2;
typedef struct
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
} MColor;

typedef struct {
    float e11, e12, e13, e14;
    float e21, e22, e23, e24;
    float e31, e32, e33, e34;
    float e41, e42, e43, e44;
} Matrix4x4;

Vector3 normalize(Vector3 v)
{
    Vector3 result = v;
    float d = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    result.x = result.x / d;
    result.y = result.y / d;
    result.z = result.z / d;
    return result;
}

float4 TransformVectorWithMatrix3(Vector3 v, Matrix4x4 m)
{    
    float4 result;
    result.x =  v.x * m.e11 + v.y * m.e12 + v.z * m.e13 + m.e14;
    result.y =  v.x * m.e21 + v.y * m.e22 + v.z * m.e23 + m.e24;
    result.z =  v.x * m.e31 + v.y * m.e32 + v.z * m.e33 + m.e34;
    result.w =  v.x * m.e41 + v.y * m.e42 + v.z * m.e43 + m.e44;
    return result;
}
float4 TransformVectorWithMatrix4(float4 v, Matrix4x4  m)
{    
    float4 result;
    result.x =  v.x * m.e11 + v.y * m.e12 + v.z * m.e13 + v.w *m.e14;
    result.y =  v.x * m.e21 + v.y * m.e22 + v.z * m.e23 +  v.w *m.e24;
    result.z =  v.x * m.e31 + v.y * m.e32 + v.z * m.e33 +  v.w *m.e34;
    result.w =  v.x * m.e41 + v.y * m.e42 + v.z * m.e43 +  v.w *m.e44;
    return result;
}