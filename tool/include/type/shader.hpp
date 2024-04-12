#pragma once

#include <string>

#include <glslang/Public/ShaderLang.h>

bool compileGLSLToSPIRV(const std::string &glslSource, const std::string &outputFilename, EShLanguage shaderType);

TBuiltInResource InitResources();
