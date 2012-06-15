#include "MD5.h"

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
				jointList = (struct Joint*)calloc(numJoints, sizeof(struct Joint));
			}
		}
		else if(sscanf(lineBuffer, "numMeshes %d", &numMeshes) == 1)
		{
			if(numMeshes > 0)
			{
				meshList = (struct Mesh*)calloc(numMeshes, sizeof(struct Mesh));
			}
		}
		else if(strncmp(lineBuffer, "joints {", 8) == 0)
		{
			for(int i = 0; i < numJoints; ++i)
			{
				fgets(lineBuffer, sizeof(lineBuffer), input);
				if(sscanf(lineBuffer, "\t\"%[^\"]\"\t%d ( %f %f %f ) ( %f %f %f )", jointList[i].name, &jointList[i].parentIndex, &jointList[i].x, &jointList[i].y, &jointList[i].z, &jointList[i].ox, &jointList[i].oy, &jointList[i].oz) == 8)
				{
					float w = 1.0f - (jointList[i].ox * jointList[i].ox)
						- (jointList[i].oy * jointList[i].oy)
						- (jointList[i].oz * jointList[i].oz);

					if(w < 0.0f)
						jointList[i].ow = 0.0f;
					else
						jointList[i].ow = -sqrt(w);
				}
			}
		}
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
					;
				}
				else if(sscanf(lineBuffer, "\tnumverts %d", &meshList[currentMesh].numVerts) == 1)
				{
					if(meshList[currentMesh].numVerts > 0)
					{
						meshList[currentMesh].vertex = (struct Vertex*) calloc(meshList[currentMesh].numVerts, sizeof(struct Vertex));
					}
				}
				else if(sscanf(lineBuffer, "\tnumtris %d", &meshList[currentMesh].numTris) == 1)
				{
					if(meshList[currentMesh].numTris > 0)
					{
						meshList[currentMesh].tri = (struct Tri*) calloc(meshList[currentMesh].numTris, sizeof(struct Tri));
					}
				}
				else if(sscanf(lineBuffer, "\tnumweights %d", &meshList[currentMesh].numWeights) == 1)
				{
					if(meshList[currentMesh].numWeights > 0)
					{
						meshList[currentMesh].weight = (struct Weight*) calloc(meshList[currentMesh].numWeights, sizeof(struct Weight));
					}
				}
				else if(sscanf(lineBuffer, "\tvert %d ( %f %f ) %d %d",
					&vertIndex, &fdata[0], &fdata[1], &idata[0], &idata[1])
					== 5)
				{
					meshList[currentMesh].vertex[vertIndex].u = fdata[0];
					meshList[currentMesh].vertex[vertIndex].v = fdata[1];
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
					meshList[currentMesh].weight[weightIndex].x = fdata[1];
					meshList[currentMesh].weight[weightIndex].y = fdata[2];
					meshList[currentMesh].weight[weightIndex].z = fdata[3];
				}
			}
			++currentMesh;
		}
	}

	fclose(input);
	return true;
}

void MD5::save(const char* filename)
{

}

void render(glm::mat4 mvp)
{

}