#pragma once

#include <box3d/box3d.h>

int b3InternalAssert(const char *condition, const char *fileName, int lineNumber);
void *createDebugShape(const b3DebugShape *debugShape, void *context);
void destroyDebugShape(void *userShape, void *context);
bool drawShape(void *userShape, b3WorldTransform transform, b3HexColor color, void *context);

void initLineRenderer(struct App *app);
