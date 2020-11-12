#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "lodepng.h"

struct box_size {
	int width;
	int height;
	int depth;
};

struct primitive {
	enum primitive_type {
		TRANSLATE, // Δx if pop, Δy if pop, pop?
		GLYPH, // x, y, glyph index, lower (1) - upper (2) - both (3), color
		LINE, // x, y, Δx, Δy, color
		ELLIPSE, // x center, y center, a, b, color
		// y ↑+
	} type;
	int param[5];
} pstk[1000];

struct {
	bool known;
	struct box_size size;
} pstk_sizes[1000];

size_t pstk_count = 0;

struct glyph {
	uint8_t bitmap[20];
	struct box_size size;
	int italic_correction;
	int delimiter_lower_x;
	int delimiter_lower_y;
	int delimiter_upper_x;
	int delimiter_upper_y;
} glyphs[4096];

void load_glyphs() {
	uint8_t *image = NULL;
	unsigned image_width, image_height;
	unsigned error = lodepng_decode24_file(&image, &image_width, &image_height, "slzee2-font.png");
	if (error) {
		printf("LodePNG error %u: %s\n", error, lodepng_error_text(error));
		exit(1);
	}
	for (size_t i = 0; i < image_height / 20; i++) {
		for (size_t j = 0; j < image_width / 8; j++) {
			struct glyph *glyph = &glyphs[i * 1024 + j];
			glyph->size.width = 0;
			glyph->size.height = 0;
			glyph->size.depth = INT_MIN;
			glyph->italic_correction = INT_MIN;
			glyph->delimiter_lower_x = INT_MIN;
			glyph->delimiter_lower_y = INT_MIN;
			glyph->delimiter_upper_x = INT_MIN;
			glyph->delimiter_upper_y = INT_MIN;
			for (size_t y = 0; y < 20; y++) {
				glyph->bitmap[y] = 0;
				for (size_t x = 0; x < 8; x++) {
					uint8_t *pixel = &image[((i * 20 + (20 - y - 1)) * image_width + j * 8 + x) * 3];
					if (pixel[2] >= 128) {
						if (x + 1 > glyph->size.width) {
							glyph->size.width = x + 1;
						}
						if (x == 0) {
							glyph->size.height = (int) y - 20 / 2 + 1;
							if (glyph->size.depth == INT_MIN) glyph->size.depth = 20 / 2 - (int) y;
						}
						if (glyph->size.depth != INT_MIN && y == 20 / 2 - glyph->size.depth + 2) {
							glyph->italic_correction = x + 1;
						}
					}
					if (pixel[1] >= 128) {
						glyph->bitmap[y] |= 1 << x;
					}
					if (pixel[0] == 255) {
						if (y < 20 / 2) {
							glyph->delimiter_lower_x = x;
							glyph->delimiter_lower_y = y - 20 / 2;
						} else {
							glyph->delimiter_upper_x = x;
							glyph->delimiter_upper_y = y - 20 / 2;
						}
					}
				}
			}
			if (glyph->size.height == INT_MIN) glyph->size.depth = 0;
			if (glyph->italic_correction == INT_MIN) {
				glyph->italic_correction = 0;
			} else {
				glyph->italic_correction = glyph->size.width - glyph->italic_correction;
			}
		}
	}
	free(image);
}

size_t pstk_match(size_t pstk_index) {
	if (pstk[pstk_index].type != TRANSLATE) {
		fprintf(stderr, "Internal error: trying to match a non-translate\n");
		exit(1);
	}
	int i = 0;
	ptrdiff_t dir = 1 - 2 * pstk[pstk_index].param[2];
	for (;;) {
		if (pstk[pstk_index].type == TRANSLATE) {
			if (pstk[pstk_index].param[2]) {
				i--;
			} else {
				i++;
			}
		}
		if (!i) break;
		pstk_index += dir;
		if (pstk_index >= pstk_count) {
			fprintf(stderr, "Internal error: pstk is not balanced\n");
			exit(1);
		}
	}
	return pstk_index;
}

