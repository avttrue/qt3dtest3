#version 330

vec3 normals(vec3 pos)
{
    vec3 fdx = dFdx(pos);
    vec3 fdy = dFdy(pos);
    return normalize(cross(fdx, fdy));
}

in vec3 vViewPos;
out vec4 fragColor;

void main()
{
    vec3 normal = normals(vViewPos);
    vec3 color = vec3(0.9, 0.9, 0.9);
    float theta = dot(normal, vec3(0, 0, 1)) / length(normal);
    fragColor = vec4(color * theta , 0.9);
}
