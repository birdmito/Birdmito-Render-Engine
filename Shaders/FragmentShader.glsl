#version 330 core
//out
//-------------------------------------------------
out vec4 FragColor;

//in
//-------------------------------------------------
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//Struct
//-------------------------------------------------
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float glossy;
};

//Light
struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 spotDirection;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//Function
//-------------------------------------------------
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

//Uniform Variables
//-------------------------------------------------
//Material
uniform Material material;

//Light
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

//Light Attributes
uniform vec3 viewPos;

//Main
//-------------------------------------------------
void main()
{
    //Attributes
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);
    
    //Dircetion Light
    vec3 result = calculateDirLight(dirLight, normal, viewDirection);
    
    //Point Light
    result += calculatePointLight(pointLight, normal, FragPos, viewDirection);

    //Spot Light
    result += calculateSpotLight(spotLight, normal, FragPos, viewDirection);
    
    FragColor = vec4(result, 1.0f);
}

//Function Body
//-------------------------------------------------
//Direction Light
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDirection){
    //Light Direction
    //------------
    vec3 lightDirection = normalize(-light.direction);
    
    //ambient
    //------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)).rgb;
    
    //diffuse
    //------------
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * max(dot(normal, lightDirection), 0.0);

    //specular(Blinn-Phong Shading)
    //------------
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * pow(max(dot(normal, halfDirection), 0.0), material.glossy);
    
    //final = ambient + diffuse + specular
    //------------
    return vec3(ambient + diffuse + specular);
}

//Point light
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection){
    //Attenuation
    //------------
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    //Light Direction
    //------------
    vec3 lightDirection = normalize(light.position - fragPos);
    
    //ambient
    //------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)).rgb;
    
    //diffuse
    //------------
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * max(dot(normal, lightDirection), 0.0);
    
    //specular(Blinn-Phong Shading)
    //------------
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * pow(max(dot(normal, halfDirection), 0.0), material.glossy);
    
    //final = attenuation * (ambient + diffuse + specular)
    //------------
    return vec3(attenuation * (ambient + diffuse + specular));
}

//Spot light
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection){
    //Light Direction
    //------------
    vec3 lightDirection = normalize(light.position - fragPos);
    
    //Attenuation
    //-------------
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    //Spot light intensity
    //-------------
    float theta = dot(normalize(-light.spotDirection), lightDirection);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //ambient
    //------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)).rgb;
    
    //diffuse
    //------------
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * max(dot(normal, lightDirection), 0.0);
    
    //specular(Blinn-Phong Shading)
    //------------
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * pow(max(dot(normal, halfDirection), 0.0), material.glossy);
    
    //final = attenuation * intensity * (ambient + diffuse + specular)
    //------------
    return vec3(attenuation * intensity * (ambient + diffuse + specular));
}