// returns the pstk index of the corresponding pop
size_t measure(size_t pstk_index) {
	if (pstk_sizes[pstk_index].known) {
		if (pstk[pstk_index].type == TRANSLATE && !pstk[pstk_index].param[2]) pstk_index = pstk_match(pstk_index);
		return pstk_index;
	}
	int width = 0;
	int height = 0;
	int depth = 0;
	if (pstk[pstk_index].type == TRANSLATE) {
		if (pstk[pstk_index].param[2]) pstk_index = pstk_match(pstk_index);
		size_t j;
		for (j = pstk_index + 1; !(pstk[j].type == TRANSLATE && pstk[j].param[2]); j++) {
			if (j >= pstk_count) {
				fprintf(stderr, "Internal error: unexpected measure-out-of-bound\n");
				exit(1);
			}
			size_t new_j = measure(j);
			int new_width = pstk[new_j].param[0] + pstk_sizes[j].size.width;
			int new_height = pstk[new_j].param[1] + pstk_sizes[j].size.height;
			int new_depth = -pstk[new_j].param[1] + pstk_sizes[j].size.depth;
			j = new_j;
			if (new_width > width) width = new_width;
			if (new_height > height) height = new_height;
			if (new_depth > depth) depth = new_depth;
		}
		pstk_sizes[pstk_index].size = (struct box_size) {
			.width = width,
			.height = height,
			.depth = depth,
		};
		pstk_sizes[pstk_index].known = true;
		pstk_index = j;
	} else if (pstk[pstk_index].type == GLYPH) {
		width = glyphs[pstk[pstk_index].param[2]].size.width;
		if (pstk[pstk_index].param[3] & 2) height = glyphs[pstk[pstk_index].param[2]].size.height;
		if (pstk[pstk_index].param[3] & 1) depth = glyphs[pstk[pstk_index].param[2]].size.depth;
	} else if (pstk[pstk_index].type == LINE) {
		width = pstk[pstk_index].param[2];
		if (width < 0) width = 0;
		height = pstk[pstk_index].param[3];
		depth = 0;
		if (height < 0) {
			depth = -height;
			height = 0;
		}
	} else if (pstk[pstk_index].type == ELLIPSE) {
		width = pstk[pstk_index].param[2];
		height = pstk[pstk_index].param[3];
		depth = pstk[pstk_index].param[3];
	}
	pstk_sizes[pstk_index].size = (struct box_size) {
		.width = width,
		.height = height,
		.depth = depth,
	};
	pstk_sizes[pstk_index].known = true;
	return pstk_index;
}

void apply_translation(size_t pstk_index) {
	if (pstk[pstk_index].type != TRANSLATE) {
		fprintf(stderr, "Internal error: trying to measure a non-translate\n");
		exit(1);
	}
	int i = 0;
	if (!pstk[pstk_index].param[2]) pstk_index = pstk_match(pstk_index);
	int dx = pstk[pstk_index].param[0];
	int dy = pstk[pstk_index].param[1];
	if (!dx && !dy) return;
	i--;
	do {
		pstk_index--;
		if (pstk[pstk_index].type == TRANSLATE) {
			if (pstk[pstk_index].param[2]) {
				i--;
			} else {
				i++;
			}
		} else {
			pstk[pstk_index].param[0] += dx;
			pstk[pstk_index].param[1] += dy;
		}
	} while (i);
}

