#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <glad/gl.h>

#define MSDFGL_VERSION "@msdfgl_VERSION@"


typedef struct _msdfgl_context *msdfgl_context_t;
typedef struct _msdfgl_font *msdfgl_font_t;
typedef struct _msdfgl_atlas *msdfgl_atlas_t;


msdfgl_context_t msdfgl_create_context(const char *version);

void msdfgl_destroy_context(msdfgl_context_t ctx);

typedef struct _msdfgl_glyph
{
  GLfloat x;
  GLfloat y;
  GLuint color;

  GLint key;
  GLfloat size;
  GLfloat offset;
  GLfloat skew;
  GLfloat strength;

} msdfgl_glyph_t;

msdfgl_atlas_t msdfgl_create_atlas(msdfgl_context_t ctx, int texture_width, int padding);

void msdfgl_destroy_atlas(msdfgl_atlas_t atlas);

msdfgl_font_t
  msdfgl_load_font(msdfgl_context_t ctx, const char *font_name, float range, float scale, msdfgl_atlas_t atlas);

msdfgl_font_t msdfgl_load_font_mem(msdfgl_context_t ctx,
  void *font_buffer,
  size_t font_buffer_size,
  float range,
  float scale,
  msdfgl_atlas_t atlas);

float msdfgl_vertical_advance(msdfgl_font_t font, float size);

void msdfgl_destroy_font(msdfgl_font_t font);

int msdfgl_generate_glyph(msdfgl_font_t font, int32_t char_code, void *_user);

int msdfgl_generate_glyphs(msdfgl_font_t font, int32_t start, int32_t end);

int msdfgl_generate_glyph_list(msdfgl_font_t font, int32_t *list, size_t n);

#define msdfgl_generate_ascii(font) msdfgl_generate_glyphs(font, 0, 127)
#define msdfgl_generate_ascii_ext(font) msdfgl_generate_glyphs(font, 0, 255)


void msdfgl_render(msdfgl_font_t font, msdfgl_glyph_t *glyphs, int n, GLfloat *projection);

enum msdfgl_printf_flags {
  MSDFGL_KERNING = 0x01,
  MSDFGL_WCHAR = 0x02,
  MSDFGL_VERTICAL = 0x04,
  MSDFGL_UTF8 = 0x08,
};

float msdfgl_printf(float x,
  float y,
  msdfgl_font_t font,
  float size,
  int32_t color,
  GLfloat *projection,
  enum msdfgl_printf_flags flags,
  const void *fmt,
  ...);

void msdfgl_geometry(float *x,
  float *y,
  msdfgl_font_t font,
  float size,
  enum msdfgl_printf_flags flags,
  const void *fmt,
  ...);

void msdfgl_set_missing_glyph_callback(msdfgl_context_t, int (*)(msdfgl_font_t, int32_t, void *), void *);

void msdfgl_set_dpi(msdfgl_context_t context, float horizontal, float vertical);

void _msdfgl_ortho(GLfloat left,
  GLfloat right,
  GLfloat bottom,
  GLfloat top,
  GLfloat nearVal,
  GLfloat farVal,
  GLfloat dest[][4]);

GLuint _msdfgl_atlas_texture(msdfgl_font_t font);

GLuint _msdfgl_index_texture(msdfgl_font_t font);
