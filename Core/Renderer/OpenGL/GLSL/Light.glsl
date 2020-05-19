#ifndef ALIT_LIGHT_GLSL
#define ALIT_LIGHT_GLSL

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define AMBIENT_LIGHT 3
#define POLYGONAL_LIGHT 4


struct Light {
    uint type;

    vec3 color;
    vec3 position;
    vec3 direction;
    float cosInnerAngle;
    float cosOuterAngle;
};

vec3 lightVector(Light l, vec3 position) {
    switch(l.type) {
    case POINT_LIGHT:
        return normalize(l.position - position);
    case SPOT_LIGHT:
        return normalize(l.position - position);
    case DIRECTIONAL_LIGHT:
        return -l.direction;
    default:
        return vec3(0.f);
    }
}

float distanceAttenuation(float dist) {
    return 1 / (dist * dist);
}

float radialAttenuation(const Light light, const vec3 lightVector) {
    float cosLightDirAngle = dot(lightVector, -light.direction);

    return smoothstep(light.cosOuterAngle, light.cosInnerAngle, cosLightDirAngle);
}

float attenuation(Light l, vec3 lightVector, vec3 position) {
    switch(l.type) {
    case POINT_LIGHT:
        return distanceAttenuation(distance(position, l.position));
    case SPOT_LIGHT:
        return distanceAttenuation(distance(position, l.position)) * radialAttenuation(l, lightVector);
    default:
        return 1.f;
    }
}

#endif