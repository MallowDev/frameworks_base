/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rsContext.h"
#include "rsProgramRaster.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

using namespace android;
using namespace android::renderscript;


ProgramRaster::ProgramRaster(Context *rsc,
                             Element *in,
                             Element *out,
                             bool pointSmooth,
                             bool lineSmooth,
                             bool pointSprite) :
    Program(rsc, in, out)
{
    mPointSmooth = pointSmooth;
    mLineSmooth = lineSmooth;
    mPointSprite = pointSprite;

    mPointSize = 1.0f;
    mLineWidth = 1.0f;
}

ProgramRaster::~ProgramRaster()
{
}

void ProgramRaster::setLineWidth(float s)
{
    mLineWidth = s;
}

void ProgramRaster::setPointSize(float s)
{
    mPointSize = s;
}

void ProgramRaster::setupGL(const Context *rsc, ProgramRasterState *state)
{
    if (state->mLast.get() == this) {
        return;
    }
    state->mLast.set(this);

    glPointSize(mPointSize);
    if (mPointSmooth) {
        glEnable(GL_POINT_SMOOTH);
    } else {
        glDisable(GL_POINT_SMOOTH);
    }

    glLineWidth(mLineWidth);
    if (mLineSmooth) {
        glEnable(GL_LINE_SMOOTH);
    } else {
        glEnable(GL_LINE_SMOOTH);
    }

    if (rsc->checkVersion1_1()) {
        if (mPointSprite) {
            glEnable(GL_POINT_SPRITE_OES);
        } else {
            glDisable(GL_POINT_SPRITE_OES);
        }
    }
}



ProgramRasterState::ProgramRasterState()
{
}

ProgramRasterState::~ProgramRasterState()
{
}

void ProgramRasterState::init(Context *rsc, int32_t w, int32_t h)
{
    ProgramRaster *pr = new ProgramRaster(rsc, NULL, NULL, false, false, false);
    mDefault.set(pr);
}


namespace android {
namespace renderscript {

RsProgramRaster rsi_ProgramRasterCreate(Context * rsc, RsElement in, RsElement out,
                                      bool pointSmooth,
                                      bool lineSmooth,
                                      bool pointSprite)
{
    ProgramRaster *pr = new ProgramRaster(rsc,
                                          static_cast<Element *>(in),
                                          static_cast<Element *>(out),
                                          pointSmooth,
                                          lineSmooth,
                                          pointSprite);
    pr->incUserRef();
    return pr;
}

void rsi_ProgramRasterSetPointSize(Context * rsc, RsProgramRaster vpr, float s)
{
    ProgramRaster *pr = static_cast<ProgramRaster *>(vpr);
    pr->setPointSize(s);
}

void rsi_ProgramRasterSetLineWidth(Context * rsc, RsProgramRaster vpr, float s)
{
    ProgramRaster *pr = static_cast<ProgramRaster *>(vpr);
    pr->setLineWidth(s);
}


}
}

