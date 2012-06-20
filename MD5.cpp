#include "MD5.h"
MD5::MD5(void)
{
    count = 0;
    numJoints = 0;
    numMeshes = 0;
    numTris = 0;
    numVerts = 0;
}

MD5::~MD5(void)
{
    glDeleteProgram(shaderProgram);
    for(int i = 0; i < numMeshes; ++i)
    {
        delete meshList[i].vertex;
        delete meshList[i].tri;
        delete meshList[i].weight;
    }

    delete meshList;
    delete jointList;
}

void MD5::prepModel(void)
{
    // Prepare Buffer Objects
    int offset = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*24*numVerts, NULL, GL_STATIC_DRAW);

    // Generate Vertex Buffer Object
    for(int i = 0; i < numMeshes; ++i)
    {
        for(int j = 0; j < meshList[i].numVerts; ++j)
        {
            // Add UV coords
            glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec2), glm::value_ptr(meshList[i].vertex[j].uv));

            // Add vertex weights
            glm::vec4 bias(0.0f);
            glm::mat4 pos(0.0f);
            int index = meshList[i].vertex[j].weightIndex;
            for(int k = 0; k < meshList[i].vertex[j].weightElem; ++k)
            {
                bias[k] = meshList[i].weight[index+k].bias;
                glm::vec4 posi(0);
                posi.x = meshList[i].weight[index+k].position.x;
                posi.y = meshList[i].weight[index+k].position.y;
                posi.z = meshList[i].weight[index+k].position.z;
                posi.w = meshList[i].weight[index+k].jointIndex;
                pos[k] = posi;
            }

            glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec2), sizeof(glm::vec4), glm::value_ptr(bias));
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec2) + sizeof(glm::vec4), sizeof(glm::mat4), glm::value_ptr(pos));

            offset += sizeof(float)*24;
        }
    }

    offset = 0;

    // Generate Index Buffer Object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*3*numTris, NULL, GL_STATIC_DRAW);

    for(int i = 0; i < numMeshes; ++i)
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(GLushort)*3*meshList[i].numTris, meshList[i].tri);
        offset += meshList[i].numTris;
    }

    GLuint uvLoc = glGetAttribLocation(shaderProgram, "uv");
    GLuint wBias = glGetAttribLocation(shaderProgram, "wBias");
    GLuint wPos = glGetAttribLocation(shaderProgram, "wPos");

    glBindAttribLocation(shaderProgram, uvLoc, "uv");
    glBindAttribLocation(shaderProgram, wBias, "wBias");
    glBindAttribLocation(shaderProgram, wPos, "wPos");
}

