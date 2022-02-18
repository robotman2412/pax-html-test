
#include "ph_base_element.hpp"

namespace phtml {


// Remove the child element without deleting it.
Element *Element::removeChild(Element *element) {
	this->children.remove(element);
	return element;
}

// Remove the child element without deleting it.
Element *Element::removeChild(int index) {
	ElemList::iterator i;
	for (i = this->children.begin(); i != this->children.end(); i++) {
		if (index == 0) {
			return this->removeChild(*i);
		} else {
			index --;
		}
	}
	return NULL;
}


// Internal render method for elements which do more than just their children.
void Element::renderInternal(pax_buf_t *to) {}

// Internal bounds method for elements which do more than just their children.
pax_rect_t Element::boundsInternal() {
	return (pax_rect_t) {
		.x = 0,
		.y = 0,
		.w = 0,
		.h = 0
	};
}

// Helper method to apply the transform to a bounds rectangle.
pax_rect_t Element::transformBounds(pax_rect_t in) {
	if (!this->transform || in.w <= 0 || in.h <= 0) {
		// Ignore the rest if there's no transformation.
		return in;
	}
	// Construct a quad.
	pax_quad_t quad = (pax_quad_t) {
		.x0 = in.x,      .y0 = in.y,
		.x1 = in.x+in.w, .y1 = in.y,
		.x2 = in.x+in.w, .y2 = in.y+in.h,
		.x3 = in.x,      .y3 = in.y+in.h
	};
	// Transform the points.
	matrix_2d_transform(*this->transform, &quad.x0, &quad.y0);
	matrix_2d_transform(*this->transform, &quad.x1, &quad.y1);
	matrix_2d_transform(*this->transform, &quad.x2, &quad.y2);
	matrix_2d_transform(*this->transform, &quad.x3, &quad.y3);
	// Turn it back into a rectangle.
	float x0 = fminf(fminf(quad.x0, quad.x1), fminf(quad.x2, quad.x3));
	float y0 = fminf(fminf(quad.y0, quad.y1), fminf(quad.y2, quad.y3));
	float x1 = fmaxf(fmaxf(quad.x0, quad.x1), fmaxf(quad.x2, quad.x3));
	float y1 = fmaxf(fmaxf(quad.y0, quad.y1), fmaxf(quad.y2, quad.y3));
	// And return it.
	return (pax_rect_t) {
		.x = x0,
		.y = y0,
		.w = x1 - x0,
		.h = y1 - y0
	};
}

		
// Constructor.
Element::Element() {
	this->dirty     = true;
	this->parent    = NULL;
	this->transform = NULL;
}

// Deconstructor.
Element::~Element() {
	if (this->transform) {
		delete this->transform;
	}
}


// Add the given element as a child to this.
// The given element must be made using the 'new' operator.
void Element::appendChild(Element *element) {
	this->appendChild(element, -1);
}

// Add the given element as a child to this.
// The given element must be made using the 'new' operator.
void Element::appendChild(Element *element, int index) {
	// Negative indices are relative to the end of the list.
	if (index < 0) index = this->children.size();
	if (index <= 0) {
		// If still <= 0, then add to front.
		this->children.push_front(element);
	} else if (index >= this->children.size()) {
		// If beyond end, then add to end.
		this->children.push_back(element);
	} else {
		// Try in a for loop.
		ElemList::iterator i;
		for (i = this->children.begin(); i != this->children.end(); i++) {
			if (index == 0) {
				this->children.insert(i, element);
				return;
			} else {
				index --;
			}
		}
		// If that doesn't work, then add to end.
		this->children.push_back(element);
	}
}


// Display the element.
// Force: Force rendering even if the element did not change.
void Element::render(pax_buf_t *to, bool force) {
	if (!dirty && !force) return;
	
	// Apply the transformation, if any.
	matrix_2d_t *transform = this->transform;
	if (transform) {
		pax_push_2d (to);
		pax_apply_2d(to, *this->transform);
	}
	
	// Render self.
	this->calculateBounds(force);
	this->renderInternal(to);
	
	// Render all the children, of.
	ElemList::iterator i;
	float lastY = 0;
	for (i = this->children.begin(); i != this->children.end(); i++) {
		Element *child = *i;
		pax_push_2d  (to);
		pax_apply_2d (to, matrix_2d_translate(0, lastY));
		child->calculateBounds(force);
		child->render(to, force);
		lastY = fmaxf(lastY, lastY + child->bounds.y + child->bounds.h);
		pax_pop_2d   (to);
	}
	
	// Pop if required.
	if (transform) {
		pax_pop_2d (to);
	}
}

// Calculate bounds.
// Does not take parent element into account.
void Element::calculateBounds(bool force) {
	if (!dirty && !force) return;
	pax_rect_t initial = this->boundsInternal();
	
	float x0, y0, x1, y1;
	if (initial.w > 0 || initial.h > 0) {
		x0 = initial.x;
		y0 = initial.y;
		x1 = initial.x + initial.w;
		y1 = initial.y + initial.h;
	} else {
		x0 = INFINITY;
		y0 = INFINITY;
		x1 = -INFINITY;
		y1 = -INFINITY;
	}
	
	// Compute all children.
	ElemList::iterator i;
	for (i = this->children.begin(); i != this->children.end(); i++) {
		Element *child = *i;
		// Let it calculate.
		child->calculateBounds(force);
		if (child->bounds.w > 0 && child->bounds.h > 0) {
			// And merge it in.
			if (child->bounds.x < x0) {
				x0 = child->bounds.x;
			}
			if (child->bounds.x + child->bounds.w > x1) {
				x1 = child->bounds.x + child->bounds.w;
			}
			if (child->bounds.y < y0) {
				y0 = child->bounds.y;
			}
			if (child->bounds.y + child->bounds.h > y1) {
				y1 = child->bounds.y + child->bounds.h;
			}
		}
	}
	
	// And store it.
	if (isfinite(x0) && isfinite(y0) && isfinite(x1) && isfinite(y1)) {
		this->bounds = (pax_rect_t) {
			.x = x0,
			.y = y0,
			.w = x1 - x0,
			.h = y1 - y0
		};
	} else {
		this->bounds = (pax_rect_t) {
			.x = 0,
			.y = 0,
			.w = 0,
			.h = 0
		};
	}
}


// Whether the element is dirty.
bool Element::isDirty() {
	return this->dirty;
}

// Mark the element as dirty.
void Element::markDirty() {
	this->dirty = true;
	if (this->parent) {
		this->parent->markDirty();
	}
}

}; // namespace phtml
