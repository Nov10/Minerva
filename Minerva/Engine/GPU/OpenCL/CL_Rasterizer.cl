inline void swap_vector3(Vector3* v1, Vector3* v2)
{
    Vector3 temp = *v1;
    *v1 = *v2;
    *v2 = temp;
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

    if(0 > (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x))
    {
        return;
    }

    if (p1.y > p2.y)
    {
        swap_vector3(&p1, &p2);
    }
    if (p2.y > p3.y)
    {
        swap_vector3(&p2, &p3); 
    }
    if (p1.y > p2.y)
    {       
        swap_vector3(&p1, &p2); 
    }
    int tileCountY = (int)((float)height / tileSize);
    int tileCountX = (int)((float)width / tileSize);

    int tileStartY =  (int)(fmax(0.0f, floor(p1.y / tileSize)));
    int tileEndY =    (int)(fmin(tileCountY, ceil(p3.y / tileSize)));
    int tileMiddleY = (int)fmin(tileEndY, fmax(tileStartY, floor(p2.y / tileSize)));


    //수학적으로 아래 6줄은 같은 코드임
    //float inv_a_1_2 = 1.0f / ((p2.y - p1.y) / (p2.x - p1.x));
    //float inv_a_1_3 = 1.0f / ((p3.y - p1.y) / (p3.x - p1.x));
    //float inv_a_2_3 = 1.0f / ((p3.y - p2.y) / (p3.x - p2.x)); 
    float inv_a_1_2t = ((p2.x - p1.x) / (p2.y - p1.y)) * tileSize;
    float inv_a_1_3t = ((p3.x - p1.x) / (p3.y - p1.y)) * tileSize;
    float inv_a_2_3t = ((p3.x - p2.x) / (p3.y - p2.y)) * tileSize;

    float x1 = (tileStartY * tileSize - p1.y) * inv_a_1_2t/tileSize + p1.x - inv_a_1_2t;
    float x2 = (tileStartY * tileSize - p1.y) * inv_a_1_3t/tileSize + p1.x - inv_a_1_3t;
    float x1p = x1 + inv_a_1_2t;
    float x2p = x2 + inv_a_1_3t;


    for (int ty = tileStartY; ty < tileMiddleY; ty++)
    {
        x1 += inv_a_1_2t;
        x2 += inv_a_1_3t;
        
        x1p += inv_a_1_2t;
        x2p += inv_a_1_3t;

        float min = fmin(fmin(x1, x2), fmin(x1p, x2p));
        float max = fmax(fmax(x1, x2), fmax(x1p, x2p));
        int txs = (int)fmax(0.0f, floor(min / tileSize));
        int txe = (int)fmin(tileCountX, ceil(max / tileSize));
        for (int tx = txs; tx <= txe; tx++)
        {
            int tileIndex = ty * tileCountX + tx;
            //if(0 <= tileIndex && tileIndex < tileCountY * tileCountX)
            {
                int writeIndex = atomic_add(&(triangleCountPerTile[tileIndex]), 1);

                if (writeIndex < maxTriangleCountPerTile)
                {
                    triangleIndicesPerTile[tileIndex * maxTriangleCountPerTile + writeIndex] = id;
                }
            }            
        }
    }

    x1 = (tileMiddleY * tileSize - p3.y) * inv_a_1_3t/tileSize + p3.x - inv_a_1_3t;
    x2 = (tileMiddleY * tileSize - p3.y) * inv_a_2_3t/tileSize + p3.x - inv_a_2_3t;
    x1p = x1 + inv_a_1_3t;
    x2p = x2 + inv_a_2_3t;
    for (int ty = tileMiddleY; ty <= tileEndY; ty++)
    {
        x1 += inv_a_1_3t;
        x2 += inv_a_2_3t;
        
        x1p += inv_a_1_3t;
        x2p += inv_a_2_3t;

        float min = fmin(fmin(x1, x2), fmin(x1p, x2p));
        float max = fmax(fmax(x1, x2), fmax(x1p, x2p));

        int txs = (int)fmax(0.0f, floor(min / tileSize));
        int txe = (int)fmin(tileCountX, ceil(max / tileSize));

        for (int tx = txs; tx <= txe; tx++)
        {
            int tileIndex = ty * tileCountX + tx;
            //if(0 <= tileIndex && tileIndex < tileCountY * tileCountX)
            {
                int writeIndex = atomic_add(&(triangleCountPerTile[tileIndex]), 1);

                if (writeIndex < maxTriangleCountPerTile)
                {
                    triangleIndicesPerTile[tileIndex * maxTriangleCountPerTile + writeIndex] = id;
                }
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

    int triangleCount = (int)min(triangleCountPerTile[idx], maxTriangleCountPerTile);

    Vector3 P;
    int record;
    float area;
    float z;
    Vector3 n;
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            P.x = x;
            P.y = y;
            for (int i = 0; i < triangleCount; i++)
            {
                record = triangleIndicesPerTile[idx * maxTriangleCountPerTile + i];
                Vector3 p1 = vertices[triangles[record * 3 + 0]];
                Vector3 p2 = vertices[triangles[record * 3 + 1]];
                Vector3 p3 = vertices[triangles[record * 3 + 2]];

               float lambda1 = EdgeFunction(p2, p3, P);
                if (lambda1 > 0) continue;

               float lambda2 = EdgeFunction(p3, p1, P);
                if (lambda2 > 0) continue;

               float lambda3 = EdgeFunction(p1, p2, P);
                if (lambda3 > 0) continue;

                area = lambda1 + lambda2 + lambda3;
                lambda1 = lambda1 / area;
                lambda2 = lambda2 / area;
                lambda3 = lambda3 / area;

                z = p1.z *lambda1 + p2.z*lambda2 + p3.z*lambda3;
                if(z < zBuffer[x + y*width])
                {
                    zBuffer[x + y*width] = z;
                    Vector3 n1 = normals[triangles[record * 3 + 0]];
                    Vector3 n2 = normals[triangles[record * 3 + 1]];
                    Vector3 n3 = normals[triangles[record * 3 + 2]];

                    
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