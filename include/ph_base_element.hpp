
#ifndef PH_BASE_ELEMENT_H
#define PH_BASE_ELEMENT_H

#include <pax_gfx.h>
#include <list>
#include <map>

#include "ph_css.hpp"

namespace phtml {

class Element;

typedef std::list<Element*> ElemList;
typedef std::list<pax_rect_t> BoundsList;

// The document class.
// A container for information relevant to all elements and CSS styles.
// Most functions here aren't relevant outside this library.
class Document {
	public:
		// List of all elements.
		ElemList elements;
};

// The base element class.
// Implements a basic element, with no size nor background.
class Element {
	protected:
		// Parent document.
		Document          *parentDocument;
		
		// Whether the element changed since last render call.
		bool               dirty;
		// Transformation applied by style to this object.
		matrix_2d_t       *transform;
		// Child elements.
		ElemList           children;
		
		// Remove the child element without deleting it.
		Element           *removeChild(Element *element);
		// Remove the child element without deleting it.
		Element           *removeChild(int index);
		
		// Internal render method for elements which do more than just their children.
		virtual void       renderInternal(pax_buf_t *to);
		// Internal bounds method for elements which do more than just their children.
		virtual pax_rect_t boundsInternal();
		
		// Helper method to apply the transform to a bounds rectangle.
		pax_rect_t         transformBounds(pax_rect_t in);
	public:
		// Parent element, may be null.
		Element           *parent;
		// Calculated bounding box.
		pax_rect_t         bounds;
		
		// Constructor.
		                   Element();
		// Deconstructor.
		virtual           ~Element();
		
		// Add the given element as a child to this.
		// The given element must be made using the 'new' operator.
		void               appendChild(Element *element);
		// Add the given element as a child to this.
		// The given element must be made using the 'new' operator.
		void               appendChild(Element *element, int index);
		
		// Display the element.
		// Force: Force rendering even if the element did not change.
		void               render(pax_buf_t *to, bool force);
		// Calculate bounds.
		// Force: Force calculation even if the element did not change.
		void               calculateBounds(bool force);
		
		// Whether the element is dirty.
		bool               isDirty();
		// Mark the element as dirty.
		void               markDirty();
};

}; // namespace phtml

#endif //PH_BASE_ELEMENT_H
