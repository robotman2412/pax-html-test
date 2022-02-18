
#ifndef PH_COMPOSITOR_H
#define PH_COMPOSITOR_H

#include "ph_base_element.hpp"

namespace phtml {

class Text : public Element {
	protected:
		// The text to show.
		char      *text;
		// The length of the text to show.
		size_t     text_len;
        // The point size of the text to show.
        float      text_size;
		// Internal render method for elements which do more than just their children.
		void       renderInternal(pax_buf_t *to);
		// Internal render method for elements which do more than just their children.
		pax_rect_t boundsInternal();
	public:
		Text();
		Text(char *cstr);
		Text(char *cstr, size_t len);
		~Text();
};

}; // namespace phtml

#endif //PH_COMPOSITOR_H
