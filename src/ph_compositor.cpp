
#include "ph_compositor.hpp"
#include <string.h>

static const char *TAG = "phtml";

namespace phtml {


// Internal render method for elements which do more than just their children.
void Text::renderInternal(pax_buf_t *to) {
	// EXPLAIN MATRIX!
	pax_draw_text(to, 0xff000000, NULL, this->text_size, 0, 0, this->text);
}

// Internal render method for elements which do more than just their children.
pax_rect_t Text::boundsInternal() {
	pax_vec1_t dim = pax_text_size(NULL, this->text_size, this->text);
	pax_rect_t out = (pax_rect_t) {
		.x = 0,
		.y = 0,
		.w = dim.x,
		.h = dim.y
	};
	return this->transformBounds(out);
}

Text::Text() {
	// Add a dummy string.
	this->text      = new char[1];
	*this->text     = 0;
	this->text_size = 18;
}

Text::Text(char *cstr) {
	if (cstr) {
		// Copy the text.
		this->text      = strdup(cstr);
		this->text_len  = strlen(cstr);
	} else {
		this->text      = new char[1];
		*this->text     = 0;
	}
	this->text_size = 18;
}

Text::Text(char *cstr, size_t len) {
	if (!cstr) {
		// We can't be having a NULL.
		this->text      = new char[1];
		*this->text     = 0;
	} else if (cstr[len]) {
		// Ensure null termination.
		this->text      = new char[len + 1];
		memcpy(this->text, cstr, len);
		this->text[len] = 0;
		this->text_len  = len;
	} else {
		// Already terminated.
		this->text      = strdup(cstr);
		this->text_len  = len;
	}
	this->text_size     = 18;
}

Text::~Text() {
	delete this->text;
}


}; // namespace phtml