bool MD5::load(const char* filename)
{
    FILE* input;
	char lineBuffer[256];
	int version = 0;
	int currentMesh = 0;

	numJoints = 0;
	numMeshes = 0;

	input = fopen(filename, "r");

	if(0 == input)
	{
		puts("Cannot open file");
		return false;
	}

	while(fgets(lineBuffer, sizeof(lineBuffer), input) != 0 && !feof(input))
	{
		char* nl = strchr(lineBuffer, '\n');
		if(nl)
			*nl = '\0';

		if(sscanf(lineBuffer, "MD5Version %d", &version) == 1)
		{
			if(version != 10)
			{
				puts("Incorrect version");
				return -1;
			}
		}
		else if(sscanf(lineBuffer, "numJoints %d", &numJoints) == 1)
		{
			if(numJoints > 0)
			{
				jointList = new Joint[numJoints]();
			}
		}
		else if(sscanf(lineBuffer, "numMeshes %d", &numMeshes) == 1)
		{
			if(numMeshes > 0)
			{
				meshList = new Mesh[numMeshes]();
			}
		}
		// Load joints for entire mesh cluster model
		else if(strncmp(lineBuffer, "joints {", 8) == 0)
		{
			for(int i = 0; i < numJoints; ++i)
			{
				fgets(lineBuffer, sizeof(lineBuffer), input);
				if(sscanf(lineBuffer, "\t\"%[^\"]\"\t%d ( %f %f %f ) ( %f %f %f )", jointList[i].name, &jointList[i].parentIndex,
                    &jointList[i].position.x, &jointList[i].position.y, &jointList[i].position.z,
                    &jointList[i].orientation.x, &jointList[i].orientation.y, &jointList[i].orientation.z) == 8)
				{
					float w = 1.0f - (jointList[i].orientation.x * jointList[i].orientation.x)
						- (jointList[i].orientation.y * jointList[i].orientation.y)
						- (jointList[i].orientation.z * jointList[i].orientation.z);

					if(w < 0.0f)
						jointList[i].orientation.w = 0.0f;
					else
						jointList[i].orientation.w = -sqrt(w);
				}
			}
		}
		// Load singular mesh data
		else if(strncmp(lineBuffer, "mesh {", 6) == 0)
		{
			int vertIndex = 0;
			int triIndex = 0;
			int weightIndex = 0;
			float fdata[4];
			int idata[3];

			while((lineBuffer[0] != '}') && !feof(input))
			{
				fgets(lineBuffer, sizeof(lineBuffer), input);

				if(sscanf(lineBuffer, "\tshader \"%[^\"]\"", meshList[currentMesh].material) == 1)
				{
				    // Load mesh texture
					;
				}
				else if(sscanf(lineBuffer, "\tnumverts %d", &meshList[currentMesh].numVerts) == 1)
				{
					if(meshList[currentMesh].numVerts > 0)
					{
						meshList[currentMesh].vertex = new Vertex[meshList[currentMesh].numVerts]();
						numVerts += meshList[currentMesh].numVerts;
					}
				}
				else if(sscanf(lineBuffer, "\tnumtris %d", &meshList[currentMesh].numTris) == 1)
				{
					if(meshList[currentMesh].numTris > 0)
					{
						meshList[currentMesh].tri = new Tri[meshList[currentMesh].numTris]();
						numTris += meshList[currentMesh].numTris;
					}
				}
				else if(sscanf(lineBuffer, "\tnumweights %d", &meshList[currentMesh].numWeights) == 1)
				{
					if(meshList[currentMesh].numWeights > 0)
					{
						meshList[currentMesh].weight = new Weight[meshList[currentMesh].numWeights]();
					}
				}
				else if(sscanf(lineBuffer, "\tvert %d ( %f %f ) %d %d",
					&vertIndex, &fdata[0], &fdata[1], &idata[0], &idata[1])
					== 5)
				{
					meshList[currentMesh].vertex[vertIndex].uv.s = fdata[0];
					meshList[currentMesh].vertex[vertIndex].uv.t = fdata[1];
					meshList[currentMesh].vertex[vertIndex].weightIndex = idata[0];
					meshList[currentMesh].vertex[vertIndex].weightElem = idata[1];
				}
				else if(sscanf(lineBuffer, "\ttri %d %d %d %d", &triIndex,
					&idata[0], &idata[1], &idata[2]) == 4)
				{
					meshList[currentMesh].tri[triIndex].vert1 = idata[0];
					meshList[currentMesh].tri[triIndex].vert2 = idata[1];
					meshList[currentMesh].tri[triIndex].vert3 = idata[2];
				}
				else if(sscanf(lineBuffer, "\tweight %d %d %f ( %f %f %f)",
					&weightIndex, &idata[0], &fdata[0],
					&fdata[1], &fdata[2], &fdata[3]) == 6)
				{
					meshList[currentMesh].weight[weightIndex].jointIndex = idata[0];
					meshList[currentMesh].weight[weightIndex].bias = fdata[0];
					meshList[currentMesh].weight[weightIndex].position.x = fdata[1];
					meshList[currentMesh].weight[weightIndex].position.y = fdata[2];
					meshList[currentMesh].weight[weightIndex].position.z = fdata[3];
				}
			}
			++currentMesh;
		}
	}

	fclose(input);
	return true;
}

bool MD5::loadShader(const char* vshader, const char* fshader)
{
    GLint vsSize;
    GLint fsSize;
    char* vsBuffer;
    char* fsBuffer;

    // Load Vertex Shader
    FILE* vs = fopen(vshader, "r");
    if(NULL == vs)
        return false;
    fseek(vs, 0, SEEK_END);
    vsSize = ftell(vs);
    rewind(vs);
    vsBuffer = new char[vsSize];
    if(0 == vsBuffer)
        return false;
    if(vsSize != fread(vsBuffer, 1, vsSize, vs))
        return false;
    fclose(vs);

    // Load Fragment Shader
    FILE* fs = fopen(fshader, "r");
    if(NULL == fs)
        return false;
    fseek(fs, 0, SEEK_END);
    fsSize = ftell(fs);
    rewind(fs);
    fsBuffer = new char[fsSize];
    if(0 == fsBuffer)
        return false;
    if(fsSize != fread(fsBuffer, 1, fsSize, fs))
        return false;

    // Create Vertex and Fragment Shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertShader, 1, (const GLchar**)&vsBuffer, &vsSize);
    glShaderSource(fragShader, 1, (const GLchar**)&fsBuffer, &fsSize);

    //Compile Shaders
    GLint compiled;
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    if(compiled == false)
    {
        printf("Vert Shader failedto compile\n");
        glDeleteShader(vertShader);
        vertShader = 0;
        glDeleteShader(fragShader);
        fragShader = 0;

        return false;
    }
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    if(compiled == false)
    {
        printf("Frag Shader failedto compile\n");
        glDeleteShader(vertShader);
        vertShader = 0;
        glDeleteShader(fragShader);
        fragShader = 0;

        return false;
    }

    // Create and link GLSL program
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    GLint linked;
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if(linked == false)
    {
        printf("Program linking failed\n");
        glDetachShader(shaderProgram, vertShader);
        glDetachShader(shaderProgram, fragShader);
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        vertShader = 0;
        fragShader = 0;
        shaderProgram = 0;

        return false;
    }

    // Clean up
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    delete vsBuffer;
    delete fsBuffer;

    return true;
}

void MD5::save(const char* filename)
{

}

void render(glm::mat4 mvp)
{

}
