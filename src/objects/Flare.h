//
// Created by Mika412 on 23.10.19.
//

#ifndef AVT7_FLARE_H
#define AVT7_FLARE_H

#include "../GameManager.h"
#include "../flare.h"
//#include "flare.h"


class Flare {

    bool enabled = true;
    //GLint lPos_uniformId;
    float lightPos[4] = {0.25f, 1.25f, -5.75f, 3.14f}; //position of point light in World coordinates
    //Flare effect
    float lightScreenPos[3];  //Position of the light in Window Coordinates
    int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

public:

    Flare(Vector3 pos){
        changePosition(pos);
    }


    void changePosition(Vector3 pos){
        lightPos[0] = pos.x;
        lightPos[1] = pos.y;
        lightPos[2] = pos.z;
    }

    void render(FLARE_DEF AVTflare, GLuint *FlareTextureArray, GLint lPos_uniformId)  {
        if(!enabled)
            return;
        int flarePos[2];
        int m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);
        float eyeLightPos[4];  //position of light in eye coordinates
        multMatrixPoint(VIEW, lightPos, eyeLightPos);
        glUniform4fv(lPos_uniformId, 1, eyeLightPos); //sending the position of point light in eye coordinates
        if(!project(eyeLightPos, lightScreenPos, m_viewport))
            printf("Error in getting projected light in screen\n");  //Calculate the window Coordinates of the light position: the projected position of light on viewport
        flarePos[0] = clampi((int)lightScreenPos[0], m_viewport[0], m_viewport[0] + m_viewport[2] - 1);
        flarePos[1] = clampi((int)lightScreenPos[1], m_viewport[1], m_viewport[1] + m_viewport[3] - 1);

