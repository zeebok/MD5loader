#version 120

attribute vec2 uv;
attribute vec4 wBias;
attribute mat4 wPos;

uniform vec4 jOrt[72];
uniform vec3 jPos[72];

uniform mat4 mvpMatrix;

vec3 calcOrt(vec4 j, vec3 w)
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
        if(jID != -1)
        {
            finalPos += (jPos[jID] + calcOrt(jOrt[jID], wPos[i].xyz)) * wBias [i];
        }
        else
        {
            break;
        }
    }

    gl_Position = mvpMatrix * vec4(finalPos, 1.0);
}
