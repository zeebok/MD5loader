#version 330

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 wBias;
layout(location = 2) in mat4 wPos;

layout(std140) uniform JointBlock
{
    vec3 position[42];
    vec4 orientation[42];
} joints;

uniform mat4 mvpMatrix;

vec3 calcOrt(vec4  j, vec3 w)
{
    float dp = -dot(j.xyz, w);
    vec3 conj = -1.0 * j.xyz;
    vec3 intVec = (j.w * w) + cross(j.xyz, w);

    return (j.w * intVec) + (dp * conj) + cross(intVec, conj);
}

void main(void)
{
    vec3 finalPos = vec3(0, 0, 0);

    for(int i = 0; i < 4; ++i)
    {
        int jID = int(wPos[i].w);
        if(jID > -1)
        {
            finalPos += (joints.position[jID] + calcOrt(joints.orientation[jID], wPos[i].xyz)) * wBias[i];
        }
        else
        {
            break;
        }
    }

    gl_Position = mvpMatrix * vec4(finalPos, 1.0);
}