signed char vram[546][512];
void draw(struct primitive *p) {
	if (p->type == GLYPH) {
		size_t y0 = p->param[3] & 1 ? 0 : 20 / 2;
		size_t y1 = p->param[3] & 2 ? 20 : 20 / 2;
		for (size_t y = y0; y < y1; y++) {
			for (size_t x = 0; x < 8; x++) {
				if (glyphs[p->param[2]].bitmap[y] & (1 << x)) {
					vram[p->param[1] + y - 10][p->param[0] + x] = p->param[4];
				}
			}
		}
	} else if (p->type == LINE) {
		int x = p->param[0];
		int y = p->param[1];
		int tx = p->param[0] + p->param[2];
		int ty = p->param[1] + p->param[3];
		int dx = abs(p->param[2]);
		int sx = p->param[2] >= 0 ? 1 : -1;
		int dy = -abs(p->param[3]);
		int sy = p->param[3] >= 0 ? 1 : -1;
		int err = dx + dy;
		while (x != tx || y != ty) {
			vram[y][x] = p->param[4];
			if (err * 2 >= dy) { err += dy; x += sx; }
			if (err * 2 <= dx) { err += dx; y += sy; }
		}
	} else if (p->type == ELLIPSE) {
		int x0 = p->param[0] - p->param[2];
		int x1 = p->param[0] + p->param[2];
		int y0 = p->param[1] - p->param[3];
		int y1 = p->param[1] + p->param[3];
		int a = abs(x1 - x0), b = abs(y1 - y0); /* values of diameter */
		int dx = 4 * (1 - a) * b * b;
		int dy = 4 * a * a; /* error increment */
		int err = dx + dy; /* error of 1.step */

		// if called with swapped points, exchange them
		if (x0 > x1) { x0 = x1; x1 += a; }
		if (y0 > y1) y0 = y1;
		y0 += (b + 1) / 2;
		y1 = y0;   /* starting pixel */
		a *= 8 * a;
		int b1 = 8 * b * b;

		do {
			vram[y0][x1] = p->param[4]; //   I. Quadrant
			vram[y0][x0] = p->param[4]; //  II. Quadrant
			vram[y1][x0] = p->param[4]; // III. Quadrant
			vram[y1][x1] = p->param[4]; //  IV. Quadrant
			if (err * 2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */
			if (err * 2 >= dx || err * 2 > dy) { x0++; x1--; err += dx += b1; } /* x step */
		} while (x0 <= x1);

		// too early stop of flat ellipses a = 1
		while (y0 - y1 < b) {
			vram[y0][x0 - 1] = p->param[4];
			vram[y0][x1 + 1] = p->param[4];
			vram[y1][x0 - 1] = p->param[4];
			vram[y1][x1 + 1] = p->param[4];
			y0++; y1--;
		}
	}
}

void pstk_push_translation() {
	pstk[pstk_count++] = (struct primitive) {
		.type = TRANSLATE,
		.param = {0, 0, 0, INT_MIN, INT_MIN},
	};
}

void pstk_pop_translation() {
	pstk[pstk_count++] = (struct primitive) {
		.type = TRANSLATE,
		.param = {0, 0, 1, INT_MIN, INT_MIN},
	};
}

void pstk_add_box(int x, int y, int dx, int dy, int color) {
	pstk[pstk_count++] = (struct primitive) {
		.type = LINE,
		.param = {x, y, dx, 0, color},
	};
	pstk[pstk_count++] = (struct primitive) {
		.type = LINE,
		.param = {x + dx, y, 0, dy, color},
	};
	pstk[pstk_count++] = (struct primitive) {
		.type = LINE,
		.param = {x + dx, y + dy, -dx, 0, color},
	};
	pstk[pstk_count++] = (struct primitive) {
		.type = LINE,
		.param = {x, y + dy, 0, -dy, color},
	};
}

void pstk_add_delimiter(int x, int y, int dy, int glyph, int color) {
	pstk_push_translation();
	pstk[pstk_count++] = (struct primitive) {
		.type = GLYPH,
		.param = {0, glyphs[glyph].size.depth, glyph, 1, color},
	};
	pstk[pstk_count++] = (struct primitive) {
		.type = GLYPH,
		.param = {0, dy - glyphs[glyph].size.height, glyph, 2, color},
	};
	if (glyphs[glyph].delimiter_lower_x != INT_MIN && glyphs[glyph].delimiter_upper_x != INT_MIN) {
		pstk[pstk_count++] = (struct primitive) {
			.type = LINE,
			.param = {0, dy - glyphs[glyph].size.height, glyph, 2, color},
		};
		pstk[pstk_count++] = (struct primitive) {
			.type = LINE,
			.param = {
				glyphs[glyph].delimiter_lower_x,
				pstk[pstk_count - 2].param[1] + glyphs[glyph].delimiter_lower_y,
				glyphs[glyph].delimiter_upper_x,
				pstk[pstk_count - 1].param[1] + glyphs[glyph].delimiter_upper_y,
				color,
			},
		};
		pstk[pstk_count - 1].param[2] -= pstk[pstk_count - 1].param[0];
		pstk[pstk_count - 1].param[3] -= pstk[pstk_count - 1].param[1];
	}
	pstk_pop_translation();
	pstk[pstk_count - 1].param[0] = x;
	pstk[pstk_count - 1].param[1] = y;
}

void pstk_add_piecewise_delimiter(int x, int y, int dy, int glyph_upper, int glyph_lower, int color) {
	pstk_add_delimiter(x, y, (dy + 1) / 2, glyph_lower, color);
	pstk_add_delimiter(x, y + (dy + 1) / 2, dy / 2, glyph_upper, color);
}

void input(int glyph_base, int color) {
	bool more_lines = false;
	size_t line = pstk_count;
	pstk_push_translation();
	for (int x = 0; ; ) {
		int op = getchar();
		if (op == EOF) {
			exit(2);
		} else if (strchr(".\\", op)) { // also when op is '\0'
			more_lines = (op == '\\');
			break;
		} else if (op == '!') {
			x -= 2;
		} else if (op == ',') {
			x += 2;
		} else if (op == '>') {
			x += 3;
		} else if (op == ';') {
			x += 4;
		} else if (strchr("rbiz", op)) {
			int glyph;
			scanf("%d", &glyph);
			pstk[pstk_count++] = (struct primitive) {
				.type = GLYPH,
				.param = {x, 0, glyph_base + glyph, 3, color},
			};
			x += glyphs[glyph].size.width + 1;
		} else if (op == 'F') {
			size_t numerator = pstk_count;
			pstk_push_translation();
			input(glyph_base, color);
			pstk_pop_translation();
			measure(numerator);
			size_t denominator = pstk_count;
			pstk_push_translation();
			input(glyph_base, color);
			pstk_pop_translation();
			measure(denominator);
			int max_width = pstk_sizes[numerator].size.width;
			if (pstk_sizes[denominator].size.width > max_width) max_width = pstk_sizes[denominator].size.width;
			pstk[denominator - 1].param[0] = x + (max_width - pstk_sizes[numerator].size.width) / 2 + 1;
			pstk[denominator - 1].param[1] = pstk_sizes[numerator].size.depth + 2;
			pstk[pstk_count - 1].param[0] = x + (max_width - pstk_sizes[denominator].size.width) / 2 + 1;
			pstk[pstk_count - 1].param[1] = -pstk_sizes[denominator].size.height - 1;
			pstk[pstk_count++] = (struct primitive) {
				.type = LINE,
				.param = {x, 0, max_width + 2, 0, color},
			};
			x += max_width + 3;
		} else if (op == 'S') {
			int prev_height = 0;
			int prev_depth = 0;
			int prev_italic_correction = 0;
			if (pstk_count && !(pstk[pstk_count - 1].type == TRANSLATE && !pstk[pstk_count - 1].param[2])) {
				measure(pstk_count - 1);
				prev_height = pstk[pstk_count - 1].param[1] + pstk_sizes[pstk_count - 1].size.height;
				prev_depth = -pstk[pstk_count - 1].param[1] + pstk_sizes[pstk_count - 1].size.depth;
				if (pstk[pstk_count - 1].type == GLYPH) {
					prev_italic_correction = glyphs[pstk[pstk_count - 1].param[2]].italic_correction;
				}
			}
			if (prev_height < 3) prev_height = 3;
			if (prev_depth < 3) prev_depth = 3;
			size_t superscript = pstk_count;
			pstk_push_translation();
			input(glyph_base | 1024, color);
			pstk_pop_translation();
			measure(superscript);
			size_t subscript = pstk_count;
			pstk_push_translation();
			input(glyph_base | 1024, color);
			pstk_pop_translation();
			measure(subscript);
			int max_width = pstk_sizes[superscript].size.width;
			if (pstk_sizes[subscript].size.width > max_width) max_width = pstk_sizes[subscript].size.width;
			pstk[subscript - 1].param[0] = x;
			pstk[subscript - 1].param[1] = prev_height + pstk_sizes[superscript].size.depth - 2;
			pstk[pstk_count - 1].param[0] = x - prev_italic_correction;
			pstk[pstk_count - 1].param[1] = -prev_depth - pstk_sizes[subscript].size.height + 3;
			x += max_width + 1;
		} else if (op == 'c') {
			scanf("%d", &color);
		} else if (op == 's') {
			scanf("%d", &glyph_base);
		} else if (op == 'X') {
			size_t translation = pstk_count;
			pstk_push_translation();
			input(glyph_base, color);
			pstk_pop_translation();
			measure(translation);
			pstk[pstk_count - 1].param[0] = x + 2;
			pstk[pstk_count - 1].param[1] = 0;
			pstk_add_box(x, -pstk_sizes[translation].size.depth - 2, pstk_sizes[translation].size.width + 3, pstk_sizes[translation].size.height + pstk_sizes[translation].size.depth + 3, color);
			x += pstk_sizes[translation].size.width + 5;
		} else if (op == 'B') {
			struct {
				size_t translation;
				bool ending_delimiter_piecewise;
				int ending_delimiter_glyph_upper;
				int ending_delimiter_glyph_lower;
				int ending_delimiter_x;
			} parts[8];
			size_t part_count;
			{
				int part_count_;
				scanf("%d", &part_count_);
				if (part_count_ < 0) part_count_ = 0;
				part_count = part_count_;
			}
			part_count += 2;
			for (size_t i = 0; i < part_count; i++) {
				scanf("%d%d", &parts[i].ending_delimiter_glyph_upper, &parts[i].ending_delimiter_glyph_lower);
				parts[i].ending_delimiter_piecewise = parts[i].ending_delimiter_glyph_lower >= 0;
			}
			int max_height = 0;
			int max_depth = 0;
			parts[0].translation = SIZE_MAX;
			parts[0].ending_delimiter_x = x;
			x += glyphs[parts[0].ending_delimiter_glyph_upper].size.width + 1;
			for (size_t i = 1; i < part_count; i++) {
				parts[i].translation = pstk_count;
				pstk_push_translation();
				input(glyph_base, color);
				pstk_pop_translation();
				measure(parts[i].translation);
				if (pstk_sizes[pstk_count - 1].size.height > max_height) max_height = pstk_sizes[pstk_count - 1].size.height;
				if (pstk_sizes[pstk_count - 1].size.depth > max_depth) max_depth = pstk_sizes[pstk_count - 1].size.depth;
				pstk[pstk_count - 1].param[0] = x;
				pstk[pstk_count - 1].param[1] = 0;
				x += pstk_sizes[pstk_count - 1].size.width + 1;
				parts[i].ending_delimiter_x = x;
				x += glyphs[parts[i].ending_delimiter_glyph_upper].size.width + 1;
				parts[i].translation = pstk_count - 1;
			}
			max_height++;
			max_depth++;
			for (size_t i = 0; i < part_count; i++) {
				if (parts[i].ending_delimiter_piecewise) {
					pstk_add_piecewise_delimiter(parts[i].ending_delimiter_x, -max_depth, max_height + max_depth, parts[i].ending_delimiter_glyph_upper, parts[i].ending_delimiter_glyph_lower, color);
				} else {
					pstk_add_delimiter(parts[i].ending_delimiter_x, -max_depth, max_height + max_depth, parts[i].ending_delimiter_glyph_upper, color);
				}
			}
		} else if (op < 0x80 && !isspace(op)) {
			exit(3);
		}
	}
	pstk_pop_translation();
	for (int dy = line ? pstk_sizes[line - 1].size.depth + pstk_sizes[line].size.height + 3 : INT_MIN; line && (pstk[line - 1].type == TRANSLATE && pstk[line - 1].param[2]); line = pstk_match(line - 1)) {
		measure(line - 1);
		measure(line);
		pstk[line - 1].param[1] += dy;
	}
	if (more_lines) input(glyph_base, color);
}

int main(int argc, char *argv[]) {
	load_glyphs();
	pstk_push_translation();
	input(0, 4);
	pstk_pop_translation();
	measure(0);
	int margin = 19;
	pstk[pstk_count - 1].param[0] = margin;
	pstk[pstk_count - 1].param[1] = pstk_sizes[0].size.depth + margin;
	for (size_t i = 0; i < pstk_count; i++) {
		if (pstk[i].type == TRANSLATE && pstk[i].param[2]) {
			apply_translation(i);
		}
	}
	for (size_t i = 0; i < pstk_count; i++) {
		printf("%d %d %d\n", pstk[i].param[0], pstk[i].param[1], pstk[i].param[2]);
		draw(&pstk[i]);
	}

	int width = (pstk_sizes[0].size.width + margin * 2) * 2, height = (pstk_sizes[0].size.height + pstk_sizes[0].size.depth + margin * 2) * 2;
	uint8_t *image = calloc((width * height * 4 + 7) / 8, 1);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int color = vram[height / 2 - y / 2 - 1][x / 2] & 15;
			image[(y * width + x) / 2] |= (uint8_t) (color << ((y * width + x) & 1 ? 0 : 4));
		}
	}

	LodePNGState state;
	lodepng_state_init(&state);

	const uint8_t palette[16][3] = {
		{  0,  43,  54}, // base03
		{  7,  54,  66}, // base02
		{ 88, 110, 117}, // base01
		{101, 123, 131}, // base00
		{131, 148, 150}, // base0
		{147, 161, 161}, // base1
		{238, 232, 213}, // base2
		{253, 246, 227}, // base3
		{181, 137,   0}, // yellow
		{203,  75,  22}, // orange
		{220,  50,  47}, // red
		{211,  54, 130}, // magenta
		{108, 113, 196}, // violet
		{ 38, 139, 210}, // blue
		{ 42, 161, 152}, // cyan
		{133, 153,   0}, // green
	};
	for (size_t i = 0; i < 16; i++) {
		lodepng_palette_add(&state.info_png.color, palette[i][0], palette[i][1], palette[i][2], 255);
		lodepng_palette_add(&state.info_raw, palette[i][0], palette[i][1], palette[i][2], 255);
	}

	state.info_png.color.colortype = LCT_PALETTE;
	state.info_png.color.bitdepth = 4;
	state.info_raw.colortype = LCT_PALETTE;
	state.info_raw.bitdepth = 4;
	state.encoder.auto_convert = false;
	state.encoder.filter_palette_zero = true;
	state.encoder.add_id = false;
	state.encoder.text_compression = false;
	state.encoder.zlibsettings.nicematch = 128;
	state.encoder.zlibsettings.lazymatching = false;
	state.encoder.zlibsettings.windowsize = 256;

	unsigned char *png;
	size_t pngsize;
	unsigned error = lodepng_encode(&png, &pngsize, image, width, height, &state);
	if (error) {
		printf("LodePNG error %u: %s\n", error, lodepng_error_text(error));
	} else {
		lodepng_save_file(png, pngsize, argc >= 2 ? argv[1] : "slzee2-output.png");
	}

	lodepng_state_cleanup(&state);
	free(png);
	return 0;
}
