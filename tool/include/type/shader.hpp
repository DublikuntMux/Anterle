#pragma once

#include <glslang/Public/ShaderLang.h>

bool compileGLSLToSPIRV(const char *glslSource, const char *outputFilename, EShLanguage shaderType);

TBuiltInResource InitResources();
