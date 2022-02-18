
#ifndef PH_CSS_H
#define PH_CSS_H

#include <pax_gfx.h>
#include <map>

namespace phtml {

// Types of CSS properties.
enum PropertyType {
    // Enumeration (from a global table of all keywords).
    PropEnum,
    // Numbers stored as float.
    PropNumeric,
    // Colors stored as pax_col_t.
    PropColor,
    // Strings stored as char *.
    PropString,
    // Font stored as pax_font_t *.
    PropFont,
};

// Recognised CSS properties.
enum Properties {
    // Background color.
    BackgroundColor,
    
    // Text color.
    Color,
    // Text font.
    FontFamily,
    // Text size.
    FontSize,
    
    // Display type.
    Display,
    // Top offset.
    Top,
    // Left offset.
    Left,
    // Bottom offset.
    Bottom,
    // Right offset.
    Right,
    
    // Minimum width.
    MinWidth,
    // Maximum width.
    MaxWidth,
    // Minimum Height.
    MinHeight,
    // Maximum Height.
    MaxHeight,
};

// A CSS property.
class Property {
	public:
		// Set as string.
		void        setStr(char *newValue);
		// Set as float.
		void        setfloat(float newValue);
		// Set as color.
		void        setColor(pax_col_t newValue);
		// Set as font.
		void        setFont(pax_font_t *newValue);
		
		// Get as string.
		char       *getStr();
		// Get as float.
		float       getFloat();
		// Get as color.
		float       getColor();
		// Get as font.
		pax_font_t *getFont();
};

// A style to apply to an element.
class Style {
    private:
        // Parent style.
        Style *parent;
	public:
		// Operator to index a property by name.
		Property &operator[](char *name);
		// Operator to index a property by id.
		Property &operator[](int id);
};

}; // namespace phtml

#endif //PH_CSS_H
