__kernel void TransformVertices(
    __global const Vector3* vectors,
    __global const Vector3* normals,
    Matrix4x4 modelMatrix, Matrix4x4 vpMatrix, Matrix4x4 rotationMatrix, int width, int height,
    __global Vector3* results,
    __global Vector3* transformedNormals) 
{
    int id = get_global_id(0);
    Vector3 v = vectors[id];

    float4 worldPos = TransformVectorWithMatrix3(v, modelMatrix);
    float4 clipPos = TransformVectorWithMatrix4(worldPos, vpMatrix);

    clipPos.x = (-(clipPos.x / clipPos.w) + 1) * 0.5f * (width);
    clipPos.y = (-(clipPos.y / clipPos.w) + 1) * 0.5f * (height);
    clipPos.z = clipPos.z / clipPos.w;

    Vector3 screenPos;
    screenPos.x = clipPos.x;
    screenPos.y =  clipPos.y;
    screenPos.z = clipPos.z;

    results[id] = screenPos;

    float4 n = TransformVectorWithMatrix3(normals[id], rotationMatrix);
    //n = TransformVectorWithMatrix4(n, vpMatrix2);
    n = n / n.w;
    Vector3 tn;
    tn.x = n.x;
    tn.y = n.y ;
    tn.z = n.z;
    transformedNormals[id] = normalize(tn);
}