        //viewer looking down at  negative z direction
        pushMatrix(PROJECTION);
        loadIdentity(PROJECTION);
        pushMatrix(VIEW);
        loadIdentity(VIEW);
        ortho(m_viewport[0], m_viewport[0] + m_viewport[2] - 1, m_viewport[1], m_viewport[1] + m_viewport[3] - 1, -1, 1);
        render_flare(&AVTflare, flarePos[0], flarePos[1], m_viewport, FlareTextureArray);
        popMatrix(PROJECTION);
        popMatrix(VIEW);
    }

    void setEnabled(bool state){
        this->enabled = state;
    }

    void render_flare(FLARE_DEF *flare, int lx, int ly, int *m_viewport, GLuint *FlareTextureArray) {  //lx, ly represent the projected position of light on viewport

        int     dx, dy;          // Screen coordinates of "destination"
        int     px, py;          // Screen coordinates of flare element
        int		cx, cy;
        float    maxflaredist, flaredist, flaremaxsize, flarescale, scaleDistance;
        int     width, height, alpha;    // Piece parameters;
        int     i;
        float	diffuse[4];

        GLint loc;

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        objId = 6;

        int screenMaxCoordX = m_viewport[0] + m_viewport[2] - 1;
        int screenMaxCoordY = m_viewport[1] + m_viewport[3] - 1;

        //viewport center
        cx = m_viewport[0] + (int)(0.5f * (float)m_viewport[2]) - 1;
        cy = m_viewport[1] + (int)(0.5f * (float)m_viewport[3]) - 1;

        // Compute how far off-center the flare source is.
        maxflaredist = sqrt(cx*cx + cy * cy);
        flaredist = sqrt((lx - cx)*(lx - cx) + (ly - cy)*(ly - cy));
        scaleDistance = (maxflaredist - flaredist) / maxflaredist;
        flaremaxsize = (int)(m_viewport[2] * flare->fMaxSize);
        flarescale = (int)(m_viewport[2] * flare->fScale);

        // Destination is opposite side of centre from source
        dx = clampi(cx + (cx - lx), m_viewport[0], screenMaxCoordX);
        dy = clampi(cy + (cy - ly), m_viewport[1], screenMaxCoordY);

        // Render each element. To be used Texture Unit 0

        glUniform1i(texMode_uniformId, 5); // draw modulated textured particles

        for (i = 0; i < flare->nPieces; ++i)
        {
            // Position is interpolated along line between start and destination.
            px = (int)((1.0f - flare->element[i].fDistance)*lx + flare->element[i].fDistance*dx);
            py = (int)((1.0f - flare->element[i].fDistance)*ly + flare->element[i].fDistance*dy);
            px = clampi(px, m_viewport[0], screenMaxCoordX);
            py = clampi(py, m_viewport[1], screenMaxCoordY);

            // Piece size are 0 to 1; flare size is proportion of screen width; scale by flaredist/maxflaredist.
            width = (int)(scaleDistance*flarescale*flare->element[i].fSize);

            // Width gets clamped, to allows the off-axis flaresto keep a good size without letting the elements get big when centered.
            if (width > flaremaxsize)  width = flaremaxsize;

            height = (int)((float)m_viewport[3] / (float)m_viewport[2] * (float)width);
            memcpy(diffuse, flare->element[i].matDiffuse, 4 * sizeof(float));
            diffuse[3] *= scaleDistance;   //scale the alpha channel

            if (width > 1)
            {
                // send the material - diffuse color modulated with texture
                loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
                glUniform4fv(loc, 1, diffuse);

                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, FlareTextureArray[flare->element[i].textureId]);
                pushMatrix(MODEL);
                translate(MODEL, (float)(px - width * 0.0f), (float)(py - height * 0.0f), 0.0f);
                scale(MODEL, (float)width, (float)height, 1);
                computeDerivedMatrix(PROJ_VIEW_MODEL);
                glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
                glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
                computeNormalMatrix3x3();
                glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

                glBindVertexArray(mesh[objId].vao);
                glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
                popMatrix(MODEL);
            }
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    inline double clamp(const double x, const double min, const double max) {
        return (x < min ? min : (x > max ? max : x));
    }

    inline int clampi(const int x, const int min, const int max) {
        return (x < min ? min : (x > max ? max : x));
    }

    unsigned int getTextureId(char *name) {
        int i;

        for (i = 0; i < NTEXTURES; ++i)
        {
            if (strncmp(name, flareTextureNames[i], strlen(name)) == 0)
                return i;
        }
        return -1;
    }
    void    loadFlareFile(FLARE_DEF *flare, char *filename)
    {
        int     n = 0;
        FILE    *f;
        char    buf[256];
        int fields;

        memset(flare, 0, sizeof(FLARE_DEF));

        f = fopen(filename, "r");
        if (f)
        {
            fgets(buf, sizeof(buf), f);
            sscanf(buf, "%f %f", &flare->fScale, &flare->fMaxSize);

            while (!feof(f))
            {
                char            name[8] = { '\0', };
                double          dDist = 0.0, dSize = 0.0;
                float			color[4];
                int				id;

                fgets(buf, sizeof(buf), f);
                fields = sscanf(buf, "%4s %lf %lf ( %f %f %f %f )", name, &dDist, &dSize, &color[3], &color[0], &color[1], &color[2]);
                if (fields == 7)
                {
                    for (int i = 0; i < 4; ++i) color[i] = clamp(color[i] / 255.0f, 0.0f, 1.0f);
                    id = getTextureId(name);
                    if (id < 0) printf("Texture name not recognized\n");
                    else
                        flare->element[n].textureId = id;
                    flare->element[n].fDistance = (float)dDist;
                    flare->element[n].fSize = (float)dSize;
                    memcpy(flare->element[n].matDiffuse, color, 4 * sizeof(float));
                    ++n;
                }
            }

            flare->nPieces = n;
            fclose(f);
        }
        else printf("Flare file opening error\n");
    }


};

/**/

#endif //AVT7_FLARE_H
