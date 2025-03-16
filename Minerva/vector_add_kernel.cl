__kernel void transform_vector(
    __global const Vector3* vectors,
    __global const Vector3* normals,
    Matrix4x4 modelMatrix, Matrix4x4 vpMatrix, Matrix4x4 rotationMatrix, Matrix4x4 vpMatrix2, int width, int height,
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

__kernel void CalculateCacheTrianglesPerTile(
    __global int* triangleIndicesPerTile,
    __global int* triangleCountPerTile,
    __global const Vector3* vertices,
    __global const int* triangles,
    int width, int height,
    int tileSize, int maxTriangleCountPerTile) 
{
    int id = get_global_id(0);

    
    Vector3 p1 = vertices[triangles[id * 3 + 0]];
    Vector3 p2 = vertices[triangles[id * 3 + 1]];
    Vector3 p3 = vertices[triangles[id * 3 + 2]];

    Vector3 e1;
    e1.x = p2.x - p1.x;
    e1.y = p2.y - p1.y;

    Vector3 e2;
    e2.x = p3.x - p1.x;
    e2.y = p3.y - p1.y;

    if(0 > e1.x * e2.y - e1.y * e2.x)
    {
        return;
    }

    int tileStartX = (int)fmax(0.0f, floor(fmin(p1.x, fmin(p2.x, p3.x)) / tileSize));
    int tileStartY = (int)fmax(0.0f, floor(fmin(p1.y, fmin(p2.y, p3.y)) / tileSize));
    int tileEndX = (int)fmin( (float)width / tileSize, ceil(fmax(p1.x, fmax(p2.x, p3.x)) / tileSize));
    int tileEndY = (int)fmin( (float)width / tileSize, ceil(fmax(p1.y, fmax(p2.y, p3.y)) / tileSize));

    for (int ty = tileStartY; ty < tileEndY; ty++)
    {
        for (int tx = tileStartX; tx < tileEndX; tx++)
        {
            int tileIndex = ty * (width / tileSize) + tx;

            int oldVal = atom_add(&(triangleCountPerTile[tileIndex]), 1);
            if (oldVal < maxTriangleCountPerTile)
            {
                triangleIndicesPerTile[tileIndex * maxTriangleCountPerTile + oldVal] = id;
            }
        }
    }
}
float EdgeFunction(Vector3 a, Vector3 b, Vector3 c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
__kernel void CalculateRastersPerTile(
    __global int* triangleIndicesPerTile,
    __global volatile  int* triangleCountPerTile,
    __global const Vector3* vertices,
    __global const Vector3* normals,
    __global const int* triangles,
    __global MColor*  frameBuffer,
    __global float* zBuffer,
    int width, int height,
    int tileSize, int maxTriangleCountPerTile,
    Vector3 light) 
{
    int idx = get_global_id(0);


    int tilesX = width / tileSize;   // 타일의 가로 개수
                                        // idx -> tileX, tileY 계산
    int tileY = idx / tilesX;
    int tileX = idx % tilesX;

    // 실제 픽셀 범위
    int startX = tileX * tileSize;
    int startY = tileY * tileSize;
    int endX = min(startX + tileSize, width);
    int endY = min(startY + tileSize, height);

    int triangleCount = triangleCountPerTile[idx];

    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Vector3 n1;
    Vector3 n2;
    Vector3 n3;

    Vector3 P;

    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            P.x = x;
            P.y = y;
            // 타일에 속한 삼각형들
            for (int i = 0; i < triangleCount; i++)
            {
                int record = triangleIndicesPerTile[idx * maxTriangleCountPerTile + i];
                p1 = vertices[triangles[record * 3 + 0]];
                p2 = vertices[triangles[record * 3 + 1]];
                p3 = vertices[triangles[record * 3 + 2]];

                float lambda1 = EdgeFunction(p2, p3, P);
                if (lambda1 > 0) continue;

                float lambda2 = EdgeFunction(p3, p1, P);
                if (lambda2 > 0) continue;

                float lambda3 = EdgeFunction(p1, p2, P);
                if (lambda3 > 0) continue;

                float area = lambda1 + lambda2 + lambda3;
                lambda1 = lambda1 / area;
                lambda2 = lambda2 / area;
                lambda3 = lambda3 / area;

                float z = p1.z *lambda1 + p2.z*lambda2 + p3.z*lambda3;
                if(z > zBuffer[x + y*width])
                {
                    zBuffer[x + y*width] = z;
                    n1 = normals[triangles[record * 3 + 0]];
                    n2 = normals[triangles[record * 3 + 1]];
                    n3 = normals[triangles[record * 3 + 2]];

                    Vector3 n;
                    n.x = n1.x *lambda1 + n2.x*lambda2 + n3.x*lambda3;
                    n.y = n1.y *lambda1 + n2.y*lambda2 + n3.y*lambda3;
                    n.z = n1.z *lambda1 + n2.z*lambda2 + n3.z*lambda3;
                    //n = normalize(n);


                    float s = (n.x *light.x + n.y*light.y + n.z*light.z);
                    s = s *0.5f +0.5f;
                    int ndl = (int)(s*255);
                    ndl = max(0, min(255, ndl));

                    (frameBuffer[x  + y * width]).R = ndl;
                    (frameBuffer[x  + y * width]).G = ndl;
                    (frameBuffer[x  + y * width]).B = ndl;
                }
            }
        }
    }
}